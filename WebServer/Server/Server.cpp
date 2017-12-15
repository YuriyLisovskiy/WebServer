#include "Server.h"

HttpServer::HttpServer()
{
	this->start();
}

void HttpServer::start()
{
	std::ofstream logFile("logFile.txt", std::ios::app);
	if (logFile.is_open())
	{
		std::cout << "Log file was created and opened.\n";
	}
	else
	{
		std::cerr << "Error\n";
	}
	int status;
	WSADATA wsaData;
	status = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (status != 0)
	{
		std::cerr << "WSAStartup failed: " << status << '\n';
		throw std::exception("WSAStartup failed: " + status + '\n');
	}
	std::thread newThread(&HttpServer::startThread, this, START_PORT, ref(logFile));
	if (newThread.joinable())
	{
		newThread.join();
	}
	logFile.close();
	WSACleanup();
	std::cin.ignore();
}

void HttpServer::startThread(const int port, std::ofstream& logFile)
{
	//create my listening socket and client socket which will be different client each time
	SOCKET listenSock;
	SOCKET client;
	int status;			//error handling
	sockaddr_in addr;	//structure to hold socket data
	socklen_t sa_size = sizeof(sockaddr_in);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, SERVER_IP, &(addr.sin_addr));
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSock == INVALID_SOCKET)
	{
		std::cerr << "Socket function failed with error: " << WSAGetLastError() << '\n';
		WSACleanup();
	}
	status = bind(listenSock, (sockaddr*)&addr, sizeof(sockaddr_in));
	if (status == SOCKET_ERROR)
	{
		std::cerr << "Bind function failed with error " << WSAGetLastError() << '\n';
		status = closesocket(listenSock);
		if (status == SOCKET_ERROR)
		{
			std::cerr << "Closesocket function failed with error " << WSAGetLastError() << '\n';
		}
		WSACleanup();
	}
	if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cerr << "Listen function failed with error: " << WSAGetLastError() << '\n';
	}
	int breakAfterServed = 100; //this is the number of clients that will be served by the server
	bool listening = true;
	while (listening)
	{
		//if you want to change the number of clients than you have to change the number when server stop running
		if ((clientNum == breakAfterServed / 5 && port != START_PORT) || (clientNum == breakAfterServed && port == START_PORT))
		{
			std::chrono::milliseconds duration(2000);
			std::this_thread::sleep_for(duration);
			break;
		}
		else if ((client = accept(listenSock, (sockaddr*)&addr, &sa_size)) != INVALID_SOCKET)
		{
			std::thread newClient(&HttpServer::serveClient, this, client, this->portNumber++, std::ref(logFile));
			newClient.detach();
			this->clientId++;
			this->clientNum++;
	//		g_lockPrint.lock();
	//		g_lockPrint.unlock();
		}
		else
		{
			std::cerr << "Invalid client socket\n";
			std::cin.ignore();
		}
	}
}

void HttpServer::serveClient(SOCKET clientInstance, int port, std::ofstream& logfile)
{
	clock_t start, finish;
	start = clock();

	this->getClientPortAndIP(clientInstance, port, logfile);

	getClientResource(clientInstance);	//this function will get the webpage for client or page not found

	finish = clock();
	float servingTime = (float)(finish - start) / CLOCKS_PER_SEC;
	
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%a %b %d %Y [%r]", &tstruct);
	g_lockPrint.lock();
	logfile << buf << "\nRequest took: " + std::to_string(servingTime) + " seconds.\n";
	g_lockPrint.unlock();
}

void HttpServer::getClientPortAndIP(SOCKET clientInstance, int port, std::ofstream& logfile)
{
	//this gets clients ip from sock_addr_in
	struct sockaddr_in addr;
	socklen_t addr_size = sizeof(struct sockaddr_in);
	int res = getsockname(clientInstance, (struct sockaddr *)&addr, &addr_size);
	sockaddr_in* pV4Addr = (struct sockaddr_in*)&addr;
	int ipAddr = pV4Addr->sin_addr.s_addr;
	char clientIp[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipAddr, clientIp, INET_ADDRSTRLEN);
	std::string cId = "ID: " + std::to_string(this->clientId) + "\n";
	std::string portStr = "The Client port is: " + std::to_string(port) + "=P\n";
	std::string clientIP = "The Client IP is: ";
	clientIP.append(clientIp);
	clientIP.append("\n");

	g_lockPrint.lock();
	logfile << cId << portStr << clientIP;
	g_lockPrint.unlock();
}

void HttpServer::getClientResource(SOCKET clientInstance)
{
	std::string filePath("");
	char buffer[1024];
	int recvMsgSize, bufError;
	do
	{
		recvMsgSize = recv(clientInstance, buffer, 1024, 0);
		if (recvMsgSize > 0)
		{
			filePath = processRequest(buffer);
			openFileWithPathAndSend(filePath, clientInstance);
			bufError = shutdown(clientInstance, SD_SEND);		// shutdown the connection since no more data will be sent
			if (bufError == SOCKET_ERROR)
			{
				std::cerr << "shutdown failed with error: " << WSAGetLastError() << '\n';
				closesocket(clientInstance);
				WSACleanup();
			}
		}
		else if (recvMsgSize < 0)
		{
			std::cerr << "recv failed: " << WSAGetLastError() << '\n';
		}
	} while (recvMsgSize > 0);
	bufError = shutdown(clientInstance, SD_RECEIVE);
	if (bufError == SOCKET_ERROR)
	{
		std::cerr << "shutdown failed with error: " << WSAGetLastError() << '\n';
		closesocket(clientInstance);
		WSACleanup();
	}
	bufError = closesocket(clientInstance);		// close the socket
	if (bufError == SOCKET_ERROR)
	{
		std::cerr << "close failed with error: " << WSAGetLastError() << '\n';
		WSACleanup();
	}
}

void HttpServer::openFileWithPathAndSend(std::string filePath, SOCKET clientInstance)
{
	FILE* file;
	errno_t err;
	err = fopen_s(&file, filePath.c_str(), "r");
	if (err == 0)
	{
		sendFile(file, clientInstance);
	}
	else
	{
		std::string responseNotFound = "HTTP/1.0 404 Not Found \r\n";
		std::cout << "Client response: " << responseNotFound;
		responseNotFound.append("Content-Type: text/html \r\n");
		responseNotFound.append("<!DOCTYPE html>\n<html>\n\n<head>\n<title>Not Found</title>\n</head>\n\n<body>\n<h1>Not Found</h1>\n</body>\n\n</html>\n");
		send(clientInstance, responseNotFound.c_str(), (int)responseNotFound.size(), 0);
	}
	if (file)
	{
		err = fclose(file);
		if (err != 0)
		{
			std::cerr << "The file was not closed\n";
		}
	}
}

void HttpServer::sendFile(FILE* file, SOCKET clientInstance)
{
	char statusLine[] = "HTTP/1.0 200 OK\r\n";
	char contentTypeLine[] = "Content-Type: text/html\r\n";

	fseek(file, 0, SEEK_END);
	int bufferSize = ftell(file);
	rewind(file);

	std::unique_ptr<char[]> myBufferedFile = std::make_unique<char[]>(bufferSize);	// this creates unique pointer to my array 

	int numRead = fread_s(myBufferedFile.get(), bufferSize, sizeof(char), bufferSize, file);	//this reads whole file into buffer.

	int totalSend = bufferSize + strlen(statusLine) + strlen(contentTypeLine);

	std::unique_ptr<char[]> myUniqueBufferToSend = std::make_unique<char[]>(totalSend);

	std::memcpy(myUniqueBufferToSend.get(), &statusLine, strlen(statusLine));
	std::memcpy(myUniqueBufferToSend.get() + strlen(statusLine), &contentTypeLine, strlen(contentTypeLine));
	std::memcpy(myUniqueBufferToSend.get() + strlen(statusLine) + strlen(contentTypeLine), myBufferedFile.get(), bufferSize);

	int iResult = send(clientInstance, myUniqueBufferToSend.get(), totalSend, 0);
	if (iResult == SOCKET_ERROR)
	{
		std::cerr << "send failed with error: " << WSAGetLastError() << '\n';
		closesocket(clientInstance);
		WSACleanup();
	}
	std::cout << "Client response: " << statusLine << '\n';
}

std::string HttpServer::processRequest(char* bufferPtr)
{
	std::string firstLine("");
	while (*bufferPtr != '\r')	//extract the first line from buffer
	{
		firstLine += *bufferPtr;
		bufferPtr++;
	}
	std::cout << "\nClient request: " << firstLine << '\n';
	return getFilePath(firstLine);
}

std::string HttpServer::getFilePath(std::string toParse)
{
	std::regex rx(REGEX_GET);
	std::string extractedSubmatchPath("");
	std::smatch pieces_match;
	if (std::regex_match(toParse, pieces_match, rx))
	{
		extractedSubmatchPath = pieces_match[2].str();
	}
	return extractedSubmatchPath;	//if there is no match so the request is not HTTP it will return empty string
}

//support function to check what is in the buffer
void HttpServer::printBuffer(char* bufferPtr, int size)
{
	for (int i = 0; i < size; i++)
	{
		std::cout << *bufferPtr;
		bufferPtr++;
	}
}
