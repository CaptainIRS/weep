/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef AODV_FCFS_SCHEDULER_H
#define AODV_FCFS_SCHEDULER_H

#include "ns3/random-variable-stream.h"
#include "packet-scheduler-base.h"

namespace ns3 {

namespace weep {

class AodvFcfsScheduler : public PacketScheduler
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  AodvFcfsScheduler ();
  AodvFcfsScheduler (uint32_t maxLen);

  bool Enqueue (Ptr<PacketQueueEntry> entry);

  void SendPacket ();

private:
  /// Packet queue
  std::vector<Ptr<PacketQueueEntry>> m_queue;
  /// Random variable generator to generate jitter
  Ptr<UniformRandomVariable> m_uniformRandomVariable;
  /// The maximum number of packets that we allow in the scheduler queue
  uint32_t m_maxLen;
};

NS_OBJECT_ENSURE_REGISTERED (AodvFcfsScheduler);

} // namespace weep

} // namespace ns3

#endif /* AODV_FCFS_SCHEDULER_H */
