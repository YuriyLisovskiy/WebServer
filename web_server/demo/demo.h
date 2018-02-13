#ifndef WEB_SERVER_DEMO_H
#define WEB_SERVER_DEMO_H

#pragma once
#include "../src/include/application.h"
#include "../src/include/response.h"

using http::Request;
using http::Response;
using std::string;

class DemoApp : public Application
{
public:
    explicit DemoApp() : Application("demo/", "demo/static/")
    {
        std::vector<std::pair<std::string, appFunc>> urlPatterns = {
                { "welcome/", std::bind(&DemoApp::index, this, std::placeholders::_1) }
        };
        this->urls.set(urlPatterns);
    };
    std::string index(Request& request)
    {
        return Response::HttpResponse(this->templateDir + "demo.html");
    }
};


#endif
