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

#include "hash_function.h"

#define CRC32_DIGEST_SIZE 4

#define CRC32_HEX_DIGEST_SIZE 9

const unsigned int CRC32_BASE_VALUE = 0xffffffffL;

#ifdef MARIACHI_LITTLE_ENDIAN
#define CRC32_INDEX(character) (character & 0xff)
#define CRC32_SHIFTED(character) (character >> 8)
#else
#define CRC32_INDEX(character) (character >> 24)
#define CRC32_SHIFTED(character) (character << 8)
#endif

namespace mariachi {
    namespace algorithms {
        class Crc32 : public HashFunction {
            private:
                /**
                 * The table containing the crc polynomial values.
                 * This table is used in the interanl computation
                 * of the crc value.
                 */
                static const unsigned int crcTable[256];

                /**
                 * The size of the digest.
                 */
                static const int DIGEST_SIZE = CRC32_DIGEST_SIZE;

                /**
                 * The result of the digest.
                 */
                unsigned char digest[DIGEST_SIZE];

                /**
                 * The current crc value.
                 */
                unsigned int crcValue;

                inline const char getByte(unsigned int index);

            public:
                Crc32();
                ~Crc32();
                void update(const unsigned char *buffer, unsigned int size);
                void finalize();
                void reset();
                std::string hexdigest() const;
        };
    }
}
