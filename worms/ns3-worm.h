
#ifndef WORM_APPLICATION_H
#define WORM_APPLICATION_H

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/application.h"
#include "ns3/address.h"
#include "ns3/internet-module.h"
#include "ns3/rng-stream.h"

// Typedefs from GTNets
typedef unsigned long Size_t;
typedef unsigned long Count_t;
typedef unsigned long PortId_t;
typedef unsigned long Seq_t;
typedef uint32_t IPAddr_t; //typedef unsigned long IPAddr_t;
typedef double Time_t;

/**
 * \class Worm Worm.h
 * \brief Worm application
 */
class Worm : public ns3::Application
{
public:
    /**
     * \brief Constructs worm application object
     */
    Worm();

    virtual void AttachNode(ns3::Node*); // Note which node attached to
    virtual ~Worm() {};
    virtual ns3::Application* Copy() const;// Make a copy of the application

    // newly defined in this class
    virtual void Initialize();

    void Infect();
    bool IsInfected();

    ns3::Node* AttachedNode();

    virtual void SetPatchTime(Time_t t)
    {
        patchtime = t;
    }

    virtual void SetPatchable(bool b)
    {
        patchable = b;
    }

    static void SetPayloadLength(Size_t);
    static void SetBaseIP(IPAddr_t );
    static void SetScanRange(Count_t);

    static void SetVulnerability(double);
    static void SetInfectionPort(PortId_t);
//    static void SetTargetVector(const WormTargetVector &);
    static void SetSignature(std::string s);

    static Count_t  TotalInfected() {return totalinfected;};
    static Count_t  TotalVulnerable() {return totalvulnerable;};
    static Count_t  TotalInstances() {return totalinstances;};

    void DelayInitProbe(bool b) { delayinitprobe = b;}


protected:
    bool          infected;      // indicates if this node is in infected stage
    bool          vulnerable;
    bool          started;       // used to know if the application of worm has started
    bool          patchable;     // Can this worm be patched
    Time_t        patchtime;     // Time before the worm application is patched
    bool          delayinitprobe; // Delay the initial probe after infection

    IPAddr_t GenerateNextIPAddress();
    bool PacketIsWorm(ns3::Packet *);   // I am going to use this to check if packet
                                        // is worm
    virtual void PrepareWormData(char *&);

    virtual void Activate();
    virtual void StartApp();    // Called at time specified by Start
    virtual void StopApp();     // Called at time specified by Stop

    ns3::Node* node;

    static Time_t defaultPatchTime;
    static Size_t payloadlength;
    static PortId_t infectionport;
    static double vulnerability;
    static IPAddr_t baseIP;
    static std::string signature;    // signature string that is sent in packets
    static char* wormdata;           // the contents of the worm  (this is sent usually sent)

    virtual void Receive(ns3::Packet *, ns3::Ipv4L4Protocol/*L4Protocol*/*, Seq_t); // Data received
    virtual void Sent(Count_t , ns3::Ipv4L4Protocol*);
    virtual void ConnectionComplete(ns3::Ipv4L4Protocol*);
    virtual void ConnectionFailed(ns3::Ipv4L4Protocol*, bool);
    virtual void CloseRequest(ns3::Ipv4L4Protocol*);
    virtual void Closed(ns3::Ipv4L4Protocol*);

private:
    static Count_t  totalinfected;    // keeps count of total infected instances
    static Count_t  totalvulnerable;  // keeps count of total vulnerable instances
    static Count_t  totalinstances;

//    WormTargetVector *targetV;
//    static WormTargetVector *defaultTV;

    static bool initialized;  // set to see if worm class was initialized
    static ns3::RandomVariable* rngV;
};

#endif // WORM_APPLICATION_H
