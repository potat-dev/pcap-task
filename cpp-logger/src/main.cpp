#include <iostream>
#include <unordered_map>

#include <IPv4Layer.h>
#include <UdpLayer.h>
#include <TcpLayer.h>
#include <Packet.h>
#include <PcapFileDevice.h>
#include <PcapLiveDeviceList.h>

#include "types.hpp"
#include "utils.hpp"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: app <file.pcap>" << std::endl;
		return 0;
	}
	// open a pcap file for reading
	pcpp::PcapFileReaderDevice reader(argv[1]);
	// pcpp::PcapFileReaderDevice reader("/code/projects/pcap-task/test.pcap");

	if (!reader.open())
	{
		std::cerr << "Error opening the pcap file" << std::endl;
		return 1;
	}

	std::unordered_map<Connection, int> counts;

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
				<< "Layer type: " << getProtocolTypeAsString(curLayer->getProtocol()) << "; "
				<< "Total data: " << curLayer->getDataLen() << " [bytes]; "
				<< "Layer data: " << curLayer->getHeaderLen() << " [bytes]; "
				<< "Layer payload: " << curLayer->getLayerPayloadSize() << " [bytes]"
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

			counts[{{srcIP.toString(), srcPort}, {dstIP.toString(), dstPort}}] += 1;

			std::cout
				<< "\n-------- LOG ---------\n"
				<< std::endl;

			for (auto i : counts)
			{
				std::cout << i.first << " - " << i.second << std::endl;
			}
		}
	}

	reader.close();

	std::cout
		<< "\n-------- LOG ---------\n"
		<< std::endl;

	for (auto i : counts)
	{
		std::cout << i.first << " - " << i.second << std::endl;
	}

	return 0;
}
