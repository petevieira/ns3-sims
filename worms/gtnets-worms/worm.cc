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
// $Id: worm.cc 391 2005-07-20 15:12:49Z mofta7 $




// Georgia Tech Network Simulator - Worm Application Base class

// Monirul I Sharif,  Georgia Tech, Fall 2003
// All worm applications are descendents of this class.

// These classes will be updated soon with a completely new framework.

//#define DEBUG_MASK 0x1

#include "debug.h"
#include "worm.h"

#include "node.h"
#include "simulator.h"
#include <map>

#ifdef HAVE_QT
#include <qnamespace.h>
#endif

using namespace std;

#define DEFAULTSCANRANGE 65536

bool     Worm::initialized = false;
// this is for setting vulnerabilities
Random*  Worm::rngV = nil;
Time_t   Worm::defaultPatchTime = 1.0;
Size_t   Worm::payloadlength = 376;
PortId_t Worm::infectionport = 1434;
//100% machines are vulnerable, please change the folowing for
// for making host's vulnerability be selected randomly
double   Worm::vulnerability = 1.0;

IPAddr_t Worm::baseIP = IPAddr("192.168.0.0");

Count_t  Worm::totalinfected = 0;
Count_t  Worm::totalvulnerable = 0;
Count_t  Worm::totalinstances = 0;

string   Worm::signature = "WORM";
char *   Worm::wormdata = NULL;

// By default we are setting the target vector to be NULL
// The code then selects the uniform target vector object by default

WormTargetVector* Worm::defaultTV = nil;

// Constructors

Worm::Worm()
  : infected(false), vulnerable(true), started(false), patchable(false), patchtime(defaultPatchTime), delayinitprobe(false), node(nil), targetV(nil)
{
  copyOnConnect=false;

  if (!initialized) {
    Initialize();
    initialized = true;
  }

  if (!wormdata) {
    PrepareWormData(wormdata);
  }

  if (vulnerability!=1.0)   {
    if (!rngV) rngV = new Uniform(0, 1);
  }

  // if there is no target vector object defined then we are setting the 
  // it to be the Uniform random target vector class.
  if (!defaultTV) {
    defaultTV = new WTVUniform(DEFAULTSCANRANGE);
  }
}

// Application Methods
void Worm::StartApp()    // Called at time specified by Start
{
  if (infected)
    {
      Activate();
    }
  started = true;
}

void Worm::StopApp()     // Called at time specified by Stop
{
  started = false;
}

void Worm::AttachNode(Node* n) 
{
  node = n;
  totalinstances++;
  
  if (vulnerability!=1.0)
    vulnerable = rngV->Value()<=vulnerability;

  if (vulnerable)
    totalvulnerable++;
}

Application* Worm::Copy() const
{
  return new Worm(*this);
}

void Worm::Infect()
{
  infected = true;
  totalinfected++;

  if (started)        // if the application has started then Activate the worm
    Activate();
}

// this is a virtual class that is actually overriden in the subclasses
void Worm::Activate()
{
#ifdef HAVE_QT
  node->Color(Qt::red);
#endif
}

// the place where you put your initialization code
void Worm::Initialize()
{
  cout<<"worm initialize"<<endl;
}

bool Worm::IsInfected()
{
  return infected;
}

void Worm::ConnectionComplete(L4Protocol* proto)
{
}

void Worm::Sent(Count_t c, L4Protocol* proto)
{
}

void Worm::ConnectionFailed(L4Protocol* proto, bool)
{
}

void Worm::CloseRequest(L4Protocol* proto)
{
}

void Worm::Closed(L4Protocol* proto)
{
}

IPAddr_t Worm::GenerateNextIPAddress()
{
  // randomx = randomx * 214013 + 0xffd9613c;
  // I am not following the random number generator of the Slammer worm
  IPAddr_t IP;
  DEBUG0((cout<<"Generating next IP address"<<endl));

  if (!targetV){
    targetV = defaultTV->Copy();
    targetV->Initialize(node->GetIPAddr());
  }

  IP = baseIP + targetV->Generate();

  return IP;
}

void Worm::PrepareWormData(char *&buffer)
{
  buffer = new char[payloadlength];
  for (unsigned int i=0; i < payloadlength; i++)
    buffer[i]='0'+i%10;
  strncpy(buffer, signature.c_str(), signature.length());
  DEBUG(1,(cout<<"WORM CONTENTS: "<<buffer<<endl));
}

void Worm::SetBaseIP(IPAddr_t base)
{
  baseIP = base;
}

void Worm::SetVulnerability(double v)
{
  vulnerability = v;
}

void Worm::SetInfectionPort(PortId_t p)
{
  infectionport = p;
}

void Worm::SetPayloadLength(Size_t l)
{
  payloadlength = l;
}

void Worm::SetTargetVector(const WormTargetVector &targetvector)
{
  defaultTV = targetvector.Copy();
}

void Worm::SetSignature(const string s)
{
  signature = s;
}

void Worm::Receive(Packet *p, L4Protocol *proto, Seq_t)
{
  delete p;
}

// Right now every packet received is a worm
bool Worm::PacketIsWorm(Packet *p)
{
  // for now let's just return true
  return true;

  /*
  char *ptr,*buffer;
  PDU *pdu;
  bool flag = false;
  Size_t size = p->SSize();

  buffer = new char[size];

  DEBUG(1,(cout<<"Entering Packet is worm"<<endl));
  DEBUG(1,(cout<<"Packet size "<<p->Size()<<endl));
  DEBUG(1,(cout<<"PDUs "<<p->PDUs.size()<<endl));

  pdu = p->PeekPDU();

  DEBUG(1,(cout<<"Packet size "<<pdu->Size()<<endl));
  DEBUG(1,(cout<<"Serializable size "<<pdu->SSize()<<endl));

  ptr = pdu->Serialize(buffer, size);
  DEBUG(1,(cout<<"Serialize size "<<size<<endl));


  DEBUG(1,(cout<<"Contents:"<<ptr<<endl));
  for (int i=0; i<pdu->Size(); i++) {
    cout<<ptr[i];
  }
  cout<<endl;

  DEBUG(1,(cout<<"Now comparing..."<<endl));

  if (strncmp(ptr, signature.c_str(), signature.length())==0) {
    DEBUG(1,(cout<<"Worm signature found..."<<endl));
    flag = true;
  }

  delete buffer;
  

  return flag;

  */
}

