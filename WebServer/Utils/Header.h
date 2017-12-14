#pragma once

#include <ws2tcpip.h>
#include <winsock2.h>
#include <process.h>
#include <memory>
#include <thread>
#include <stdio.h>
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <mutex>
#include <chrono>
#include <fstream>
#pragma comment (lib, "Ws2_32.lib")

static const char SERVER_IP[] = "127.0.0.1"; //default server ip (localhost)

static const std::string REGEX_GET = R"((GET)\s\/(.+)\s(HTTP.+))";
