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

#include "../exceptions/exceptions.h"

#include "bit_util.h"

using namespace mariachi::util;
using namespace mariachi::exceptions;

/**
 * Constructor of the class.
 */
BitStream::BitStream() {
    this->initBufferPointer();
    this->initBufferSize();
    this->initBitCounter();
    this->initCurrentByte();
    this->initEndOfFile();
}

/**
 * Constructor of the class.
 *
 * @param stream The base stream to be used to do operations.
 */
BitStream::BitStream(std::iostream *stream) {
    this->initBufferPointer();
    this->initBufferSize();
    this->initBitCounter();
    this->initCurrentByte();
    this->initEndOfFile();
    this->initStream(stream);
}

/**
 * Destructor of the class.
 */
BitStream::~BitStream() {
}

inline void BitStream::initBufferPointer() {
    this->readBufferPointer = 0;
}

inline void BitStream::initBufferSize() {
    this->readBufferSize = 0;
    this->writeBufferSize = 0;
}

inline void BitStream::initBitCounter() {
    this->readBitCounter = 0;
    this->writeBitCounter = 0;
}

inline void BitStream::initCurrentByte() {
    this->readCurrentByte = 0;
    this->writeCurrentByte = 0;
}

inline void BitStream::initEndOfFile() {
    this->endOfFile = false;
}

inline void BitStream::initStream(std::iostream *stream) {
    this->stream = stream;
}

unsigned int BitStream::read(unsigned char *readBuffer, unsigned int numberBits) {
    // calculates the number of bytes to be read
    unsigned int numberBytes = numberBits / BIT_STREAM_SYMBOL_SIZE;

    // calculates the remaining bits
    unsigned int remainingBits = numberBits % BIT_STREAM_SYMBOL_SIZE;

    // initializes the number of read bits
    unsigned int numberReadBits = 0;

    // iterates over all the bytes to be read
    for(unsigned int index = 0; index < numberBytes; index++) {
        if(this->endOfFile) {
            readBuffer[index] = 0;
        } else {
            // reads the current byte
            numberReadBits += this->readByte(readBuffer[index], BIT_STREAM_SYMBOL_SIZE);
        }
    }

    // in case there are remainnig bits
    if(remainingBits > 0) {
        if(this->endOfFile) {
            readBuffer[numberBytes] = 0;
        } else {
            // reads the remaining byte
            numberReadBits += this->readByte(readBuffer[numberBytes], remainingBits);
        }
    }

    // returns the number of read bits
    return numberReadBits;
}

unsigned int BitStream::write(unsigned char *writeBuffer, unsigned int numberBits) {
    // calculates the number of bytes to be written
    unsigned int numberBytes = numberBits / BIT_STREAM_SYMBOL_SIZE;

    // calculates the remaining bits
    unsigned int remainingBits = numberBits % BIT_STREAM_SYMBOL_SIZE;

    // iterates over all the bytes to be writen
    for(unsigned int index = 0; index < numberBytes; index++) {
        // writes the current byte
        this->writeByte(writeBuffer[index], BIT_STREAM_SYMBOL_SIZE);
    }

    // in case there are remainnig bits
    if(remainingBits > 0) {
        // writes the remaining byte
        this->writeByte(writeBuffer[numberBytes], remainingBits);
    }

    // returns the number of bits
    return numberBits;
}

inline unsigned int BitStream::readByte(unsigned char &byte, unsigned int numberBits) {
    // resets the byte value
    byte = 0;

    // calculates the number of available bits
    char numberAvailableBits = BIT_STREAM_SYMBOL_SIZE - this->readBitCounter;

    // calculates the number of extra bits
    char numberExtraBits = numberBits - numberAvailableBits;

    // in case the number of extra bits is valid
    // the number of bits to be read is the sames as the available bits
    // otherwise the number of extra bits is zero
    numberExtraBits > 0 ? numberBits = numberAvailableBits : numberExtraBits = 0;

    // creates the first mask (for the base bits)
    unsigned char firstMask = BIT_STREAM_MASK[numberBits];

    // creates the shift value (for the base bits)
    unsigned char shiftValue = BIT_STREAM_SYMBOL_SIZE - this->readBitCounter - numberBits;

    // puts the base bits value in the byte
    byte |= (this->readCurrentByte >> shiftValue) & firstMask;

    // increments the bit counter by the number of bits
    this->readBitCounter += numberBits;

    // checks the stream for the need to read
    // and checks if it's not valid
    if(!this->_checkRead()) {
        // shifts the byte by the number of extra bits
        byte <<= numberExtraBits;

        // returns the number of bits read
        return numberBits;
    }

    // in case there are extra bits to be read
    if(numberExtraBits > 0) {
        // shifts the byte by the number of extra bits
        byte <<= numberExtraBits;

        // creates the second mask (for the extra bits)
        unsigned char secondMask = BIT_STREAM_MASK[numberExtraBits];

        // creates the shift value (for the extra bits)
        unsigned char shiftValue = BIT_STREAM_SYMBOL_SIZE - numberExtraBits;

        // puts the extra bit values in the byte
        byte |= (this->readCurrentByte >> shiftValue) & secondMask;

        // increments the bit counter by the number of extra bits
        this->readBitCounter += numberExtraBits;
    }

    // returns the number of bits read
    return numberBits + numberExtraBits;
}

inline unsigned int BitStream::writeByte(unsigned char byte, unsigned int numberBits) {
    // calculates the number of available bits
    char numberAvailableBits = BIT_STREAM_SYMBOL_SIZE - this->writeBitCounter;

    // calculates the number of extra bits
    char numberExtraBits = numberBits - numberAvailableBits;

    // in case the number of extra bits is valid
    // the number of bits to be writen is the sames as the available bits
    // otherwise the number of extra bits is zero
    numberExtraBits > 0 ? numberBits = numberAvailableBits : numberExtraBits = 0;

    // shifts the current byte value (by the number of bits)
    this->writeCurrentByte <<= numberBits;

    // adds the valid bits of the current byte (bits excluding the extra ones)
    // to the current byte (shifts the byte the number of extra bits)
    this->writeCurrentByte |= byte >> numberExtraBits;

    // increments the bit counter by the number of bits
    this->writeBitCounter += numberBits;

    // checks the stream for the need to write
    this->_checkWrite();

    // in case there are extra bits to be written
    if(numberExtraBits > 0) {
        // sets the current byte as the extra bits value
        // creates an and mask to mask the byte and "show" only the extra bits
        this->writeCurrentByte |= byte & ((0x1 << numberExtraBits) - 1);

        // increments the bit counter by the number of extra bits
        this->writeBitCounter += numberExtraBits;
    }

    // returns the number of read bits
    return numberBits;
}

void BitStream::flush() {
    // in case there is a byte waiting to be writen
    if(this->writeBitCounter > 0) {
        // sets the write buffer value
        this->writeBuffer[this->writeBufferSize] = this->writeCurrentByte << (BIT_STREAM_SYMBOL_SIZE - this->writeBitCounter);

        // increments the write buffer size
        this->writeBufferSize++;

        // resets the bit counter
        this->writeBitCounter = 0;
    }

    // flushes the bit stream
    this->stream->write(this->writeBuffer, this->writeBufferSize);

    // in case the writing failed
    if(this->stream->fail()) {
        // throws a runtime exception
        throw RuntimeException("Problem while writing into stream");
    }

    // resets the write buffer size
    this->writeBufferSize = 0;
}

void BitStream::open(BitStreamMode_t mode) {
    // sets the mode
    this->mode = mode;

    // switches over the mode
    switch(this->mode) {
        // in case it's read mode
        case BIT_STREAM_READ:
            // flushes the read
            this->_flushRead();

            // retrieves the new current byte
            this->readCurrentByte = this->readBuffer[this->readBufferPointer];

            break;

        // in case it's write mode
        case BIT_STREAM_WRITE:
            break;

        // in case it's read and write mode
        case BIT_STREAM_READ_WRITE:
            // flushes the read
            this->_flushRead();

            // retrieves the new current byte
            this->readCurrentByte = this->readBuffer[this->readBufferPointer];

            break;
    }
}

void BitStream::close(bool closeStream) {
    // switches over the mode
    switch(this->mode) {
        // in case it's read mode
        case BIT_STREAM_READ:
            break;

        // in case it's write mode
        case BIT_STREAM_WRITE:
            // flushes the stream
            this->flush();

            break;

        // in case it's read and write mode
        case BIT_STREAM_READ_WRITE:
            // flushes the stream
            this->flush();

            break;
    }

    // flushes the stream
    this->stream->flush();

    // initializes the various internal
    // structures of the bit stream
    initBufferSize();
    initBitCounter();
    initCurrentByte();
}

void BitStream::seekRead(int relativePosition) {
    // allocates space for the number
    // of available bits
    char numberAvailableBits;

    // in case the relative position is positive
    if(relativePosition > 0){
        // calculates the number of available bits (for positive values)
        numberAvailableBits = BIT_STREAM_SYMBOL_SIZE - this->readBitCounter;
    } else {
        // calculates the number of available bits (for negative values)
        numberAvailableBits = this->readBitCounter;
    }

    // calculates the number of extra bits
    char numberExtraBits = abs(relativePosition) - numberAvailableBits;

    // in case there are extra bits
    if(numberExtraBits > 0) {
        // calculates the number of extra bytes
        char numberExtraBytes = numberExtraBits / BIT_STREAM_SYMBOL_SIZE;

        // calculates the new read bit counter
        char newReadBitCounter = numberExtraBits % BIT_STREAM_SYMBOL_SIZE;

        if(newReadBitCounter > 0) {
            numberExtraBytes++;
        }

        if(relativePosition > 0) {
            // calculates the size delta
            int sizeDelta = numberExtraBytes - (this->readBufferSize - this->readBufferPointer);

            if(sizeDelta > 0) {
                // updates the stream position
                this->stream->seekg(sizeDelta, std::fstream::cur);

                // flushes the read
                this->_flushRead();
            } else {
                // increments the read buffer pointer
                this->readBufferPointer += numberExtraBytes;

                // sets the new current byte
                this->readCurrentByte = this->readBuffer[this->readBufferPointer];
            }

            this->readBitCounter = newReadBitCounter;
        } else {
            int sizeDelta = numberExtraBytes - this->readBufferPointer;

            if(sizeDelta > 0) {
                // updates the stream position
                this->stream->seekg((sizeDelta + this->readBufferSize) * -1, std::fstream::cur);

                // flushes the read
                this->_flushRead();
            } else {
                // increments the read buffer pointer
                this->readBufferPointer += numberExtraBytes * -1;
            }

            if(newReadBitCounter > 0) {
                // sets the new read bit counter
                this->readBitCounter = BIT_STREAM_SYMBOL_SIZE - newReadBitCounter;
            } else {
                // sets the read bit counter to the initial position
                this->readBitCounter = 0;
            }
        }

        // sets the new current byte
        this->readCurrentByte = this->readBuffer[this->readBufferPointer];
    } else {
        // sets the new read bit counter
        this->readBitCounter += relativePosition;

        // checks the stream for the need to read
        this->_checkRead();
    }

    // in case the seek is negative
    // the stream end of file flag is cleared
    if(relativePosition < 0) {
        // clears the end of file flag
        this->endOfFile = false;
    }
}

/**
 * Tests if the end of file has been reached.
 *
 * @return If the end of file has been reached.
 */
bool BitStream::eof() {
    return this->endOfFile;
}

inline bool BitStream::_checkRead() {
    // in case the end of file was reached
    if(this->endOfFile) {
        // returns false
        return false;
    }

    // in case the bit counter reached the limit
    // a flush is required
    if(this->readBitCounter == BIT_STREAM_SYMBOL_SIZE) {
        // resets the bit counter
        this->readBitCounter = 0;

        // increments the read buffer pointer
        this->readBufferPointer++;

        // in case the bit counter reached the limit
        // a flush is required
        if(this->readBufferPointer == this->readBufferSize) {
            // flushes the read
            this->_flushRead();

            // in case no data available to read
            if(!this->readBufferSize) {
                // sets the end of file flag
                this->endOfFile = true;

                // returns false
                return false;
            }
        }

        // sets the new current byte
        this->readCurrentByte = this->readBuffer[this->readBufferPointer];
    }

    // returns true
    return true;
}

inline bool BitStream::_checkWrite() {
    // in case the bit counter reached the limit
    // a flush is required
    if(this->writeBitCounter == BIT_STREAM_SYMBOL_SIZE) {
        // sets the write buffer value
        this->writeBuffer[this->writeBufferSize] = this->writeCurrentByte;

        // increments the write buffer size
        this->writeBufferSize++;

        // resets the bit counter
        this->writeBitCounter = 0;

        // resets the current byte
        this->writeCurrentByte = 0;

        // in case the write buffer is full
        if(this->writeBufferSize == BIT_STREAM_BUFFER_SIZE) {
            // flushes the bit stream
            this->flush();
        }
    }

    // returns true
    return true;
}

void BitStream::_flushRead() {
    // reads data to the the bit stream read buffer
    this->stream->read(this->readBuffer, BIT_STREAM_BUFFER_SIZE);

    // retrieves the read size
    unsigned int readSize = this->stream->gcount();

    // in case the end of file was reached
    if(this->stream->eof()) {
         // clears the error bits
         this->stream->clear();
    }

    // sets the read buffer size
    this->readBufferSize = readSize;

    // resets the read buffer pointer
    this->readBufferPointer = 0;
}
