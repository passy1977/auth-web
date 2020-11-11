#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
using namespace Poco::Net;


namespace auth::controllers
{


class AuthController final : public HTTPRequestHandler
{
public:
    AuthController() = default;
    AuthController(const AuthController&) = delete;
    AuthController& operator = (const AuthController&) = delete;
    AuthController(AuthController&&) = delete;
    AuthController& operator = (AuthController&&) = delete;

    void handleRequest(HTTPServerRequest &, HTTPServerResponse &) override;
};


}
