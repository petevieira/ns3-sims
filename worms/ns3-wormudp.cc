
#include "ns3-wormudp.h"

#ifdef HAVE_QT
#include <qnamespace.h>
#endif

Count_t  WormUDP::scanrate = 50;

// In this version of the worm application I am using a constant
//    packets per second method to send UDP packets.
// I will later change this into a more robust method.

// Constructors

WormUDP::WormUDP()
  : udp(NULL)/*, timeoutevent(NULL)*/
{

}

WormUDP::~WormUDP()
{
}

// Application Methods
void WormUDP::StartApp()    // Called at time specified by Start
{
  Worm::StartApp();
}

void WormUDP::StopApp()     // Called at time specified by Stop
{
  Worm::StopApp();
  timer.Cancel();
//  if (timeoutevent) timer.Cancel(timeoutevent);
}

void WormUDP::AttachNode(ns3::Node* n)
{
  Worm::AttachNode(n);

  udp = new ns3::UdpL4Protocol;
//  udp = new UDP(n);

    udp->SetNode(n);
//  udp->Attach(n);
//  udp->Bind(infectionport);
//  udp->AttachApplication(this);
//  std::cout << "udp " << udp << " , attach App " << this << std::endl;
}

ns3::Application* WormUDP::Copy() const
{
  return new WormUDP(*this);
}


void WormUDP::Activate()
{
  ns3::Ptr<ns3::Ipv4> ipv4 = node->GetObject<ns3::Ipv4>();
  ns3::Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1,0);
  ns3::Ipv4Address addri = iaddr.GetLocal ();
  std::cout << "Infected machine:" << addri/*IPAddr::ToDotted(node->GetIPAddr())*/ << std::endl;
  std::cout << " Now starting timer event" << std::endl;
  // Set the node to red on animation
#ifdef HAVE_QT
  node->Color(Qt::red);
#endif
  patchtime += ns3::Simulator::Now().GetDouble();

  if(!delayinitprobe)
    {
      SendWorm();
    }
  else
    {
      patchtime += (1.0 / (double)scanrate);
    }
  
  ScheduleNextPacket();
}


void WormUDP::Initialize()
{

}

void WormUDP::SendWorm()
{
  IPAddr_t target = GenerateNextIPAddress();

  ns3::Ptr<ns3::Ipv4> ipv4 = node->GetObject<ns3::Ipv4>();
  ns3::Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1,0);
  ns3::Ipv4Address addri = iaddr.GetLocal ();
  std::cout << "SendWorm[" << addri
            << "]: Sending worm packet to "
//            << (std::string)ns3::Ipv4Address(target)
            << std::endl;

//  udp->SendTo(payloadlength, target, infectionport);
}

void WormUDP::SetScanRate(Count_t rate)
{
  scanrate = rate;
}

void WormUDP::Receive(ns3::Packet *p, ns3::IpL4Protocol *proto, Seq_t)
{
   ns3::Ptr<ns3::Ipv4> ipv4 = node->GetObject<ns3::Ipv4>();
   ns3::Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1,0);
   ns3::Ipv4Address addri = iaddr.GetLocal ();
   std::cout << "Receive[" << addri/*(string)IPAddr(node->GetIPAddr())*/
             << "]: Received TCP packet of size " << p->GetSize()
             << std::endl;
   
   if (PacketIsWorm(p)) {
     if (vulnerable && !infected) {
       Infect();
      }
   }
   delete p;
}

void WormUDP::ScheduleNextPacket()
{
  if(patchable)
    {
      if (ns3::Simulator::Now().GetDouble() >= patchtime)
        {
          return;
        }
    }

//  if (!timeoutevent) timeoutevent = new TimerEvent();
  timer.Schedule(ns3::Time(1.0 / (double)scanrate));
//  timer.Schedule(timeoutevent, 1.0 / (double)scanrate, this);
}

void WormUDP::Timeout(/*TimerEvent *ev*/)
{
  if (!node)
    {
      std::cout << "WormUDP::Timeout with no attached node " << std::endl;
      return;
    }

  std::cout << "WormUDPWorm::Timeout " << ns3::Simulator::Now().GetDouble() << std::endl;

  // Sending the worm packet
  SendWorm();
  ScheduleNextPacket();
}
