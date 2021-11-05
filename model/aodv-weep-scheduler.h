/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef AODV_WEEP_SCHEDULER_H
#define AODV_WEEP_SCHEDULER_H

#include <vector>
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/packet.h"
#include "ns3/random-variable-stream.h"
#include "packet-scheduler-base.h"
#include "aodv-weep-queue.h"

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
    m_uniformRandomVariable = CreateObject<UniformRandomVariable> ();
  }
  AodvWeepScheduler (uint32_t maxLen, Time maxDelay)
  {
    m_size = maxLen;
    m_maxDelay = maxDelay;
  }

  bool Enqueue (Ptr<PacketQueueEntry> entry);
  void SendPacket ();

private:
  /// Calculate weight of a packet
  /// Product of
  /// 1. External priority (ep).
  /// 2. Estimated lifetime index (eli).
  /// 3. Fragility index (fi).
  /// 4. Path performance index (ppi).
  double CalculateWeight (Ptr<DataPacketQueueEntry> entry);
  /// Calculate Estimated lifetime index (eli)
  double CalculateEstimatedLifetimeIndex (Ptr<DataPacketQueueEntry> entry);
  /// Calculate Fragility index (fi)
  double CalculateFragilityIndex (Ptr<DataPacketQueueEntry> entry);
  /// Calculate Path performance index (ppi)
  double CalculatePathPerformanceIndex (Ptr<DataPacketQueueEntry> entry);
  /// Forward packets from level3 to level2
  void ForwardPackets ();
  /// Update node data from packet
  void UpdateNodeData (Ptr<PacketQueueEntry> entry);
  /// Packet queues
  std::vector<Ptr<PacketQueueEntry>> m_level1Queue;
  std::set<std::pair<double, Ptr<DataPacketQueueEntry>>> m_level2Queue, m_level3Queue;
  /// Random variable generator to generate jitter
  Ptr<UniformRandomVariable> m_uniformRandomVariable;
  /// Depletion rates
  std::map<Ipv4Address, double> m_depletionRates;
  /// Maximum energies
  std::map<Ipv4Address, double> m_maxEnergies;
  /// Current residual energies
  std::map<Ipv4Address, double> m_currentEnergies;
  /// Last update times
  std::map<Ipv4Address, uint64_t> m_lastUpdateTimes;
  /// Fragility indices
  std::map<std::string, double> m_fragilityIndices;
  /// Number of hops
  std::map<std::string, uint32_t> m_hops;
  /// Sessions in l2 queue
  std::set<std::string> m_sessionsInL2Queue;
  std::vector<Ptr<PacketQueueEntry>> m_queue;
};

NS_OBJECT_ENSURE_REGISTERED (AodvWeepScheduler);

} // namespace weep

} // namespace ns3

#endif /* AODV_WEEP_SCHEDULER_H */
