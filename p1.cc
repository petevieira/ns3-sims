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
#include "ns3/bulk-send-application.h"
#include "ns3/uinteger.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Project_01-TCP_Throughput_Measurments");

// Network topology (TCP/IP Protocol)
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
  LogComponentEnable ("PacketSinkApplication", LOG_LEVEL_INFO);

  // Set constants
  uint32_t bulkSenderMaxBytes = 1000000000;

  // Set default values for simulation variables
  std::string traceName = "tcp-send.tr";
  bool tracing = false;
  uint32_t numNodes   = 4;
  uint32_t winSize    = 2000;
  uint32_t queueLimit = 2000;
  uint32_t segSize    = 128;

  // Parse command line arguments
  CommandLine cmd;
  cmd.AddValue ("traceName",  "Name of trace file",                      traceName);
  cmd.AddValue ("tracing",    "Flag to enable/disable tracing",          tracing);
  cmd.AddValue ("winSize",    "Receiver advertised window size (bytes)", winSize);
  cmd.AddValue ("queueLimit", "Queue limit on the bottleneck link",      queueLimit);
  cmd.AddValue ("segSize",    "TCP segment size",                        segSize);
  cmd.Parse(argc, argv);

  // Create nodes 0 and 1, then add 1 to container for nodes 1 to 2, etc
  // until node 3. So we have three links.
  NS_LOG_INFO ("Creating nodes.");
  std::vector<NodeContainer> nodeLinks (numNodes-1);
  nodeLinks[0].Create (2);
  for(uint32_t i=1; i<nodeLinks.size (); ++i)
    {
      nodeLinks[i].Add (nodeLinks[i-1].Get (1));
    }

  // Create point-to-point helpers for two link types
  // Put DropTailQueue on output node (first node) of link 2
  NS_LOG_INFO ("Creating channels.");
  std::vector<PointToPointHelper> p2pHelpers (numNodes-1);
  p2pHelpers[0].SetDeviceAttribute  ("DataRate", StringValue ("5Mbps"));
  p2pHelpers[0].SetChannelAttribute ("Delay",    StringValue ("10ms"));
  p2pHelpers[1].SetQueue("ns3::DropTailQueue");
  p2pHelpers[1].SetDeviceAttribute  ("DataRate", StringValue ("1Mbps"));
  p2pHelpers[1].SetChannelAttribute ("Delay",    StringValue ("20ms"));
  p2pHelpers[2] = p2pHelpers[0];

  // Install devices and channels with point-to-point helpers
  NS_LOG_INFO ("Installing network devices");
  std::vector<NetDeviceContainer> devices (numNodes-1);
  for(uint32_t i=0; i<devices.size (); ++i)
    {
      devices[i] = p2pHelpers[i].Install (nodeLinks[i]);
    }


  // Add TCP/IP stack to all nodes (Transmission Control Protocol / Internet Protocol)
  // Application (data): Encodes the data being sent.
  // Transport (UDP)   : Splits data into manageable chunks, adds port # info.
  // Internet (IP)     : Adds IP addresses stating where data is from and going.
  // Link (frame)      : Adds MAC address info to tell which HW device the message
  //                     is from and which HW device it is going to.
  NS_LOG_INFO ("Intalling TCP/IP stack to all nodes");
  InternetStackHelper stack;
  stack.InstallAll ();
  
  // Hardware is in place. Now assign IP addresses
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  // for(uint32_t i=0; i<nodes
  // address.SetBase ("10.1.1.0", "255.255.255.0");
  // Ipv4InterfaceContainer i = ipv4.Assign (

  uint32_t port = 9;
  // Create BulkSendApplication source and install node 0 on it
//  BulkSendApplication source ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address.GetAny (), port));
//  source.SetMaxBytes (UintergerValue (bulkSenderMaxBytes));
//  source.SetNode (nodeLinks[0].Get (0));
//  source.SetStartTime (Seconds (0.0));
//  source.SetStopTime  (Seconds (10.0));

//  // PacketSinkApplication sink and install right-most node on it
//  PacketSinkHeper sink ("ns3::TcpSocketFactory",
//                         InetSocketAddress (Ipv4Address::GetAny (), port));
//  sink.Install (nodeLinks[nodeLinks.size () - 1].Get (1));
//  sink.Start (Seconds (0.0));
//  sink.Stop  (Seconds (10.0));

//  // Set up tracing
//  if (tracing)
//    {
//      AsciiTraceHelper ascii;
//      p2pHelpers.EnableAsciiAll (ascii.CreateFileStream (traceName));
//      p2pHelpers.EnablePcapAll ("p1", false);
//    }

  // Animation setup
//  AnimationInterface animInterface("p1.anim.xml");

  // Run simulation
  NS_LOG_INFO ("Run Simulation...");
  Simulator::Stop (Seconds (10.0));
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done!");

  // Print out Goodput of the network communication from source to sink
  // Goodput: Amount of useful information (bytes) per unit time (seconds)
//  Ptr<PacketSink> sink1 = DynamicCast<PacketSink> (sinkApp.Get (0));
//  std::cout << "Total Bytes Received: " << sink1->GetTotalRx () << std::endl;

  return 0;
}
