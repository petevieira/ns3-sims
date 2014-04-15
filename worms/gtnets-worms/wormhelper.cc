// GENERAL PUBLIC LICENSE AGREEMENT
// 
// PLEASE READ THIS DOCUMENT CAREFULLY BEFORE UTILIZING THE PROGRAM
// 
// BY UTILIZING THIS PROGRAM, YOU AGREE TO BECOME BOUND BY THE TERMS OF
// THIS LICENSE.  IF YOU DO NOT AGREE TO THE TERMS OF THIS LICENSE, DO
// NOT USE THIS PROGRAM OR ANY PORTION THEREOF IN ANY FORM OR MANNER.
// 
// This Program is licensed, not sold to you by GEORGIA TECH RESEARCH
// CORPORATION ("GTRC"), owner of all code and accompanying documentation
// (hereinafter "Program"), for use only under the terms of this License,
// and GTRC reserves any rights not expressly granted to you.
// 
// 1.  This License allows you to:
// 
// (a) make copies and distribute copies of the Program's source code
// provide that any such copy clearly displays any and all appropriate
// copyright notices and disclaimer of warranty as set forth in Article 5
// and 6 of this License.  All notices that refer to this License and to
// the absence of any warranty must be kept intact at all times.  A copy
// of this License must accompany any and all copies of the Program
// distributed to third parties.
// 
// A fee may be charged to cover the cost associated with the physical
// act of transferring a copy to a third party.  At no time shall the
// program be sold for commercial gain either alone or incorporated with
// other program(s) without entering into a separate agreement with GTRC.
//  
// 
// (b) modify the original copy or copies of the Program or any portion
// thereof ("Modification(s)").  Modifications may be copied and
// distributed under the terms and conditions as set forth above,
// provided the following conditions are met:
// 
//     i) any and all modified files must be affixed with prominent
// notices that you have changed the files and the date that the changes
// occurred.
// 		
//     ii) any work that you distribute, publish, or make available, that
// in whole or in part contains portions of the Program or derivative
// work thereof, must be licensed at no charge to all third parties under
// the terms of this License.
// 
//    iii) if the modified program normally reads commands interactively
// when run, you must cause it, when started running for such interactive
// use in the most ordinary way, to display and/or print an announcement
// with all appropriate copyright notices and disclaimer of warranty as
// set forth in Article 5 and 6 of this License to be clearly displayed.
// In addition, you must provide reasonable access to this License to the
// user.
// 
// Any portion of a Modification that can be reasonably considered
// independent of the Program and separate work in and of itself is not
// subject to the terms and conditions set forth in this License as long
// as it is not distributed with the Program or any portion thereof.
// 
// 
// 2. This License further allows you to copy and distribute the Program
//    or a work based on it, as set forth in Article 1 Section b in
//    object code or executable form under the terms of Article 1 above
//    provided that you also either:
// 
//    i) accompany it with complete corresponding machine-readable source
// code, which must be distributed under the terms of Article 1, on a
// medium customarily used for software interchange; or,
// 
//   ii) accompany it with a written offer, valid for no less than three
// (3) years from the time of distribution, to give any third party, for
// no consideration greater than the cost of physical transfer, a
// complete machine-readable copy of the corresponding source code, to be
// distributed under the terms of Article 1 on a medium customarily used
// for software interchange; or,
// 
// 
// 3.  Export Law Assurance.
// 
// You agree that the Software will not be shipped, transferred or
// exported, directly into any country prohibited by the United States
// Export Administration Act and the regulations thereunder nor will be
// used for any purpose prohibited by the Act.
//  
// 4.  Termination.
// 
// If at anytime you are unable to comply with any portion of this
// License you must immediately cease use of the Program and all
// distribution activities involving the Program or any portion thereof.
// 
// 
// 5.  Disclaimer of Warranties and Limitation on Liability.
// 
// YOU ACCEPT THE PROGRAM ON AN "AS IS" BASIS.  GTRC MAKES NO WARRANTY
// THAT ALL ERRORS CAN BE OR HAVE BEEN ELIMINATED FROM PROGRAM.  GTRC
// SHALL NOT BE RESPONSIBLE FOR LOSSES OF ANY KIND RESULTING FROM THE USE
// OF PROGRAM AND ITS ACCOMPANYING DOCUMENT(S), AND CAN IN NO WAY PROVIDE
// COMPENSATION FOR ANY LOSSES SUSTAINED, INCLUDING BUT NOT LIMITED TO
// ANY OBLIGATION, LIABILITY, RIGHT, CLAIM OR REMEDY FOR TORT, OR FOR ANY
// ACTUAL OR ALLEGED INFRINGEMENT OF PATENTS, COPYRIGHTS, TRADE SECRETS,
// OR SIMILAR RIGHTS OF THIRD PARTIES, NOR ANY BUSINESS EXPENSE, MACHINE
// DOWNTIME OR DAMAGES CAUSED TO YOU BY ANY DEFICIENCY, DEFECT OR ERROR
// IN PROGRAM OR MALFUNCTION THEREOF, NOR ANY INCIDENTAL OR CONSEQUENTIAL
// DAMAGES, HOWEVER CAUSED.  GTRC DISCLAIMS ALL WARRANTIES, BOTH EXPRESS
// AND IMPLIED RESPECTING THE USE AND OPERATION OF PROGRAM AND ITS
// ACCOMPANYING DOCUMENTATION, INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR PARTICULAR PURPOSE AND ANY IMPLIED
// WARRANTY ARISING FROM COURSE OF PERFORMANCE, COURSE OF DEALING OR
// USAGE OF TRADE.  GTRC MAKES NO WARRANTY THAT PROGRAM IS ADEQUATELY OR
// COMPLETELY DESCRIBED IN, OR BEHAVES IN ACCORDANCE WITH ANY
// ACCOMPANYING DOCUMENTATION.  THE USER OF PROGRAM IS EXPECTED TO MAKE
// THE FINAL EVALUATION OF PROGRAM'S USEFULNESS IN USER'S OWN
// ENVIRONMENT.
// 
// GTRC represents that, to the best of its knowledge, the software
// furnished hereunder does not infringe any copyright or patent.
// 
// GTRC shall have no obligation for support or maintenance of Program.
// 
// 6.  Copyright Notice.
// 
// THE SOFTWARE AND ACCOMPANYING DOCUMENTATION ARE COPYRIGHTED WITH ALL
// RIGHTS RESERVED BY GTRC.  UNDER UNITED STATES COPYRIGHT LAWS, THE
// SOFTWARE AND ITS ACCOMPANYING DOCUMENTATION MAY NOT BE COPIED EXCEPT
// AS GRANTED HEREIN.
// 
// You acknowledge that GTRC is the sole owner of Program, including all
// copyrights subsisting therein.  Any and all copies or partial copies
// of Program made by you shall bear the copyright notice set forth below
// and affixed to the original version or such other notice as GTRC shall
// designate.  Such notice shall also be affixed to all improvements or
// enhancements of Program made by you or portions thereof in such a
// manner and location as to give reasonable notice of GTRC's copyright
// as set forth in Article 1.
// 
// Said copyright notice shall read as follows:
// 
// Copyright 2004
// Dr. George F. Riley
// Georgia Tech Research Corporation
// Atlanta, Georgia 30332-0415
// All Rights Reserved
//
// $Id: wormhelper.cc 92 2004-09-14 18:18:43Z dheeraj $



// Georgia Tech Network Simulator - Worm Application class

// Monirul I Sharif.  Georgia Tech, Spring 2004

// Helper Class for worm

#include "common-defs.h"
#include "application.h"
#include "randomtree.h"
#include "worm.h"
#include "tcp.h"
#include "rng.h"
#include "node.h"
#include "application-webserver.h"
#include "application-webbrowser.h"
#include "wormhelper.h"

#ifdef HAVE_QT
#include <qnamespace.h>
#endif

using namespace std;

Count_t AddressRangeOfRandomTreeNetworks
(int trees, int depth, int fanout, int depth_fs, int fanout_fs
 )
{
  Count_t rangeHostsPerTree = (Count_t) pow((double)fanout, (double)depth-depth_fs-1) * 
    (Count_t) pow((double)fanout_fs, (double)depth_fs);
  return rangeHostsPerTree * trees;
}

void CreateRandomTreeNetworksWithWorms(int trees,
				       int depth,
				       int fanout,
				       const Linkp2p &lk,
				       int depth_fs,
				       int fanout_fs,
				       const Linkp2p &hlk,
				       double treelegprob,
				       const Linkp2p &blk,
				       IPAddr_t baseIP,
				       const Worm &worm
				       )
{
  Node *gateway = new Node();
  gateway->SetLocation(Location(5,10.3));

  // Setting colors let's see if it works
#ifdef HAVE_QT
  gateway->Color(Qt::blue);
#endif 
  Count_t rangeHostsPerTree = (Count_t) pow((double)fanout, (double)depth-depth_fs-1) * 
    (Count_t) pow((double)fanout_fs, (double)depth_fs);
  
  IPAddr_t incrIP = rangeHostsPerTree;
  
  for (int treenum=0; treenum<trees; treenum++) {
    RandomTree* t= new RandomTree(depth, fanout, lk,
				  depth_fs, fanout_fs, hlk, 
				  baseIP + incrIP * treenum, treelegprob);
    
    t->BoundingBox(Location(3,4), Location(7,10.3), treenum*(360/trees));
    t->GetRoot()->AddDuplexLink(gateway, blk);
    t->GetRoot()->DefaultRoute(gateway);
    
    cout<<"Tree has "<<t->leafCount<<" leaves"<<endl;
    
    for (int j=0; j<(int)t->leafCount; j++) {
      DEBUG0((cout<<"Setting worm app on tree"<< treenum<<" leaf "<<j));
      Node *n =t->GetLeaf(j);
      DEBUG0((cout<<" IP Address:"<<IPAddr::ToDotted(n->GetIPAddr())<<endl));
            
      Worm* app = (Worm*)n->AddApplication(worm);
      
      if (treenum==0 && j==0) 
	      app->Infect();
      app->Start(0.0);
    } 
  }

}

// Histogram* CreateRandomNetworksWithWorms(const Worm &worm, bool usethrottle, bool partial)
// {
//   Histogram* h = new Histogram(7.0, 70);  // 0-7.0 seconds, 70 bins
//   IPAddrVec_t servers;
  
//   Linkp2p l1(Rate("1Mb"), Time("10ms"));
//   Linkp2p l10(Rate("10Mb"), Time("10ms"));
//   Linkp2p l50(Rate("50Mb"), Time("10ms"));
//   Linkp2p l500(Rate("500Mb"), Time("10ms"));
//   Linkp2p l100(Rate("100Mb"), Time("10ms"));
//   Linkp2p l1g(Rate("1Gb"), Time("10ms"));

  
//   Node *gateway = new Node();
//   gateway->SetLocation(Location(15,15));

//   // Setting colors let's see if it works
// #ifdef HAVE_QT
//   gateway->Color(Qt::blue);
// #endif 

//   // creating ECE servers 
//   //Star star1(10, l10, IPAddr("192.170.11.1"));
//   Star star1(20, l10, IPAddr("10.0.0.1"));

//   // Assign a location to the hub to test the "relative" bounding box
//   star1.GetHub()->SetLocation(Location(20, 35));
//   // Specify the bounding box
//   star1.BoundingBox(Location(15,30), Location(25,40));
   
//   // Create the servers
//   for (Count_t i = 0; i < star1.LeafCount(); ++i)
//     {
//       Node* l = star1.GetLeaf(i);
//       l->UseThrottle(usethrottle);
//       servers.push_back(l->GetIPAddr());
//       WebServer* WebApp = (WebServer*)l->AddApplication(WebServer());
//       WebApp->Start(0.0);

//       Worm* app = (Worm*)l->AddApplication(worm);
//       app->Start(0.0);
//     }


//   // creating COC servers 
//   //  Star star2(10, l10, IPAddr("192.171.10.1"));
//   Star star4(10, l10, IPAddr("10.0.0.135"));

//   // Assign a location to the hub to test the "relative" bounding box
//   star4.GetHub()->SetLocation(Location(5, 22));
//   // Specify the bounding box
//   star4.BoundingBox(Location(0,17), Location(10,27));
   
//   // Create the servers
//   for (Count_t i = 0; i < (star4.LeafCount() - 5); ++i)
//     {
//       Node* l = star4.GetLeaf(i);
//       l->UseThrottle(usethrottle);
//       servers.push_back(l->GetIPAddr());
//       WebServer* WebApp = (WebServer*)l->AddApplication(WebServer());
//       WebApp->Start(0.0);

//       Worm* app = (Worm*)l->AddApplication(worm);
//       app->Start(0.0);
//     }
  
//   // creating external servers 
//   //Star star3(20, l1, IPAddr("192.168.10.1"));
//   Star star5(20, l1, IPAddr("10.0.0.160"));

//   // Assign a location to the hub to test the "relative" bounding box
//   star5.GetHub()->SetLocation(Location(25, 5));
//   // Specify the bounding box
//   star5.BoundingBox(Location(20,0), Location(35,10));
   
//   // Create the servers
//   for (Count_t i = 0; i < (star5.LeafCount() - 15); ++i)
//     {
//       Node* l = star5.GetLeaf(i);
//       l->UseThrottle(usethrottle);
//       //l->UseThrottle(true);
//       servers.push_back(l->GetIPAddr());
//       WebServer* WebApp = (WebServer*)l->AddApplication(WebServer());
//       WebApp->Start(0.0);

//       Worm* app = (Worm*)l->AddApplication(worm);
//       app->Start(0.0);
//     }

//   Uniform* RandomStart = new Uniform(0,1);

//   // creating web browsers in COC lab
//   for (Count_t i = (star4.LeafCount() - 5); i < star4.LeafCount() ; ++i)
//      {
//        Node* l = star4.GetLeaf(i);
//        l->UseThrottle(usethrottle);
//        //l->UseThrottle(true);
//        WebBrowser* wb = (WebBrowser*)l->AddApplication(WebBrowser(servers, Uniform(0, servers.size())));
//        wb->ThinkTimeBound(60.0);
//        wb->SetStatistics(h);
//        wb->Start(RandomStart->Value());

//        Worm* app = (Worm*)l->AddApplication(worm);
//       app->Start(0.0);       
//       }

//   // creating web browsers in external lab
//   for (Count_t i = (star5.LeafCount() - 15); i < star5.LeafCount() ; ++i)
//     {
//       Node* l = star5.GetLeaf(i);
//       l->UseThrottle(usethrottle);
//       //l->UseThrottle(true);
//       WebBrowser* wb = (WebBrowser*)l->AddApplication(WebBrowser(servers, Uniform(0, servers.size())));
//       wb->ThinkTimeBound(60.0);
//       wb->SetStatistics(h);
//       wb->Start(RandomStart->Value());

//       Worm* app = (Worm*)l->AddApplication(worm);
//       app->Start(0.0);
//     }

//   // creating web browsers in ECE lab
//   //  Star star4(20, l10, IPAddr("192.170.12.1"));
//   Star star2(20, l10, IPAddr("10.0.0.43"));

//   // Assign a location to the hub to test the "relative" bounding box
//   star2.GetHub()->SetLocation(Location(5, 35));
//   // Specify the bounding box
//   star2.BoundingBox(Location(0,30), Location(10,40));
   
//   // Create the browsers
//   for (Count_t i = 0; i < star2.LeafCount() ; ++i)
//      {
//        Node* l = star2.GetLeaf(i);
//        l->UseThrottle(usethrottle);

//        //l->UseThrottle(true);
       
//        WebBrowser* wb = (WebBrowser*)l->AddApplication(WebBrowser(servers, Uniform(0, servers.size())));
//        wb->ThinkTimeBound(60.0);
//        wb->SetStatistics(h);
//        wb->Start(RandomStart->Value());

//        Worm* app = (Worm*)l->AddApplication(worm);
//       app->Start(0.0);

//      }

//   // creating web browsers in office
//   //  Star star5(20, l10, IPAddr("192.169.10.1"));
//   Star star3(20, l10, IPAddr("10.0.0.90"));

//   // Assign a location to the hub to test the "relative" bounding box
//   star3.GetHub()->SetLocation(Location(30, 30));
//   // Specify the bounding box
//   star3.BoundingBox(Location(25,25), Location(35,35));
   
//   // Create the browsers
//   for (Count_t i = 0; i < star3.LeafCount() ; ++i)
//     {
//       Node* l = star3.GetLeaf(i);
//       l->UseThrottle(usethrottle);
//       //l->UseThrottle(true);

//       WebBrowser* wb = (WebBrowser*)l->AddApplication(WebBrowser(servers, Uniform(0, servers.size())));
//       wb->ThinkTimeBound(60.0);
//       wb->SetStatistics(h);
//       wb->Start(RandomStart->Value());
      
//       Worm* app = (Worm*)l->AddApplication(worm);
//       app->Start(0.0);      
//     }

//   // creating web browsers in second external lab
//   //  Star star6(20, l10, IPAddr("192.168.11.1"));
//   Star star6(20, l10, IPAddr("10.0.0.205"));

//   // Assign a location to the hub to test the "relative" bounding box
//   star6.GetHub()->SetLocation(Location(5, 5));
//   // Specify the bounding box
//   star6.BoundingBox(Location(0,0), Location(10,10));
   
//   // Create the browsers
//   for (Count_t i = 0; i < star6.LeafCount() ; ++i)
//     {
//       Node* l = star6.GetLeaf(i);
//       l->UseThrottle(usethrottle);
//       // l->UseThrottle(true);

//       WebBrowser* wb = (WebBrowser*)l->AddApplication(WebBrowser(servers, Uniform(0, servers.size())));
//       //wb->ThinkTimeBound(60.0);
//       wb->ThinkTimeBound(60.0);
//       wb->SetStatistics(h);
//       wb->Start(RandomStart->Value());

//       Worm* app = (Worm*)l->AddApplication(worm);
//       app->Start(0.0);
//     }

//   // creating router nodes
//   Node* r1 = new Node();
//   Node* r2 = new Node();
//   r1->SetLocation(Location(15, 30));
//   r2->SetLocation(Location(15, 20));
 

//   // connecting everything together
//   r1->AddDuplexLink(star2.GetHub(), l100);//, IPAddr("10.0.0.83"), IPAddr("10.0.0.84"));
//   r1->AddDuplexLink(star1.GetHub(), l100);//, IPAddr("10.0.0.41"), IPAddr("10.0.0.42"));
//   r1->AddDuplexLink(r2, l500);//, IPAddr("10.0.0.85"), IPAddr("10.0.0.86"));

//   r2->AddDuplexLink(star4.GetHub(), l100);//, IPAddr("10.0.0.89"), IPAddr("10.0.0.134"));
//   r2->AddDuplexLink(star3.GetHub(), l50);//, IPAddr("10.0.0.87"), IPAddr("10.0.0.88"));
//   r2->AddDuplexLink(gateway, l500);//, IPAddr("10.0.0.132"), IPAddr("10.0.0.133"));

//   gateway->AddDuplexLink(star5.GetHub(), l1g);//,IPAddr("10.0.0.156"), IPAddr("10.0.0.157"));
//   gateway->AddDuplexLink(star6.GetHub(), l1g);//, IPAddr("10.0.0.202"), IPAddr("10.0.0.203"));


//   r1->UseThrottle(partial);
//   r2->UseThrottle(partial);
//   gateway->UseThrottle(partial);


//   // // add worm
//   Worm* app = (Worm*)star6.GetLeaf(0)->AddApplication(worm);
  
//   app->Infect();
//   app->Start(0.0);
  

//   return h;
// }
