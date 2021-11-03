/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef AODV_WEEP_SCHEDULER_H
#define AODV_WEEP_SCHEDULER_H

#include <vector>
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/random-variable-stream.h"
#include "packet-scheduler-base.h"


namespace ns3 {

namespace weep {

class AodvWeepScheduler : public PacketScheduler
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  AodvWeepScheduler ()
  {
    
  }
  AodvWeepScheduler (uint32_t maxLen, Time maxDelay)
  {
    m_size = maxLen;
    m_maxDelay = maxDelay;
  }

  bool Enqueue (Ptr<PacketQueueEntry> entry);

  void SendPacket ();

private:
  /// Packet queue
  std::vector<Ptr<PacketQueueEntry>> m_sendBuffer;
  /// Random variable generator to generate jitter
  Ptr<UniformRandomVariable> m_uniformRandomVariable;
};

} // namespace weep

} // namespace ns3

#endif /* AODV_WEEP_SCHEDULER_H */
