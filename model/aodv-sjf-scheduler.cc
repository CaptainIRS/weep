/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "aodv-sjf-scheduler.h"
#include "aodv-weep-queue.h"
#include "packet-tags.h"
#include "ns3/log.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "packet-scheduler-base.h"
#include "ns3/simulator.h"
#include <cstdint>
#include <utility>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("AodvSjfScheduler");

namespace weep {

AodvSjfScheduler::AodvSjfScheduler ()
{
  m_uniformRandomVariable = CreateObject<UniformRandomVariable> ();
}

AodvSjfScheduler::AodvSjfScheduler (uint32_t maxLen)
{
  m_uniformRandomVariable = CreateObject<UniformRandomVariable> ();
  m_maxLen = maxLen;
}

TypeId
AodvSjfScheduler::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::weep::AodvSjfScheduler")
                          .SetParent<PacketScheduler> ()
                          .SetGroupName ("AodvWeep")
                          .AddConstructor<AodvSjfScheduler> ();
  return tid;
}

bool
AodvSjfScheduler::Enqueue (Ptr<PacketQueueEntry> entry)
{
  HopCountTag hopsCountTag;
  auto packet = entry->GetPacket ();
  uint32_t hops = 0;
  if (packet->PeekPacketTag (hopsCountTag))
    {
      hops = hopsCountTag.GetHopCount ();
    }
  m_queue.push (std::make_tuple (hops, Simulator::Now ().GetNanoSeconds (), entry));
  Simulator::Schedule (Seconds (0), &AodvSjfScheduler::SendPacket, this);
  return true;
}

void
AodvSjfScheduler::SendPacket ()
{
  if (!m_queue.empty ())
    {
      auto entry = m_queue.top ();
      auto time = std::get<1> (entry);
      auto packet = std::get<2> (entry);
      m_perPacketWaitingTimeTrace (Simulator::Now ().GetNanoSeconds () - time);
      packet->Send ();
      m_queue.pop ();
      Simulator::Schedule (Seconds (0), &AodvSjfScheduler::SendPacket, this);
    }
}

} // namespace weep
} // namespace ns3
