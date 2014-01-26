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
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/uinteger.h"
#include "ns3/animation-interface.h"
#include "ns3/wifi-net-device.h"
#include "ns3/point-to-point-dumbbell.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Project_01-TCP_Throughput_Measurments");

// Network topology (TCP/IP Protocol)
//
//                                   q1
//
// Node:            n0 ------------- n1 ------------ n2 ------------- n3
// Bandwidth:             5 Mbps           1 Mbps           5 Mbps
// Delay:                 10 ms            20 ms            10 ms
//
// - Flow from n0 to n3 using BulkSendApplication
// - Receipt of bulk send at n3 using PacketSinkApplication
// - Output trace file to p1.tr

int
main (int argc, char *argv[])
{
  Time::SetResolution (Time::NS);
  LogComponentEnable ("BulkSendApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("PacketSink", LOG_LEVEL_INFO);
 
  // Set default values for simulation variables
  std::string tcpType = "TcpTahoe";
  bool tracing = false;
  uint32_t numLtNodes = 1;
  uint32_t numRtNodes = 1;
  uint32_t winSize    = 2000;
  uint32_t queueSize  = 2000;
  uint32_t segSize    = 512;
  uint32_t maxBytes   = 1000000;

  // Parse command line arguments
  CommandLine cmd;
  cmd.AddValue ("tcpType",    "TCP type (use TcpReno or TcpTahoe)",      tcpType);
  cmd.AddValue ("tracing",    "Flag to enable/disable tracing",          tracing);
  cmd.AddValue ("winSize",    "Receiver advertised window size (bytes)", winSize);
  cmd.AddValue ("queueSize",  "Queue limit on the bottleneck link",      queueSize);
  cmd.AddValue ("segSize",    "TCP segment size",                        segSize);
  cmd.AddValue ("maxBytes",   "Max bytes soure will send",               maxBytes);
  cmd.Parse(argc, argv);

  // Set default values
  // Config::SetDefault ("ns3::DropTailQueue::Mode", EnumValue(DropTailQueue::BYTES));
  // Config::SetDefault ("ns3::DropTailQueue::MaxBytes", UintegerValue(queueSize));
  // Config::SetDefault ("ns3::TcpSocket::RcvBufSize", UintegerValue(winSize));
  // Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue(segSize));

  // Set transport protocol based on user input
  // if (tcpType.compare("TcpTahoe") == 0) {
  //     Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue("ns3::TcpReno"));
  // } else if(tcpType.compare("TcpReno") == 0) {
  //   Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue("ns3::TcpTahoe"));
  // } else {
  //   exit(-1);
  // }

  // Let's use the dumbbell helper
  std::cerr << "Create Dumbbell Helper" << std::endl;
  PointToPointHelper p2pLeaf, p2pRouters;
  p2pLeaf.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2pLeaf.SetChannelAttribute ("Delay", StringValue ("10ms"));
  p2pRouters.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2pRouters.SetChannelAttribute ("Delay", StringValue ("20ms"));
  PointToPointDumbbellHelper dumbbell (numLtNodes, p2pLeaf, numRtNodes, p2pLeaf, p2pLeaf);

  // Add TCP/IP stack to all nodes (Transmission Control Protocol / Internet Protocol)
  // Application (data): Encodes the data being sent.
  // Transport (UDP)   : Splits data into manageable chunks, adds port # info.
  // Internet (IP)     : Adds IP addresses stating where data is from and going.
  // Link (frame)      : Adds MAC address info to tell which HW device the message
  //                     is from and which HW device it is going to.
  NS_LOG_INFO ("Intalling TCP/IP stack to all nodes");
  dumbbell.InstallStack (InternetStackHelper());
  
  // Hardware is in place. Now assign IP addresses
  NS_LOG_INFO ("Assign IP Addresses.");
  std::cerr << "Assigning IP Addresses" << std::endl;
  Ipv4AddressHelper ltIps = Ipv4AddressHelper ("10.0.1.0", "255.255.255.0");
  Ipv4AddressHelper rtIps = Ipv4AddressHelper ("10.1.1.0", "255.255.255.0");
  Ipv4AddressHelper routerIps = Ipv4AddressHelper ("10.2.1.0", "255.255.255.0");
  dumbbell.AssignIpv4Addresses(ltIps, rtIps, routerIps);

  std::cerr << "ip addresses:\n";
  std::cerr << dumbbell.GetLeftIpv4Address(0) << "\n"
            << dumbbell.GetRightIpv4Address(0) << "\n"
            << std::endl;

  uint16_t port = 9;

  // Create BulkSendApplication source and install node 0 on it
  // BulkSendApplication source ("ns3::TcpL4Protocol", InetSocketAddress (Ipv4Address.GetAny (), port));
  std::cerr << "Creating BulkSendApplication" << std::endl;
  BulkSendHelper source ("ns3::TcpSocketFactory",
                         InetSocketAddress (dumbbell.GetLeftIpv4Address (0), port));
  source.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  ApplicationContainer sourceApps = source.Install (dumbbell.GetLeft ());
  sourceApps.Start (Seconds (0.0));
  sourceApps.Stop  (Seconds (10.0));

  // PacketSinkApplication sink and install right-most node on it
  std::cerr << "Creating PacketSinkHelper" << std::endl;
  PacketSinkHelper sink ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), port));
  ApplicationContainer sinkApps = sink.Install (dumbbell.GetRight ());
  sinkApps.Start (Seconds (0.0));
  sinkApps.Stop  (Seconds (10.0));

  //  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // Animation setup
  // AnimationInterface animInterface("p1.anim.xml");

  // Run simulation
  std::cerr << "Run Simulation... cerr" << std::endl;
  NS_LOG_INFO ("Run Simulation...");
  Simulator::Stop (Seconds (10.0));
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done!");
  std::cerr << "Done cerr" << std::endl;

  // Print out Goodput of the network communication from source to sink
  // Goodput: Amount of useful information (bytes) per unit time (seconds)
  Ptr<PacketSink> sink1 = DynamicCast<PacketSink> (sinkApps.Get (0));
  std::cout << "Total Bytes Received: " << sink1->GetTotalRx () << std::endl;

  return 0;
}
