#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/core-module.h"

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
   * \brief Construct a new PacketQueueEntry
   */
  PacketQueueEntry (Ptr<Packet> packet);

  /**
   * \brief Send the packet
   */
  virtual void Send () = 0;

protected:
  Ptr<Packet> m_packet;
};

/**
 * Abstract class for packet scheduler
 * 
 */
class PacketScheduler : public Object
{
public:
  PacketScheduler ();
  PacketScheduler (uint32_t maxLen, Time maxDelay);

  virtual bool Enqueue (PacketQueueEntry &entry) = 0;
  virtual void SendPacket () = 0;

protected:
  uint32_t m_size;
  uint32_t m_maxSize;
  Time m_maxDelay;
};

} // namespace weep

} // namespace ns3