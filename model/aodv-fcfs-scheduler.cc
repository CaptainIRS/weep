/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "aodv-fcfs-scheduler.h"
#include "aodv-weep-queue.h"
#include "ns3/log.h"
#include "ns3/ptr.h"
#include "packet-scheduler-base.h"
#include "ns3/simulator.h"

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
  m_queue.push_back (entry);
  Simulator::Schedule (MicroSeconds (m_uniformRandomVariable->GetInteger (0, 100)),
                       &AodvFcfsScheduler::SendPacket, this);
  return true;
}

void
AodvFcfsScheduler::SendPacket ()
{
  if (!m_queue.empty ())
    {
      Ptr<PacketQueueEntry> packet = m_queue.back ();
      packet->Send ();
      m_queue.pop_back ();
      Simulator::Schedule (MicroSeconds (m_uniformRandomVariable->GetInteger (0, 100)),
                           &AodvFcfsScheduler::SendPacket, this);
    }
}

} // namespace weep
} // namespace ns3
