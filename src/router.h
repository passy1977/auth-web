#pragma once

#include <Poco/Net/HTTPRequestHandlerFactory.h>
using namespace Poco::Net;

namespace auth {

//  Url Router
class Router final : public HTTPRequestHandlerFactory
{
public:
    Router() = default;
    Router(const Router&) = delete;
    Router& operator = (const Router&) = delete;
    Router(Router&&) = delete;
    Router& operator = (Router&&) = delete;

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &) override;
};

}
