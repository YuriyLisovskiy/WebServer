# Web Server C++ Linux/Windows

### Download and Open

Download as a zip archive or clone the repository:

```
git clone https://github.com/YuriyLisovskiy/WebServer.git
```
Move to the project folder:
```
cd WebServer
```

### Usage
* Include `src/include/HttpServer.h` for using http server.
    > For using built-in http response, include `src/include/HttpResponse.h`
    
    > For using views , include `src/include/BaseView.h`
* Implement a class inherited from `BaseView` and overload all 
necessary methods which must return `std::string` data type. Available methods: `Get()`, `Post()`, `Put()`, `Delete()`
(Default: `Response::MethodNotAllowed()`).

Example:

```
class TestView : public View
{
public:
	TestView() : View("template_dir/")
	{
		this->url = "site/url";
	};
	std::string Get(Request& request) final
	{
		return Response::HttpResponse(this->dir + "test.html");
	}
};
```
* Set all views and run the server.

Example:
```
HttpServer server;
server.setView(new TestView());
server.run();
```

### Author

* **[Yuriy Lisovskiy](https://github.com/YuriyLisovskiy)**

> Note: the project is not tested completely, so, there still can be bugs.