#include <cstdint>
#include <iostream>

#include "ns3/core-module.h"
#include "ns3/ipv4-header.h"
#include "ns3/log.h"
#include "ns3/network-module.h"
#include "ns3/netanim-module.h"
#include "ns3/internet-module.h"
#include "ns3/node-container.h"
#include "ns3/packet.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/gnuplot.h"
using namespace ns3;
class Benchmark
{
  Gnuplot2dDataset throughputDataset, packetDeliveryRatioDataset;

public:
  Benchmark ();
  void AodvFcfsScheduler ();
  void AodvWeepScheduler ();
  void AodvSjfScheduler ();
  void AodvSmfScheduler ();

  Gnuplot2dDataset GetThroughputDataset (std::string title);
  Gnuplot2dDataset GetPacketDeliveryRatioDataset (std::string title);
};