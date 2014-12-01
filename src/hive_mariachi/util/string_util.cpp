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

#include "string_util.h"

using namespace mariachi::util;

void StringUtil::tokenize(const std::string &stringValue, std::vector<std::string> &tokens, const std::string &delimiters = " ") {
    // skips the delimiters at the beggining
    std::string::size_type lastPosition = stringValue.find_first_not_of(delimiters, 0);

    // finds the first "non-delimiter"
    std::string::size_type position = stringValue.find_first_of(delimiters, lastPosition);

    while(std::string::npos != position || std::string::npos != lastPosition) {
        // Found a token, add it to the vector
        tokens.push_back(stringValue.substr(lastPosition, position - lastPosition));

        // skips delimiters, note the "not_of"
        lastPosition = stringValue.find_first_not_of(delimiters, position);

        // find next "non-delimiter"
        position = stringValue.find_first_of(delimiters, lastPosition);
    }
}
