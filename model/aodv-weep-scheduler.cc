/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "aodv-weep-scheduler.h"
#include "packet-scheduler-base.h"
#include "ns3/simulator.h"

namespace ns3 {
namespace weep {

TypeId
AodvWeepScheduler::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::weep::AodvWeepScheduler")
    .SetParent<PacketScheduler> ()
    .SetGroupName ("AodvWeep")
    .AddConstructor<AodvWeepScheduler> ()
  ;
  return tid;
}

bool
AodvWeepScheduler::Enqueue (Ptr<PacketQueueEntry> entry)
{
  if (m_sendBuffer.size () > m_maxSize)
    {
      return false;
    }
  m_sendBuffer.push_back (entry);
  Simulator::Schedule (MicroSeconds (m_uniformRandomVariable->GetInteger (0, 100)),
                       &AodvWeepScheduler::SendPacket, this);
  return true;
}

void
AodvWeepScheduler::SendPacket ()
{
  if (!m_sendBuffer.empty ())
    {
      Ptr<PacketQueueEntry> packet = m_sendBuffer.back ();
      m_sendBuffer.pop_back ();
      packet->Send ();
      Simulator::Schedule (MicroSeconds (m_uniformRandomVariable->GetInteger (0, 100)),
                           &AodvWeepScheduler::SendPacket, this);
    }
}

} // namespace weep
} // namespace ns3
