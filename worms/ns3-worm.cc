
#include "ns3-worm.h"

bool     Worm::initialized = false;
// this is for setting vulnerabilities
ns3::RandomVariable*  Worm::rngV = NULL;
Time_t   Worm::defaultPatchTime = 1.0;
Size_t   Worm::payloadlength = 376;
PortId_t Worm::infectionport = 1434;
//100% machines are vulnerable, please change the folowing for
// for making host's vulnerability be selected randomly
double   Worm::vulnerability = 1.0;

IPAddr_t Worm::baseIP = ns3::Ipv4Address("192.168.0.0").Get();

Count_t  Worm::totalinfected = 0;
Count_t  Worm::totalvulnerable = 0;
Count_t  Worm::totalinstances = 0;

std::string   Worm::signature = "WORM";
char *   Worm::wormdata = NULL;

//// By default we are setting the target vector to be NULL
//// The code then selects the uniform target vector object by default

//WormTargetVector* Worm::defaultTV = NULL;

// Constructors

Worm::Worm()
  : infected(false), vulnerable(true), started(false),
    patchable(false), patchtime(defaultPatchTime),
    delayinitprobe(false), node(NULL)/*, targetV(NULL)*/
{
  // Whether or not the application object should
  // be copied when connection requests are processed
//  copyOnConnect=false; member variable of GTNets::Application

  if (!initialized) {
    Initialize();
    initialized = true;
  }

  if (!wormdata) {
    PrepareWormData(wormdata);
  }

  if (vulnerability!=1.0)   {
    if (!rngV) rngV = new ns3::UniformVariable(0, 1);
  }

//  // if there is no target vector object defined then we are setting the
//  // it to be the Uniform random target vector class.
//  if (!defaultTV) {
//    defaultTV = new WTVUniform(DEFAULTSCANRANGE);
//  }
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

void Worm::AttachNode(ns3::Node* n)
{
  node = n;
  totalinstances++;

  if (vulnerability!=1.0)
    vulnerable = rngV->GetValue()<=vulnerability;

  if (vulnerable)
    totalvulnerable++;
}

ns3::Application* Worm::Copy() const
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
}

// the place where you put your initialization code
void Worm::Initialize()
{
  std::cout << "worm initialize" << std::endl;
}

bool Worm::IsInfected()
{
  return infected;
}

void Worm::ConnectionComplete(ns3::IpL4Protocol* proto)
{
}

void Worm::Sent(Count_t c, ns3::IpL4Protocol* proto)
{
}

void Worm::ConnectionFailed(ns3::IpL4Protocol* proto, bool)
{
}

void Worm::CloseRequest(ns3::IpL4Protocol* proto)
{
}

void Worm::Closed(ns3::IpL4Protocol* proto)
{
}

IPAddr_t Worm::GenerateNextIPAddress()
{
  // randomx = randomx * 214013 + 0xffd9613c;
  // I am not following the random number generator of the Slammer worm
  IPAddr_t IP;
  std::cout << "Generating next IP address" << std::endl;

//  if (!targetV){
//    targetV = defaultTV->Copy();
//    targetV->Initialize(node->GetIPAddr());
//  }

//  IP = baseIP + targetV->Generate();

  return IP;
}

void Worm::PrepareWormData(char *&buffer)
{
  buffer = new char[payloadlength];
  for (unsigned int i=0; i < payloadlength; i++)
    buffer[i]='0'+i%10;
  strncpy(buffer, signature.c_str(), signature.length());
  std::cout << "WORM CONTENTS: " << buffer << std::endl;
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

//void Worm::SetTargetVector(const WormTargetVector &targetvector)
//{
//  defaultTV = targetvector.Copy();
//}

void Worm::SetSignature(const std::string s)
{
  signature = s;
}

void Worm::Receive(ns3::Packet *p, ns3::IpL4Protocol *proto, Seq_t)
{
  delete p;
}

// Right now every packet received is a worm
bool Worm::PacketIsWorm(ns3::Packet *p)
{
  // for now let's just return true
  return true;

//  /*
//  char *ptr,*buffer;
//  PDU *pdu;
//  bool flag = false;
//  Size_t size = p->SSize();

//  buffer = new char[size];

//  DEBUG(1,(std::cout << "Entering Packet is worm" << std::endl));
//  DEBUG(1,(std::cout << "Packet size " << p->Size() << std::endl));
//  DEBUG(1,(std::cout << "PDUs " << p->PDUs.size() << std::endl));

//  pdu = p->PeekPDU();

//  DEBUG(1,(std::cout << "Packet size "<< pdu->Size() << std::endl));
//  DEBUG(1,(std::cout << "Serializable size " << pdu->SSize() << std::endl));

//  ptr = pdu->Serialize(buffer, size);
//  DEBUG(1,(std::cout << "Serialize size " << size << std::endl));


//  DEBUG(1,(std::cout << "Contents:" << ptr << std::endl));
//  for (int i=0; i<pdu->Size(); i++) {
//    cout<<ptr[i];
//  }
//  cout<<endl;

//  DEBUG(1,(std::cout << "Now comparing..." << std::endl));

//  if (strncmp(ptr, signature.c_str(), signature.length())==0) {
//    DEBUG(1,(std::cout << "Worm signature found..." << std::endl));
//    flag = true;
//  }

//  delete buffer;


//  return flag;

//  */
}
