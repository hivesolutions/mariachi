// Hive Mariachi Engine
// Copyright (c) 2008-2017 Hive Solutions Lda.
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
// __copyright__ = Copyright (c) 2008-2017 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#pragma once

#define HASH_STREAM_BUFFER_SIZE 10240

namespace mariachi {
    namespace algorithms {
        class HashFunction {
            private:

            protected:
                /**
                 * Flag to control the finalization of the hash value
                 * computation.
                 */
                bool finalized;

            public:
                HashFunction();
                HashFunction(const std::string &text);
                ~HashFunction();
                virtual void init();
                virtual void init(const std::string &text);
                virtual void init(std::istream &stream);
                virtual void init(std::fstream &fileStream, bool closeStream = true);
                virtual void update(const unsigned char *buffer, unsigned int size);
                virtual void finalize();
                virtual void reset();
                virtual std::string hexdigest() const { return std::string(); };
                friend std::ostream &operator<<(std::ostream &outStream, const HashFunction &value);
        };

        std::ostream &operator<<(std::ostream &outStream, const HashFunction &value);
    }
}
