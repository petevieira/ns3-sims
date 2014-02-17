/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Pete Vieira <pete.vieira@gatech.edu>
 * Date: Jan 2014
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *   * Neither the name of the Humanoid Robotics Lab nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

// NS3 Includes
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
#include "ns3/applications-module.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/uinteger.h"
#include "ns3/point-to-point-dumbbell.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/random-variable-stream.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Project_02-Comparison of RED vs. DropTail Queuing");

// Network topology (TCP/IP Protocol)
//
//
//               100 Mbps  
//             n ----                                                 ---- n
//                   |  10 Mbs                                       |
//             n --- n -----                                   ----- n --- n
//                         |                                   |
//             n ----      |                                   |      ---- n
//                   |     |                                   |     |  
// Node:       n --- n --- n0 ---------- n1 --------- n2 ----- n --- n --- n 
// Bandwidth:              |   5 Mbps        1 Mbps    10 Mbps |
//             n ----      |                                   |      ---- n
//                   |     |                                   |     |
//             n --- n ----                                     ---- n --- n
//
// - Flow from n0 to n3 using BulkSendApplication
// - Receipt of bulk send at n3 using PacketSinkApplication
// - Output trace file to p1.tr
// - Vary DropTail queue size (30, 60, 120, 190, 240)
//        RED minTh \ (queue length threshold to trigger probabilistic drops) 5 15 30 60 120
//            maxTh \ (queue length threshold to trigger forced drops)        15 45 90 180 360
//            maxP  \ (max probability of doing an early drop)                1/20 1/10 1/4
//            Wq    \ (weighting factor for average queue length computation) 1/128 1/256 1/512
//            qlen  \ (max # of packets that can be enqueued)                 480
// - Compare goodput, which we assume is proportional to response time

int main (int argc, char *argv[])
{
  std::cout << "\n----------------------------" << std::endl;
  std::cout << "    Running Simulation P2   " << std::endl;
  std::cout << "----------------------------" << std::endl;
  Time::SetResolution (Time::NS);
  // LogComponentEnable ("BulkSendApplication", LOG_LEVEL_INFO);
  // LogComponentEnable ("PacketSink", LOG_LEVEL_INFO);
 
  // Set default values for simulation variables
  std::string dataFileName= "p1.data";
  std::string animFile= "p1-anim.xml";
  std::string tcpType = "TcpTahoe";
  bool tracing = false;
  uint32_t numLtNodes = 1;
  uint32_t numRtNodes = 1;
  uint32_t winSize    = 2000;
  uint32_t queSize    = 2000;
  uint32_t segSize    = 512;
  uint32_t maxBytes   = 100000000;
  uint32_t numBulkSendApps = 1;
  std::string queueType = "DropTail";

  // Parse command line arguments
  CommandLine cmd;
  cmd.AddValue ("dataFileName",   "Data file name",                      dataFileName);
  cmd.AddValue ("animFile",   "Animation file name",                     animFile);
  cmd.AddValue ("tcpType",    "TCP type (use TcpReno or TcpTahoe)",      tcpType);
  cmd.AddValue ("tracing",    "Flag to enable/disable tracing",          tracing);
  cmd.AddValue ("winSize",    "Receiver advertised window size (bytes)", winSize);
  cmd.AddValue ("queSize",    "Queue limit on the bottleneck link",      queSize);
  cmd.AddValue ("segSize",    "TCP segment size",                        segSize);
  cmd.AddValue ("maxBytes",   "Max bytes soure will send",               maxBytes);
  // RED Parameters
  cmd.AddValue ("queueType",  "Set Queue type to DropTail or RED",                       queueType);
  cmd.AddValue ("minTh",      "Queue length threshold to trigger probabilistic drops",   minTh);
  cmd.AddValue ("maxTh",      "Queue length threshold to trigger forced drops",          maxTh);
  cmd.AddValue ("maxP",       "Max probability of doing an early drop",                  maxP);
  cmd.AddValue ("Wq",         "Weighting factor for average queue length computation",   Wq);
  cmd.AddValue ("qlen",       "Max number of bytes that can be enqueued",                qlen);
  cmd.Parse(argc, argv);

  // Set default values
  Config::SetDefault ("ns3::DropTailQueue::Mode", EnumValue(DropTailQueue::QUEUE_MODE_BYTES));
  Config::SetDefault ("ns3::DropTailQueue::MaxBytes", UintegerValue(queSize));
  Config::SetDefault ("ns3::RedQueue::Mode", EnumValue(RedQueue::QUEUE_MODE_BYTES));
  Config::SetDefault ("ns3::RedQueue::QueueLimit", UintegerValue (maxBytes));
  Config::SetDefault ("ns3::TcpSocket::RcvBufSize", UintegerValue(winSize));
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue(segSize));

  if ((queueType != "RED") && (queueType != "DropTail")) {
    NS_ABORT_MSG ("Invalid queue type: Use --queueType=RED or --queueType=DropTail");
  }

  // Set transport protocol based on user input
  if (tcpType.compare("TcpTahoe") == 0) {
    std::cout << "\nSetting default protocol to Tcp-Tahoe" << std::endl;
    Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue("ns3::TcpTahoe"));
  } else if(tcpType.compare("TcpReno") == 0) {
    std::cout << "\nSetting default protocol to Tcp-Reno" << std::endl;
    Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue("ns3::TcpReno"));
  } else {
    std::cerr << "\nInvalid protocol: " << tcpType << std::endl;
    exit(-1);
  }

  //---------------------------------------------------------------
  //                     CREATE TOPOLOGY
  //---------------------------------------------------------------

  // Let's use the dumbbell helper to setup left and right nodes with
  // two routers in the middle
  PointToPointHelper p2pLeaf, p2pRouters;
  p2pLeaf.SetDeviceAttribute     ("DataRate", StringValue ("5Mbps"));
  p2pLeaf.SetChannelAttribute    ("Delay",    StringValue ("10ms"));
  p2pRouters.SetDeviceAttribute  ("DataRate", StringValue ("1Mbps"));
  p2pRouters.SetChannelAttribute ("Delay",    StringValue ("20ms"));
  PointToPointDumbbellHelper dumbbell (numLtNodes, p2pLeaf,
                                       numRtNodes, p2pLeaf, 
                                       p2pRouters);

  // Add TCP/IP stack to all nodes (Transmission Control Protocol / Internet Protocol)
  // Application (data): Encodes the data being sent.
  // Transport (UDP)   : Splits data into manageable chunks, adds port # info.
  // Internet (IP)     : Adds IP addresses stating where data is from and going.
  // Link (frame)      : Adds MAC address info to tell which HW device the message
  //                     is from and which HW device it is going to.
  InternetStackHelper stack;
  dumbbell.InstallStack (stack);
  
  // Hardware is in place. Now assign IP addresses
  Ipv4AddressHelper ltIps     = Ipv4AddressHelper ("10.1.1.0", "255.255.255.0");
  Ipv4AddressHelper rtIps     = Ipv4AddressHelper ("10.2.1.0", "255.255.255.0");
  Ipv4AddressHelper routerIps = Ipv4AddressHelper ("10.3.1.0", "255.255.255.0");
  dumbbell.AssignIpv4Addresses(ltIps, rtIps, routerIps);

  uint16_t port = 9;

  ApplicationContainer sourceApps;
  ApplicationContainer sinkApps;
  Ptr<UniformRandomVariable> randNum = CreateObject<UniformRandomVariable> ();

  for (uint16_t i=0; i<numBulkSendApps; ++i) {
    // Create BulkSendApplication source using a BulkSendHelper, whose constructor
    // specifies the protocol to use and the address of the remote node to send
    // traffic to. We'll install it on the left most node    
    BulkSendHelper source ("ns3::TcpSocketFactory",
                           InetSocketAddress (dumbbell.GetRightIpv4Address (0), port + i));
    source.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
    sourceApps.Add (source.Install (dumbbell.GetLeft (0)));

    uint32_t randfloat = (float)(randNum->GetInteger (0, 100)) / 100.0;
    sourceApps.Start (Seconds (randfloat));
    sourceApps.Stop  (Seconds (10.0));

    // Create PacketSinkApplication sink using a PacketSinkHelper, whose constructor
    // specifies the protocol to use and the address of the sink.
    // We'll install it on the right-most node.
    PacketSinkHelper sink ("ns3::TcpSocketFactory",
                           InetSocketAddress (Ipv4Address::GetAny (), port + i));
    sinkApps.Add (sink.Install (dumbbell.GetRight (0)));
    sinkApps.Start (Seconds (0.0));
    sinkApps.Stop  (Seconds (10.0));
  }


  // Animation setup and bounding box for animation
  dumbbell.BoundingBox (1, 1, 100, 100);
  AnimationInterface animInterface(animFile);
  animInterface.EnablePacketMetadata(true);
  std::cerr << "\nSaving animation file: " << animFile << std::endl;

  // Uses shortest path search from every node to every possible destination
  // to tell nodes how to route packets. A routing table is the next top route
  // to the possible routing destinations.
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // Run simulation
  std::cout << "\nRuning simulation..." << std::endl;
  Simulator::Stop (Seconds (10.0));
  Simulator::Run ();
  Simulator::Destroy ();
  std::cout << "\nSimulation finished!" << std::endl;

  // Print out Goodput of the network communication from source to sink
  // Goodput: Amount of useful information (bytes) per unit time (seconds)
  std::vector<uint32_t> goodputs;
  uint32_t i = 0;
  for(ApplicationContainer::Iterator it = sinkApps.Begin(); it != sinkApps.End(); ++it) {
    Ptr<PacketSink> sink1 = DynamicCast<PacketSink> (*it);
    uint32_t bytesRcvd = sink1->GetTotalRx ();
    goodputs.push_back(bytesRcvd / 10.0);
    std::cout << "\nFlow " << i << ":" << std::endl;
    std::cout << "\tTotal Bytes Received: " << bytesRcvd << std::endl;
    std::cout << "\tGoodput: " << goodputs.back() << " Bytes/seconds" << std::endl;
    ++i;
  }

  // Write results to data file
  std::ofstream dataFile;
  dataFile.open(dataFileName.c_str(), std::fstream::out | std::fstream::app);
  dataFile << tcpType << "\t"
           << winSize << "\t"
           << queSize << "\t"
           << segSize;
  for(std::vector<uint32_t>::iterator gp = goodputs.begin(); gp != goodputs.end(); ++gp) {
    dataFile << "\t" << *gp;
  }
  dataFile  << "\n";
  dataFile.close();

  return 0;
}
