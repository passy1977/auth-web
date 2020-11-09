#pragma once
#include <drogon/HttpController.h>
using namespace drogon;
namespace api::v1
{

using namespace std;

class Auth final : public drogon::HttpController<Auth>
{
  public:
    METHOD_LIST_BEGIN

    /**
     * example POST:http://localhost:9100/api/v1/auth
     */
    METHOD_ADD(Auth::login,"",Post);
    METHOD_LIST_END

    /**
     * @brief login login to get JWT token
     * @param request HttpRequest
     * @param callback response
     */
    void login(const HttpRequestPtr &request, function<void (const HttpResponsePtr &)> &&callback) const;

};

}
