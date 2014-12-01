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

#include "../../util/util.h"
#include "../../system/system.h"

#include "md5.h"

using namespace mariachi::util;
using namespace mariachi::algorithms;

const unsigned char Md5::md5Padding[] = {
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/**
 * Constructor of the class.
 */
Md5::Md5() : HashFunction() {
    // resets the md5 value
    this->reset();
}

/**
 * Destructor of the class.
 */
Md5::~Md5() {
}

/**
 * Updates the hash function to a new value using the
 * buffer with the given size.
 *
 * @param buffer The buffer to be used to update the hash.
 * @param size The size of the buffer used to update
 * the hash.
 */
void Md5::update(const unsigned char *buffer, unsigned int size) {
    // calls the super
    HashFunction::update(buffer, size);

    // computes the number of bytes modulus 64
    unsigned int index = this->count[0] / 8 % MD5_BLOCK_SIZE;

    // updates the number of bits
    if((this->count[0] += (size << 3)) < (size << 3)) {
        this->count[1]++;
    }

    this->count[1] += (size >> 29);

    // number of bytes we need to fill in buffer
    unsigned int firstpart = 64 - index;

    unsigned int i;

    // transform as many times as possible
    if(size >= firstpart) {
        // fills the  buffer first
        memcpy(&this->hashBuffer[index], buffer, firstpart);

        // transform the buffer
        this->transform(this->hashBuffer, MD5_BLOCK_SIZE);

        // transform chunks of blocksize (64 bytes)
        for(i = firstpart; i + MD5_BLOCK_SIZE <= size; i += MD5_BLOCK_SIZE) {
            this->transform(&buffer[i], MD5_BLOCK_SIZE);
        }

        index = 0;
    } else {
        i = 0;
    }

    // buffer remaining input
    memcpy(&this->hashBuffer[index], &buffer[i], size - i);
}

/**
 * Finalizes the hash calculation, closing the calculation.
 */
void Md5::finalize() {
    // saves the number of bits
    unsigned char bits[8];
    ByteUtil::encode(bits, this->count, 8);

    // pads out to 56 modulus 64
    unsigned int index = this->count[0] / 8 % 64;
    unsigned int paddingSize = (index < 56) ? (56 - index) : (120 - index);

    // updates the hash value with the padding
    this->update(Md5::md5Padding, paddingSize);

    // appends length (before padding)
    this->update(bits, 8);

    // store the state in the digest
    ByteUtil::encode(this->digest, this->state, MD5_DIGEST_SIZE);

    // invalidates sensitive information
    memset(this->hashBuffer, 0, sizeof(this->hashBuffer));
    memset(this->count, 0, sizeof(this->count));

    // calls the super
    HashFunction::finalize();
}

void Md5::reset() {
    // unsets the finalized flag
    this->finalized = false;

    // resets the count values
    this->count[0] = 0;
    this->count[1] = 0;

    // loads the magic initialization constants
    this->state[0] = 0x67452301;
    this->state[1] = 0xefcdab89;
    this->state[2] = 0x98badcfe;
    this->state[3] = 0x10325476;

    // calls the super
    HashFunction::reset();
}

void Md5::transform(const unsigned char *block, unsigned int blocksize) {
    unsigned int a = this->state[0];
    unsigned int b = this->state[1];
    unsigned int c = this->state[2];
    unsigned int d = this->state[3];

    unsigned int x[MD5_DIGEST_SIZE];

    ByteUtil::decode(x, block, blocksize);

    // the first round of md5 computation
    Md5::FF(a, b, c, d, x[0], MD5_S11, 0xd76aa478);
    Md5::FF(d, a, b, c, x[1], MD5_S12, 0xe8c7b756);
    Md5::FF(c, d, a, b, x[2], MD5_S13, 0x242070db);
    Md5::FF(b, c, d, a, x[3], MD5_S14, 0xc1bdceee);
    Md5::FF(a, b, c, d, x[4], MD5_S11, 0xf57c0faf);
    Md5::FF(d, a, b, c, x[5], MD5_S12, 0x4787c62a);
    Md5::FF(c, d, a, b, x[6], MD5_S13, 0xa8304613);
    Md5::FF(b, c, d, a, x[7], MD5_S14, 0xfd469501);
    Md5::FF(a, b, c, d, x[8], MD5_S11, 0x698098d8);
    Md5::FF(d, a, b, c, x[9], MD5_S12, 0x8b44f7af);
    Md5::FF(c, d, a, b, x[10], MD5_S13, 0xffff5bb1);
    Md5::FF(b, c, d, a, x[11], MD5_S14, 0x895cd7be);
    Md5::FF(a, b, c, d, x[12], MD5_S11, 0x6b901122);
    Md5::FF(d, a, b, c, x[13], MD5_S12, 0xfd987193);
    Md5::FF(c, d, a, b, x[14], MD5_S13, 0xa679438e);
    Md5::FF(b, c, d, a, x[15], MD5_S14, 0x49b40821);

    // the second round of md5 computation
    Md5::GG(a, b, c, d, x[1], MD5_S21, 0xf61e2562);
    Md5::GG(d, a, b, c, x[6], MD5_S22, 0xc040b340);
    Md5::GG(c, d, a, b, x[11], MD5_S23, 0x265e5a51);
    Md5::GG(b, c, d, a, x[0], MD5_S24, 0xe9b6c7aa);
    Md5::GG(a, b, c, d, x[5], MD5_S21, 0xd62f105d);
    Md5::GG(d, a, b, c, x[10], MD5_S22, 0x2441453);
    Md5::GG(c, d, a, b, x[15], MD5_S23, 0xd8a1e681);
    Md5::GG(b, c, d, a, x[4], MD5_S24, 0xe7d3fbc8);
    Md5::GG(a, b, c, d, x[9], MD5_S21, 0x21e1cde6);
    Md5::GG(d, a, b, c, x[14], MD5_S22, 0xc33707d6);
    Md5::GG(c, d, a, b, x[3], MD5_S23, 0xf4d50d87);
    Md5::GG(b, c, d, a, x[8], MD5_S24, 0x455a14ed);
    Md5::GG(a, b, c, d, x[13], MD5_S21, 0xa9e3e905);
    Md5::GG(d, a, b, c, x[2], MD5_S22, 0xfcefa3f8);
    Md5::GG(c, d, a, b, x[7], MD5_S23, 0x676f02d9);
    Md5::GG(b, c, d, a, x[12], MD5_S24, 0x8d2a4c8a);

    // the third round of md5 computation
    Md5::HH(a, b, c, d, x[5], MD5_S31, 0xfffa3942);
    Md5::HH(d, a, b, c, x[8], MD5_S32, 0x8771f681);
    Md5::HH(c, d, a, b, x[11], MD5_S33, 0x6d9d6122);
    Md5::HH(b, c, d, a, x[14], MD5_S34, 0xfde5380c);
    Md5::HH(a, b, c, d, x[1], MD5_S31, 0xa4beea44);
    Md5::HH(d, a, b, c, x[4], MD5_S32, 0x4bdecfa9);
    Md5::HH(c, d, a, b, x[7], MD5_S33, 0xf6bb4b60);
    Md5::HH(b, c, d, a, x[10], MD5_S34, 0xbebfbc70);
    Md5::HH(a, b, c, d, x[13], MD5_S31, 0x289b7ec6);
    Md5::HH(d, a, b, c, x[0], MD5_S32, 0xeaa127fa);
    Md5::HH(c, d, a, b, x[3], MD5_S33, 0xd4ef3085);
    Md5::HH(b, c, d, a, x[6], MD5_S34, 0x4881d05);
    Md5::HH(a, b, c, d, x[9], MD5_S31, 0xd9d4d039);
    Md5::HH(d, a, b, c, x[12], MD5_S32, 0xe6db99e5);
    Md5::HH(c, d, a, b, x[15], MD5_S33, 0x1fa27cf8);
    Md5::HH(b, c, d, a, x[2], MD5_S34, 0xc4ac5665);

    // the fourth round of md5 computation
    Md5::II(a, b, c, d, x[0], MD5_S41, 0xf4292244);
    Md5::II(d, a, b, c, x[7], MD5_S42, 0x432aff97);
    Md5::II(c, d, a, b, x[14], MD5_S43, 0xab9423a7);
    Md5::II(b, c, d, a, x[5], MD5_S44, 0xfc93a039);
    Md5::II(a, b, c, d, x[12], MD5_S41, 0x655b59c3);
    Md5::II(d, a, b, c, x[3], MD5_S42, 0x8f0ccc92);
    Md5::II(c, d, a, b, x[10], MD5_S43, 0xffeff47d);
    Md5::II(b, c, d, a, x[1], MD5_S44, 0x85845dd1);
    Md5::II(a, b, c, d, x[8], MD5_S41, 0x6fa87e4f);
    Md5::II(d, a, b, c, x[15], MD5_S42, 0xfe2ce6e0);
    Md5::II(c, d, a, b, x[6], MD5_S43, 0xa3014314);
    Md5::II(b, c, d, a, x[13], MD5_S44, 0x4e0811a1);
    Md5::II(a, b, c, d, x[4], MD5_S41, 0xf7537e82);
    Md5::II(d, a, b, c, x[11], MD5_S42, 0xbd3af235);
    Md5::II(c, d, a, b, x[2], MD5_S43, 0x2ad7d2bb);
    Md5::II(b, c, d, a, x[9], MD5_S44, 0xeb86d391);

    // sets the new values in the state
    this->state[0] += a;
    this->state[1] += b;
    this->state[2] += c;
    this->state[3] += d;

    // invalidates sensitive information
    memset(x, 0, sizeof(x));
}

/**
 * Returns an hexadecimal representation of the
 * code.
 *
 * @return The hexadecimal representation of the code.
 */
std::string Md5::hexdigest() const {
    // in case the hash computation is not finalized
    if(!this->finalized) {
        // returns empty string
        return std::string();
    }

    // allocates the buffer state
    char buffer[MD5_HEX_DIGEST_SIZE];

    // iterates over all the values in the buffer
    for(int i = 0; i < MD5_DIGEST_SIZE; i++) {
        SPRINTF(&buffer[i * 2], MD5_HEX_DIGEST_SIZE - i * 2, "%02x", this->digest[i]);
    }

    // sets the last value to end of string
    buffer[MD5_HEX_DIGEST_SIZE - 1] = 0;

    // returns the string value
    return std::string(buffer);
}

/**
 * The md5 f computation function.
 *
 * @param x The x value for computation.
 * @param y The y value for computation.
 * @param z The z value for computation.
 */
inline unsigned int Md5::F(unsigned int x, unsigned int y, unsigned int z) {
    return (x & y) | (~x & z);
}

/**
 * The md5 g computation function.
 *
 * @param x The x value for computation.
 * @param y The y value for computation.
 * @param z The z value for computation.
 */
inline unsigned int Md5::G(unsigned int x, unsigned int y, unsigned int z) {
    return (x & z) | (y & ~z);
}

/**
 * The md5 h computation function.
 *
 * @param x The x value for computation.
 * @param y The y value for computation.
 * @param z The z value for computation.
 */
inline unsigned int Md5::H(unsigned int x, unsigned int y, unsigned int z) {
    return x ^ y ^ z;
}

/**
 * The md5 i computation function.
 *
 * @param x The x value for computation.
 * @param y The y value for computation.
 * @param z The z value for computation.
 */
inline unsigned int Md5::I(unsigned int x, unsigned int y, unsigned int z) {
    return y ^ (x | ~z);
}

/**
 * The md5 ff transofrmation fuction.
 *
 * @param a The a value for transformation.
 * @param b The b value for transformation.
 * @param c The c value for transformation.
 * @param d The d value for transformation.
 * @param x The x value for transformation.
 * @param s The s value for transformation.
 * @param ac The ac value for transformation.
 */
inline void Md5::FF(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac) {
    a = CpuUtil::rotateLeft(a + F(b, c, d) + x + ac, s) + b;
}

/**
 * The md5 gg transofrmation fuction.
 *
 * @param a The a value for transformation.
 * @param b The b value for transformation.
 * @param c The c value for transformation.
 * @param d The d value for transformation.
 * @param x The x value for transformation.
 * @param s The s value for transformation.
 * @param ac The ac value for transformation.
 */
inline void Md5::GG(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac) {
    a = CpuUtil::rotateLeft(a + G(b, c, d) + x + ac, s) + b;
}

/**
 * The md5 hh transofrmation fuction.
 *
 * @param a The a value for transformation.
 * @param b The b value for transformation.
 * @param c The c value for transformation.
 * @param d The d value for transformation.
 * @param x The x value for transformation.
 * @param s The s value for transformation.
 * @param ac The ac value for transformation.
 */
inline void Md5::HH(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac) {
    a = CpuUtil::rotateLeft(a + H(b, c, d) + x + ac, s) + b;
}

/**
 * The md5 ii transofrmation fuction.
 *
 * @param a The a value for transformation.
 * @param b The b value for transformation.
 * @param c The c value for transformation.
 * @param d The d value for transformation.
 * @param x The x value for transformation.
 * @param s The s value for transformation.
 * @param ac The ac value for transformation.
 */
inline void Md5::II(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac) {
    a = CpuUtil::rotateLeft(a + I(b, c, d) + x + ac, s) + b;
}
