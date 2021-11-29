#include <cstdint>
#include <iostream>
#include <cmath>
#include "ns3/core-module.h"
#include "ns3/energy-source-container.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/gnuplot.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/internet-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/aodv-weep-helper.h"
#include "ns3/stats-module.h"

using namespace ns3;

class WeepWifiSimulation
{
public:
  WeepWifiSimulation ();

  void CaseRun (std::string scheduler, uint32_t nWifis, uint32_t nSinks, double totalTime,
                double range, uint32_t nodeSpeed, double dataStart);
  long double GetThroughput ();
  long double GetPDR ();
  long double GetAverageDelay ();
  double GetPerPacketPerRouterWaitingTime ();
  long double GetAverageEnergyConsumption ();
  void SinkTrace (Ptr<const Packet> packet, const Address &from, const Address &localAddress,
                  const SeqTsSizeHeader &header);

private:
  uint32_t m_nWifis;
  uint32_t m_nSinks;
  double m_totalTime;
  double m_range;
  uint32_t m_nodeSpeed;
  double m_dataStart;
  long double m_bytesReceived;
  long double m_packetsReceived;
  long double m_bytesSent;
  long double m_packetsSent, m_packetsQueued;
  long double m_totalDelay;
  long double m_totalRemainingEnergy;
  uint64_t m_perPacketPerRouterWaitingTime;
  std::string m_scheduler;

  NodeContainer nodes;
  NetDeviceContainer devices;
  Ipv4InterfaceContainer interfaces;
  EnergySourceContainer sources;

private:
  void CreateNodes ();
  void CreateDevices ();
  void InstallInternetStack ();
  void InstallApplications ();
  void SetupMobility ();
  void InstallEnergyModels ();
  void UpdatePerPacketPerRouterWaitingTime (uint64_t waitingTime);
  void SetupTrace ();
  Ptr<Socket> SetupPacketReceive (Ipv4Address addr, Ptr<Node> node);
};