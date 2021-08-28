#include "snowDBService.h"
#include <string>

void SnowDBService::Get(::google::protobuf::RpcController *controller,
                        const ::snowDB::Key *request,
                        ::snowDB::ResultCode *response,
                        ::google::protobuf::Closure *done)
{
    std::string key = request->key();

    // 做本地业务
    std::string value;
    int ret = db_->get(key, value);

    response->set_errcode(ret);
    if (ret == 1)
        response->set_errmsg("successfully get");
    else
        response->set_errmsg("the key is not exsist");

    response->set_value(value);

    done->Run();
}

void SnowDBService::Set(::google::protobuf::RpcController *controller,
                        const ::snowDB::Key *request,
                        ::snowDB::ResultCode *response,
                        ::google::protobuf::Closure *done)
{
    std::string key = request->key();
    if (!request->has_value())
    {
        if (controller)
        {
            controller->Failed();
        }
        return;
    }
    std::string value = request->value();

    // 做本地业务
    db_->set(key, value);

    response->set_errcode(0);
    response->set_errmsg("successfully set");

    done->Run();
}

void SnowDBService::Remove(::google::protobuf::RpcController *controller,
                           const ::snowDB::Key *request,
                           ::snowDB::ResultCode *response,
                           ::google::protobuf::Closure *done)
{
    std::string key = request->key();
    // 做本地业务
    int ret = db_->remove(key);

    response->set_errcode(ret);
    if (ret == 1)
        response->set_errmsg("successfully remove");
    else
        response->set_errmsg("the key is not exsist");

    done->Run();
}

int main(int argc, char **argv)
{
    //init
    RpcApp::Init(argc, argv);

    SnowDB snowDB;
    if (snowDB.init())
        return 1;
    //publish the rpc service
    RpcProvider provider;
    provider.NotifyService(new SnowDBService(&snowDB));

    provider.Run();

    return 0;
}
