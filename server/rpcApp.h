#pragma once

#include "config.h"
//#include "mprpcchannel.h"
//#include "mprpccontroller.h"

class RpcApp
{
private:
    RpcApp() = default;
    RpcApp(const RpcApp &) = delete;
    RpcApp(RpcApp &&) = delete;

public:
    static void Init(int argc, char **argv);
    static RpcApp &GetInstance();
    static Config &GetConfig();

private:
    static Config config_;
};