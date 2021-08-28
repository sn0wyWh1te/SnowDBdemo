#include <iostream>

#include "snowDb.pb.h"
#include "snowRpcChannel.h"
#include <google/protobuf/descriptor.h>

int main()
{
    // 整个程序启动以后，想使用mprpc框架来享受rpc服务调用，一定需要先调用框架的初始化函数（只初始化一次）
    //MprpcApplication::Init(argc, argv);

    // 演示调用远程发布的rpc方法Login
    snowDB::SnowDBRpc_Stub stub(new SnowRpcChannel());
    // rpc方法的请求参数
    snowDB::Key request;
    request.set_key("zhangsan");
    request.set_value("123456");
    // rpc方法的响应
    snowDB::ResultCode response;
    // 发起rpc方法的调用  同步的rpc调用过程  MprpcChannel::callmethod
    stub.Set(nullptr, &request, &response, nullptr); // RpcChannel->RpcChannel::callMethod 集中来做所有rpc方法调用的参数序列化和网络发送

    // 一次rpc调用完成，读调用的结果
    if (0 == response.errcode())
    {
        std::cout << "rpc login response success:" << response.errmsg() << std::endl;
    }
    else
    {
        std::cout << "rpc login response error : " << response.errmsg() << std::endl;
    }
    return 0;
}