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

#define MD5_DIGEST_SIZE 16

#define MD5_BLOCK_SIZE 64

#define MD5_HEX_DIGEST_SIZE 33

#define MD5_S11 7
#define MD5_S12 12
#define MD5_S13 17
#define MD5_S14 22
#define MD5_S21 5
#define MD5_S22 9
#define MD5_S23 14
#define MD5_S24 20
#define MD5_S31 4
#define MD5_S32 11
#define MD5_S33 16
#define MD5_S34 23
#define MD5_S41 6
#define MD5_S42 10
#define MD5_S43 15
#define MD5_S44 21

namespace mariachi {
    namespace algorithms {
        /**
         * Class used to calculate md5 hashes.
         *
         * @see wikipedia - http://en.wikipedia.org/wiki/MD5
         */
        class Md5 : public HashFunction {
            private:
                /**
                 * The padding used in the md5 computation.
                 */
                static const unsigned char md5Padding[64];

                /**
                 * The size of the digest.
                 */
                static const int DIGEST_SIZE = MD5_DIGEST_SIZE;

                /**
                 * The result of the digest.
                 */
                unsigned char digest[DIGEST_SIZE];

                /**
                 * Buffer used to hold the bytes that didn't fit
                 * in the last 64 byte chunk.
                 */
                unsigned char hashBuffer[MD5_BLOCK_SIZE];

                /**
                 * Counter of 64 bit used for number of bits (lo, hi).
                 */
                unsigned int count[2];

                /**
                 * The state of the digest.
                 */
                unsigned int state[4];

                void transform(const unsigned char *block, unsigned int blocksize);

                static inline unsigned int F(unsigned int x, unsigned int y, unsigned int z);
                static inline unsigned int G(unsigned int x, unsigned int y, unsigned int z);
                static inline unsigned int H(unsigned int x, unsigned int y, unsigned int z);
                static inline unsigned int I(unsigned int x, unsigned int y, unsigned int z);
                static inline void FF(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac);
                static inline void GG(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac);
                static inline void HH(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac);
                static inline void II(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac);

            public:
                Md5();
                ~Md5();
                void update(const unsigned char *buffer, unsigned int size);
                void finalize();
                void reset();
                std::string hexdigest() const;
        };
    }
}
