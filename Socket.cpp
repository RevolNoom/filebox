#include "Socket.hpp" 
#include <iostream>
    
    /*
Socket::Socket(const std::string &ip, int port, Type type): _ad(), _ipv4(), _ipv6(), _adList(nullptr)
{
    _ad.ai_addr = reinterpret_cast<sockaddr*>(&_ipv4);
    _ipv4.sin_family = AF_INET;
    _ipv6.sin6_family = AF_INET6;

    SetIP(ip);
    SetType(type);
    SetPort(port);
}

Socket::~Socket()
{
    if (_adList != nullptr)
        freeaddrinfo(_adList);
}

bool Socket::IsIPv6()
{
    return reinterpret_cast<sockaddr_storage*>(_ad.ai_addr)->ss_family == AF_INET6;
}

void Socket::SetType(Type type)
{
    switch (type)
    {
        case TCP: 
            _ad.ai_socktype = SOCK_STREAM;
            break;
        case UDP:
            _ad.ai_socktype = SOCK_DGRAM;
            break;
    }
}

void Socket::SetIP(const std::string& ip)
{
    if (QuickcheckIPv6(ip))
    {
        inet_pton(AF_INET6, ip.c_str(), &(_ipv6.sin6_addr));
        _ipv6.sin6_port = GetPort();
        _ad.ai_addr = reinterpret_cast<sockaddr*>(&_ipv6);
    }
    else
    {
        inet_pton(AF_INET, ip.c_str(), &(_ipv4.sin_addr));
        _ipv4.sin_port = GetPort();
        _ad.ai_addr = reinterpret_cast<sockaddr*>(&_ipv4);
    }
}

void Socket::SetPort(int port)
{
    if (IsIPv6())
        reinterpret_cast<sockaddr_in6*>(_ad.ai_addr)->sin6_port = port;
    else
        reinterpret_cast<sockaddr_in*>(_ad.ai_addr)->sin_port = port;
}

Socket::Type Socket::GetType()
{
    switch (_ad.ai_socktype)
    {
        case SOCK_STREAM:
            return TCP;
        default:
            return UDP;
    }
}

std::string Socket::GetIP()
{
    if (IsIPv6())
    {
        char ip[INET6_ADDRSTRLEN];
        return inet_ntop(AF_INET6, &reinterpret_cast<sockaddr_in6*>(_ad.ai_addr)->sin6_addr, ip, INET6_ADDRSTRLEN);
    }

    // IPv4
    char ip[INET_ADDRSTRLEN];
    return inet_ntop(AF_INET, &reinterpret_cast<sockaddr_in*>(_ad.ai_addr)->sin_addr, ip, INET_ADDRSTRLEN);
}

int Socket::GetPort()
{
    if (IsIPv6())
        return reinterpret_cast<sockaddr_in6*>(_ad.ai_addr)->sin6_port;
    return reinterpret_cast<sockaddr_in*>(_ad.ai_addr)->sin_port;
}

bool Socket::QuickcheckIPv6(const std::string& ip)
{
    return ip.find(':', 0) != std::string::npos;
}

addrinfo* Socket::GetAddrInfo()
{
    // TODO: Error check
    if (_adList == nullptr)
        getaddrinfo(GetIP().c_str(), std::to_string(GetPort()).c_str(), &_ad, &_adList);
    return _adList;
}

/*
void Socket::SetAddrinfoInvalid()
{
    if (_adList != nullptr)
        freeaddrinfo(_adList);
    _adList = nullptr;
}*/