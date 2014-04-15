
#ifndef _WORM_UDP_H
#define _WORM_UDP_H

#include "ns3/timer.h"
#include "ns3-worm.h"


// This version only supports TCP protocol

//Doc:ClassXRef
class WormUDP : public Worm/*, public TimerHandler*/ {
  //Doc:Class Define a application that works as the Sapphire worm

public:
  // Define a number of constructors for different needs
  //Doc:Method
  WormUDP();   // Just specify endpoint
    //Doc:Desc Specifies a node to attach the worm application to. 

  virtual void AttachNode(ns3::Node*); // Note which node attached to
  virtual ~WormUDP();
  virtual Application* Copy() const;// Make a copy of the application
  
  static void SetScanRate(Count_t );

  virtual void Initialize();
public:
  // newly defined in this class
  virtual void Activate();


private:
  ns3::UdpL4Protocol/*UDP*/* udp;
  ns3::Timer         timer;
//  TimerEvent*   timeoutevent;

  static Count_t scanrate;

  void SendWorm();

  void ScheduleNextPacket();

//  // the following method is called when packets are received from the port
  virtual void Receive(ns3::Packet *, ns3::IpL4Protocol *, Seq_t); // Data received

  void Timeout(/*TimerEvent **/);  // Timer Handler

  virtual void StartApp();    // Called at time specified by Start
  virtual void StopApp();     // Called at time specified by Stop
};

#endif

