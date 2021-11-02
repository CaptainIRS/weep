/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <vector>
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/inet-socket-address.h"
#include "aodv-weep-queue.h"
#include "aodv-weep-routing-protocol.h"

namespace ns3 {
namespace weep {

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

void
ControlPacketQueueEntry::Send ()
{
  m_socket->SendTo (m_packet, 0,
                    InetSocketAddress (m_destination, AodvWeepRoutingProtocol::AODV_WEEP_PORT));
}

} // namespace weep
} // namespace ns3
