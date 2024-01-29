#include "utils.hpp"

// pcap utils

void listPcapLiveDevices()
{
	std::vector<pcpp::PcapLiveDevice *> devices = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();

	for (pcpp::PcapLiveDevice *device : devices)
	{
		std::string desc = device->getDesc();
		if (desc == "")
			desc = "No description";

		std::cout
			<< "Interface info:" << std::endl
			<< "├╴ Interface name:        " << device->getName() << std::endl
			<< "├╴ Interface description: " << desc << std::endl
			<< "├╴ MAC address:           " << device->getMacAddress() << std::endl
			<< "├╴ IPv4 address:          " << device->getIPv4Address() << std::endl
			<< "└╴ Default gateway:       " << device->getDefaultGateway() << "\n"
			<< std::endl;
	}
}

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
