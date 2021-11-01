#include <cstdint>
#include <fstream>
#include <iostream>
#include <cmath>
#include "ns3/core-module.h"
#include "ns3/gnuplot.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/config-store-module.h"

#include "benchmark.h"
#include "ns3/vector.h"
#include "weep-wifi-sim.h"

void
Benchmark::AodvFcfsQueue()
{
  WeepWifiSimulation test;
  double totalTime = 100.0;
  double range = 10.0;
  double dataStart = 1.0;
  std::vector<int> nodes = {20, 50, 100, 150, 200};
  std::vector<int> speeds = {5, 10, 25, 35, 50};

  for (int i = 0; i < 1; i++) {
    uint32_t nWifis = nodes[i];
    uint32_t nSinks = 1;
    std::cout << "Runnning AODV FCFS Queue for " << nWifis << " nodes" << std::endl;
    auto test = WeepWifiSimulation ();
    test.CaseRun (nWifis, nSinks, totalTime, range, speeds[i], dataStart);
    std::cout << "Throughput: " << test.GetBytesReceived() / totalTime << " Bps" << std::endl;
    std::cout << "Packet Delivery Ratio: " << test.GetPacketsReceived() / test.GetPacketsSent() << std::endl;
    throughputDataset.Add(nWifis, test.GetBytesReceived() / totalTime);
    packetDeliveryRatioDataset.Add(nWifis, test.GetPacketsReceived() / test.GetPacketsSent());
  }

}

int main (int argc, char **argv)
{

  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);

  SeedManager::SetSeed (12345);

  std::ofstream throughputFile ("throughput-aodv.plt");
  std::ofstream packetDeliveryRatioFile ("packet-delivery-ratio-aodv.plt");
  Gnuplot throughputPlot = Gnuplot ("Throughput vs Number of Nodes");
  Gnuplot packetDeliveryRatioPlot = Gnuplot ("Packet Delivery Ratio vs Number of Nodes");

  Benchmark benchmark;

  benchmark.AodvFcfsQueue();
  throughputPlot.AddDataset (benchmark.GetThroughputDataset());
  packetDeliveryRatioPlot.AddDataset (benchmark.GetPacketDeliveryRatioDataset());
  
  
  throughputPlot.SetTerminal ("png");
  throughputPlot.SetLegend ("Number of Nodes", "Throughput (Bytes/s)");
  throughputPlot.SetTitle ("Throughput vs Number of Nodes");
  throughputPlot.GenerateOutput(throughputFile);

  packetDeliveryRatioPlot.SetTerminal ("png");
  packetDeliveryRatioPlot.SetLegend ("Number of Nodes", "Packet Delivery Ratio");
  packetDeliveryRatioPlot.SetTitle ("Packet Delivery Ratio vs Number of Nodes");
  packetDeliveryRatioPlot.GenerateOutput(packetDeliveryRatioFile);

  return 0;
}