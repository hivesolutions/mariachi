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

#include "../../util/bit_util.h"

#define HUFFMAN_SYMBOL_SIZE 8

#define HUFFMAN_SYMBOL_INVALID_SIZE 255

#define HUFFMAN_SYMBOL_TABLE_SIZE 256

#define HUFFMAN_SYMBOL_TABLE_EXTRA_SIZE 257

#define HUFFMAN_SYMBOL_TABLE_EXTRA_VALUE HUFFMAN_SYMBOL_TABLE_EXTRA_SIZE - 1

#define HUFFMAN_FILE_BUFFER_SIZE 10240

#define HUFFMAN_LOOKUP_TABLE_MAXIMUM_CODE_SIZE 20

#define HUFFMAN_LOOKUP_MAXIMUM_CODE_SIZE 64

namespace mariachi {
    namespace algorithms {
        typedef enum HuffmanType_t {
            HUFFMAN_TYPE_NORMAL = 1,
            HUFFMAN_TYPE_LOOKUP_TABLE
        } HuffmanType;

        typedef struct HuffmanNode_t {
            unsigned int value;
            int symbol;
            std::string code;
            HuffmanNode_t *left;
            HuffmanNode_t *right;
            HuffmanNode_t *parent;
        } HuffmanNode;

        typedef struct HuffmanPartialByte_t {
            unsigned char byte;
            unsigned char numberBits;
        } HuffmanPartialByte;

        typedef struct HuffmanCode_t {
            unsigned int quadWord;
            unsigned char numberBits;
        } HuffmanCode;

        typedef struct HuffmanCodeTable_t {
            unsigned long long size;
            HuffmanCode_t *buffer;
        } HuffmanCodeTable;

        typedef struct HuffmanLookupTable_t {
            unsigned long long size;
            unsigned int *buffer;
        } HuffmanLookupTable;

        typedef struct HuffmanHeader_t {
            HuffmanType_t type;
            unsigned long long originalFileSize;
            unsigned int longestCodeSize;
            HuffmanCodeTable_t codeTable;
            HuffmanLookupTable_t loookupTable;
        } HuffmanHeader;

        class Huffman {
            private:
                /**
                 * The occurrence count list, to count the number of
                 * occurrences of each symbol.
                 */
                unsigned int occurrenceCountList[HUFFMAN_SYMBOL_TABLE_SIZE];

                /**
                 * The file stream tath holds the reference to the file.
                 */
                std::fstream *fileStream;

                /**
                 * The huffman table mapping the symbol with the code
                 * represented as a string.
                 */
                std::string huffmanTable[HUFFMAN_SYMBOL_TABLE_EXTRA_SIZE];

                /**
                 * The huffman table mapping the symbol with the code
                 * represented as a vector of partial byte values.
                 */
                std::vector<HuffmanPartialByte_t> huffmanComputedTable[HUFFMAN_SYMBOL_TABLE_EXTRA_SIZE];

                /**
                 * The huffman table mapping the symbol with the code
                 * represented as an huffman codes.
                 */
                HuffmanCode_t huffmanCodeTable[HUFFMAN_SYMBOL_TABLE_EXTRA_SIZE];

                /**
                 * The current type of coding/decoding being used.
                 */
                HuffmanType_t type;

                /**
                 * The size of the longest code value.
                 */
                unsigned int longestCodeSize;

                /**
                 * The original file size.
                 */
                unsigned long long originalFileSize;

                /**
                 * The lookup table structure value.
                 */
                HuffmanLookupTable_t lookupTable;

                inline void initType();
                inline void initFileStream();
                inline void initLookupTable();
                inline void initOccurrenceCountList();
                inline void initLongestCodeSize();
                inline void updateOccurrenceValues(char *buffer, unsigned int size);
                inline int encodeData(char *buffer, util::BitStream *bitStream, unsigned int size);
                inline int decodeData(char *buffer, util::BitStream *bitStream, unsigned int size);
                inline void writeHuffmanStream(util::BitStream *bitStream, unsigned char byte, unsigned char numberBits);
                inline void computeTable();
                inline std::vector<HuffmanPartialByte_t> computeCode(std::string &code);
                inline void cleanStructures(HuffmanNode *node);
                inline void cleanLookupTable();
                inline void cleanFileStream();
                inline void _readHeader(std::iostream *sourceStream);
                inline void _writeHeader(std::iostream *targetStream);
                inline void _readCodeTable(std::iostream *sourceStream);
                inline void _writeCodeTable(std::iostream *targetStream);
                inline void _readLookupTable(std::iostream *sourceStream);
                inline void _writeLookupTable(std::iostream *targetStream);
                void _generateTable(HuffmanNode *node, const std::string &code = std::string(""));
                void _generatePermutations(std::vector<HuffmanCode_t> *huffmanCodeValuesList, char *stringBuffer, unsigned int count);
                HuffmanCode_t _generateHuffmanCode(const char *stringCodeValue, unsigned char stringCodeValueSize = -1);

            public:
                Huffman();
                ~Huffman();
                void encode(const std::string &filePath, const std::string &targetFilePath);
                void encode(const std::string &filePath, std::iostream *targetStream);
                void decode(const std::string &filePath, const std::string &targetFilePath);
                void decode(const std::string &filePath, std::iostream *targetStream);
                void generateTable(const std::string &filePath);
                void generateTable(std::fstream *fileStream);
                void generateLookupTable();
                void printTable();
        };

        class HuffmanNodeCompare {
            private:

            public:
                bool operator()(HuffmanNode_t *firstElement, HuffmanNode_t *secondElement);
        };

        class HuffmanCodeCompare {
            private:

            public:
                bool operator()(HuffmanCode_t *firstElement, HuffmanCode_t *secondElement);
        };
    }
}
