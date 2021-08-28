#pragma once

#include <unordered_map>
#include <string>

// format : rpcserverip=xxx.xxx.xxx.xxx  rpcserverport=8080
class Config
{
public:
    void LoadConfigFile(const char *config_file);
    std::string GetConfigInfo(const std::string &key);

private:
    void Trim(std::string &src_buf);

private:
    std::unordered_map<std::string, std::string> configMap_;
};