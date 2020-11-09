#include "api_v1_Auth.h"
using namespace api::v1;
//add definition of your processing function here

void Auth::login(const HttpRequestPtr &req, function<void (const HttpResponsePtr &)> &&callback) const
{
    LOG_DEBUG << "body:" << req->getBody();

    Json::Value ret;
    ret["result"]="ok";
    ret["token"]=drogon::utils::getUuid();
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);

}
