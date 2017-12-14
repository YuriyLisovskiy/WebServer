#include "WebServer.h"
int main()
{
	FILE* logFile;
	int err;
	err = fopen_s(&logFile, "logFile.txt", "wb");
	if (err == 0)
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
		return 1;
	}
	std::vector<std::thread> portThreads;	//this is vector which holds all my diferent servers on diferent ports
	int startPort = 2000;					//the starting and also main listening port which will accept all trafic and sends out the ports for connecting
	int numServerPorts = 5;					//number of ports which is server listening on
	for (int i = 0; i <= numServerPorts; i++)
	{
		portThreads.push_back(std::thread(startThreadPorts, startPort + i, logFile));	//create one thread per one port
	}
	for (auto& portThread : portThreads)
	{
		portThread.join();
	}
	fclose(logFile);
	WSACleanup();
	std::cin.ignore();
	return 0;
}