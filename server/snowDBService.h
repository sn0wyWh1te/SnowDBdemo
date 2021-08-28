#pragma once
#include <iostream>
#include <string>
#include "snowDb.pb.h"
#include "rpcApp.h"
#include "rpcProvider.h"
#include "./db/db/include/db.h"

class SnowDBService : public snowDB::SnowDBRpc
{
public:
    SnowDBService(SnowDB *db)
        : db_(db)
    {
    }

public:
    virtual void Get(::google::protobuf::RpcController *controller,
                     const ::snowDB::Key *input,
                     ::snowDB::ResultCode *resCode,
                     ::google::protobuf::Closure *done) override;
    virtual void Set(::google::protobuf::RpcController *controller,
                     const ::snowDB::Key *request,
                     ::snowDB::ResultCode *response,
                     ::google::protobuf::Closure *done) override;
    virtual void Remove(::google::protobuf::RpcController *controller,
                        const ::snowDB::Key *request,
                        ::snowDB::ResultCode *response,
                        ::google::protobuf::Closure *done) override;

private:
    SnowDB *db_;
};