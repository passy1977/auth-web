// MIT License
//
// Copyright (c) 2020 Antonio Salsi
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include "application.h"
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


// Macros to setup maim function to run as app or deamon (service in windows)
POCO_SERVER_MAIN(auth::Application);
