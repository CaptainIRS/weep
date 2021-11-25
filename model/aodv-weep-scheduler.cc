/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "aodv-weep-scheduler.h"
#include "ns3/aodv-weep-packet.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-header.h"
#include "ns3/log.h"
#include "ns3/node-list.h"
#include "ns3/ptr.h"
#include "packet-scheduler-base.h"
#include "packet-tags.h"
#include "ns3/simulator.h"
#include "aodv-queue.h"
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("AodvWeepScheduler");

namespace weep {

TypeId
AodvWeepScheduler::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::weep::AodvWeepScheduler")
                          .SetParent<PacketScheduler> ()
                          .SetGroupName ("AodvWeep")
                          .AddConstructor<AodvWeepScheduler> ();
  ;
  return tid;
}

bool
AodvWeepScheduler::Enqueue (Ptr<PacketQueueEntry> entry)
{
  if (auto dataPacketEntry = DynamicCast<DataPacketQueueEntry> (entry))
    {
      if (dataPacketEntry->IsLocalDelivery ())
        {
          UpdateNodeData (entry);
          return true;
        }
      auto session = dataPacketEntry->GetSessionId ();
      if (m_sessionsInL2Queue.find (session) == m_sessionsInL2Queue.end ())
        {
          m_sessionsInL2Queue.insert (session);
          m_level2Queue.push_back (
              std::make_pair (Simulator::Now ().GetNanoSeconds (), dataPacketEntry));
          if (m_level2Queue.size () > 2)
            {
              NS_LOG_DEBUG ("Size of level 2 queue: " << m_level2Queue.size ());
            }
        }
      else
        {
          auto weight = CalculateWeight (dataPacketEntry);
          m_level3Queue.insert (
              std::make_tuple (weight, Simulator::Now ().GetNanoSeconds (), dataPacketEntry));
          if (m_level3Queue.size () > 2)
            {
              NS_LOG_DEBUG ("Size of level 3 queue: " << m_level3Queue.size ());
            }
        }
    }
  else if (auto controlPacketEntry = DynamicCast<ControlPacketQueueEntry> (entry))
    {
      if (controlPacketEntry->IsLocalDelivery ())
        {
          UpdateNodeData (entry);
          return true;
        }
      m_level1Queue.push_back (std::make_pair (Simulator::Now ().GetNanoSeconds (), entry));
      if (m_level1Queue.size () > 2)
        {
          NS_LOG_DEBUG ("Size of level 1 queue: " << m_level1Queue.size ());
        }
    }
  auto jitter = m_uniformRandomVariable->GetValue ();
  Simulator::Schedule (MicroSeconds (jitter), &AodvWeepScheduler::SendPacket, this);
  return true;
}

void
AodvWeepScheduler::SendPacket ()
{
  if (!m_level1Queue.empty ())
    {
      auto packet = m_level1Queue.back ().second;
      auto time = m_level1Queue.back ().first;
      m_perPacketWaitingTimeTrace (Simulator::Now ().GetNanoSeconds () - time);
      m_level1Queue.pop_back ();
      packet->Send ();
      auto jitter = m_uniformRandomVariable->GetValue ();
      Simulator::Schedule (MicroSeconds (jitter), &AodvWeepScheduler::SendPacket, this);
      return;
    }
  auto lenq2 = m_level2Queue.size ();
  auto lenq3 = m_level3Queue.size ();
  auto packetsToBeForwarded = lenq2 != 0 ? lenq3 / lenq2 : lenq3;
  m_sessionsInL2Queue.clear ();
  ClearLevel2Queue ();
  ForwardPackets (packetsToBeForwarded);
}

void
AodvWeepScheduler::ClearLevel2Queue ()
{
  while (!m_level2Queue.empty ())
    {
      auto packet = m_level2Queue.begin ()->second;
      auto time = m_level2Queue.begin ()->first;
      m_perPacketWaitingTimeTrace (Simulator::Now ().GetNanoSeconds () - time);
      m_level2Queue.erase (m_level2Queue.begin ());
      packet->Send ();
    }
}

void
AodvWeepScheduler::ForwardPackets (uint32_t packetsToBeForwarded)
{
  if (packetsToBeForwarded <= 0)
    return;
  auto packet = std::get<2> (*m_level3Queue.begin ());
  auto entryTime = std::get<1> (*m_level3Queue.begin ());
  auto session = packet->GetSessionId ();
  uint32_t forwardedPackets = 0;
  while (forwardedPackets < packetsToBeForwarded)
    {
      if (m_sessionsInL2Queue.find (session) == m_sessionsInL2Queue.end ())
        {
          m_sessionsInL2Queue.insert (session);
          m_level2Queue.push_back (std::make_pair (entryTime, packet));
          m_level3Queue.erase (m_level3Queue.begin ());
        }
      forwardedPackets++;
    }
}

void
AodvWeepScheduler::UpdateNodeData (Ptr<PacketQueueEntry> entry)
{
  Ptr<const Packet> packet = entry->GetPacket ();
  Ipv4Address source, destination;

  SourceIPTag sourceIPTag;
  DestinationIPTag destinationIPTag;

  if (packet->PeekPacketTag (sourceIPTag))
    {
      source = sourceIPTag.GetSourceIP ();
    }
  if (packet->PeekPacketTag (destinationIPTag))
    {
      destination = destinationIPTag.GetDestinationIP ();
    }
  std::ostringstream oss;
  if (source < destination)
    {
      oss << source << "-" << destination;
    }
  else
    {
      oss << destination << "-" << source;
    }
  std::string session = oss.str ();

  MaximumEnergyTag maximumEnergyTag;
  if (packet->PeekPacketTag (maximumEnergyTag))
    {
      m_maxEnergies[source] = maximumEnergyTag.GetMaximumEnergy ();
    }

  TimestampTag timestampTag;
  if (packet->PeekPacketTag (timestampTag))
    {
      uint64_t timestamp = timestampTag.GetTimestamp ().GetNanoSeconds ();
      CurrentResidualEnergyTag currentResidualEnergyTag;
      if (packet->PeekPacketTag (currentResidualEnergyTag))
        {
          double currentResidualEnergy = currentResidualEnergyTag.GetCurrentResidualEnergy ();
          if (m_depletionRates.find (source) == m_depletionRates.end ())
            {
              m_depletionRates[source] = 5.0;
            }
          else
            {
              if ((m_currentEnergies[source] - currentResidualEnergy) > 0.0)
                {
                  m_depletionRates[source] = (m_currentEnergies[source] - currentResidualEnergy) *
                                             10e9 / (timestamp - m_lastUpdateTimes[source]);
                }
            }
          m_currentEnergies[source] = currentResidualEnergy;
        }
      m_lastUpdateTimes[source] = timestamp;
    }

  HopCountTag hopsCountTag;
  if (packet->PeekPacketTag (hopsCountTag))
    {
      if (m_hops.find (session) == m_hops.end ())
        {
          m_hops[session] = hopsCountTag.GetHopCount ();
          m_fragilityIndices[session] = 0;
        }
      else
        {
          if (m_hops[session] < hopsCountTag.GetHopCount ())
            {
              m_fragilityIndices[session] = hopsCountTag.GetHopCount () - m_hops[session];
              m_hops[session] = hopsCountTag.GetHopCount ();
            }
          else
            {
              m_fragilityIndices[session] = 0;
              m_hops[session] = hopsCountTag.GetHopCount ();
            }
        }
    }

  if (m_fragilityIndices[session] != 0)
    {
      NS_LOG_DEBUG ("Source: " << source << "; Destination: " << destination
                               << "; Session: " << session << "; Hop count: " << m_hops[session]
                               << "; Fragility index: " << m_fragilityIndices[session]
                               << "; Max energy: " << m_maxEnergies[source]
                               << "; Current energy: " << m_currentEnergies[source]
                               << "; Depletion rate: " << m_depletionRates[source]);
    }
}

double
AodvWeepScheduler::CalculateEstimatedLifetimeIndex (Ptr<DataPacketQueueEntry> entry)
{
  auto destination = entry->GetDestination ();
  if (m_currentEnergies.find (destination) == m_currentEnergies.end ())
    {
      return 1;
    }
  auto currentEnergy = m_currentEnergies[destination];
  auto depletionRate = m_depletionRates[destination];
  auto survivalTimeRemaining = (m_maxEnergies[destination] - currentEnergy) / depletionRate;
  return std::min (survivalTimeRemaining, 10.0) / 10.0;
}

double
AodvWeepScheduler::CalculatePathPerformanceIndex (Ptr<DataPacketQueueEntry> entry)
{
  std::string session = entry->GetSessionId ();

  if (m_hops.find (session) == m_hops.end ())
    {
      return 1;
    }
  else
    {
      return (1 - m_hops[session] / (float) NodeList::GetNNodes ());
    }
}

double
AodvWeepScheduler::CalculateFragilityIndex (Ptr<DataPacketQueueEntry> entry)
{
  std::string session = entry->GetSessionId ();
  return m_fragilityIndices[session];
}

double
AodvWeepScheduler::CalculateWeight (Ptr<DataPacketQueueEntry> entry)
{
  auto weight = 1.0 * CalculateEstimatedLifetimeIndex (entry) *
                CalculatePathPerformanceIndex (entry) * CalculateFragilityIndex (entry);
  if (weight > 0)
    NS_LOG_DEBUG ("Weight for packet from " << entry->GetSource () << " to "
                                            << entry->GetDestination () << " is " << weight);
  return weight;
}

} // namespace weep
} // namespace ns3
