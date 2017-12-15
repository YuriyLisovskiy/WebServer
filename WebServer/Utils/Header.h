#pragma once

#include <ws2tcpip.h>
#include <winsock2.h>
#include <process.h>
#include <memory>
#include <string>
#include <regex>
#include <vector>
#include <mutex>
#include <chrono>
#pragma comment (lib, "Ws2_32.lib")

static const char SERVER_IP[] = "127.0.0.1"; //default server ip (localhost)

#define START_PORT 8000
