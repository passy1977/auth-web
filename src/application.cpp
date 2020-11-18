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
#include "globals.h"
using namespace auth;

#include "services/lockservice.h"
using auth::services::LockService;

int auth::Application::main(const vector<string> &args)
{

    LockService lockService;

    ///check if service alterady run
    if (!lockService.start()) {
        poco_warning_f1(Logger::root(), "Unable start the process: %s",  lockService.getSerrviceOnMessage());
        return EXIT_NOINPUT;
    }

    ///initialize global params
    Globals::getInstance()->init(args.empty() ? PATH_CONFIG : args[0]);


    ///load config from global configuration
    auto &&config = Globals::getInstance()->getConfig();
    if (!config.isNull())
    {
        if (config->has(CONFIG_SERVER_PORT))
        {
            port = config->getInt(CONFIG_SERVER_PORT);
        }

        if (config->has(CONFIG_SERVER_MAX_QUEUED))
        {
            maxQueued = config->getInt(CONFIG_SERVER_MAX_QUEUED);
        }

        if (config->has(CONFIG_SERVER_MAX_THREAD))
        {
            maxThreads = config->getInt(CONFIG_SERVER_MAX_THREAD);
        }
    }


    string msg = "Server conf port:";
    msg += to_string(port);
    msg += " maxQueued:";
    msg += to_string(maxQueued);
    msg += " maxThreads:";
    msg += to_string(maxThreads);

    AUTH_GLOBAL_LOG(DBG, msg);

    HTTPServerParams *params = new HTTPServerParams();

    ///Sets the maximum number of queued connections.
    params->setMaxQueued(maxQueued);

    ///Sets the maximum number of simultaneous threads available for this Server
    params->setMaxThreads(maxThreads);

    ///Instanciate HandlerFactory
    HTTPServer server(new Router(), ServerSocket(port), params);

    server.start();

    waitForTerminationRequest();

    server.stop();

    lockService.stop();

    return EXIT_OK;

}
