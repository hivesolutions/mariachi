// Hive Mariachi Engine
// Copyright (c) 2008-2020 Hive Solutions Lda.
//
// This file is part of Hive Mariachi Engine.
//
// Hive Mariachi Engine is free software: you can redistribute it and/or modify
// it under the terms of the Apache License as published by the Apache
// Foundation, either version 2.0 of the License, or (at your option) any
// later version.
//
// Hive Mariachi Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// Apache License for more details.
//
// You should have received a copy of the Apache License along with
// Hive Mariachi Engine. If not, see <http://www.apache.org/licenses/>.

// __author__    = João Magalhães <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008-2020 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#pragma once

namespace mariachi {
    namespace util {
        class ByteUtil {
            private:

            public:
                /**
                 * Decodes multiple unsigned characters into an unsigned integer buffer.
                 * Assumes size is a multiple of 4.
                 *
                 * @param output The output buffer.
                 * @param input The input buffer.
                 * @param size The size of the input buffer (in bytes).
                 */
                inline static void decode(unsigned int *output, const unsigned char *input, unsigned int size) {
                    for(unsigned int i = 0, j = 0; j < size; i++, j += 4) {
                        output[i] = ((unsigned int) input[j]) | (((unsigned int) input[j + 1]) << 8) |
                            (((unsigned int) input[j + 2]) << 16) | (((unsigned int) input[j + 3]) << 24);
                    }
                }

                /**
                 * Encodes multiple unsigned integers into an unsigned character buffer.
                 * Assumes size is a multiple of 4.
                 *
                 * @param output The output buffer.
                 * @param input The input buffer.
                 * @param size The size of the input buffer (in bytes).
                 */
                inline static void encode(unsigned char *output, const unsigned int *input, unsigned int size) {
                    for(unsigned int i = 0, j = 0; j < size; i++, j += 4) {
                        output[j] = input[i] & 0xff;
                        output[j+1] = (input[i] >> 8) & 0xff;
                        output[j+2] = (input[i] >> 16) & 0xff;
                        output[j+3] = (input[i] >> 24) & 0xff;
                    }
                }
        };
    }
}
