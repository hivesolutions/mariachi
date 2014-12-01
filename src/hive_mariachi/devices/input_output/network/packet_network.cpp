// Hive Mariachi Engine
// Copyright (C) 2008 Hive Solutions Lda.
//
// This file is part of Hive Mariachi Engine.
//
// Hive Mariachi Engine is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Hive Mariachi Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Hive Mariachi Engine. If not, see <http://www.gnu.org/licenses/>.

// __author__    = João Magalhães <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#include "stdafx.h"

#include "../../../exceptions/exceptions.h"
#include "../../../system/socket.h"
#include "packet_network.h"

using namespace mariachi::exceptions;
using namespace mariachi::devices;

/**
 * Constructor of the class.
 */
PacketNetwork::PacketNetwork() : NetworkDevice() {
}

/**
 * Destructor of the class.
 */
PacketNetwork::~PacketNetwork() {
}

/**
 * Starts the network subsystem setting up the socket.
 *
 * @param arguments The arguments for the starting of the network.
 */
void PacketNetwork::start(void *arguments) {
    // retrieves the arguments map from the arguments
    std::map<std::string, void *> argumentsMap = *(std::map<std::string, void *> *) arguments;

    // retrieves and sets the socket type
    this->socketType = (unsigned long long) argumentsMap["socketType"];

    // allocates space for the socket data
    SOCKET_DATA socketData;

    // initializes the socket system
    SOCKET_INITIALIZE(&socketData);

    // creates a new socket
    this->socketHandle = SOCKET_CREATE(this->socketType, SOCKET_PACKET_TYPE, SOCKET_PROTOCOL_TCP);

    // tests the socket for errors
    if(SOCKET_TEST_SOCKET(this->socketHandle)) {
        // retrieves the socket error code
        SOCKET_ERROR_CODE socketErrorCode = SOCKET_GET_ERROR_CODE(this->socketHandle);

        // throws a runtime exception
        throw RuntimeException("Problem creating socket: " + socketErrorCode);
    }
}

/**
 * Stops the network subsystem closing the socket.
 *
 * @param arguments The arguments for the stopping of the network.
 */
void PacketNetwork::stop(void *arguments) {
    // finishes the socket system
    SOCKET_FINISH();
}

/**
 * Binds a connection to the given ip address and port.
 * After the binding the connection is in a state of listening to client connections.
 *
 * @param ipAddress The ip address to bind. If empty string the binding is done to all the available ip
 * addresses.
 * @param port The port to be used in the binding.
 */
void PacketNetwork::bindConnection(const std::string &ipAddress, unsigned int port) {
    // allocates the socket address structure
    SOCKET_ADDRESS socketAddress;

    // allocate the ip address c string
    const char *ipAddressCString;

    // in case the ip address is empty string
    if(ipAddress == "") {
        // sets the ip address c string as null
        ipAddressCString = NULL;
    }
    else {
        // sets the ip address as the c string representation
        // of the origianl ip address
        ipAddressCString = ipAddress.c_str();
    }

    // creates the socket address
    SOCKET_ADDRESS_CREATE(socketAddress, this->socketType, ipAddressCString, port);

    // binds the socket
    int bindResult = SOCKET_BIND(this->socketHandle, socketAddress);

    // in case there was an error binding the socket
    if(SOCKET_TEST_ERROR(bindResult)) {
        // retrieves the binding error code
        SOCKET_ERROR_CODE bindingErrorCode = SOCKET_GET_ERROR_CODE(bindResult);

        // throws a runtime exception
        throw RuntimeException("Problem binding socket: " + bindingErrorCode);
    }

    while(1) {
        // listens the socket
        int listenResult = SOCKET_LISTEN(this->socketHandle);

        // in case there was an error listening the connection
        if(SOCKET_TEST_ERROR(listenResult)) {
            // retrieves the listen error code
            SOCKET_ERROR_CODE listenErrorCode = SOCKET_GET_ERROR_CODE(listenResult);

            // throws a runtime exception
            throw RuntimeException("Problem listing in the socket: " + listenErrorCode);
        }

        // accepts the client socket
        SOCKET_HANDLE clientSocketHandle = accept(this->socketHandle, NULL, NULL);

        // in case there was an error accepting the connection
        if(SOCKET_TEST_SOCKET(clientSocketHandle)) {
            // retrieves the socket error code
            SOCKET_ERROR_CODE socketErrorCode = SOCKET_GET_ERROR_CODE(clientSocketHandle);

            // throws a runtime exception
            throw RuntimeException("Problem accepting client socket: " + socketErrorCode);
        }

        // prints a debug message
        std::cout << "New client connection established\n";

        // adds the client socket handle to the client socket handles list
        this->clientSocketHandlesList.push_back(clientSocketHandle);
    }
}

void PacketNetwork::openConnection(const std::string &ipAddress, unsigned int port, unsigned int timeout) {
    // allocates the socket address structure
    SOCKET_ADDRESS socketAddress;

    // creates the socket address
    SOCKET_ADDRESS_CREATE(socketAddress, this->socketType, ipAddress.c_str(), port);

    // connects the socket
    int connectResult = SOCKET_CONNECT(this->socketHandle, socketAddress);

    // in case there was an error connecting the socket
    if(SOCKET_TEST_ERROR(connectResult)) {
        // retrieves the connect error code
        SOCKET_ERROR_CODE connectErrorCode = SOCKET_GET_ERROR_CODE(connectResult);

        // throws a runtime exception
        throw RuntimeException("Problem connecting in the socket: " + connectErrorCode);
    }

    // prints a debug message
    std::cout << "Connection with server established\n";
}

void PacketNetwork::closeConnection() {
    // closes the socket
    SOCKET_CLOSE(this->socketHandle);
}

void PacketNetwork::read(char *buffer, unsigned int size) {
    // se calhar aki o melhor e ter um buffer grande ja pre-alocado e devolver o mesmo
    // senao pode ser chato (muitos mallocs)
}

void PacketNetwork::write(char *buffer, unsigned int size) {
}
