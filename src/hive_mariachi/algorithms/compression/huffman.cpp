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

#include "../../exceptions/exceptions.h"

#include "huffman.h"

using namespace mariachi::util;
using namespace mariachi::exceptions;
using namespace mariachi::algorithms;

/**
 * Constructor of the class.
 */
Huffman::Huffman() {
    this->initType();
    this->initFileStream();
    this->initLookupTable();
    this->initOccurrenceCountList();
    this->initLongestCodeSize();
}

/**
 * Destructor of the class.
 */
Huffman::~Huffman() {
    // cleans the file stream
    this->cleanFileStream();

    // cleans the lookup table
    this->cleanLookupTable();
}

inline void Huffman::initType() {
    this->type = HUFFMAN_TYPE_LOOKUP_TABLE;
}

inline void Huffman::initFileStream() {
    // invalidates the file stream
    this->fileStream = NULL;
}

inline void Huffman::initLookupTable() {
    // invalidates the lookup buffer
    this->lookupTable.buffer = NULL;

    // sets the lookup table size to zero
    this->lookupTable.size = 0;
}


inline void Huffman::initOccurrenceCountList() {
    // resets the occurrence count list
    memset(this->occurrenceCountList, NULL, sizeof(unsigned int) * HUFFMAN_SYMBOL_TABLE_SIZE);
}

inline void Huffman::initLongestCodeSize() {
    this->longestCodeSize = 0;
}

void Huffman::encode(const std::string &filePath, const std::string &targetFilePath) {
    // creates the target file stream to be used
    std::fstream targetFileStream(targetFilePath.c_str(), std::fstream::out | std::fstream::binary);

    // in case the opening of the file fails
    if(targetFileStream.fail()) {
        // throws a runtime exception
        throw RuntimeException("Problem while loading file: " + filePath);
    }

    // encodes the file
    this->encode(filePath, &targetFileStream);

    // closes the target file stream
    targetFileStream.close();
}

void Huffman::encode(const std::string &filePath, std::iostream *targetStream) {
    // generates the table
    this->generateTable(filePath);

    // generates the lookup table
    this->generateLookupTable();

    // writes the header to the target stream
    this->_writeHeader(targetStream);

    // allocates space for the read size
    unsigned int readSize;

    // allocates the file buffer
    char fileBuffer[HUFFMAN_FILE_BUFFER_SIZE];

    // creates the target bit stream
    BitStream targetBitStream(targetStream);

    // opens the target bit stream in write mode
    targetBitStream.open(BIT_STREAM_WRITE);

    // iterates continuously
    while(1) {
        // reads the buffer
        this->fileStream->read(fileBuffer, HUFFMAN_FILE_BUFFER_SIZE);

        // retrieves the read size
        readSize = this->fileStream->gcount();

        // encodes the data to the target stream
        this->encodeData(fileBuffer, &targetBitStream, readSize);

        // in case the end of file was reached
        if(this->fileStream->eof()) {
            // breaks the cycle
            break;
        }
    }

    // closes the target bit stream
    targetBitStream.close();

    // cleans the file stream
    this->cleanFileStream();
}

void Huffman::decode(const std::string &filePath, const std::string &targetFilePath) {
    // creates the target file stream to be used
    std::fstream targetFileStream(targetFilePath.c_str(), std::fstream::out | std::fstream::binary);

    // in case the opening of the file fails
    if(targetFileStream.fail()) {
        // throws a runtime exception
        throw RuntimeException("Problem while loading file: " + filePath);
    }

    // decodes the file
    this->decode(filePath, &targetFileStream);

    // closes the target file stream
    targetFileStream.close();
}

void Huffman::decode(const std::string &filePath, std::iostream *targetStream) {
    // creates the file stream to be used
    std::fstream *fileStream = new std::fstream(filePath.c_str(), std::fstream::in | std::fstream::binary);

    // in case the opening of the file fails
    if(fileStream->fail()) {
        // throws a runtime exception
        throw RuntimeException("Problem while loading file: " + filePath);
    }

    // reads the header from the file stream
    this->_readHeader(fileStream);

    // allocates space for the read size
    unsigned int readSize;

    // allocates the file buffer
    char fileBuffer[HUFFMAN_FILE_BUFFER_SIZE];

    // creates the target bit stream
    BitStream bitStream(fileStream);

    // opens the bit stream in read mode
    bitStream.open(BIT_STREAM_READ);

    // starts the size counter with the original file size
    unsigned long long sizeCounter = this->originalFileSize;

    // iterates continuously
    while(1) {
        // decodes the data to the target buffer
        // retrieving the read size
        readSize = this->decodeData(fileBuffer, &bitStream, HUFFMAN_FILE_BUFFER_SIZE);

        // retrieves the number of symbols to be writen
        // in case end of file is reached the number of symbols to be writen
        // is number in the size counter (remaining symbols)
        sizeCounter < (unsigned long long) readSize ? readSize = (unsigned int) sizeCounter : readSize = readSize;

        // writes the buffer
        targetStream->write(fileBuffer, readSize);

        // decrements the size by the
        // number of symbols read
        sizeCounter -= readSize;

        // in case the end of file was reached
        if(!sizeCounter) {
            // breaks the cycle
            break;
        }
    }

    // closes the bit stream
    bitStream.close();

    // closes the file stream
    fileStream->close();
}

void Huffman::generateTable(const std::string &filePath) {
    // creates the file stream to be used
    std::fstream *fileStream = new std::fstream(filePath.c_str(), std::fstream::in | std::fstream::binary);

    // in case the opening of the file fails
    if(fileStream->fail()) {
        // throws a runtime exception
        throw RuntimeException("Problem while loading file: " + filePath);
    }

    // generates the table for the file stream
    this->generateTable(fileStream);
}

void Huffman::generateTable(std::fstream *fileStream) {
    // sets the file stream
    this->fileStream = fileStream;

    // seeks to the end of the file
    this->fileStream->seekg(0, std::fstream::end);

    // get length of file:
    this->originalFileSize = this->fileStream->tellg();

    // seeks to the beginning of the file
    this->fileStream->seekg(0, std::fstream::beg);

    // allocates the file buffer
    char fileBuffer[HUFFMAN_FILE_BUFFER_SIZE];

    // allocates space for the read size
    unsigned int readSize;

    // allocates space for the lowest huffman node
    HuffmanNode_t *lowestHuffmanNode;

    // allocates space for the second lowest huffman node
    HuffmanNode_t *secondLowestHuffmanNode;

    // iterates continuously
    while(1) {
        // reads the buffer
        this->fileStream->read(fileBuffer, HUFFMAN_FILE_BUFFER_SIZE);

        // retrieves the read size
        readSize = this->fileStream->gcount();

        // updates the occurence values for the file buffer
        this->updateOccurrenceValues(fileBuffer, readSize);

        // in case the end of file was reached
        if(this->fileStream->eof()) {
            // breaks the cycle
            break;
        }
    }

    // clears the error bits
    this->fileStream->clear();

    // seeks to the beginning of the file
    this->fileStream->seekg(0, std::fstream::beg);

    // creates the code table priority queue
    std::priority_queue<HuffmanNode_t *, std::vector<HuffmanNode_t *>, HuffmanNodeCompare> codeTable;

    // allocates space for the huffman nodes buffer
    HuffmanNode_t huffmanNodesBuffer[HUFFMAN_SYMBOL_TABLE_SIZE];

    // iterates over all the huffman symbols
    for(unsigned int index = 0; index < HUFFMAN_SYMBOL_TABLE_SIZE; index++) {
        // in case the digit did not occurred any time
        if(!this->occurrenceCountList[index]) {
            // continues the loop
            continue;
        }

        // retrieves the current huffman node
        HuffmanNode_t &currentHuffmanNode = huffmanNodesBuffer[index];

        // sets the current huffman node values
        currentHuffmanNode.value = this->occurrenceCountList[index];
        currentHuffmanNode.symbol = index;
        currentHuffmanNode.code = std::string();
        currentHuffmanNode.left = NULL;
        currentHuffmanNode.right = NULL;
        currentHuffmanNode.parent = NULL;

        // adds the current huffman node to the code table
        codeTable.push(&currentHuffmanNode);
    }

    // iterates continuously
    while(1) {
        // the lowest value node
        lowestHuffmanNode = codeTable.top();

        // pops the code table
        codeTable.pop();

        // in case
        if(codeTable.empty()) {
            // breaks the cycle
            break;
        } else {
            // the second lowest value node
            secondLowestHuffmanNode = codeTable.top();

            // pops the code table
            codeTable.pop();

            // creates the merged huffman node
            HuffmanNode_t *mergedHuffmanNode = new HuffmanNode_t();

            // sets the merged huffman node values
            mergedHuffmanNode->value = lowestHuffmanNode->value + secondLowestHuffmanNode->value;
            mergedHuffmanNode->symbol = HUFFMAN_SYMBOL_TABLE_EXTRA_VALUE;
            mergedHuffmanNode->code = std::string();
            mergedHuffmanNode->left = secondLowestHuffmanNode;
            mergedHuffmanNode->right = lowestHuffmanNode;
            mergedHuffmanNode->parent = NULL;

            // sets the parent in the huffman nodes
            lowestHuffmanNode->parent = mergedHuffmanNode;
            secondLowestHuffmanNode->parent = mergedHuffmanNode;

            // adds the merged node to the priority_queue
            codeTable.push(mergedHuffmanNode);
        }
    }

    // generates the huffman table
    this->_generateTable(lowestHuffmanNode);

    // computes the table
    this->computeTable();

    // cleans the structures
    this->cleanStructures(lowestHuffmanNode);
}

/**
 * Generates the lookup table for the current huffman
 * table values.
 */
void Huffman::generateLookupTable() {
    // in case the longest code size exceeds the limit
    if(this->longestCodeSize > HUFFMAN_LOOKUP_TABLE_MAXIMUM_CODE_SIZE) {
        // throws runtime exception
        throw RuntimeException("Unable to create lookup table maximum code size limit exceeded");
    }

    // calculates the lookup table size
    size_t lookupTableSize = 2 << (this->longestCodeSize - 1) ;

    // allocates space for the lookup table
    unsigned int *lookupTable = (unsigned int *) malloc(sizeof(unsigned int) * lookupTableSize);

    // resets the lookup table
    memset(lookupTable, NULL, sizeof(unsigned int) * lookupTableSize);

    // allocates the string buffer
    char *stringBuffer = (char *) malloc(sizeof(char) * (this->longestCodeSize + 1));

    // puts the end of string value in the string buffer
    stringBuffer[this->longestCodeSize] = 0;

    // iterates over all the symbols
    for(unsigned int index = 0; index < HUFFMAN_SYMBOL_TABLE_SIZE; index++) {
        // retrieves the current code
        std::string &currentCode = this->huffmanTable[index];

        // retrieves the current code size
        unsigned int currentCodeSize = currentCode.size();

        // in case the current code size is valid (valid code)
        if(currentCodeSize) {
            // copies the current code string to the string buffer
            memcpy(stringBuffer, currentCode.c_str(), currentCodeSize);

            // creates the list to hold the permutation huffman codes values
            std::vector<HuffmanCode_t> permutationHuffmanCodesValuesList;

            // generates the permutation values
            this->_generatePermutations(&permutationHuffmanCodesValuesList, stringBuffer, currentCodeSize);

            // retrieves the permutation huffman codes values list
            std::vector<HuffmanCode_t>::iterator permutationHuffmanCodesValuesListIterator = permutationHuffmanCodesValuesList.begin();

            // iterates over all the permutations
            while(permutationHuffmanCodesValuesListIterator != permutationHuffmanCodesValuesList.end()) {
                // retrieves the current permutation huffman code
                HuffmanCode_t &permutationHuffmanCode = *permutationHuffmanCodesValuesListIterator;

                // sets the permutation string for the index in the lookup table
                lookupTable[permutationHuffmanCode.quadWord] = index;

                // increments the permutation huffman codes values list iterator
                permutationHuffmanCodesValuesListIterator++;
            }
        }
    }
    // cleans the lookup table
    this->cleanLookupTable();

    // sets the lookup table size
    this->lookupTable.size = (unsigned long long) lookupTableSize;

    // sets the lookup table buffer
    this->lookupTable.buffer = lookupTable;

    // releases the string buffer
    free(stringBuffer);
}

/**
 * Prints the huffman table information to the standard output.
 * The information contained is pretty printed with the symbol and
 * the associated code information.
 */
void Huffman::printTable() {
    // iterates over all the symbols
    for(unsigned int index = 0; index < HUFFMAN_SYMBOL_TABLE_SIZE; index++) {
        // retrieves the current symbol
        int currentSymbol = index;

        // retrieves the current code
        std::string &currentCode = this->huffmanTable[index];

        // prints the table line information
        std::cout << currentSymbol << " (" << (char) currentSymbol << "): " << currentCode << std::endl;
    }
}

/**
 * Updates the occurrence values, using the values in the given
 * buffer with the given size.
 *
 * @param buffer The buffer to calculate the occurrence values.
 * @param size The size of the buffer to calculate the occurrence values.
 */
inline void Huffman::updateOccurrenceValues(char *buffer, unsigned int size) {
    // allocates the current byte value
    unsigned char currentByte;

    // iterates over all the read bytes
    for(unsigned int index  = 0; index < size; index++) {
        // retrieves the current byte
        currentByte = buffer[index];

        // increments the current byte value
        this->occurrenceCountList[currentByte] += 1;
    }
}

inline int Huffman::encodeData(char *buffer, BitStream *bitStream, unsigned int size) {
    // iterates over all the symbols in the buffer
    for(unsigned int index = 0; index < size; index++) {
        // retrieves te current symbol
        unsigned char currentSymbol = buffer[index];

        // retrieves the computed code for the current symbol
        std::vector<HuffmanPartialByte_t> &computedCode  = this->huffmanComputedTable[currentSymbol];

        // retrieves the computed code size
        unsigned int computedCodeSize = computedCode.size();

        // iterates over all the computed code partial bytes
        for(unsigned int _index = 0; _index < computedCodeSize; _index++) {
            // retrieves the computed code partial byte
            HuffmanPartialByte_t &computedCodePartialByte = computedCode[_index];

            // writes the computed code partial byte to the bit stream
            bitStream->write(&computedCodePartialByte.byte, computedCodePartialByte.numberBits);
        }
    }

    // returns the number of encoded symbols
    return size;
}

inline int Huffman::decodeData(char *buffer, BitStream *bitStream, unsigned int size) {
    // calculates the number of bytes to be read
    unsigned int numberBytes = this->longestCodeSize / BIT_STREAM_SYMBOL_SIZE;

    // calculates the remaining bits
    unsigned int remainingBits = this->longestCodeSize % BIT_STREAM_SYMBOL_SIZE;

    // allocates the bit buffer
    unsigned char bitBuffer[HUFFMAN_LOOKUP_MAXIMUM_CODE_SIZE];

    // iterates over all the symbols to be decoded
    for(unsigned int index = 0; index < size; index++) {
        // reads the value to the bit buffer
        unsigned int numberReadBits = bitStream->read(bitBuffer, this->longestCodeSize);

        // starts the code value
        unsigned long long code = 0;

        // iterates over all the bytes to convert the byte buffer
        // into a quad word code value
        for(unsigned _index = 0; _index < numberBytes; _index++) {
            // retrieves the current byte
            unsigned char currentByte = bitBuffer[_index];

            // shifts the code value by the
            // huffman symbol syze value
            code <<= HUFFMAN_SYMBOL_SIZE;

            // adds the current byte to the code
            code |= currentByte;
        }

        // in case there are remaining bits
        if(remainingBits) {
            // retrieves the current byte
            unsigned char currentByte = bitBuffer[numberBytes];

            // shifts the code value by the
            // huffman symbol syze value
            code <<= remainingBits;

            // adds the current byte to the code
            code |= currentByte;
        }

        // retrieves the symbol
        unsigned int symbol = this->lookupTable.buffer[code];

        // sets the symbol in the buffer
        buffer[index] = symbol;

        // retrives the huffman code for the symbol
        HuffmanCode_t huffmanCode = this->huffmanCodeTable[symbol];

        // calculates the extra bits
        int extraBits = huffmanCode.numberBits - numberReadBits;

        // in case the end of the stream has been reached
        // and no seek is beeing made (end of huffman stream)
        if(bitStream->eof() && extraBits == 0) {
            // returns the number of encoded symbols
            return index + 1;
        }

        // seeks the extra positions
        bitStream->seekRead(extraBits);
    }

    // returns the number of decoded symbols
    return size;
}

inline void Huffman::computeTable() {
    // iterates over all the symbols
    for(unsigned int index = 0; index < HUFFMAN_SYMBOL_TABLE_SIZE; index++) {
        // retrieves the current symbol
        int currentSymbol = index;

        // retrieves the current code
        std::string &currentCode = this->huffmanTable[index];

        // retrieves the current code size
        unsigned int currentCodeSize = currentCode.size();

        // in case the current code size is bigger than
        // the current longest code size
        if(currentCodeSize > this->longestCodeSize) {
            this->longestCodeSize = currentCodeSize;
        }

        // computes the current code
        const std::vector<HuffmanPartialByte_t> &currentComputedCode = this->computeCode(currentCode);

        // sets the current computed code in the huffman computed table
        this->huffmanComputedTable[currentSymbol] = currentComputedCode;

        // generetes the huffman code for the the current code
        const HuffmanCode &currentHuffmanCode = this->_generateHuffmanCode(currentCode.c_str(), currentCodeSize);

        // sets the current huffman code in the huffman code table
        this->huffmanCodeTable[currentSymbol] = currentHuffmanCode;
    }
}

inline std::vector<HuffmanPartialByte_t> Huffman::computeCode(std::string &code) {
    // creates the code vector
    std::vector<HuffmanPartialByte_t> codeVector;

    // allocates space for the partial byte
    HuffmanPartialByte_t partialByte;

    // retrieves the code iterator
    std::string::iterator codeIterator = code.begin();

    // creates the bit counter
    unsigned char bitCounter = HUFFMAN_SYMBOL_INVALID_SIZE;

    // sets the current char
    unsigned char currentChar = 0;

    // iterates over the code characters
    while(codeIterator != code.end()) {
        // switches over the bit counter
        switch(bitCounter) {
            // in case the bit counter is invalid (initial iteration)
            case HUFFMAN_SYMBOL_INVALID_SIZE:
                // resets the bit counter
                bitCounter = 0;

                break;
            // in case the bit counter is the symbol size (end of byte)
            case HUFFMAN_SYMBOL_SIZE:
                // creates the partial byte
                partialByte.byte = currentChar;
                partialByte.numberBits = bitCounter;

                // adds the partial byte to the code vector
                codeVector.push_back(partialByte);

                // resets the current char
                currentChar = 0;

                // resets the bit counter
                bitCounter = 0;

                break;
            default:
                // shift the current char value
                currentChar <<= 1;

                break;
        }

        // retrieves the current code character
        char currentCodeCharacter = *codeIterator;

        // updates the current char value
        currentChar = currentChar | (currentCodeCharacter & 0x01);

        // increments the code iterator
        codeIterator++;

        // increments the bit counter
        bitCounter++;
    }

    // in case there are bits left to be added
    // to the code vector
    if(bitCounter > 0) {
        // creates the partial byte
        partialByte.byte = currentChar;
        partialByte.numberBits = bitCounter;

        // adds the partial byte to the code vector
        codeVector.push_back(partialByte);
    }

    // returns the code vector
    return codeVector;
}

inline void Huffman::cleanStructures(HuffmanNode *node) {
    // in case it's not a leaf ndoe
    if(node->left && node->right) {
        this->cleanStructures(node->left);
        this->cleanStructures(node->right);

        // deletes the node
        delete node;
    }
}

inline void Huffman::cleanFileStream() {
    // in case the file stream is valid
    if(this->fileStream) {
        // closes the file stream
        this->fileStream->close();

        // deletes the file stream
        delete this->fileStream;

        // invalidates the file stream
        this->fileStream = NULL;
    }
}

inline void Huffman::cleanLookupTable() {
    // in case the lookup table buffer is valid
    if(this->lookupTable.buffer) {
        // releases the lookup buffer
        free(this->lookupTable.buffer);

        // invaidates the lookup table buffer
        this->lookupTable.buffer = NULL;

        // sets the lookup table size
        this->lookupTable.size = 0;
    }
}

inline void Huffman::_readHeader(std::iostream *sourceStream) {
    // retrieves the type size
    size_t typeSize = sizeof(HuffmanType_t);

    // reads the type from the source stream
    sourceStream->read((char *) &this->type, typeSize);

    // retrieves the original file size size
    size_t originalFileSizeSize = sizeof(unsigned long long);

    // reads the original file size from the source stream
    sourceStream->read((char *) &this->originalFileSize, originalFileSizeSize);

    // retrieves the longest code size size
    size_t longestCodeSizeSize = sizeof(unsigned int);

    // reads the longest code size from the source stream
    sourceStream->read((char *) &this->longestCodeSize, longestCodeSizeSize);

    // reads the code table from the source stream
    this->_readCodeTable(sourceStream);

    // cleans the lookup table
    this->cleanLookupTable();

    // reads the lookup table from the source stream
    this->_readLookupTable(sourceStream);
}

inline void Huffman::_writeHeader(std::iostream *targetStream) {
    // retrieves the type size
    size_t typeSize = sizeof(HuffmanType_t);

    // writes the type to the target stream
    targetStream->write((char *) &this->type, typeSize);

    // retrieves the original file size size
    size_t originalFileSizeSize = sizeof(unsigned long long);

    // writes the original file size to the target stream
    targetStream->write((char *) &this->originalFileSize, originalFileSizeSize);

    // retrieves the longest code size size
    size_t longestCodeSize = sizeof(unsigned int);

    // writes the longest code size to the target stream
    targetStream->write((char *) &this->longestCodeSize, longestCodeSize);

    // writes the code table to the target stream
    this->_writeCodeTable(targetStream);

    // writes the lookup table to the target stream
    this->_writeLookupTable(targetStream);
}

inline void Huffman::_readCodeTable(std::iostream *sourceStream) {
    // allocates space for the code table size
    unsigned long long codeTableSize;

    // retrieves the code table base size
    size_t codeTableBaseSize = sizeof(unsigned long long);

    // reads the code table from the source stream
    sourceStream->read((char *) &codeTableSize, codeTableBaseSize);

    // retrieves the code table buffer size
    size_t codeTableBufferSize = sizeof(HuffmanCode_t) * (size_t) codeTableSize;

    // reads the code table buffer from the source stream
    sourceStream->read((char *) this->huffmanCodeTable, codeTableBufferSize);
}

inline void Huffman::_writeCodeTable(std::iostream *targetStream) {
    // allocates space for the code table size
    unsigned long long codeTableSize = HUFFMAN_SYMBOL_TABLE_SIZE;

    // retrieves the code table base size
    size_t codeTableBaseSize = sizeof(unsigned long long);

    // retrieves the code table buffer size
    size_t codeTableBufferSize = sizeof(HuffmanCode_t) * (size_t) HUFFMAN_SYMBOL_TABLE_SIZE;

    // writes the code table to the target stream
    targetStream->write((char *) &codeTableSize, codeTableBaseSize);

    // writes the code table buffer to the target stream
    targetStream->write((char *) this->huffmanCodeTable, codeTableBufferSize);
}

inline void Huffman::_readLookupTable(std::iostream *sourceStream) {
    // retrieves the lookup table base size
    size_t lookupTableBaseSize = sizeof(unsigned long long);

    // reads the lookup table from the source stream
    sourceStream->read((char *) &this->lookupTable, lookupTableBaseSize);

    // retrieves the lookup table buffer size
    size_t lookupTableBufferSize = sizeof(unsigned int) * (size_t) this->lookupTable.size;

    // allocates space for the lookup table
    this->lookupTable.buffer = (unsigned int *) malloc(lookupTableBufferSize);

    // reads the lookup table buffer from the source stream
    sourceStream->read((char *) this->lookupTable.buffer, lookupTableBufferSize);
}

inline void Huffman::_writeLookupTable(std::iostream *targetStream) {
    // retrieves the lookup table base size
    size_t lookupTableBaseSize = sizeof(unsigned long long);

    // retrieves the lookup table buffer size
    size_t lookupTableBufferSize = sizeof(unsigned int) * (size_t) this->lookupTable.size;

    // writes the lookup table to the target stream
    targetStream->write((char *) &this->lookupTable, lookupTableBaseSize);

    // writes the lookup table buffer to the target stream
    targetStream->write((char *) this->lookupTable.buffer, lookupTableBufferSize);
}

void Huffman::_generateTable(HuffmanNode *node, const std::string &code) {
    // in case it's not a leaf ndoe
    if(node->left && node->right) {
        this->_generateTable(node->left, code + "0");
        this->_generateTable(node->right, code + "1");
    }

    // sets the code fot the current symbol
    // in the huffman table
    this->huffmanTable[node->symbol] = code;
}

void Huffman::_generatePermutations(std::vector<HuffmanCode_t> *huffmanCodeValuesList, char *stringBuffer, unsigned int count) {
    // in case the count is invalid
    if(count == this->longestCodeSize) {
        // generates the huffman code for the current string buffer
        const HuffmanCode_t &huffmanCode = this->_generateHuffmanCode(stringBuffer, count);

        // adds the huffman code to the huffman code values list list
        huffmanCodeValuesList->push_back(huffmanCode);

        // returns immediately
        return;
    }

    // iterates over the binary numbers
    for(unsigned char index = 0; index < 2; index++) {
        // retrieves the index value in character mode
        char indexValue = 0x30 | index;

        // sets the index value in the string buffer
        stringBuffer[count] = indexValue;

        // tests if it's not the last digit to be generated
        if(count < this->longestCodeSize - 1) {
            // continues to generates permutations
            this->_generatePermutations(huffmanCodeValuesList, stringBuffer, count + 1);
        } else {
            // generates the huffman code for the current string buffer
            const HuffmanCode_t &huffmanCode = this->_generateHuffmanCode(stringBuffer, count + 1);

            // adds the huffman code to the huffman code values list list
            huffmanCodeValuesList->push_back(huffmanCode);
        }
    }
}

HuffmanCode_t Huffman::_generateHuffmanCode(const char *stringCodeValue, unsigned char stringCodeValueSize) {
    // allocates the space for the huffman code
    HuffmanCode_t huffmanCode = { 0, stringCodeValueSize };

    // iterates over all the bits in the string code value
    for(unsigned char index = 0; index < stringCodeValueSize; index++) {
        // shifts the double word one bit ot the left
        huffmanCode.quadWord <<= 1;

        // retrives the character representation of the bit
        char stringCodeCharValue = stringCodeValue[index];

        // sets the bit value in the double word
        huffmanCode.quadWord |= stringCodeCharValue & 0x01;
    }

    // returns the huffman code
    return huffmanCode;
}

bool HuffmanNodeCompare::operator()(HuffmanNode_t *firstElement, HuffmanNode_t *secondElement) {
    // returns the result of the comparison between the first element
    // and the second element
    return firstElement->value > secondElement->value;
}

bool HuffmanCodeCompare::operator ()(HuffmanCode_t *firstElement, HuffmanCode_t *secondElement) {
    // returns the result of the comparison between the first element
    // and the second element
    return firstElement->quadWord > secondElement->quadWord;
}
