/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef PACKET_SCHEDULER_BASE_H
#define PACKET_SCHEDULER_BASE_H

#include "ns3/ipv4-address.h"
#include "ns3/node.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
#include "ns3/packet.h"

namespace ns3 {

namespace weep {

/**
 * Abstract class for queue entry
 * 
 */
class PacketQueueEntry : public Object
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId
  GetTypeId ()
  {
    static TypeId tid =
        TypeId ("ns3::weep::PacketQueueEntry").SetParent<Object> ().SetGroupName ("AodvWeep");
    return tid;
  }
  /**
   * \brief Construct a new PacketQueueEntry
   */
  PacketQueueEntry () = default;
  PacketQueueEntry (Ptr<const Packet> packet) : m_packet (packet)
  {
  }

  /**
   * \brief Send the packet
   */
  virtual void Send () = 0;
  virtual Ptr<const Packet>
  GetPacket ()
  {
    return m_packet;
  }

protected:
  Ptr<const Packet> m_packet;
};

NS_OBJECT_ENSURE_REGISTERED (PacketQueueEntry);

/**
 * Abstract class for packet scheduler
 * 
 */
class PacketScheduler : public Object
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId
  GetTypeId ()
  {
    static TypeId tid =
        TypeId ("ns3::weep::PacketScheduler").SetParent<Object> ().SetGroupName ("AodvWeep");
    return tid;
  }
  PacketScheduler () = default;
  PacketScheduler (uint32_t maxLen, Time maxDelay);

  virtual bool Enqueue (Ptr<PacketQueueEntry> entry) = 0;
  virtual void SendPacket () = 0;
  void
  SetNodeAddress (Ipv4Address addr)
  {
    m_nodeAddress = addr;
  }

protected:
  uint32_t m_size;
  uint32_t m_maxSize;
  Time m_maxDelay;
  Ipv4Address m_nodeAddress;
};

NS_OBJECT_ENSURE_REGISTERED (PacketScheduler);

} // namespace weep

} // namespace ns3

#endif /* PACKET_SCHEDULER_BASE_H */
