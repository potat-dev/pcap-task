#include <iostream>
#include <IPv4Layer.h>
#include <UdpLayer.h>
#include <TcpLayer.h>
#include <Packet.h>
#include <PcapFileDevice.h>
#include <PcapLiveDeviceList.h>

std::string getProtocolTypeAsString(pcpp::ProtocolType protocolType)
{
	switch (protocolType)
	{
	case pcpp::Ethernet:
		return "Ethernet";
	case pcpp::IPv4:
		return "IPv4";
	case pcpp::TCP:
	case pcpp::UDP:
		return "TCP";
	case pcpp::HTTPRequest:
	case pcpp::HTTPResponse:
		return "HTTP";
	default:
		return "Unknown";
	}
}

int main(int argc, char *argv[])
{
	// if (argc < 2)
	// {
	// 	std::cout << "Usage: app <file.pcap>" << std::endl;
	// 	return 0;
	// }
	// open a pcap file for reading
	// pcpp::PcapFileReaderDevice reader(argv[1]);

	// pcpp::PcapLiveDevice
	// pcpp::PcapLiveDevice* dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp(interfaceIPAddr);
	std::vector<pcpp::PcapLiveDevice *> devices = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();

	for (pcpp::PcapLiveDevice *dev : devices)
	{
		std::cout
			<< "Interface info:" << std::endl
			<< "   Interface name:        " << dev->getName() << std::endl			 // get interface name
			<< "   Interface description: " << dev->getDesc() << std::endl			 // get interface description
			<< "   MAC address:           " << dev->getMacAddress() << std::endl	 // get interface MAC address
			<< "   Default gateway:       " << dev->getDefaultGateway() << std::endl // get default gateway
			<< "   Interface MTU:         " << dev->getMtu() << std::endl;			 // get interface MTU
	}

	return 0;
	// pcpp::PcapLiveDevice *reader = devices[0];
	pcpp::PcapFileReaderDevice reader(argv[1]);

	if (!reader.open())
	{
		std::cerr << "Error opening the pcap file" << std::endl;
		return 1;
	}

	pcpp::RawPacket rawPacket;

	while (reader.getNextPacket(rawPacket))
	{
		auto length = rawPacket.getRawDataLen();
		// parse the raw packet into a parsed packet
		pcpp::Packet parsedPacket(&rawPacket);

		std::cout << "--------------------\n"
				  << std::endl
				  << "Packet size: " << length << "\n"
				  << std::endl;

		// first let's go over the layers one by one and find out its type, its total length, its header length and its payload length
		for (pcpp::Layer *curLayer = parsedPacket.getFirstLayer(); curLayer != NULL; curLayer = curLayer->getNextLayer())
		{
			std::cout
				<< "Layer type: " << getProtocolTypeAsString(curLayer->getProtocol()) << "; " // get layer type
				<< "Total data: " << curLayer->getDataLen() << " [bytes]; "					  // get total length of the layer
				<< "Layer data: " << curLayer->getHeaderLen() << " [bytes]; "				  // get the header length of the layer
				<< "Layer payload: " << curLayer->getLayerPayloadSize() << " [bytes]"		  // get the payload length of the layer (equals total length minus header length)
				<< std::endl;
		}

		// verify the packet is IPv4
		if (parsedPacket.isPacketOfType(pcpp::IPv4))
		{
			// extract source and dest IPs
			pcpp::IPv4Address srcIP = parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getSrcIPv4Address();
			pcpp::IPv4Address dstIP = parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getDstIPv4Address();

			auto srcPort = parsedPacket.getLayerOfType<pcpp::TcpLayer>()->getSrcPort();
			auto dstPort = parsedPacket.getLayerOfType<pcpp::TcpLayer>()->getDstPort();

			// print source and dest IPs
			std::cout
				<< "Src IP is " << srcIP << ":" << srcPort << std::endl
				<< "Dst IP is " << dstIP << ":" << dstPort << std::endl;
		}
	}

	// close the file
	reader.close();

	return 0;
}
