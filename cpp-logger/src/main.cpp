#include <iostream>
#include <iomanip>
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
	// if (argc < 2)
	// {
	// 	std::cout << "Usage: app <file.pcap>" << std::endl;
	// 	return 0;
	// }
	// open a pcap file for reading
	// pcpp::PcapFileReaderDevice reader(argv[1]);
	pcpp::PcapFileReaderDevice reader("/code/projects/pcap-task/test.pcap");

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
		pcpp::Packet parsed(&rawPacket);

		std::cout << "--------------------\n"
				  << std::endl
				  << "Packet size: " << length << "\n"
				  << std::endl;

		// first let's go over the layers one by one and find out its type, its total length, its header length and its payload length
		for (pcpp::Layer *curLayer = parsed.getFirstLayer(); curLayer != NULL; curLayer = curLayer->getNextLayer())
		{
			std::cout
				<< "Layer type: " << getProtocolTypeAsString(curLayer->getProtocol()) << "; "
				<< "Total data: " << curLayer->getDataLen() << " [bytes]; "
				<< "Layer data: " << curLayer->getHeaderLen() << " [bytes]; "
				<< "Layer payload: " << curLayer->getLayerPayloadSize() << " [bytes]"
				<< std::endl;
		}

		// verify the packet is IPv4
		if (parsed.isPacketOfType(pcpp::IPv4) && (parsed.isPacketOfType(pcpp::TCP) || parsed.isPacketOfType(pcpp::UDP)))
		{
			// extract source and dest IPs
			pcpp::IPv4Layer *ipLayer = parsed.getLayerOfType<pcpp::IPv4Layer>();
			if (ipLayer == nullptr)
			{
				std::cout << "" << std::endl;
				return 0;
			}
			pcpp::IPv4Address srcIP = ipLayer->getSrcIPv4Address();
			pcpp::IPv4Address dstIP = ipLayer->getDstIPv4Address();

			// get port

			uint16_t srcPort;
			uint16_t dstPort;

			if (parsed.isPacketOfType(pcpp::TCP))
			{
				pcpp::TcpLayer *tcpLayer = parsed.getLayerOfType<pcpp::TcpLayer>();
				if (tcpLayer == nullptr)
				{
					std::cout << "layer not found" << std::endl;
					return 0;
				}
				else
				{
					srcPort = tcpLayer->getSrcPort();
					dstPort = tcpLayer->getDstPort();
				}
			}

			if (parsed.isPacketOfType(pcpp::UDP))
			{
				pcpp::UdpLayer *udpLayer = parsed.getLayerOfType<pcpp::UdpLayer>();
				if (udpLayer == nullptr)
				{
					std::cout << "layer not found" << std::endl;
					return 0;
				}
				else
				{
					srcPort = udpLayer->getSrcPort();
					dstPort = udpLayer->getDstPort();
				}
			}

			// print source and dest IPs
			std::cout
				<< "Src IP is " << srcIP << ":" << srcPort << std::endl
				<< "Dst IP is " << dstIP << ":" << dstPort << std::endl;

			Connection conn = {{srcIP.toString(), srcPort}, {dstIP.toString(), dstPort}};
			counts[conn] += 1;

			std::cout
				<< "\n-------- LOG ---------\n"
				<< std::endl;

			for (auto i : counts)
			{
				std::cout << std::right << std::setw(42) << i.first << " - " << i.second << std::endl;
			}
		}
	}

	reader.close();

	std::cout
		<< "\n-------- FINAL LOG ---------\n"
		<< std::endl;

	for (auto i : counts)
	{
		std::cout << i.first << " - " << i.second << std::endl;
	}

	return 0;
}
