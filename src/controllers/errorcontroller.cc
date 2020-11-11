#include "errorcontroller.h"

#include <string>
using namespace std;

#include <Poco/Net/HTTPServerResponse.h>
using namespace Poco::Net;

using namespace auth::controllers;

void ErrorController::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
    response.setChunkedTransferEncoding(true);

    //Sets mime type text/html application/json etc.
    response.setContentType("application/json");

    //Sets the response status 404, 200 etc.
    response.setStatus(to_string(static_cast<uint16_t>(httpStatus)));
}
