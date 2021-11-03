/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef AODV_WEEP_QUEUE_H
#define AODV_WEEP_QUEUE_H

#include <vector>
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-routing-protocol.h"
#include "packet-scheduler-base.h"

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

  static TypeId GetTypeId (void);
  DataPacketQueueEntry() = default;
  /**
   * Constructor for unicast forward
   *
   * \param ucb the UnicastForwardCallback function
   * \param route the route to the destination
   * \param packet the packet to add to the queue
   * \param header the Ipv4Header
   */
  DataPacketQueueEntry (UnicastForwardCallback ucb, Ptr<Ipv4Route> route, Ptr<const Packet> packet,
                        Ipv4Header const &header)
      : PacketQueueEntry (packet),
        m_route (route),
        m_header (header),
        m_ucb (ucb),
        m_ecb (ErrorCallback ()),
        m_callbackType (UNICAST_FORWARD),
        m_socketErrno (Socket::ERROR_NOTERROR)
  {
  }

  /**
   * Constructor for error callback
   *
   * \param ecb the ErrorCallback function
   * \param packet the packet to add to the queue
   * \param header the Ipv4Header
   * \param socketErrorNo the socket error number
   */
  DataPacketQueueEntry (ErrorCallback ecb, Ptr<const Packet> packet, Ipv4Header const &header,
                        Socket::SocketErrno socketErrorNo)
      : PacketQueueEntry (packet),
        m_route (0),
        m_header (header),
        m_ucb (UnicastForwardCallback ()),
        m_ecb (ecb),
        m_callbackType (ERROR),
        m_socketErrno (socketErrorNo)
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
  static TypeId GetTypeId (void);
  ControlPacketQueueEntry() = default;
  ControlPacketQueueEntry (Ptr<const Packet> pa, Ipv4Address d,
                           Ptr<Socket> socket)
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
