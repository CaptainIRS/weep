/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <cstdint>
#include <iostream>
#include <cmath>
#include <math.h>
#include <sstream>
#include "ns3/energy-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-monitor.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/log-macros-enabled.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/internet-module.h"
#include "ns3/node-list.h"
#include "ns3/nstime.h"
#include "ns3/packet-scheduler-base.h"
#include "ns3/aodv-weep-scheduler.h"
#include "ns3/aodv-weep-routing-protocol.h"
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/string.h"
#include "ns3/wifi-radio-energy-model-helper.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/aodv-weep-helper.h"

#include "weep-wifi-sim.h"

using namespace ns3;

uint16_t port = 8000;

NS_LOG_COMPONENT_DEFINE ("WeepWifiSimulation");

WeepWifiSimulation::WeepWifiSimulation ()
    : m_bytesReceived (0), m_packetsReceived (0), m_bytesSent (0), m_packetsSent (0)
{
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode",
                      StringValue ("DsssRate11Mbps"));
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("1000"));
}

long double
WeepWifiSimulation::GetThroughput ()
{
  return (double) m_bytesReceived / m_totalTime;
}

long double
WeepWifiSimulation::GetPDR ()
{
  return (double) m_packetsReceived / m_packetsSent;
}

double
WeepWifiSimulation::GetPerPacketPerRouterWaitingTime ()
{
  return (double) m_perPacketPerRouterWaitingTime / 10e9 / m_packetsQueued;
}

void
WeepWifiSimulation::UpdatePerPacketPerRouterWaitingTime (uint64_t time)
{
  m_packetsQueued++;
  m_perPacketPerRouterWaitingTime += time;
}

long double
WeepWifiSimulation::GetAverageDelay ()
{
  return m_totalDelay / m_packetsReceived;
}

long double
WeepWifiSimulation::GetAverageEnergyConsumption ()
{
  return m_totalRemainingEnergy / nodes.GetN ();
}

void
WeepWifiSimulation::CaseRun (std::string scheduler, uint32_t nWifis, uint32_t nSinks,
                             double totalTime, double range, uint32_t nodeSpeed, double dataStart)
{
  Ptr<UniformRandomVariable> var = CreateObject<UniformRandomVariable> ();

  m_scheduler = scheduler;
  m_nWifis = nWifis;
  m_nSinks = nSinks;
  m_totalTime = totalTime;
  m_range = range;
  m_nodeSpeed = nodeSpeed;
  m_dataStart = dataStart;
  m_perPacketPerRouterWaitingTime = 0;
  m_packetsQueued = 0;
  m_totalDelay = 0;

  CreateNodes ();
  CreateDevices ();
  SetupMobility ();
  InstallInternetStack ();
  InstallApplications ();
  InstallEnergyModels ();

  FlowMonitorHelper flowHelper;
  auto monitor = flowHelper.InstallAll ();

  SetupTrace ();

  Simulator::Stop (Seconds (m_totalTime + 10));
  Simulator::Run ();

  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier =
      DynamicCast<Ipv4FlowClassifier> (flowHelper.GetClassifier ());
  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();

  for (auto stat : stats)
    {
      m_packetsSent += stat.second.txPackets;
      m_packetsReceived += stat.second.rxPackets;
      m_bytesSent += stat.second.txBytes;
      m_bytesReceived += stat.second.rxBytes;
      m_totalDelay += stat.second.delaySum.GetMilliSeconds ();
    }
  monitor->SerializeToXmlFile ("flow.xml", true, true);

  for (uint32_t i = 0; i < sources.GetN (); i++)
    {
      m_totalRemainingEnergy += 1000 - sources.Get (i)->GetRemainingEnergy ();
    }

  Simulator::Destroy ();
}

void
WeepWifiSimulation::CreateNodes ()
{
  nodes.Create (m_nWifis);
  NS_ASSERT_MSG (m_nWifis > m_nSinks,
                 "Sinks must be less or equal to the number of nodes in network");
}

void
WeepWifiSimulation::SetupMobility ()
{
  MobilityHelper mobility;
  ObjectFactory pos;
  std::ostringstream positionVariableStream;
  positionVariableStream << "ns3::UniformRandomVariable[Min=-250|Max=250]";
  // positionVariableStream << "ns3::UniformRandomVariable[Min=-50|Max=50]";
  pos.SetTypeId ("ns3::RandomRectanglePositionAllocator");
  pos.Set ("X", StringValue (positionVariableStream.str ()));
  pos.Set ("Y", StringValue (positionVariableStream.str ()));

  std::ostringstream speedConstantRandomVariableStream;
  speedConstantRandomVariableStream << "ns3::ConstantRandomVariable[Constant=" << m_nodeSpeed
                                    << "]";

  Ptr<PositionAllocator> taPositionAlloc = pos.Create ()->GetObject<PositionAllocator> ();
  mobility.SetMobilityModel ("ns3::RandomWaypointMobilityModel", "Speed",
                             StringValue (speedConstantRandomVariableStream.str ()),
                             "PositionAllocator", PointerValue (taPositionAlloc));
  mobility.SetPositionAllocator (taPositionAlloc);
  mobility.Install (nodes);
}

void
WeepWifiSimulation::CreateDevices ()
{
  WifiMacHelper wifiMac;
  wifiMac.SetType ("ns3::AdhocWifiMac");
  YansWifiPhyHelper wifiPhy;
  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  Ptr<UniformRandomVariable> var = CreateObject<UniformRandomVariable> ();
  wifiChannel.AddPropagationLoss ("ns3::RangePropagationLossModel", "MaxRange",
                                  DoubleValue (var->GetValue (10.0, 50.0)));
  wifiPhy.SetChannel (wifiChannel.Create ());
  WifiHelper wifi;
  wifi.SetStandard (WIFI_STANDARD_80211b);
  devices = wifi.Install (wifiPhy, wifiMac, nodes);
}

void
WeepWifiSimulation::InstallInternetStack ()
{
  AodvWeepHelper aodv;
  aodv.Set ("PacketScheduler", StringValue (m_scheduler));
  InternetStackHelper stack;
  stack.SetRoutingHelper (aodv);
  stack.Install (nodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  interfaces = address.Assign (devices);
}

void
WeepWifiSimulation::InstallApplications ()
{
  for (uint32_t clientNode = 0; clientNode < m_nWifis; clientNode++)
    {
      Ptr<UniformRandomVariable> var = CreateObject<UniformRandomVariable> ();
      for (uint32_t j = 0; j < m_nSinks; j++)
        {
          OnOffHelper onoff ("ns3::UdpSocketFactory",
                             Address (InetSocketAddress (interfaces.GetAddress (j), port)));
          onoff.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"));
          onoff.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"));
          onoff.SetConstantRate (DataRate ("1Kbps"));
          if (j != clientNode)
            {
              ApplicationContainer apps = onoff.Install (nodes.Get (clientNode));
              apps.Start (Seconds (var->GetValue (m_dataStart, m_dataStart + 1)));
              apps.Stop (Seconds (m_totalTime));
            }
        }
    }
}

void
WeepWifiSimulation::InstallEnergyModels ()
{
  BasicEnergySourceHelper basicSourceHelper;
  basicSourceHelper.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (10000));
  sources = basicSourceHelper.Install (nodes);

  WifiRadioEnergyModelHelper radioEnergyHelper;
  DeviceEnergyModelContainer deviceModels = radioEnergyHelper.Install (devices, sources);
}

void
WeepWifiSimulation::SetupTrace ()
{
  for (uint32_t i = 0; i < NodeList::GetNNodes (); i++)
    {
      auto scheduler = DynamicCast<weep::AodvWeepScheduler> (
          NodeList::GetNode (i)->GetObject<weep::AodvWeepRoutingProtocol> ()->GetScheduler ());
      scheduler->TraceConnectWithoutContext (
          "PerPacketWaitingTime",
          MakeCallback (&WeepWifiSimulation::UpdatePerPacketPerRouterWaitingTime, this));
    }
}
