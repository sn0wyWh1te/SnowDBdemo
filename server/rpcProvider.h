#pragma once

#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include "TcpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "TcpConnection.h"

#include <string>
#include <functional>
#include <unordered_map>

class RpcProvider
{
public:
    void NotifyService(google::protobuf::Service *service);
    void Run();

private:
    EventLoop m_eventLoop;

    struct ServiceInfo
    {
        google::protobuf::Service *m_service;
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor *> m_methodMap;
    };
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;

    void OnConnection(const TcpConnectionPtr &);
    void OnMessage(const TcpConnectionPtr &, Buffer *, Timestamp);
    void SendRpcResponse(const TcpConnectionPtr &, google::protobuf::Message *);
};