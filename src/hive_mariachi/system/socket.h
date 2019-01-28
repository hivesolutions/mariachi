// Hive Mariachi Engine
// Copyright (c) 2008-2019 Hive Solutions Lda.
//
// This file is part of Hive Mariachi Engine.
//
// Hive Mariachi Engine is free software: you can redistribute it and/or modify
// it under the terms of the Apache License as published by the Apache
// Foundation, either version 2.0 of the License, or (at your option) any
// later version.
//
// Hive Mariachi Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// Apache License for more details.
//
// You should have received a copy of the Apache License along with
// Hive Mariachi Engine. If not, see <http://www.apache.org/licenses/>.

// __author__    = João Magalhães <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008-2019 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#pragma once

#include "../exceptions/exception.h"

#ifdef MARIACHI_PLATFORM_WIN32
#include <winsock.h>
#pragma comment(lib, "Ws2_32.lib")
#define SOCKET_DATA WSADATA
#define SOCKET_HANDLE SOCKET
#define SOCKET_ADDRESS SOCKADDR_IN
#define SOCKET_ERROR_CODE int
#define SOCKET_INTERNET_TYPE AF_INET
#define SOCKET_INTERNET6_TYPE AF_INET6
#define SOCKET_PACKET_TYPE SOCK_STREAM
#define SOCKET_DATAGRAM_TYPE SOCK_DGRAM
#define SOCKET_PROTOCOL_TCP IPPROTO_TCP
#define SOCKET_PROTOCOL_UDP IPPROTO_UDP
#define SOCKET_INITIALIZE(socketData) if(WSAStartup(MAKEWORD(2, 0), socketData) != 0) { throw Exception("Socket Initialization Error"); }
#define SOCKET_FINISH() WSACleanup()
#define SOCKET_CREATE(type, streamType, protocolType) socket(type, streamType, protocolType)
#define SOCKET_BIND(socketHandle, socketAddress) bind(socketHandle, (LPSOCKADDR) &socketAddress, sizeof(SOCKET_ADDRESS))
#define SOCKET_LISTEN(socketHandle) listen(socketHandle, SOMAXCONN)
#define SOCKET_CONNECT(socketHandle, socketAddress) connect(socketHandle, (LPSOCKADDR) &socketAddress, sizeof(SOCKET_ADDRESS));
#define SOCKET_CLOSE(socketHandle) closesocket(socketHandle)
#define SOCKET_ADDRESS_CREATE(socketAddress, type, address, port) memset(&socketAddress, 0, sizeof(SOCKET_ADDRESS));\
    socketAddress.sin_family = type;\
    socketAddress.sin_port = htons(port);\
    if(address == NULL) { socketAddress.sin_addr.s_addr = INADDR_ANY; } else { socketAddress.sin_addr.s_addr = inet_addr(address); }
#define SOCKET_GET_HOST_BY_NAME(hostname) gethostbyname(hostname)
#define SOCKET_TEST_SOCKET(socketHandle) socketHandle == INVALID_SOCKET
#define SOCKET_TEST_ERROR(result) result == SOCKET_ERROR
#define SOCKET_GET_ERROR_CODE(result) WSAGetLastError()
#elif MARIACHI_PLATFORM_UNIX
#include <sys/socket.h>
#include <arpa/inet.h>
#define SOCKET_HANDLE int
#define SOCKET_DATA void *
#define SOCKET_ADDRESS sockaddr_in
#define SOCKET_ERROR_CODE int
#define SOCKET_INTERNET_TYPE AF_INET
#define SOCKET_INTERNET6_TYPE AF_INET6
#define SOCKET_PACKET_TYPE SOCK_STREAM
#define SOCKET_DATAGRAM_TYPE SOCK_DGRAM
#define SOCKET_PROTOCOL_TCP IPPROTO_TCP
#define SOCKET_PROTOCOL_UDP IPPROTO_UDP
#define SOCKET_INITIALIZE(socketData)
#define SOCKET_FINISH()
#define SOCKET_CREATE(type, streamType, protocolType) socket(type, streamType, protocolType)
#define SOCKET_BIND(socketHandle, socketAddress) bind(socketHandle, (const sockaddr *) &socketAddress, sizeof(SOCKET_ADDRESS))
#define SOCKET_LISTEN(socketHandle) listen(socketHandle, SOMAXCONN)
#define SOCKET_CONNECT(socketHandle, socketAddress) connect(socketHandle, (const sockaddr *) &socketAddress, sizeof(SOCKET_ADDRESS))
#define SOCKET_CLOSE(socketHandle) close(socketHandle)
#define SOCKET_ADDRESS_CREATE(socketAddress, type, address, port) memset(&socketAddress, 0, sizeof(SOCKET_ADDRESS));\
    socketAddress.sin_family = type;\
    socketAddress.sin_port = htons(port);\
    if(address == NULL) { socketAddress.sin_addr.s_addr = INADDR_ANY; } else { socketAddress.sin_addr.s_addr = inet_addr(address); }
#define SOCKET_GET_HOST_BY_NAME(hostname) gethostbyname(hostname)
#define SOCKET_TEST_SOCKET(socketHandle) socketHandle < 0
#define SOCKET_TEST_ERROR(result) result < 0
#define SOCKET_GET_ERROR_CODE(result) result
#endif
