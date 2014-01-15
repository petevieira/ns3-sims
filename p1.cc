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

int
main (int argc, char *argv[])
{
  Time::SetResolution (Time::NS);
  LogComponentEnable ("BulkSendApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("PacketSinkApplication", LOG_LEVEL_INFO);
  uint32_t numNodes = 4;
  // Create nodes 0 and 1, then add 1 to container for nodes 1 to 2, etc
  // until node 3. So we have three links.
  std::vector<NodeContainer> nodeLinks (numNodes-1);
  nodeLinks[0].Create (2);
  for(uint32_t i=1; i<nodeLinks.size (); ++i)
    {
      nodeLinks[i].Add (nodeLinks[i-1].Get (1));
    }

  // Create point-to-point helpers for two link types
  std::vector<PointToPointHelper> p2pHelpers (numNodes-1);
  p2pHelpers[0].SetDeviceAttribute  ("DataRate", StringValue ("5Mbps"));
  p2pHelpers[0].SetChannelAttribute ("Delay",    StringValue ("10ms"));
  p2pHelpers[1].SetDeviceAttribute  ("DataRate", StringValue ("1Mbps"));
  p2pHelpers[1].SetChannelAttribute ("Delay",    StringValue ("20ms"));
  p2pHelpers[2] = p2pHelpers[0];

  // Install devices and channels with point-to-point helpers
  std::vector<NetDeviceContainer> devices (numNodes-1);
  for(uint32_t i=0; i<devices.size (); ++i)
    {
      devices[i] = p2pHelpers[i].Install (nodeLinks[i]);
    }


  // Add ip/tcp stack to all nodes
  InternetStackHelper stack;
  stack.InstallAll ();

  // // Hardware is in place. Now assign IP addresses
  // NS_LOG_INFO ("Assign IP Addresses.");
  // Ipv4AddressHelper ipv4;
  // address.SetBase ("10.1.1.0", "255.255.255.0");
  // // Ipv4InterfaceContainer i = ipv4.Assign (



  // BulkSendHelper source;


  // PacketSinkHelper sink;


  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
