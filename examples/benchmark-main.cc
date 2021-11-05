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
Benchmark::AodvFcfsScheduler ()
{
  WeepWifiSimulation test;
  double totalTime = 20.0;
  double range = 10.0;
  double dataStart = 1.0;
  std::vector<int> nodes = {3, 6, 9, 12};
  std::vector<int> speeds = {1, 1, 1, 1};

  for (int i = 0; i < 4; i++)
    {
      uint32_t nWifis = nodes[i];
      uint32_t nSinks = 2;
      std::cout << "Runnning AODV FCFS Schedueler for " << nWifis << " nodes" << std::endl;
      auto test = WeepWifiSimulation ();
      test.CaseRun ("ns3::weep::AodvFcfsScheduler", nWifis, nSinks, totalTime, range, speeds[i],
                    dataStart);
      std::cout << "Throughput: " << test.GetBytesReceived () / totalTime << " Bps" << std::endl;
      std::cout << "Packet Delivery Ratio: " << test.GetPacketsReceived () / test.GetPacketsSent ()
                << std::endl;
      throughputDataset.Add (nWifis, test.GetBytesReceived () / totalTime);
      packetDeliveryRatioDataset.Add (nWifis, test.GetPacketsReceived () / test.GetPacketsSent ());
    }
}

void
Benchmark::AodvWeepScheduler ()
{
  WeepWifiSimulation test;
  double totalTime = 20.0;
  double range = 10.0;
  double dataStart = 1.0;
  std::vector<int> nodes = {3, 6, 9, 12};
  std::vector<int> speeds = {1, 1, 1, 1};

  for (int i = 0; i < 4; i++)
    {
      uint32_t nWifis = nodes[i];
      uint32_t nSinks = 2;
      std::cout << "Runnning AODV Weep Scheduler for " << nWifis << " nodes" << std::endl;
      auto test = WeepWifiSimulation ();
      test.CaseRun ("ns3::weep::AodvWeepScheduler", nWifis, nSinks, totalTime, range, speeds[i],
                    dataStart);
      std::cout << "Throughput: " << test.GetBytesReceived () / totalTime << " Bps" << std::endl;
      std::cout << "Packet Delivery Ratio: " << test.GetPacketsReceived () / test.GetPacketsSent ()
                << std::endl;
      throughputDataset.Add (nWifis, test.GetBytesReceived () / totalTime);
      packetDeliveryRatioDataset.Add (nWifis, test.GetPacketsReceived () / test.GetPacketsSent ());
    }
}

int
main (int argc, char **argv)
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);

  SeedManager::SetSeed (12345);

  std::ofstream throughputFile ("throughput-aodv.plt");
  std::ofstream packetDeliveryRatioFile ("packet-delivery-ratio-aodv.plt");
  Gnuplot throughputPlot = Gnuplot ("throughput.png");
  Gnuplot packetDeliveryRatioPlot = Gnuplot ("pdr.png");

  Benchmark benchmark;

  benchmark.AodvWeepScheduler ();
  throughputPlot.AddDataset (benchmark.GetThroughputDataset ("AODV-WEEP"));
  packetDeliveryRatioPlot.AddDataset (benchmark.GetPacketDeliveryRatioDataset ("AODV-WEEP"));

  benchmark = Benchmark ();

  benchmark.AodvFcfsScheduler ();
  throughputPlot.AddDataset (benchmark.GetThroughputDataset ("AODV-FCFS"));
  packetDeliveryRatioPlot.AddDataset (benchmark.GetPacketDeliveryRatioDataset ("AODV-FCFS"));

  throughputPlot.SetTerminal ("png");
  throughputPlot.SetLegend ("Number of Nodes", "Throughput (Bytes/s)");
  throughputPlot.SetTitle ("Throughput vs Number of Nodes");
  throughputPlot.GenerateOutput (throughputFile);

  packetDeliveryRatioPlot.SetTerminal ("png");
  packetDeliveryRatioPlot.SetLegend ("Number of Nodes", "Packet Delivery Ratio");
  packetDeliveryRatioPlot.SetTitle ("Packet Delivery Ratio vs Number of Nodes");
  packetDeliveryRatioPlot.GenerateOutput (packetDeliveryRatioFile);

  return 0;
}