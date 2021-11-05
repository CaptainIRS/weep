#include <iostream>

#include "ns3/core-module.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/log.h"
#include "ns3/network-module.h"
#include "ns3/netanim-module.h"
#include "ns3/internet-module.h"
#include "ns3/node-container.h"
#include "ns3/packet.h"
#include "ns3/applications-module.h"
#include "ns3/gnuplot.h"

#include "benchmark.h"

Benchmark::Benchmark ()
{
  throughputDataset.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  packetDeliveryRatioDataset.SetStyle (Gnuplot2dDataset::LINES_POINTS);
}

Gnuplot2dDataset
Benchmark::GetThroughputDataset (std::string title)
{
  throughputDataset.SetTitle (title);
  return throughputDataset;
}

Gnuplot2dDataset
Benchmark::GetPacketDeliveryRatioDataset (std::string title)
{
  packetDeliveryRatioDataset.SetTitle (title);
  return packetDeliveryRatioDataset;
}
