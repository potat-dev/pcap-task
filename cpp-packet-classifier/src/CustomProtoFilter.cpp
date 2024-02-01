#include "CustomProtoFilter.hpp"

pcpp::GeneralFilter* CustomProtoFilter::getFilter() {
    // compose filters: IPv4 and (TCP or UDP)

    pcpp::ProtoFilter* ipFilter = new pcpp::ProtoFilter(pcpp::IPv4);
    pcpp::ProtoFilter* tcpFilter = new pcpp::ProtoFilter(pcpp::TCP);
    pcpp::ProtoFilter* udpFilter = new pcpp::ProtoFilter(pcpp::UDP);

    pcpp::OrFilter* transportFilter = new pcpp::OrFilter();
    transportFilter->addFilter(tcpFilter);
    transportFilter->addFilter(udpFilter);

    pcpp::AndFilter* protoFilter = new pcpp::AndFilter();
    protoFilter->addFilter(ipFilter);
    protoFilter->addFilter(transportFilter);

    return protoFilter;
}
