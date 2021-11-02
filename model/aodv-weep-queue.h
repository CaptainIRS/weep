/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef AODV_WEEP_QUEUE_H
#define AODV_WEEP_QUEUE_H

#include <vector>
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-routing-protocol.h"
#include "scheduler.h"

namespace ns3 {
namespace weep {

enum CallbackType { UNICAST_FORWARD, ERROR };

class DataPacketQueueEntry : public PacketQueueEntry
{
public:
  /// IPv4 routing unicast forward callback typedef
  typedef Ipv4RoutingProtocol::UnicastForwardCallback UnicastForwardCallback;
  /// IPv4 routing error callback typedef
  typedef Ipv4RoutingProtocol::ErrorCallback ErrorCallback;
  /**
   * constructor
   *
   * \param pa the packet to add to the queue
   * \param h the Ipv4Header
   * \param ucb the UnicastForwardCallback function
   * \param ecb the ErrorCallback function
   * \param exp the expiration time
   */
  DataPacketQueueEntry (Ptr<const Ipv4Route> route = 0, Ptr<const Packet> pa = 0,
                        Ipv4Header const &h = Ipv4Header (),
                        UnicastForwardCallback ucb = UnicastForwardCallback (),
                        ErrorCallback ecb = ErrorCallback (), CallbackType ct = UNICAST_FORWARD,
                        Socket::SocketErrno err = Socket::ERROR_NOTERROR)
      : PacketQueueEntry (pa),
        m_route (route),
        m_header (h),
        m_ucb (ucb),
        m_ecb (ecb),
        m_callbackType (ct),
        m_socketErrno (err)
  {
  }

  /**
   * \brief Send the packet
   * 
   */
  void Send (void);

private:
  Ptr<Ipv4Route> m_route;
  /// IP header
  Ipv4Header m_header;
  /// Unicast forward callback
  UnicastForwardCallback m_ucb;
  /// Error callback
  ErrorCallback m_ecb;
  /// Callback type
  CallbackType m_callbackType;
  /// Socket error no
  Socket::SocketErrno m_socketErrno;
};

class ControlPacketQueueEntry : public PacketQueueEntry
{
public:
  ControlPacketQueueEntry (Ptr<const Packet> pa = 0, Ipv4Address const &d = Ipv4Address (),
                           Ptr<Socket> socket = 0)
      : PacketQueueEntry (pa), m_destination (d), m_socket (socket)
  {
  }
  /**
   * \brief Send the packet
   * 
   */
  void Send (void);

private:
  /// Destination Ipv4 address
  Ipv4Address m_destination;
  /// Socket
  Ptr<Socket> m_socket;
};

} // namespace weep
} // namespace ns3

#endif /* AODV_WEEP_QUEUE_H */
