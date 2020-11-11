
#include <iostream>
#include <vector>
using namespace std;

#include <Poco/Util/ServerApplication.h>
using namespace Poco::Util;

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
using namespace Poco::Net;

#include "router.h"
using auth::Router;

//// Page renderer for / uri
//class MyPageHandler: public HTTPRequestHandler
//{
//public:

//    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response){
//                response.setChunkedTransferEncoding(true);
//                response.setContentType("text/html");

//                std::ostream& responseStream = response.send();

//                responseStream << "<html><head><head><title>My  HTTP Server in C++ </title></head>";
//                responseStream << "<body><h1>Hello World</h1><p>";
//                responseStream << "";
//                responseStream << "</p></body></html>";
//    };
//};

//// Page renderer for /Test uri

//class TestPageHandler: public HTTPRequestHandler
//{
//public:

//    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response){
//                response.setChunkedTransferEncoding(true);
//                response.setContentType("text/html");

//                std::ostream& responseStream = response.send();

//                responseStream << "<html><head><head><title>My  HTTP Server in C++ </title></head>";
//                responseStream << "<body><h1>Hello World 2</h1><p>";
//                responseStream << "";
//                responseStream << "</p></body></html>";
//    };
//};

//class ErroPageHandler: public HTTPRequestHandler
//{
//public:

//    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response){
//                response.setChunkedTransferEncoding(true);
//                //Sets mime type text/html application/json etc.
//                response.setContentType("text/html");
//                //Sets the response status 404, 200 etc.
//                response.setStatus("404");

//                //opens the file stream
//                std::ostream& responseStream = response.send();

//                responseStream << "<html><head><head><title>My  HTTP Server in C++ </title></head>";
//                responseStream << "<body><h1>PAGE NOT FOUND, SORRY!</h1><p>";
//                responseStream << "";
//                responseStream << "</p></body></html>";
//    };
//};

////  Url Router
//class HandlerFactory : public HTTPRequestHandlerFactory
//{
//public:

//    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &request){
//         // choose response based uri
//        if(request.getURI()=="/"){

//            return new MyPageHandler();
//        }

//        if(request.getURI()=="/test"){

//            return new TestPageHandler();
//        }
//         //uri not recognized
//        return new ErroPageHandler();
//    }
//};


// Server  Application
class Application : public ServerApplication
{
protected:
    int main(const std::vector<std::string> &args){

         //Port to listen
        Poco::UInt16 port = 9'100;

        ServerSocket socket(port);

        HTTPServerParams *params = new HTTPServerParams();


        //Sets the maximum number of queued connections.
        params->setMaxQueued(100);

        //Sets the maximum number of simultaneous threads available for this Server
        params->setMaxThreads(2);

        // Instanciate HandlerFactory
        HTTPServer server(new Router(), socket, params);

        server.start();

        waitForTerminationRequest();

        server.stop();

        return EXIT_OK;

    }
};



// Macros to setup maim function to run as app or deamon (service in windows)
POCO_SERVER_MAIN(::Application);
