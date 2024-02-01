#include "PacketClassifier.hpp"

#include <IPv4Layer.h>
#include <TcpLayer.h>
#include <UdpLayer.h>

Flow PacketClassifier::extractFlow(pcpp::Packet packet) {
    // get IP addresses
    pcpp::IPv4Layer* ipLayer = packet.getLayerOfType<pcpp::IPv4Layer>();
    pcpp::IPv4Address srcHost = ipLayer->getSrcIPv4Address();
    pcpp::IPv4Address dstHost = ipLayer->getDstIPv4Address();

    // get ports
    uint16_t srcPort;
    uint16_t dstPort;

    if (packet.isPacketOfType(pcpp::TCP)) {
        pcpp::TcpLayer* tcpLayer = packet.getLayerOfType<pcpp::TcpLayer>();
        srcPort = tcpLayer->getSrcPort();
        dstPort = tcpLayer->getDstPort();
    }

    if (packet.isPacketOfType(pcpp::UDP)) {
        pcpp::UdpLayer* udpLayer = packet.getLayerOfType<pcpp::UdpLayer>();
        srcPort = udpLayer->getSrcPort();
        dstPort = udpLayer->getDstPort();
    }

    return Flow(srcHost.toString(), dstHost.toString(), srcPort, dstPort);
}