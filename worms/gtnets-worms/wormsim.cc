

// Worm Demo for GTNetS
// Supports command level arguments and log files.
// Monirul I Sharif, Georgia Institute of Technology

#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/time.h>
#include "validation.h"

//#define DEBUG_MASK 0x01

#include "debug.h"
#include "common-defs.h"
#include "simulator.h"
#include "node.h"
#include "dumbbell.h"
#include "tcp-tahoe.h"
#include "randomtree.h"
#include "tree.h"
#include "ratetimeparse.h"
#include "linkp2p.h"
#include "routing.h"
#include "routing-static.h"
#include "args.h"
#include "bfs.h"

#include "wormtcp.h"
#include "wormudp.h"
#include "wormtargetvector.h"
#include "wormhelper.h"

#ifdef HAVE_QT
#include <qnamespace.h>
#endif


// ------------ Define worm types    ---------------
#define TCPWORMTYPE  1
#define UDPWORMTYPE  2
#define WORMTYPE     UDPWORMTYPE

// ------------ Define the topology  ---------------
#define TREES        4
#define DEPTH        4 
#define FANOUT       8
#define TREELEGPROB  0.85

#define LINKBW       "10Mb"
#define HLINKBW      "10Mb"
#define BLINKBW      "100Mb"

// change the following if you want to have 
#define DEPTH_FS   0
#define FANOUT_FS  0

// ------------ Define Addressing   ----------------
#define BASEIP    "10.0.0.0"

// ------------ Worm parameters -----------------------
#define VULNERABILITY  1.0
#define THREADS        3
#define SCANRATE       100
#define SCANRANGE      0
#define PAYLOAD        1000

// ------------ File names ----------------------------
#define DEFFILENAME "tcpwormsimdata"


// ----------- Simulation settings -------------------
#define SIMTIME        10.0
#define SIMINTERVAL    0.05
#define SIMSHOW        0
#define SIMRANDOM      1

using namespace std;

FILE *tracefile, *infofile;
Count_t wormtype;

// returns time since January 1, 1970 in useconds

unsigned long long int get_time()
{
  struct timeval tp;
  unsigned long long int result;
  gettimeofday(&tp, NULL);
  
  result = tp.tv_sec;
  result = (result * 1000000L) + tp.tv_usec;

  return result;
}

double in_seconds(unsigned long long int time)
{
  return ((double)time)/1000000.0;
}


// Progress hook
static void Progress(Time_t now)
{
  int infected;
  if (wormtype == UDPWORMTYPE)
    infected = WormUDP::TotalInfected();
  else
    infected = WormTCP::TotalInfected();

  fprintf(tracefile, "%6.2lf %6ld\n", Simulator::Now(), infected);  
  fflush(tracefile);

  cout 
    << "Prog " << Simulator::Now() 
    << " infected "<<infected
    << " mem " << Simulator::instance->ReportMemoryUsageMB() << "MB"
    << " bfs calls " << bfs_calls
    << " bfs avg searches " << (float)bfs_nodessearched/bfs_calls
    << endl;

  bfs_calls = 0;
  bfs_nodessearched = 0;
}


int main(int argc, char** argv)
{
  Validation::Init(argc, argv);
  string filename, tracefilename, infofilename;
  Simulator s;
  Count_t nd,nf,nt,ndfs, nffs;
  string hlinkbw, linkbw, blinkbw, baseip;
  Count_t threads, scanrange, scanrate;
  Size_t payload;
  double vulnerability, treelegprob;
  double simtime, siminterval;
  unsigned long long int start_time, end_time;
  Count_t showsim,randomsim;

  cout << "Worm Toolkit Demo on GTNetS  Version 2.0"<<endl;
  cout << "CoC G.Tech, (c)2003"<<endl<<endl;

  if (argc==1) {
    cout << "Usage:  wormtcpsim <option1=...> <option2=...> ..."<<endl;
    cout << "       Options:"<<endl;   
    cout << "             wormtype   : type of worm 1=UDP or 2=TCP"<<endl<<endl;
    cout << "             tracefile  : name portion of trace files "<<endl<<endl;    
    cout << "             trees      : number of trees"<<endl;
    cout << "             depth      : depth of each tree"<<endl;
    cout << "             fanout     : maximum fanout"<<endl;
    cout << "             depthfs    : depth of first stage in tree"<<endl;
    cout << "             fanoutfs   : maximum fanout in first stage of tree"<<endl;
    cout << "             treelegprob: probability that a leg will be created"<<endl;
    cout << "             linkbw     : bandwidth of links in the tree"<<endl;
    cout << "             hlinkbw    : bandwidth of links in first stage"<<endl;
    cout << "             blinkbw    : backbone link bandwidth"<<endl;
    cout << "             nbaseip    : base IP address of nodes in network"<<endl<<endl;
    cout << "             baseip     : base ip address of worm scan"<<endl;
    cout << "             scanrange  : IP scanrange of worm (0 scans only the network space)"<<endl;
    cout << "             threads    : (for  TCP) number of parallel TCP connection each TCP worm makes"<<endl<<endl;
    cout << "             scanrate   : (for  UDP) number of packets per second"<<endl<<endl;
    cout << "             simtime    : the total time of the simulation"<<endl;
    cout << "             siminterval: the interval for producing trace information"<<endl<<endl;
    
    cout << "Please provide atleast one argument to begin"<<endl<<endl;
    return 0;
  }

  Arg("tracefile", filename, DEFFILENAME);

  Arg("wormtype", wormtype, WORMTYPE);

  Arg("trees", nt, TREES);
  Arg("depth", nd, DEPTH);
  Arg("fanout", nf, FANOUT);
  Arg("depthfs", ndfs, DEPTH_FS);
  Arg("fanoutfs", nffs, FANOUT_FS);
  Arg("linkbw", linkbw, LINKBW);
  Arg("hlinkbw", hlinkbw, HLINKBW);
  Arg("blinkbw", blinkbw, BLINKBW);
  Arg("baseip", baseip, BASEIP);
  Arg("scanrange", scanrange, SCANRANGE);
  Arg("threads", threads, THREADS);
  Arg("scanrate", scanrate, SCANRATE);
  Arg("payload", payload, PAYLOAD);
  Arg("vulnerability", vulnerability, VULNERABILITY);
  Arg("treelegprob", treelegprob, TREELEGPROB);
  Arg("simtime", simtime, SIMTIME);
  Arg("siminterval", siminterval, SIMINTERVAL);
  Arg("show", showsim, SIMSHOW);
  Arg("random", randomsim, SIMRANDOM);

  Arg::ProcessArgs(argc, argv);

  // Set the random number generated
  //  Random::GlobalSeed(30,40,450,60,20,30);
  if (!randomsim) {
    Random::GlobalSeed(31731,44543,425345,19367,48201,72333);
  }

  tracefilename = filename+".dat";
  infofilename = filename+".inf";
  cout << "Data:"<< tracefilename <<", Info:"<< infofilename<<endl;
  
  tracefile = fopen(tracefilename.c_str(), "wt");
  infofile = fopen(infofilename.c_str(), "wt");
  // Calculate number of HOST IP's

  Count_t rangeTotalHosts = AddressRangeOfRandomTreeNetworks(nt, nd, nf, ndfs, nffs);

  if (scanrange==0) 
    scanrange = rangeTotalHosts;

  // Now let us set the worm application defaults
  if (wormtype==UDPWORMTYPE) {
    WormUDP::SetBaseIP(IPAddr(baseip));
    WormUDP::SetVulnerability(vulnerability);
    WormUDP::SetPayloadLength(payload);
    WormUDP::SetScanRate(scanrate);
    WormUDP::SetTargetVector(WTVUniform(scanrange));
  } else {
    WormTCP::SetBaseIP(IPAddr(baseip));
    WormTCP::SetVulnerability(vulnerability);
    WormTCP::SetPayloadLength(payload);
    WormTCP::SetConnections(threads);
    WormTCP::SetTargetVector(WTVUniform(scanrange));
  }

  // Set node shape to a circle for animation
  Node::DefaultShape(Node::CIRCLE);
  
  // Trace* tr = Trace::Instance(); // Get a pointer to global trace object
  //  tr->Open("testworm.txt");
  //  TCP::LogFlagsText(true);       // Log TCP flags in text mode
  //  IPV4::Instance()->SetTrace(Trace::ENABLED);
  
  Linkp2p lk(Rate(linkbw.c_str()), Time("20ms"));
  Linkp2p blk(Rate(blinkbw.c_str()), Time("20ms"));  
  Linkp2p hlk(Rate(hlinkbw.c_str()), Time("20ms"));
  
  if (wormtype == UDPWORMTYPE)
    CreateRandomTreeNetworksWithWorms(nt, nd, nf, lk, ndfs, nffs, 
				      hlk, treelegprob, blk, IPAddr(baseip), WormUDP());
  else
    CreateRandomTreeNetworksWithWorms(nt, nd, nf, lk, ndfs, nffs, 
				      hlk, treelegprob, blk, IPAddr(baseip), WormTCP());

  // Specify animation

  int TotalInstances, TotalVulnerable;

  if (wormtype == UDPWORMTYPE) {
    TotalInstances = WormUDP::TotalInstances();
    TotalVulnerable = WormUDP::TotalVulnerable();
  } else {
    TotalInstances = WormTCP::TotalInstances();
    TotalVulnerable = WormTCP::TotalVulnerable();
  }

  cout<<"--------------------------------------------"<<endl;
  cout<<"Topology used: trees="<<nt<<" depth="<<nd<<" fanout="<<nf<<endl;
  cout<<"Link bandwidths "<<linkbw<<", "<<hlinkbw<<", "<<blinkbw<<endl;

  if (wormtype==UDPWORMTYPE) {
    cout<<"Worm type : UDP"<<endl;
    cout<<"Scan rate "<<scanrate<<endl;
  } else {
    cout<<"Worm type : TCP"<<endl;
    cout<<"TCP Connections "<<threads<<endl<<endl;
  }

  cout<<"Worm scans from "<<baseip<<" scan range "<<scanrange<<endl;  
  cout<<endl;
  cout<<"Total Possible Hosts  : "<<rangeTotalHosts<<endl;
  cout<<"Total Real Hosts      : "<<TotalInstances<<endl;
  cout<<"Total Vulnerable hosts: "<<TotalVulnerable<<endl;
  

  fprintf(infofile, "Topology used: trees=%ld depth=%ld fanout=%ld fs_depth=%ld fs_fanout=%ld\n",
	  nt, nd, nf, ndfs, nffs);

  fprintf(infofile, "Link bandwidths %s, %s, %s\n", linkbw.c_str(), hlinkbw.c_str(), blinkbw.c_str());

  if (wormtype==UDPWORMTYPE) {
    fprintf(infofile, "UDP worm.\n");
    fprintf(infofile, "UDP Scanrate    :%ld\n", scanrate);
  } else {
    fprintf(infofile, "TCP worm.\n");
    fprintf(infofile, "TCP Connections :%ld\n", threads);
  }
  fprintf(infofile, "Payload length  :%ld\n", payload);
  fprintf(infofile, "Worm scan range :%ld\n", scanrange);
  fprintf(infofile, "Possible hosts  :%ld\n", rangeTotalHosts); 
  fprintf(infofile, "Real hosts      :%ld\n", TotalInstances);
  fprintf(infofile, "Vulnerable hosts:%ld\n", TotalVulnerable);
  
  
  s.ProgressHook(Progress);

  if (showsim && !Validation::noAnimation) {
    s.StartAnimation(0, true);
    s.AnimationUpdateInterval(Time("10us")); // 10us initial update rate
  }

  s.Progress(siminterval);
  s.StopAt(simtime);
  start_time = get_time();
  s.Run();
  end_time = get_time();

  fprintf(infofile, "Total run time :%0.6lf seconds\n", in_seconds(end_time-start_time)); 

  fclose(infofile);
  fclose(tracefile);
}
