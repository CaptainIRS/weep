#include <cstdint>
#include <fstream>
#include <iostream>
#include <cmath>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/config-store-module.h"

#include "ns3/vector.h"
#include "weep-wifi-sim.h"

int
main (int argc, char **argv)
{
  double totalTime = 100.0;
  double range = 10.0;
  double dataStart = 1.0;
  uint32_t nodes = 20;
  uint32_t sinks = 19;
  uint32_t speed = 5;
  std::string scheduler = "ns3::weep::AodvWeepScheduler";
  std::string fileName = "weep.csv";

  CommandLine cmd (__FILE__);
  cmd.AddValue ("TotalTime", "Total time of simulation", totalTime);
  cmd.AddValue ("Range", "Communication range", range);
  cmd.AddValue ("DataStart", "Time to start data transmission", dataStart);
  cmd.AddValue ("Nodes", "Number of nodes", nodes);
  cmd.AddValue ("Sinks", "Number of sinks", sinks);
  cmd.AddValue ("Speed", "Speed of nodes", speed);
  cmd.AddValue ("Scheduler", "Scheduler to use", scheduler);
  cmd.AddValue ("FileName", "File name to save data", fileName);

  cmd.Parse (argc, argv);

  SeedManager::SetSeed (12345);

  auto test = WeepWifiSimulation ();
  test.CaseRun (scheduler, nodes, sinks, totalTime, range, speed, dataStart);
  std::fstream out (fileName, std::fstream::out | std::fstream::app);
  out << nodes << ", " << test.GetThroughput () << ", " << test.GetPDR () << ", "
      << test.GetPerPacketPerRouterWaitingTime () << ", " << test.GetAverageDelay () << ", "
      << test.GetAverageEnergyConsumption () << "\n";
  out.close ();

  return 0;
}