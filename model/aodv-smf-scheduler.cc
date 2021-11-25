/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "aodv-smf-scheduler.h"
#include "aodv-queue.h"
#include "ns3/log.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "packet-scheduler-base.h"
#include "ns3/simulator.h"
#include <utility>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("AodvSmfScheduler");

namespace weep {

AodvSmfScheduler::AodvSmfScheduler ()
{
  m_uniformRandomVariable = CreateObject<UniformRandomVariable> ();
}

AodvSmfScheduler::AodvSmfScheduler (uint32_t maxLen)
{
  m_uniformRandomVariable = CreateObject<UniformRandomVariable> ();
  m_maxLen = maxLen;
}

TypeId
AodvSmfScheduler::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::weep::AodvSmfScheduler")
                          .SetParent<PacketScheduler> ()
                          .SetGroupName ("AodvWeep")
                          .AddConstructor<AodvSmfScheduler> ();
  return tid;
}

bool
AodvSmfScheduler::Enqueue (Ptr<PacketQueueEntry> entry)
{
  auto aodvEntry = DynamicCast<BaseAodvQueueEntry> (entry);
  if (aodvEntry->IsLocalDelivery())
    {
      NS_LOG_DEBUG ("Local delivery, not enqueued");
      return true;
    }
  auto packet = entry->GetPacket ();
  auto size = packet->GetSize();
  m_queue.push (std::make_tuple (size, Simulator::Now ().GetNanoSeconds (), entry));
  Simulator::Schedule (Seconds (0), &AodvSmfScheduler::SendPacket, this);
  return true;
}

void
AodvSmfScheduler::SendPacket ()
{
  if (!m_queue.empty ())
    {
      auto entry = m_queue.top ();
      auto time = std::get<1> (entry);
      auto packet = std::get<2> (entry);
      m_perPacketWaitingTimeTrace (Simulator::Now ().GetNanoSeconds () - time);
      packet->Send ();
      m_queue.pop ();
      Simulator::Schedule (Seconds (0), &AodvSmfScheduler::SendPacket, this);
    }
}

} // namespace weep
} // namespace ns3
