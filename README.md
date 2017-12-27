# Web Server C++ Linux/Windows

### Download and Open

Download as a zip archive or clone the repository:

```
git clone https://github.com/YuriyLisovskiy/WebServer.git
```

### Usage
* Include `web_server/src/include/server.h` for using http server.
    > For using built-in http response, include `web_server/src/include/response.h`
    
    > For using views , include `web_server/src/include/view.h`
* Implement a class inherited from base class `View` and overload all 
necessary methods which must return `std::string` data type. Available methods: `Get()`, `Post()`, `Put()`, `Delete()`,
`Head()`, `Connect()`, `Options()`, `Trace()` and `Patch()`.
(Returns `http::Response::MethodNotAllowed()` by default).

Example:

```
class SomeView : public View
{
public:
	SomeView() : View("template_dir/", "static_dir/")
	{
	    this->url = "some/site_url/";
	};
	std::string Get(http::Request& request) final
	{
	    /* Some logic */
	    return http::Response::HttpResponse(this->dir + "get_request.html");
	}
	std::string Post(http::Request& request) final
        {
            /* Some logic */
    	    return http::Response::HttpResponse(this->dir + "post_request.html");
        }        
};
```
* Setup all views and run the server.
```
std::string port("12345");
HttpServer server(port);
server.setView(new SomeView());
server.start();
```

### Author

* **[Yuriy Lisovskiy](https://github.com/YuriyLisovskiy)**

> Note: the project is not tested completely, so, there still can be bugs.