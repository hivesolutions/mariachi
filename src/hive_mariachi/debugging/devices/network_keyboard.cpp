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

#include "network_keyboard.h"

using namespace mariachi::debugging;

/**
 * Constructor of the class.
 */
NetworkKeyboard::NetworkKeyboard() : DebugDevice() {
}

/**
 * Destructor of the class.
 */
NetworkKeyboard::~NetworkKeyboard() {
}

/**
 * Starts the network keyboard, the starting is defined by the arguments.
 * in case the server setting is set a new socket is created an put into listening
 * mode. In case a client is set a new socket is created and connects to the defined
 * server.
 * In both cases a new stage is created to process the network keyboard operations.
 *
 * @param arguments The arguments for the newtork keyboard start.
 */
void NetworkKeyboard::start(void *arguments) {
    // tenho de ver aki se se trata de client ou de servidor
    // tenho de depois começar a conexao

    // tenho de tlx criar uma stage so para este gajo
    // tem de ser assim

    // no caso de ser cliente tenho de escutar pelo keyboard

    // no caso de ser servidor tem de
}

/**
 * Stops the network keyboard, stops the stages and closes the created sockets.
 *
 * @param arguments The arguments for the newtork keyboard stop.
 */
void NetworkKeyboard::stop(void *arguments) {
}
