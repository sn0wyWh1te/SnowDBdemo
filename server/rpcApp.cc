#include "rpcApp.h"

#include <iostream>
#include <unistd.h>
#include <string>

Config RpcApp::config_;

void ShowArgsHelp()
{
    std::cout << "format: command -i <configfile>" << std::endl;
}

void RpcApp::Init(int argc, char **argv)
{
    config_.LoadConfigFile("../config/config");
    return;
    if (argc < 2)
    {
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }

    int c = 0;
    std::string config_file;
    while ((c = getopt(argc, argv, "i:")) != -1)
    {
        switch (c)
        {
        case 'i':
            config_file = optarg;
            break;
        case '?':
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        case ':':
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }

    config_.LoadConfigFile(config_file.c_str());
}

RpcApp &RpcApp::GetInstance()
{
    static RpcApp app;
    return app;
}

Config &RpcApp::GetConfig()
{
    return config_;
}