#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
using namespace Poco::Net;

namespace auth::controllers
{

class ErrorController final : public HTTPRequestHandler
{
public:

    enum class HttpStatus : uint16_t {
        OK = 200,
        CREATED = 201,
        ACCEPTED = 202,
        BAD_REQUEST = 400,
        UNAUTHIRIZED = 401,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        METHOD_NOT_ALOWED = 405,
        INTERNAL_SERVER_ERROR = 500,
        SERVICE_UNAVAILABLE = 503,
    };

    ErrorController() = delete;
    ErrorController(const ErrorController&) = delete;
    ErrorController& operator = (const ErrorController&) = delete;
    ErrorController(ErrorController&&) = delete;
    ErrorController& operator = (ErrorController&&) = delete;

    void handleRequest(HTTPServerRequest &, HTTPServerResponse &) override;

    static inline ErrorController * build(HttpStatus error) noexcept {
        return new ErrorController(error);
    }

private:
    HttpStatus httpStatus;

    inline explicit ErrorController(HttpStatus httpStatus) : httpStatus(httpStatus) {}
};

}

