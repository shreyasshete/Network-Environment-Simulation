// Name: Shreyas Jagadeep Shete
// UTA ID: 1001888859

// Importing Libraries
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/network-module.h" 
#include "ns3/netanim-module.h" 
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/mobility-module.h"


// Implmentation of Topology Code
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TCPprotocol");
void CalculateThroughput();
void averageThroughput();
FlowMonitorHelper flowHelper;
Ptr<FlowMonitor> flowMonitor;

// Network Simulation Part
int main (int argc, char *argv[]) 
{
    CommandLine cmd;
    cmd.Parse(argc, argv);
    uint32_t maxBytes = 0;
    
    Time::SetResolution (Time::NS);
    LogComponentEnable ("UdpEchoClientApplication",LOG_LEVEL_INFO);
    LogComponentEnable ("UdpEchoServerApplication",LOG_LEVEL_INFO);
     //cmd lines 

//Building Topology

   //generate nodes 
  NodeContainer nodes;
  nodes.Create(5);

        //connect nodes 
NodeContainer n0n4 = NodeContainer(nodes.Get(0), nodes.Get(4));
NodeContainer n1n3 = NodeContainer(nodes.Get(1), nodes.Get(3));
NodeContainer n2n3 = NodeContainer(nodes.Get(2), nodes.Get(3));
NodeContainer n3n4 = NodeContainer(nodes.Get(3), nodes.Get(4));
 //Link Capacity Setting 
 
        PointToPointHelper point2point;
        point2point.SetDeviceAttribute("DataRate", StringValue("100Mbps"));
        point2point.SetChannelAttribute("Delay", StringValue("0.5ms"));

        NetDeviceContainer d0d4 = point2point.Install(n0n4); 
        NetDeviceContainer d1d3 = point2point.Install(n1n3); 
        NetDeviceContainer d2d3 = point2point.Install(n2n3); 
        NetDeviceContainer d3d4 = point2point.Install(n3n4); 
       
        InternetStackHelper stack; 
        stack.Install (nodes); 
 
        //assign address 
        Ipv4AddressHelper address;
		address.SetBase ("10.1.1.0", "255.255.255.0");
                Ipv4InterfaceContainer i0i4 = address.Assign(d0d4);
		address.SetBase ("10.1.2.0", "255.255.255.0");
                 Ipv4InterfaceContainer i1i3 = address.Assign(d1d3);
		address.SetBase ("10.1.3.0", "255.255.255.0");
                Ipv4InterfaceContainer i2i3 = address.Assign(d2d3);
		address.SetBase ("10.1.4.0", "255.255.255.0");
                Ipv4InterfaceContainer i3i4 = address.Assign(d3d4);
		//generate routing table 
        
        NS_LOG_INFO("Routing Table.");
        Ipv4GlobalRoutingHelper::PopulateRoutingTables();
        uint16_t sinkport = 8081;
        Address sinkAddress(InetSocketAddress(i0i4.GetAddress(1),sinkport));
        PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory",InetSocketAddress(Ipv4Address::GetAny(),sinkport));
        ApplicationContainer sinkApps = packetSinkHelper.Install(nodes.Get(4));
        sinkApps.Start(Seconds(0.));
        sinkApps.Stop(Seconds(100.));

        
        //flow setting 
        
      uint16_t bulkport = 8082;
        BulkSendHelper source("ns3::TcpSocketFactory",InetSocketAddress(i0i4.GetAddress(1),bulkport));
        source.SetAttribute("MaxBytes" , UintegerValue(maxBytes));
        ApplicationContainer sourceApp0 = source.Install(nodes.Get(0));
        sourceApp0.Start(Seconds(0.0));
        sourceApp0.Stop(Seconds(15.0));
	
	BulkSendHelper source1("ns3::TcpSocketFactory",InetSocketAddress(i1i3.GetAddress(1),8083));
        source1.SetAttribute("MaxBytes" , UintegerValue(maxBytes));
        ApplicationContainer sourceApp1 = source1.Install(nodes.Get(1));
        sourceApp1.Start(Seconds(0.0));
        sourceApp1.Stop(Seconds(15.0));
        
	BulkSendHelper source2("ns3::TcpSocketFactory",InetSocketAddress(i3i4.GetAddress(1),8083));
        source2.SetAttribute("MaxBytes" , UintegerValue(maxBytes));
        ApplicationContainer sourceApp2 = source2.Install(nodes.Get(1));
        sourceApp2.Start(Seconds(0.0));
        sourceApp2.Stop(Seconds(15.0));
        
        BulkSendHelper source3("ns3::TcpSocketFactory",InetSocketAddress(i2i3.GetAddress(1),8084));
        source3.SetAttribute("MaxBytes" , UintegerValue(maxBytes));
        ApplicationContainer sourceApp3 = source3.Install(nodes.Get(2));
        sourceApp3.Start(Seconds(3.0));
        sourceApp3.Stop(Seconds(15.0));
        
        BulkSendHelper source4("ns3::TcpSocketFactory",InetSocketAddress(i3i4.GetAddress(1),8084));
        source4.SetAttribute("MaxBytes" , UintegerValue(maxBytes));
        ApplicationContainer sourceApp4 = source4.Install(nodes.Get(2));
        sourceApp4.Start(Seconds(3.0));
        sourceApp4.Stop(Seconds(15.0));
        
        BulkSendHelper source5("ns3::TcpSocketFactory",InetSocketAddress(i2i3.GetAddress(1),8085));
        source5.SetAttribute("MaxBytes" , UintegerValue(maxBytes));
        ApplicationContainer sourceApp5 = source5.Install(nodes.Get(3));
        sourceApp5.Start(Seconds(7.0));
        sourceApp5.Stop(Seconds(12.0));
       

        BulkSendHelper source6("ns3::TcpSocketFactory",InetSocketAddress(i3i4.GetAddress(1),8085));
        source6.SetAttribute("MaxBytes" , UintegerValue(maxBytes));
        ApplicationContainer sourceApp6 = source6.Install(nodes.Get(3));
        sourceApp6.Start(Seconds(7.0));
        sourceApp6.Stop(Seconds(12.0));
        

		//sink deploy 

          
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);
  AnimationInterface anim ("test.xml");   


 Ptr<ConstantPositionMobilityModel> s1 = nodes.Get (0)->GetObject<ConstantPositionMobilityModel> ();
  Ptr<ConstantPositionMobilityModel> s2 = nodes.Get (1)->GetObject<ConstantPositionMobilityModel> ();
   Ptr<ConstantPositionMobilityModel> s3 = nodes.Get (2)->GetObject<ConstantPositionMobilityModel> ();
  Ptr<ConstantPositionMobilityModel> s4= nodes.Get (3)->GetObject<ConstantPositionMobilityModel> ();
   Ptr<ConstantPositionMobilityModel> s5 = nodes.Get (4)->GetObject<ConstantPositionMobilityModel> ();

  s1->SetPosition (Vector ( 24.0, 15.0, 0  ));//A
  s2->SetPosition (Vector ( 24.0, 45.0, 0  ));//B
  s3->SetPosition (Vector ( 24.0, 70.0, 0  ));//C
  s4->SetPosition (Vector ( 48.0, 45.0, 0  ));//D
  s5->SetPosition (Vector ( 75.0, 45.0, 0  ));//E


  anim.UpdateNodeDescription(0, "A");
  anim.UpdateNodeDescription(1, "B");
    anim.UpdateNodeDescription(2, "C");
  anim.UpdateNodeDescription(3,"D");
    anim.UpdateNodeDescription(4, "E");
 
 NS_LOG_INFO("RUN Simulation.");
 Simulator::Stop(Seconds(11.0));
  Simulator::Run ();
    Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}

// Animate Performance
        //animation generate 
       // node number, x-value, y-value
       
void CalculateThroughput() 
{ 
	//float througput=0;
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowHelper.GetClassifier ());
  	std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats ();
        for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin(); 
		i != stats.end (); ++i) 
        { 
        Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
     std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
     std::cout << "  Tx Bytes: " << i->second.txBytes << std::endl;
     std::cout << "  Rx Bytes: " << i->second.rxBytes << std::endl;
     std::cout << "  Lost Packets: " << i->second.lostPackets << std::endl;


     std::cout << "  Throughput: " << (( ((double)i->second.rxBytes*8)/(1000000) ) / .5) << std::endl;
          } 

        Simulator::Schedule (MilliSeconds (100), &CalculateThroughput); 
}


// Average Throughput calculation       
void averageThroughput() 
{ 
        
        Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowHelper.GetClassifier ());
  	std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats ();
        for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin ();
		i != stats.end (); ++i) 
        { 
                Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first); 
                std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n"; 
                std::cout << "  Tx Packets: " << i->second.txPackets << "\n"; 
                std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n"; 
                std::cout << "  TxOffered:  " << i->second.txBytes * 8.0 / 
(i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) / 1000 / 1000  << " Mbps\n";
                std::cout << "  Rx Packets: " << i->second.rxPackets << "\n"; 
                std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n"; 
                std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / 
(i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) / 1000 / 1000  << " Mbps\n"; 
                double totalthroughput = totalthroughput + ( i->second.rxBytes * 8.0 / 
(i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) / 1000 / 1000 ) ; 
        }  
        
} 

