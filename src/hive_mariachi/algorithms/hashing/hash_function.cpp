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

#include "hash_function.h"

using namespace mariachi::algorithms;

/**
 * Constructor of the class.
 */
HashFunction::HashFunction() {
}

/**
 * Destructor of the class.
 */
HashFunction::~HashFunction() {
}

/**
 * Initializes the hash function.
 */
void HashFunction::init() {
    // resets the current hash value
    this->reset();
}

/**
 * Initializes the hash function, with a text value.
 *
 * @param text The text value to be used for computation.
 */
void HashFunction::init(const std::string &text) {
    // resets the current hash value
    this->reset();

    // updates the hash value with the string value
    this->update((unsigned char *) text.c_str(), text.length());

    // finalizes the hash value
    this->finalize();
}

/**
 * Initializes the hash function, with a stream value.
 *
 * @param stream The stream value to be used for computation.
 */
void HashFunction::init(std::istream &stream) {
    // resets the current hash value
    this->reset();

    // allocates the file buffer
    unsigned char streamBuffer[HASH_STREAM_BUFFER_SIZE];

    // initializes the read size
    unsigned int readSize = 0;

    // retrieves the initial position
    std::streamoff initialPosition = stream.tellg();

    // iterates continuously
    while(1) {
        // reads the buffer
        stream.read((char *) streamBuffer, HASH_STREAM_BUFFER_SIZE);

        // retrieves the read size
        readSize = stream.gcount();

        // updates the hash value with the
        // stream buffer value
        this->update(streamBuffer, readSize);

        // in case the end of file was reached
        if(stream.eof()) {
            // breaks the cycle
            break;
        }
    };

    // finalizes the hash value
    this->finalize();

    // clears the error bits
    stream.clear();

    // seeks the the initial position
    stream.seekg(initialPosition, std::fstream::beg);
}

/**
 * Initializes the hash function, with a file stream value.
 *
 * @param fileStream The stream value to be used for computation.
 * @param closeStream If the file stream should be closed at end, if false
 * the file stream is positioned at the initial position.
 */
void HashFunction::init(std::fstream &fileStream, bool closeStream) {
    // calls the stream init method
    this->init((std::istream &) fileStream);

    // in case the file stream should be closed
    if(closeStream) {
        // closes the file stream
        fileStream.close();
    }
}

void HashFunction::update(const unsigned char *buffer, unsigned int size) {
}

void HashFunction::finalize() {
    // sets the finalized flag
    this->finalized = true;
}

void HashFunction::reset() {
    // unsets the finalized flag
    this->finalized = false;
}

std::ostream &mariachi::algorithms::operator<<(std::ostream &outStream, const HashFunction &value) {
    // puts the hex digest in the ouput stream
    return outStream << value.hexdigest();
}
