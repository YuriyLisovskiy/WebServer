# Web Server C++ Linux/Windows

### Download and Open
Download as a zip archive or clone the repository:
```
git clone https://github.com/YuriyLisovskiy/WebServer.git
```
### Usage
* Include `web_server/src/include/server.h` for using http server.
    > For using built-in http response, include `web_server/src/include/response.h`
    
    > For creating an application, include `web_server/src/include/application.h`
* Implement a class inherited from base class `Application`.

Example:
```
#include "src/include/application.h"
#include "src/include/response.h"

class SomeApp : public Application
{
public:
	SomeApp() : Application("path_to/html_docs/", "path_to/static_files/") // read note below*
	{
		std::vector<std::pair<std::string, appFunc>> urls = {
			{ "some/site/url", std::bind(&SomeApp::some_view, this, std::placeholders::_1) }
		};
		this->urls.set(urls);
	};
	std::string some_view(http::Request& request)
	{
		return http::Response::HttpResponse(this->templateDir + "some_document.html");
	}
};
```

> Note: base directory is `Path_To_Project_Folder/WebServer/web_server/`.
Use `some_app.setTemplateDir()` and `some_app.setStaticDir()` for setting up custom directories.
* Setup an application in `web_server/app_start.cpp` in `main()` function:
```
#include "src/include/server.h"
#include <iostream>
#include "path/to/some_app.h"
int main(int argc, char* argv[])
{
    try
    {
        std::string port;
        if (argc > 1)
        {
            port = argv[1];
        }
        else
        {
            port = SERVER_PORT;
        }
        Server server(port);
        server.setApp(new SomeApp());   // setup an app here
        server.start();
    }
    catch (const std::exception& exc)
    {
        cerr << exc.what();
    }
    catch (...)
    {
        cerr << "Error...";
    }
    cin.get();
    return 0;
}
```
* Build the project using CLion build tools and run the server.

Linux:
```
./Path_To_Project/WebServer/bin/web_server 12345
```
Windows (PowerShell):
```
.\Path_To_Project\WebServer\bin\web_server.exe 12345
```

### Author

* **[Yuriy Lisovskiy](https://github.com/YuriyLisovskiy)**

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

> Note: the project is not tested completely, so, there still can be bugs.