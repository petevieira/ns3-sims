// Test animation of worm infection
// Monirul I Sharif, Georgia Institute of Technology

#include <iostream>
#include <stdio.h>

#define DEBUG_MASK 0x01

#include "debug.h"
#include "common-defs.h"
#include "simulator.h"
#include "node.h"
#include "dumbbell.h"
#include "tcp-tahoe.h"
#include "tree.h"
#include "ratetimeparse.h"
#include "linkp2p.h"
#include "application-worm.h"
#include "routing.h"
#include "routing-static.h"
#include "validation.h"  

#ifdef HAVE_QT
#include <qnamespace.h>
#endif

#define N_FANOUT 4
#define N_DEPTH 3
#define N_TREES 4

using namespace std;


int main(int argc, char** argv)
{
  Validation::Init(argc, argv);
  Simulator s;
  
  Count_t nd = N_DEPTH;
  Count_t nf = N_FANOUT;
  Count_t nt = N_TREES;

  
  cout << "Worm Application Demo on GTNetS"<<endl;
  cout << "By Monirul I Sharif"<<endl;
  cout << "CoC G.Tech, (c)2003"<<endl<<endl;
  cout << "Usage: testworm <depth> <fanout> <treecount> <scanrate>"<<endl;
  //Routing::SetRouting(new RoutingStatic());

  if (argc > 1) nd = atol(argv[1]);
  if (argc > 2) nf = atol(argv[2]);
  if (argc > 3) nt = atol(argv[3]);
  if (argc > 4) 
    WormApplication::SetDefaultScanRate(atol(argv[4]));
  else
    WormApplication::SetDefaultScanRate(100);

  cout<<endl;
  cout<<"Each tree has base IP address of 192.168.x.1"<<endl;
  cout<<"Worm scans from 192.168.0.1 to specified range"<<endl<<endl;

  // Now let us set the worm application defaults
  WormApplication::SetDefaultBaseIP(IPAddr("192.168.0.1"));
  WormApplication::SetDefaultScanRange(256 * 4);

  // Set node shape to a circle for animation
  Node::DefaultShape(Node::CIRCLE);
  
  Trace* tr = Trace::Instance(); // Get a pointer to global trace object
  tr->Open("testworm.txt");
  TCP::LogFlagsText(true);       // Log TCP flags in text mode
  IPV4::Instance()->SetTrace(Trace::ENABLED);
  
  Linkp2p lk(Rate("10Mb"), Time("20ms"));
  Node *gateway = new Node();
  gateway->SetLocation(Location(5,8.5));

  // Setting colors let's see if it works
#ifdef HAVE_QT
  gateway->Color(Qt::red);
  DEBUG0((cout<<"Color set"<<endl));
#endif

  IPAddr_t baseIp = IPAddr("192.168.0.1");
  IPAddr_t addrIp = IPAddr("0.0.1.0");
  
  for (int treenum=0; treenum<nt; treenum++) {
    
    Tree* t= new Tree(nd, nf, lk, baseIp + addrIp * treenum);
    
    // Specify the bounding box
    t->BoundingBox(Location(3,5), Location(7,8.5), treenum*(360/nt));
    
    t->GetRoot()->AddDuplexLink(gateway, lk);
    
    for (int j=0; j<t->leafCount; j++) {
      cout<<"Setting worm app on tree"<< treenum<<" leaf "<<j;
      Node *n =t->GetLeaf(j);
      cout<<" IP Address:"<<IPAddr::ToDotted(n->GetIPAddr())<<endl;

      WormApplication* app = (WormApplication*)n->AddApplication(WormApplication(n));
      //n->AddApplication(*app);
      if (treenum==0 && j==0) 
	app->Infect();
      app->Start(0.0);
    }
  }
  
  // Specify animation
  if (!Validation::noAnimation)
    {
      s.StartAnimation(0, true);
      s.AnimationUpdateInterval(Time("10us")); // 10us initial update rate
    }
  s.Progress(1);
  s.StopAt(100);
  s.Run();
}
