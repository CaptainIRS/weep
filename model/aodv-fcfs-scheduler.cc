/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "aodv-fcfs-scheduler.h"
#include "ns3/log.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "packet-scheduler-base.h"
#include "aodv-queue.h"
#include "ns3/simulator.h"
#include <utility>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("AodvFcfsScheduler");

namespace weep {

AodvFcfsScheduler::AodvFcfsScheduler ()
{
  m_uniformRandomVariable = CreateObject<UniformRandomVariable> ();
}

AodvFcfsScheduler::AodvFcfsScheduler (uint32_t maxLen)
{
  m_uniformRandomVariable = CreateObject<UniformRandomVariable> ();
  m_maxLen = maxLen;
}

TypeId
AodvFcfsScheduler::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::weep::AodvFcfsScheduler")
                          .SetParent<PacketScheduler> ()
                          .SetGroupName ("AodvWeep")
                          .AddConstructor<AodvFcfsScheduler> ();
  return tid;
}

bool
AodvFcfsScheduler::Enqueue (Ptr<PacketQueueEntry> entry)
{
  auto aodvEntry = DynamicCast<BaseAodvQueueEntry> (entry);
  if (aodvEntry->IsLocalDelivery())
    {
      NS_LOG_DEBUG ("Local delivery, not enqueued");
      return true;
    }
  m_queue.push_back (std::make_pair (Simulator::Now ().GetNanoSeconds (), entry));
  Simulator::Schedule (Seconds (0), &AodvFcfsScheduler::SendPacket, this);
  return true;
}

void
AodvFcfsScheduler::SendPacket ()
{
  if (!m_queue.empty ())
    {
      auto entry = m_queue.back ();
      auto time = entry.first;
      auto packet = entry.second;
      m_perPacketWaitingTimeTrace (Simulator::Now ().GetNanoSeconds () - time);
      packet->Send ();
      m_queue.pop_back ();
      Simulator::Schedule (Seconds (0), &AodvFcfsScheduler::SendPacket, this);
    }
}

} // namespace weep
} // namespace ns3
