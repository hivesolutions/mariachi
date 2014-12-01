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

#include "value.h"

namespace Json {
    class Value;

    /**
     * \brief Abstract class for writers.
     */
    class Writer {
        public:
            virtual ~Writer();
            virtual std::string write( const Value &root ) = 0;
    };

    /**
     * \brief Outputs a Value in <a HREF="http://www.json.org">JSON</a> format without formatting (not human friendly).
     *
     * The JSON document is written in a single line. It is not intended for 'human' consumption,
     * but may be usefull to support feature such as RPC where bandwith is limited.
     * \sa Reader, Value
     */
    class FastWriter : public Writer {
        public:
            FastWriter();
            virtual ~FastWriter(){ }
            void enableYAMLCompatibility();

        public:
            virtual std::string write(const Value &root);

        private:
            void writeValue(const Value &value);
            std::string document_;
            bool yamlCompatiblityEnabled_;
    };

    /**
     * \brief Writes a Value in <a HREF="http://www.json.org">JSON</a> format in a human friendly way.
     *
     * The rules for line break and indent are as follow:
     * - Object value:
     *     - if empty then print { } without indent and line break
     *     - if not empty the print '{', line break & indent, print one value per line
     *       and then unindent and line break and print '}'.
     * - Array value:
     *     - if empty then print [] without indent and line break
     *     - if the array contains no object value, empty array or some other value types,
     *       and all the values fit on one lines, then print the array on a single line.
     *     - otherwise, it the values do not fit on one line, or the array contains
     *       object or non empty array, then print one value per line.
     *
     * If the Value have comments then they are outputed according to their #CommentPlacement.
     *
     * \sa Reader, Value, Value::setComment()
     */
    class StyledWriter: public Writer {
        public:
            StyledWriter();
            virtual ~StyledWriter(){ }

        public:
            /** \brief Serialize a Value in <a HREF="http://www.json.org">JSON</a> format.
             * \param root Value to serialize.
             * \return String containing the JSON document that represents the root value.
             */
            virtual std::string write(const Value &root);
        private:
            void writeValue(const Value &value);
            void writeArrayValue(const Value &value);
            bool isMultineArray(const Value &value);
            void pushValue(const std::string &value);
            void writeIndent();
            void writeWithIndent(const std::string &value);
            void indent();
            void unindent();
            void writeCommentBeforeValue(const Value &root);
            void writeCommentAfterValueOnSameLine(const Value &root);
            bool hasCommentForValue(const Value &value);
            static std::string normalizeEOL(const std::string &text);

            typedef std::vector<std::string> ChildValues;

            ChildValues childValues_;
            std::string document_;
            std::string indentString_;
            int rightMargin_;
            int indentSize_;
            bool addChildValues_;
   };

    /**
     * \brief Writes a Value in <a HREF="http://www.json.org">JSON</a> format in a human friendly way,
     *    to a stream rather than to a string.
     *
     * The rules for line break and indent are as follow:
     * - Object value:
     *     - if empty then print {} without indent and line break
     *     - if not empty the print '{', line break & indent, print one value per line
     *       and then unindent and line break and print '}'.
     * - Array value:
     *     - if empty then print [] without indent and line break
     *     - if the array contains no object value, empty array or some other value types,
     *       and all the values fit on one lines, then print the array on a single line.
     *     - otherwise, it the values do not fit on one line, or the array contains
     *       object or non empty array, then print one value per line.
     *
     * If the Value have comments then they are outputed according to their #CommentPlacement.
     *
     * \param indentation Each level will be indented by this amount extra.
     * \sa Reader, Value, Value::setComment()
     */
    class StyledStreamWriter {
        public:
            StyledStreamWriter(std::string indentation="\t");
            ~StyledStreamWriter() { }

        public:
            /**
             * \brief Serialize a Value in <a HREF="http://www.json.org">JSON</a> format.
             * \param out Stream to write to. (Can be ostringstream, e.g.)
             * \param root Value to serialize.
             * \note There is no point in deriving from Writer, since write() should not return a value.
             */
            void write(std::ostream &out, const Value &root);

        private:
            void writeValue(const Value &value);
            void writeArrayValue(const Value &value);
            bool isMultineArray(const Value &value);
            void pushValue(const std::string &value);
            void writeIndent();
            void writeWithIndent(const std::string &value);
            void indent();
            void unindent();
            void writeCommentBeforeValue(const Value &root);
            void writeCommentAfterValueOnSameLine(const Value &root);
            bool hasCommentForValue(const Value &value);
            static std::string normalizeEOL(const std::string &text);

            typedef std::vector<std::string> ChildValues;

            ChildValues childValues_;
            std::ostream* document_;
            std::string indentString_;
            int rightMargin_;
            std::string indentation_;
            bool addChildValues_;
    };

    std::string valueToString(Value::Int value);
    std::string valueToString(Value::UInt value);
    std::string valueToString(double value);
    std::string valueToString(bool value);
    std::string valueToQuotedString(const char *value);

    /// \brief Output using the StyledStreamWriter.
    /// \see Json::operator>>()
    std::ostream& operator<<(std::ostream&, const Value &root);
}
