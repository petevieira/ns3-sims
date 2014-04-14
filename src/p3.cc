/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 * Author: Aderinola Gbade-Alabi <aagbade@gmail.com>
 *         Jared S. Ivey         <jivey@gatech.edu>
 *         Drew Petry            <drew.petry@gatech.edu>
 *         Peter Vieira          <pete.vieira@gmail.com>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <math.h>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/aodv-helper.h"
#include "ns3/dsr-helper.h"
#include "ns3/olsr-helper.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/stats-module.h"

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("p3");

#define IEEE_80211_BANDWIDTH 20000000

int main (int argc, char *argv[])
{
  SeedManager::SetSeed(1);

  bool     reporting = false;
  uint16_t runValue  = 0;
  uint16_t numNodes  = 20;
  uint16_t pktSize   = 256;
  string   posSize   = "1000";
  double   intensity = 0.1;
  string   routeProt = "AODV";
  double   txPower   = 0.5;
  double   onTime    = 0.01;
  double   offTime   = 0.01;
  double   stopTime  = 10;

  CommandLine cmd;
  cmd.AddValue ("reporting", "Print reports of simulated data",           reporting);
  cmd.AddValue ("runValue",  "Random seed run",                           runValue);
  cmd.AddValue ("numNodes",  "Number of nodes in simulation",             numNodes);
  cmd.AddValue ("pktSize",   "Size of packets sent",                      pktSize);
  cmd.AddValue ("posSize",   "Size of simulation field in meters",        posSize);
  cmd.AddValue ("intensity", "Traffic intensity (%)",                     intensity);
  cmd.AddValue ("routeProt", "Routing protocol to use: AODV, OLSR",       routeProt);
  cmd.AddValue ("txPower",   "Transmission power range in Watts",         txPower);
  cmd.AddValue ("onTime",    "Time ON for OnOffApplication in seconds",   onTime);
  cmd.AddValue ("offTime",    "Time OFF for OnOffApplication in seconds", offTime);
  cmd.AddValue ("stopTime",   "Simulation time in seconds",               stopTime);

  cmd.Parse (argc,argv);

  SeedManager::SetRun(runValue);

  Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue(pktSize));

  ostringstream onTimeStr;
  onTimeStr << "ns3::ConstantRandomVariable[Constant=" << onTime << "]";
  Config::SetDefault ("ns3::OnOffApplication::OnTime",
                      StringValue(onTimeStr.str()));

  ostringstream offTimeStr;
  offTimeStr << "ns3::ConstantRandomVariable[Constant=" << offTime << "]";
  Config::SetDefault ("ns3::OnOffApplication::OffTime",
                      StringValue(offTimeStr.str()));
  
  if (!(routeProt.compare("AODV") == 0) &&  
      !(routeProt.compare("OLSR") == 0))
    {
      NS_LOG_DEBUG ("Invalid routing protocol");
      exit (1);
    }

  // Convert power to dBm.
  txPower = 10 * log (txPower / 0.001) / log (10);

  // Create nodes.
  NS_LOG_INFO ("Create nodes.");
  NodeContainer nodes;
  nodes.Create(numNodes);

  // Create random square based on provided size.
  MobilityHelper mobility;
  string posMax = "ns3::UniformRandomVariable[Min=0.0|Max=" + posSize + "]";
  mobility.SetPositionAllocator ("ns3::RandomRectanglePositionAllocator",
                                 "X", StringValue (posMax),
                                 "Y", StringValue (posMax));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);

  // Set the Wifi MAC as adhoc.
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifiMac.SetType ("ns3::AdhocWifiMac");

  // Set Wifi PHY transmission power range (equal so no fluctuation).
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  wifiPhy.Set ("TxPowerStart", DoubleValue(txPower));
  wifiPhy.Set ("TxPowerEnd", DoubleValue(txPower));

  // Set Wifi channel.
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());

  // Set Wifi to 802.11b and use a DSSS rate of 1Mbps.
  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue("DsssRate1Mbps"),
                                "ControlMode", StringValue("DsssRate1Mbps"));

  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, nodes);

  InternetStackHelper stack;
  
  if (routeProt.compare("AODV") == 0)
    {
      AodvHelper aodv;
      stack.SetRoutingHelper (aodv);

      if (reporting)
        {
          // Trace routing tables
          Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("p3.routes", ios::out);
          aodv.PrintRoutingTableAllEvery (Seconds (2), routingStream);
        }
    }
  else if (routeProt.compare("OLSR") == 0)
    {
      OlsrHelper olsr;
      stack.SetRoutingHelper (olsr);

      if (reporting)
        {
          // Trace routing tables
          Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("p3.routes", ios::out);
          olsr.PrintRoutingTableAllEvery (Seconds (2), routingStream);
        }
    }

  stack.Install (nodes);

  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.0.0.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = ipv4.Assign (devices);
  
  NS_LOG_INFO ("Create Applications.");
  UniformVariable uv (0, numNodes - 1);
  map<uint32_t, uint32_t> nodePairs;
  for (uint32_t i=0; i<numNodes; ++i)
    {
      // This node index has not been paired.
      if (nodePairs.find (i) == nodePairs.end ())
        {
          // The random index to pair is not the same as the current index, and
          // the random index has not already been paired.
          uint32_t pairNode = 0;
          map<uint32_t, uint32_t>::iterator iter;
          do
            {
              pairNode = uv.GetInteger (0, numNodes - 1);
              iter = nodePairs.find (pairNode);
            }
          while ((i == pairNode) || (iter != nodePairs.end ()));

          nodePairs.insert (make_pair (i, pairNode));
          nodePairs.insert (make_pair (pairNode, i));
        }
    }

  // Determine the data rate to be used based on the intensity and network capacity.
  Ptr<WifiNetDevice> wifiDevice = DynamicCast<WifiNetDevice> (devices.Get(0)); 
  uint64_t networkCap = IEEE_80211_BANDWIDTH * 
             log (1 + wifiDevice->GetPhy()->CalculateSnr (wifiDevice->GetPhy()->GetMode(0), 0.1)) / log (2);
 
  uint64_t dataRate  = (uint64_t)(networkCap * intensity) / ((onTime + offTime) * numNodes);

  // Create OnOffApplications and PacketSinks for each node such that each node sends data to
  // and receives data from its pair.
  ApplicationContainer sourceApps;
  ApplicationContainer sinkApps;
  ExponentialVariable ev (0.1);
  for (uint32_t i=0; i<numNodes; ++i)
    {
      uint16_t port = 50000 + i;
      Ptr<Node> appSource = nodes.Get(nodePairs.find(i)->first);
      Ptr<Node> appSink = nodes.Get(nodePairs.find(i)->second);
      Ipv4Address remoteAddr = appSink->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();

      OnOffHelper onoff ("ns3::UdpSocketFactory", 
                         Address (InetSocketAddress (remoteAddr, port)));
      onoff.SetAttribute ("DataRate", DataRateValue (DataRate (dataRate)));

      ApplicationContainer tempApp (onoff.Install (appSource));
      tempApp.Start(Seconds(ev.GetValue()));
      sourceApps.Add(tempApp);

      PacketSinkHelper sink ("ns3::UdpSocketFactory", 
                             InetSocketAddress (Ipv4Address::GetAny (), port));
      sinkApps.Add(sink.Install (appSink));
    }
  sourceApps.Stop (Seconds (stopTime));
  sinkApps.Start (Seconds (0.0));

  // Flow Monitor
  Ptr<FlowMonitor> flowmon;
  FlowMonitorHelper flowmonHelper;
  flowmon = flowmonHelper.InstallAll ();
  
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop(Seconds(stopTime));
  Simulator::Run ();
  NS_LOG_INFO ("Done.");

  if (reporting)
    {
      flowmon->SerializeToXmlFile ("p3.flowmon", false, false);
    }

  // Results output
  // Results will display as:
  // "GridSize  NumberOfNodes   TxPowerRange    Intensity       RouteProtocol   StopTime        TotalPktsRx     TotalPktsTx     Efficiency(%)"
  //
  // Other variables of interest may be network capacity and resultant data rate.
  cout << posSize << "\t" << numNodes << "\t" << 0.001*pow(10, txPower/10) << "\t" 
       << intensity << "\t" << routeProt << "\t" << stopTime << "\t";
  
  uint32_t totRx = 0;
  uint32_t totTx = 0;
  for (uint32_t i=0; i < numNodes; ++i)
  {
      Ptr<PacketSink> sink1 = DynamicCast<PacketSink> (sinkApps.Get (i));
      totRx += sink1->GetTotalRx ();

      Ptr<OnOffApplication> onoff1 = DynamicCast<OnOffApplication> (sourceApps.Get (i));
      totTx += onoff1->GetTotalTx ();
  }
  cout << (totRx / pktSize) << "\t" << totTx / pktSize << "\t" << (100 * (double)totRx / (double)totTx) << endl;

  Simulator::Destroy ();
  return 0;
}
