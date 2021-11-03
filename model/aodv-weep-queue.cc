/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <vector>
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/inet-socket-address.h"
#include "aodv-weep-queue.h"
#include "packet-scheduler-base.h"
#include "aodv-weep-routing-protocol.h"

namespace ns3 {
namespace weep {

TypeId
DataPacketQueueEntry::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::weep::DataPacketQueueEntry")
    .SetParent<PacketQueueEntry> ()
    .AddConstructor<DataPacketQueueEntry> ()
  ;
  return tid;
}

void
DataPacketQueueEntry::Send ()
{
  if (m_callbackType == UNICAST_FORWARD)
    {
      m_ucb (m_route, m_packet, m_header);
    }
  else if (m_callbackType == ERROR)
    {
      m_ecb (m_packet, m_header, m_socketErrno);
    }
}

TypeId
ControlPacketQueueEntry::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::weep::ControlPacketQueueEntry")
    .SetParent<PacketQueueEntry> ()
    .AddConstructor<ControlPacketQueueEntry> ()
  ;
  return tid;
}

void
ControlPacketQueueEntry::Send ()
{
  m_socket->SendTo (m_packet->Copy(), 0, InetSocketAddress(m_destination, 654));
}

} // namespace weep
} // namespace ns3
