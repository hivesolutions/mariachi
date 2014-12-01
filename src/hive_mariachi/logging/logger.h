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

#define DEFAULT_HANDLER LoggerStandardOutHandler
#define DEFAULT_FORMATTER LoggerTimeFormatter

#define NOTSET 0
#define DEBUG 1
#define INFO 2
#define WARNING 3
#define FAULT 4
#define CRITICAL 5

/**
 * The default file stream open mode.
 */
#define DEFAULT_FILE_STREAM_OPEN_MODE std::ios_base::app

/**
 * The default logger name.
 */
#define DEFAULT_LOGGER_NAME "default"

namespace mariachi {
    namespace logging {
        class Logger;
        class LoggerHandler;
        class LoggerStreamHandler;
        class LoggerStandardOutHandler;
        class LoggerFileHandler;
        class LoggerFormatter;
        class LoggerTimeFormatter;
        class LoggerStringFormatter;
        class LoggerRecord;

        typedef std::vector<LoggerHandler *> handlersListType;
        typedef std::map<std::string, Logger *> loggersMapType;

        /**
         * The class responsible for the loging of event.
         * This class control the logging event and the verbosity of them.
         */
        class Logger {
            private:
                unsigned int logLevel;
                LoggerHandler *defaultLoggerHandler;
                handlersListType handlersList;
                LoggerFormatter *loggerFormatter;
                static loggersMapType loggersMap;

            public:
                Logger();
                ~Logger();
                void debug(const std::string &value);
                void info(const std::string &value);
                void warning(const std::string &value);
                void fault(const std::string &value);
                void critical(const std::string &value);
                void handle(LoggerRecord *record);
                void setLevel(unsigned int level);
                void addHandler(LoggerHandler *loggerHandler);
                void setDefaultHandler(LoggerHandler *defaultLoggerHandler);
                void setFormatter(LoggerFormatter *loggerFormatter);
                static Logger *getLogger(const std::string &loggerName);
                static Logger *getLogger(char *loggerName);
                static Logger *getLogger();
        };

        class LoggerHandler {
            public:
                LoggerHandler();
                ~LoggerHandler();
                virtual void handle(LoggerRecord *record) { };
                void flush();
        };

        class LoggerStreamHandler : public LoggerHandler {
            protected:
                std::ostream *stream;
                bool streamOpen;

            public:
                LoggerStreamHandler();
                ~LoggerStreamHandler();
                void handle(LoggerRecord *record);
                void flush();
                virtual void openStream() { this->streamOpen = true; };
                virtual void closeStream() { this->streamOpen = false; };
                virtual void flushStream() { this->stream->flush(); };
        };

        class LoggerStandardOutHandler : public LoggerStreamHandler {
            public:
                LoggerStandardOutHandler();
                ~LoggerStandardOutHandler();
                void flush();
                void openStream();
        };

        class LoggerFileHandler : public LoggerStreamHandler {
            private:
                std::string fileName;
                std::ofstream fileStream;

            public:
                LoggerFileHandler();
                LoggerFileHandler(const std::string &fileName);
                ~LoggerFileHandler();
                void flush();
                void openStream();
                void closeStream();
                void setFileName(const std::string &fileName);
        };

        class LoggerFormatter {
            public:
                LoggerFormatter();
                ~LoggerFormatter();
                virtual void format(LoggerRecord *record);
        };

        class LoggerTimeFormatter : public LoggerFormatter {
            public:
                LoggerTimeFormatter();
                ~LoggerTimeFormatter();
                void format(LoggerRecord *record);
                void formatValue(const std::string &value);
        };

        class LoggerStringFormatter : public LoggerFormatter {
            private:
                std::string formatString;

            public:
                LoggerStringFormatter();
                LoggerStringFormatter(const std::string &formatString);
                ~LoggerStringFormatter();
                void setFormatString(const std::string &formatString);
        };

        class LoggerRecord {
            private:
                std::string value;
                unsigned int level;
                std::string formattedValue;

            public:
                LoggerRecord();
                LoggerRecord(const std::string &value, unsigned int level);
                ~LoggerRecord();
                std::string &getValue();
                void setValue(const std::string &value);
                unsigned int getLevel();
                void setLevel(unsigned int level);
                std::string &getFormattedValue();
                void setFormattedValue(const std::string &formattedValue);
        };
    }
}
