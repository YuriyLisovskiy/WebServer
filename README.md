# Simple cross-platform web server in pure C++

## Download
Download as a zip archive or clone the repository:
```
git clone https://github.com/YuriyLisovskiy/WebServer.git
```
## Usage
`demo_app.h`:
```
#include "include/application.h"
#include "include/response.h"

class SomeApp : public Application
{
public:
	SomeApp() : Application("app_root_folder/", "app_templates/", "app_static_files/")
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

`entry_point.cpp`:
```
#include "demo.h"
#include <iostream>

#include "include/server.h"

using std::cin;
using http::Server;

int main(int argc, char* argv[])
{
    Server server(argc, argv);
    server.setApp(new DemoApp());
    server.start();
    cin.get();
    return 0;
}
```
* Build the project with CMakeLists.txt and run the server.

Linux:
```
./Path_To_Project/bin/app_binary runserver 127.0.0.1 8000
```
Windows:
```
.\Path_To_Project\bin\app_binary.exe runserver 127.0.0.1 8000
```

## Author

* **[Yuriy Lisovskiy](https://github.com/YuriyLisovskiy)**

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
