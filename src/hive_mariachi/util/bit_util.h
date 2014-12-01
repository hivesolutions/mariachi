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

#pragma once

/**
 * The size of the symbol used in the bit stream.
 */
#define BIT_STREAM_SYMBOL_SIZE 8

/**
 * The size of the bit stream internal buffer.
 */
#define BIT_STREAM_BUFFER_SIZE 1024

namespace mariachi {
    namespace util {
        typedef enum BitStreamMode_t {
            BIT_STREAM_READ = 1,
            BIT_STREAM_WRITE,
            BIT_STREAM_READ_WRITE
        } BitStreamMode;

        static const unsigned int BIT_STREAM_MASK[9] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x01F, 0x3F, 0x7F, 0xFF };

        class BitStream {
            private:
                bool endOfFile;
                BitStreamMode_t mode;
                std::iostream *stream;
                char readBuffer[BIT_STREAM_BUFFER_SIZE];
                char writeBuffer[BIT_STREAM_BUFFER_SIZE];
                unsigned int readBufferPointer;
                unsigned int readBufferSize;
                unsigned int writeBufferSize;
                unsigned char readBitCounter;
                unsigned char writeBitCounter;
                unsigned char readCurrentByte;
                unsigned char writeCurrentByte;

                inline void initBufferPointer();
                inline void initBufferSize();
                inline void initBitCounter();
                inline void initCurrentByte();
                inline void initEndOfFile();
                inline void initStream(std::iostream *stream);
                inline bool _checkRead();
                inline bool _checkWrite();
                void _flushRead();

            public:
                BitStream();
                BitStream(std::iostream *stream);
                ~BitStream();
                unsigned int read(unsigned char *readBuffer, unsigned int numberBits);
                unsigned int write(unsigned char *writebuffer, unsigned int numberBits);
                inline unsigned int readByte(unsigned char &byte, unsigned int numberBits);
                inline unsigned int writeByte(unsigned char byte, unsigned int numberBits);
                void flush();
                void open(BitStreamMode_t mode);
                void close(bool closeStream = false);
                void seekRead(int relativePosition);
                bool eof();
        };
    }
}
