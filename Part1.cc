 #include <bits/stdc++.h>
 #include "ns3/core-module.h"
 #include "ns3/internet-module.h"
 #include "ns3/csma-module.h"
 #include "ns3/internet-apps-module.h"
 #include "ns3/ipv6-static-routing-helper.h"
 #include "ns3/network-module.h"
 #include "ns3/point-to-point-module.h"
 #include "ns3/ipv4-global-routing-helper.h"
 #include "ns3/applications-module.h"
 #include "ns3/ipv6-routing-table-entry.h"
 #include "ns3/flow-monitor-helper.h"
 #include "ns3/flow-monitor-module.h"
 #include <fstream>
 #include "ns3/applications-module.h"
 #include "ns3/error-model.h"
 #include "ns3/tcp-header.h"
 #include "ns3/udp-header.h"
 #include "ns3/enum.h"
 #include "ns3/event-id.h"
 #include "ns3/flow-monitor-helper.h"
 #include "ns3/ipv4-global-routing-helper.h"
 #include "ns3/traffic-control-module.h"

using namespace ns3;
 NS_LOG_COMPONENT_DEFINE ("TcpVariantsComparison");

double mx_throughput_hybla =0,mx_throughput_westwood=0,mx_throughput_yeah=0;
AsciiTraceHelper throughput;
bool firstcWndHybla = true ,firstcWndWestwood = true, firstcWndYeah = true;
int firstthroughputhybla=-1,firstthroughputwestwood=-1,firstthroughputyeah = -1;


//declaring output stream object for storing congestion window, throughput and goodput of all tcp variants
static Ptr<OutputStreamWrapper> cWndHyblaStream, cWndWestwoodStream, cWndYeahStream;
static Ptr<OutputStreamWrapper> throughputHyblaStream, throughputWestwoodStream, throughputYeahStream, goodputHyblaStream, goodputWestwoodStream, goodputYeahStream ;
static uint32_t cWndHyblaValue, cWndWestwoodValue, cWndYeahValue;



// prints congestion window data into corresponding output stream fro TCP Hybla
 static void
 CwndHyblaTracer (uint32_t oldval, uint32_t newval)
 {
   if(firstcWndHybla) {
   	*cWndHyblaStream->GetStream () << Simulator::Now ().GetSeconds () << " " << oldval << std::endl;
	firstcWndHybla = false;
   }
   *cWndHyblaStream->GetStream () << Simulator::Now ().GetSeconds () << " " << newval << std::endl;
   cWndHyblaValue = newval;
 }

// prints congestion window data into corresponding output stream fro TCP Westwood
 static void
 CwndWestwoodTracer (uint32_t oldval, uint32_t newval)
 {
   if(firstcWndWestwood) {
   	*cWndWestwoodStream->GetStream () << Simulator::Now ().GetSeconds () << " " << oldval << std::endl;
	firstcWndWestwood = false;
   }
   cWndWestwoodValue = newval;
   *cWndWestwoodStream->GetStream () << Simulator::Now ().GetSeconds () << " " << newval << std::endl;

 }

// prints congestion window data into corresponding output stream fro TCP Yeah
 static void
 CwndYeahTracer (uint32_t oldval, uint32_t newval)
 {
   if(firstcWndYeah) {
   	*cWndYeahStream->GetStream () << Simulator::Now ().GetSeconds () << " " << oldval << std::endl;
	firstcWndYeah = false;
   }
   cWndYeahValue = newval;
   *cWndYeahStream->GetStream () << Simulator::Now ().GetSeconds () << " " << newval << std::endl;

 }


//intitializing output stream object for storing congestion window data of TCP Hybla
 static void
 TraceHyblaCwnd ()
 {
   AsciiTraceHelper ascii;
   cWndHyblaStream = ascii.CreateFileStream ("TcpHybla-cwnd.data");
   Config::ConnectWithoutContext ("/NodeList/0/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow", MakeCallback (&CwndHyblaTracer));
}

//intitializing output stream object for storing congestion window data of TCP Westood
 static void
 TraceWestwoodCwnd ()
 {
   AsciiTraceHelper ascii;
   cWndWestwoodStream = ascii.CreateFileStream ("TcpWestwood-cwnd.data");
   Config::ConnectWithoutContext ("/NodeList/1/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow", MakeCallback (&CwndWestwoodTracer));
}

//intitializing output stream object for storing congestion window data of TCP Yeah
 static void
 TraceYeahCwnd ()
 {
   AsciiTraceHelper ascii;
   cWndYeahStream = ascii.CreateFileStream ("TcpYeah-cwnd.data");
   Config::ConnectWithoutContext ("/NodeList/2/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow", MakeCallback (&CwndYeahTracer));
}



// print goodput and throughput data which we get from flow Monitor into corresponding output streams
 static void
 putTracer (Ptr<FlowMonitor> monitor, Ptr<Ipv4FlowClassifier> classifier, double HyblaStopTime, double WestwoodStopTime, double YeahStopTime)
 {


 	// get flow Monitor stats
    	monitor->CheckForLostPackets ();
   	FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();
   	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
     	{
       		Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
       		if ((t.sourceAddress)== "10.0.0.1" && Simulator::Now ().GetSeconds () <= HyblaStopTime )
         	{
         		//pass goodput and throughput into output stream for TCP Hybla
         		//update maximum throughput
         		
         		if(firstthroughputhybla>0){
	         		*goodputHyblaStream->GetStream() << Simulator::Now ().GetSeconds ()<< " "<< i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds()-i->second.timeFirstTxPacket.GetSeconds()) / 1024  <<std::endl;
	       			*throughputHyblaStream->GetStream() << Simulator::Now ().GetSeconds ()<< " "<< i->second.txBytes * 8.0 / (i->second.timeLastTxPacket.GetSeconds()-i->second.timeFirstTxPacket.GetSeconds()) / 1024  <<std::endl;
	       			double thpt =  i->second.txBytes * 8.0 / (i->second.timeLastTxPacket.GetSeconds()-i->second.timeFirstTxPacket.GetSeconds()) / 1024  ;
	       			mx_throughput_hybla = std::max(mx_throughput_hybla,thpt);
       			}
       			firstthroughputhybla ++;
         	}

       		if ((t.sourceAddress)== "10.1.1.1" && Simulator::Now ().GetSeconds () <= WestwoodStopTime)
         	{
         		//pass goodput and throughput into output stream for TCP Westwood
         		//update maximum throughput
         		if(firstthroughputwestwood>0){
	       			*goodputWestwoodStream->GetStream() << Simulator::Now ().GetSeconds ()<< " "<< i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds()-i->second.timeFirstTxPacket.GetSeconds()) / 1024  <<std::endl;
	       			*throughputWestwoodStream->GetStream() << Simulator::Now ().GetSeconds ()<< " "<< i->second.txBytes * 8.0 / (i->second.timeLastTxPacket.GetSeconds()-i->second.timeFirstTxPacket.GetSeconds()) / 1024  <<std::endl;
	       			double thpt =  i->second.txBytes * 8.0 / (i->second.timeLastTxPacket.GetSeconds()-i->second.timeFirstTxPacket.GetSeconds()) / 1024  ;
	       			mx_throughput_westwood = std::max(mx_throughput_westwood,thpt);
       			}
       			firstthroughputwestwood ++;
         	}

       		if ((t.sourceAddress)== "10.2.2.1" && Simulator::Now ().GetSeconds () <= YeahStopTime)
         	{
         		//pass goodput and throughput into output stream for TCP Yeah
         		//update maximum throughput
         		if(firstthroughputyeah>0){
	       			*goodputYeahStream->GetStream() << Simulator::Now ().GetSeconds ()<< " "<< i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds()-i->second.timeFirstTxPacket.GetSeconds()) / 1024  <<std::endl;
	       			*throughputYeahStream->GetStream() << Simulator::Now ().GetSeconds ()<< " "<< i->second.txBytes * 8.0 / (i->second.timeLastTxPacket.GetSeconds()-i->second.timeFirstTxPacket.GetSeconds()) / 1024  <<std::endl;
	         		double thpt =  i->second.txBytes * 8.0 / (i->second.timeLastTxPacket.GetSeconds()-i->second.timeFirstTxPacket.GetSeconds()) / 1024  ;
	       			mx_throughput_yeah = std::max(mx_throughput_yeah,thpt );
       			}
       			firstthroughputyeah ++;
         	}
	}
    Simulator::Schedule (Seconds(0.1), &putTracer, monitor, classifier , HyblaStopTime, WestwoodStopTime, YeahStopTime);
 }

int main(int argc, char *argv[])
{

	uint64_t data_mbytes = 0;
	uint32_t mtu_bytes = 1536;	// size of packet to be sent = 1.5KB = 1.5*1025 = 1536
	bool sack = true;
	std::string recovery = "ns3::TcpClassicRecovery";
	double error_p = 0.0000;

	LogComponentEnable("TcpVariantsComparison", LOG_LEVEL_ALL);
   	LogComponentEnable("BulkSendApplication", LOG_LEVEL_INFO);


   	// getting size of header
	Header* temp_header = new Ipv4Header ();
   	uint32_t ip_header = temp_header->GetSerializedSize ();
   	NS_LOG_LOGIC ("IP Header size is: " << ip_header);
   	delete temp_header;
   	temp_header = new TcpHeader ();
   	uint32_t tcp_header = temp_header->GetSerializedSize ();
   	NS_LOG_LOGIC ("TCP Header size is: " << tcp_header);
   	delete temp_header;
   	uint32_t tcp_adu_size = mtu_bytes - 20 - (ip_header + tcp_header);
   	NS_LOG_LOGIC ("TCP ADU size is: " << tcp_adu_size);


   	// configuring default buffer size for receiver and sender
	Config::SetDefault ("ns3::TcpSocket::RcvBufSize", UintegerValue (1 << 21));
   	Config::SetDefault ("ns3::TcpSocket::SndBufSize", UintegerValue (1 << 21));
 	Config::SetDefault ("ns3::TcpSocketBase::Sack", BooleanValue (sack));


 	//creating pointer for each of the nodes
	// senders -> H1:n0 , H2:n1 , H3:n2
	//routers -> R1:r1, R2:r2
	//receivers -> H4:n3, H5:n4,H6:n5
	Ptr<Node> n0 = CreateObject<Node> ();
	Ptr<Node> n1 = CreateObject<Node> ();
	Ptr<Node> n2 = CreateObject<Node> ();
	Ptr<Node> r1 = CreateObject<Node> ();
	Ptr<Node> r2 = CreateObject<Node> ();
	Ptr<Node> n3 = CreateObject<Node> ();
	Ptr<Node> n4 = CreateObject<Node> ();
	Ptr<Node> n5 = CreateObject<Node> ();
	
	//creating nodecontainer to contain each pair of nodes in topology
	NodeContainer net0 (n0, r1);
	NodeContainer net1 (n1, r1);	//sender to router
	NodeContainer net2 (n2, r1);


	NodeContainer netr (r1, r2);	// router to router
	
	NodeContainer net3 (r2, n3);
	NodeContainer net4 (r2, n4);	// router to receiver
	NodeContainer net5 (r2, n5);

	
	//setting up error model
	Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
   	uv->SetStream (50);
   	RateErrorModel error_model;
   	error_model.SetRandomVariable (uv);
   	error_model.SetUnit (RateErrorModel::ERROR_UNIT_PACKET);
   	error_model.SetRate (error_p);


   	// creating pointtopoint helper for each type of link in our topology
   	// p2p 100Mbps , 20ms sender to router and router to receiver
   	// p2r 10Mbps , 50ms router to router
   	// setting queue size of droptail queue
	PointToPointHelper p2p, p2r;

	uint packetSize = 1.5*1024 *8 ;		// 1.5 KB in bits	
	uint queueSizeHR = (100000*20)/packetSize;	//host to router
	uint queueSizeRR = (10000*50)/packetSize;	//router to router

	std::string MX_len_HR = std::to_string(queueSizeHR);
	std::string MX_len_RR = std::to_string(queueSizeRR);
	MX_len_HR += "p";
	MX_len_RR += "p";
	
	p2r.SetQueue ("ns3::DropTailQueue<Packet>", "MaxSize", QueueSizeValue (QueueSize (MX_len_RR)));

	
	p2p.SetDeviceAttribute ("DataRate", StringValue("100Mbps"));
	p2p.SetChannelAttribute ("Delay", StringValue("20ms"));
	p2r.SetDeviceAttribute ("DataRate", StringValue("10Mbps"));
	p2r.SetChannelAttribute ("Delay", StringValue("50ms"));
	p2p.SetDeviceAttribute ("ReceiveErrorModel", PointerValue(&error_model));
	p2r.SetDeviceAttribute ("ReceiveErrorModel", PointerValue(&error_model));


	// installing point to point net devices to each of the node container
	NetDeviceContainer dev0 = p2p.Install (net0);
	NetDeviceContainer dev1 = p2p.Install (net1);
	NetDeviceContainer dev2 = p2p.Install (net2);
	NetDeviceContainer devr = p2r.Install (netr);
	NetDeviceContainer dev3 = p2p.Install (net3);
    NetDeviceContainer dev4 = p2p.Install (net4);
	NetDeviceContainer dev5 = p2p.Install (net5);

	//helper to install internet stack on nodes of topology
	InternetStackHelper internet;

	//installing TCP Hybla on H1(n0) - R1 and R2 - H4(n3)
	Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TypeId::LookupByName ("ns3::TcpHybla")));
	internet.Install(n0);
	internet.Install(n3);

	//installing TCP Westwood+ on H2(n1) - R1 and R2 - H5(n4)
	Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpWestwood::GetTypeId ()));
	Config::SetDefault ("ns3::TcpWestwood::ProtocolType", EnumValue (TcpWestwood::WESTWOODPLUS));
	internet.Install(n1);
	internet.Install(n4);

	//installing TCP Yeah on H3(n2) - R1 and R2 - H4(n5)
	Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TypeId::LookupByName ("ns3::TcpYeah")));
	internet.Install(n2);
	internet.Install(n5);

	internet.Install(r1);
	internet.Install(r2);


	// set base, mask to assign IP to nodes	
	Ipv4AddressHelper ipv4;

	ipv4.SetBase ("10.0.0.0", "255.255.255.0");
	Ipv4InterfaceContainer iface0 = ipv4.Assign (dev0);

	ipv4.SetBase ("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer iface1 = ipv4.Assign (dev1);

	ipv4.SetBase ("10.2.2.0", "255.255.255.0");
	Ipv4InterfaceContainer iface2 = ipv4.Assign (dev2);

	ipv4.SetBase ("10.3.3.0", "255.255.255.0");
	Ipv4InterfaceContainer iface3 = ipv4.Assign (dev3);

	ipv4.SetBase ("10.4.4.0", "255.255.255.0");
	Ipv4InterfaceContainer iface4 = ipv4.Assign (dev4);

	ipv4.SetBase ("10.5.5.0", "255.255.255.0");
	Ipv4InterfaceContainer iface5 = ipv4.Assign (dev5);

	ipv4.SetBase("10.6.6.0", "255.255.255.0");
	Ipv4InterfaceContainer ifacer = ipv4.Assign (devr);

	// Build a routing database and initialize the routing tables of the nodes in the simulation
	Config::SetDefault ("ns3::Ipv4GlobalRouting::RespondToInterfaceEvents", BooleanValue (true));
	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();


	// set server port to 50000
	uint16_t servPort = 50000;

	// configuring PacketSinkHelper application to install on receiver with their corresponding TCP size
	// to receive TCP connection and data
	PacketSinkHelper sink ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), servPort));
	sink.SetAttribute ("Protocol", TypeIdValue (TcpSocketFactory::GetTypeId ()));
	
	// installing packetsinkhelper appliaction on H4:n3 on TCP Hybla
	ApplicationContainer serverApps3 = sink.Install(n3);

    // Setting the destination address for the sender
    AddressValue remoteAddress3 (InetSocketAddress (iface3.GetAddress(1), servPort));
	Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(tcp_adu_size));
	
	// configuring BulkSendHelper which sends data as fast as possible up to MaxBytes or until the application is stopped
	BulkSendHelper ftp0 ("ns3::TcpSocketFactory", Address());
	ftp0.SetAttribute("Remote", remoteAddress3);
	ftp0.SetAttribute("SendSize", UintegerValue(tcp_adu_size));
	ftp0.SetAttribute("MaxBytes", UintegerValue(data_mbytes * 1000));


	//installing BulkSend application on H1:n0
	ApplicationContainer clientApp0 = ftp0.Install(n0);

	//similar installation for H5:n4
	ApplicationContainer serverApps4 = sink.Install(n4);

    AddressValue remoteAddress4 (InetSocketAddress (iface4.GetAddress(1), servPort));
	Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(tcp_adu_size));
	BulkSendHelper ftp1 ("ns3::TcpSocketFactory", Address());
	ftp1.SetAttribute("Remote", remoteAddress4);
	ftp1.SetAttribute("SendSize", UintegerValue(tcp_adu_size));
	ftp1.SetAttribute("MaxBytes", UintegerValue(data_mbytes * 1000));

	//similar installation for H2:n1	
	ApplicationContainer clientApp1 = ftp1.Install(n1);

	//similar installation for H4:n5
	ApplicationContainer serverApps5 = sink.Install(n5);

    AddressValue remoteAddress5 (InetSocketAddress (iface5.GetAddress(1), servPort));
	Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(tcp_adu_size));
	BulkSendHelper ftp2 ("ns3::TcpSocketFactory", Address());
	ftp2.SetAttribute("Remote", remoteAddress5);
	ftp2.SetAttribute("SendSize", UintegerValue(tcp_adu_size));
	ftp2.SetAttribute("MaxBytes", UintegerValue(data_mbytes * 1000));

	ApplicationContainer clientApp2 = ftp2.Install(n2);

	// Setting start and stop time for our application on receiver nodes
	serverApps3.Start (Seconds(1.0));
	serverApps3.Stop(Seconds(360.0));

	serverApps4.Start (Seconds(1.0));
	serverApps4.Stop(Seconds(360.0));

	serverApps5.Start (Seconds(1.0));
	serverApps5.Stop(Seconds(360.0));

	clientApp0.Start(Seconds(552.0));
	clientApp0.Stop(Seconds(597.0));

	clientApp1.Start(Seconds(550.0));
	clientApp1.Stop(Seconds(550));

	clientApp2.Start(Seconds(552.0));
	clientApp2.Stop(Seconds(555));


	// Setting start and stop time for our application on sender nodes
	// starting the a flow 1 sec after previous flow finished
	std::string node = "0";
	clientApp0.Start(Seconds(2.0));		clientApp0.Stop(Seconds(120.0));

	clientApp1.Start(Seconds(121.0));	clientApp1.Stop(Seconds(240.0));

	clientApp2.Start(Seconds(242.0));	clientApp2.Stop(Seconds(360.0));

	// scheduling the time when the corresponding function will be called
	// just after sender starts sending packets
    Simulator::Schedule (Seconds (2.00001), &TraceHyblaCwnd);
    Simulator::Schedule (Seconds (121.00001), &TraceWestwoodCwnd);
    Simulator::Schedule (Seconds (242.00001), &TraceYeahCwnd);

    //
	FlowMonitorHelper flowmon;
   	Ptr<FlowMonitor> monitor = flowmon.InstallAll ();

   	NS_LOG_INFO ("Run Simulation.");

   	monitor->CheckForLostPackets ();
   	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
   	FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();

   	// intializing output stream object for throughput data
  	throughputHyblaStream = throughput.CreateFileStream ("TcpHybla-throughput.data");
  	throughputWestwoodStream = throughput.CreateFileStream ("TcpWestwood-throughput.data");
  	throughputYeahStream = throughput.CreateFileStream ("TcpYeah-throughput.data");

  	// intializing output streney has been distributed to the washerman, stationary guy, canteen workers anam object for goodput data
  	goodputHyblaStream = throughput.CreateFileStream ("TcpHybla-goodput.data");
  	goodputWestwoodStream = throughput.CreateFileStream ("TcpWestwood-goodput.data");
  	goodputYeahStream = throughput.CreateFileStream ("TcpYeah-goodput.data");
	
	//scheduling putTracer function to start as soon as we begin sending packets
	// and end at corresponding stop times for each of the TCP variants
	Simulator::Schedule (Seconds (2.00001), &putTracer, monitor, classifier, 120.0, 240.0, 360.0);
   	Simulator::Stop (Seconds (362));
   	Simulator::Run ();
   	// get monitor flow stats
	stats = monitor->GetFlowStats();

	//displaying all the monitor flow stats for each of the flow
	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
     	{
       		Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
      		std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
       		std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
       		std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
       		std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
       		std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
       		std::cout << "  Goodput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds()-i->second.timeFirstTxPacket.GetSeconds()) / 1024 / 1024  << " Mbps\n";
		std::cout << "  Packets Lost "<< i->second.lostPackets << "\n";
     	}
     std::cout<<"Max throughput for TCP Hybla: "<< mx_throughput_hybla <<"\n";
     std::cout<<"Max throughput for TCP Westwood: "<< mx_throughput_westwood <<"\n";
     std::cout<<"Max throughput for TCP Yeah: "<< mx_throughput_yeah <<"\n";
     	
	Simulator::Destroy();
	return 0;

}


