/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef AODV_WEEP_QUEUE_H
#define AODV_WEEP_QUEUE_H

#include <cstdint>
#include <string>
#include <vector>
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/packet.h"
#include "packet-scheduler-base.h"

namespace ns3 {
namespace weep {

enum CallbackType { UNICAST_FORWARD, ERROR, LOCAL_DELIVERY, NONE };

class BaseWeepQueueEntry : public PacketQueueEntry
{
public:
  /// IPv4 routing unicast forward callback typedef
  typedef Ipv4RoutingProtocol::UnicastForwardCallback UnicastForwardCallback;
  /// IPv4 routing error callback typedef
  typedef Ipv4RoutingProtocol::ErrorCallback ErrorCallback;
  /// IPv4 local interface callback typedef
  typedef Ipv4RoutingProtocol::LocalDeliverCallback LocalDeliverCallback;

  static TypeId GetTypeId (void);
  BaseWeepQueueEntry () = default;
  /**
   * Constructor for unicast forward
   *
   * \param ucb the UnicastForwardCallback function
   * \param route the route to the destination
   * \param packet the packet to add to the queue
   * \param header the Ipv4Header
   */
  BaseWeepQueueEntry (UnicastForwardCallback ucb, Ptr<Ipv4Route> route, Ptr<const Packet> packet,
                      Ipv4Header const &header)
      : PacketQueueEntry (packet),
        m_route (route),
        m_header (header),
        m_ucb (ucb),
        m_callbackType (UNICAST_FORWARD)
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
  BaseWeepQueueEntry (ErrorCallback ecb, Ptr<const Packet> packet, Ipv4Header const &header,
                      Socket::SocketErrno socketErrorNo)
      : PacketQueueEntry (packet),
        m_header (header),
        m_ecb (ecb),
        m_callbackType (ERROR),
        m_socketErrno (socketErrorNo)
  {
  }

  /**
   * Constructor for local delivery callback
   *
   * \param lcb the local function
   * \param packet the packet to add to the queue
   * \param header the Ipv4Header
   * \param interface the interface index
   */
  BaseWeepQueueEntry (LocalDeliverCallback lcb, Ptr<const Packet> packet, Ipv4Header const &header,
                      uint32_t interface)
      : PacketQueueEntry (packet),
        m_header (header),
        m_lcb (lcb),
        m_callbackType (LOCAL_DELIVERY),
        m_ifIndex (interface)
  {
    lcb (packet, header, interface);
  }

  BaseWeepQueueEntry (Ptr<const Packet> pa, Ipv4Address d, Ptr<Socket> socket)
      : PacketQueueEntry (pa), m_callbackType (NONE), m_destination (d), m_socket (socket)
  {
  }

  /**
   * \brief Parse headers 
   * 
   */
  void ParseHeaders (void);

  /**
   * \brief Get the destination address
   * 
   */
  Ipv4Address
  GetDestination (void)
  {
    ParseHeaders ();
    return m_destination;
  }

  /**
   * \brief Get the source address
   * 
   */
  Ipv4Address
  GetSource (void)
  {
    ParseHeaders ();
    return m_source;
  }

  /**
   * \brief Get session id 
   * 
   */
  std::string
  GetSessionId (void)
  {
    auto source = GetSource ();
    auto destination = GetDestination ();
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
    return session;
  }
  /**
   * \brief Send the packet
   * 
   */
  void Send (void);

  /**
   * \brief Check if the packet is a local delivery callback
   * 
   */
  bool
  IsLocalDelivery (void)
  {
    return m_callbackType == LOCAL_DELIVERY;
  };

  /**
   * \brief Get header
   * 
   */
  Ipv4Header const
  GetHeader (void)
  {
    return m_header;
  };
  std::string
  GetCallbackType (void)
  {
    return m_callbackType == UNICAST_FORWARD ? "UNICAST_FORWARD"
           : m_callbackType == ERROR         ? "ERROR"
                                             : "LOCAL_DELIVERY";
  };

protected:
  Ptr<Ipv4Route> m_route;
  /// IP header
  Ipv4Header m_header;
  /// Unicast forward callback
  UnicastForwardCallback m_ucb;
  /// Error callback
  ErrorCallback m_ecb;
  /// Local delivery callback
  LocalDeliverCallback m_lcb;
  /// Callback type
  CallbackType m_callbackType;
  /// Socket error no
  Socket::SocketErrno m_socketErrno;
  /// Interface index
  uint32_t m_ifIndex;
  /// Destination Ipv4 address
  Ipv4Address m_source, m_destination;
  /// Socket
  Ptr<Socket> m_socket;
};

class DataPacketQueueEntry : public BaseWeepQueueEntry
{
public:
  static TypeId GetTypeId (void);
  using BaseWeepQueueEntry::BaseWeepQueueEntry;
};

class ControlPacketQueueEntry : public BaseWeepQueueEntry
{
public:
  static TypeId GetTypeId (void);
  using BaseWeepQueueEntry::BaseWeepQueueEntry;
};

} // namespace weep
} // namespace ns3

#endif /* AODV_WEEP_QUEUE_H */
