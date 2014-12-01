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

#include "../system/system.h"

#include "logger.h"

using namespace mariachi::logging;

/**
 * Constuctor of the class.
 */
Logger::Logger() {
    LoggerHandler *defaultLoggerHandler = new DEFAULT_HANDLER();
    LoggerFormatter *defaultLoggerFormatter = new DEFAULT_FORMATTER();

    this->addHandler(defaultLoggerHandler);
    this->setFormatter(defaultLoggerFormatter);
}

/**
 * Destructor of the class.
 */
Logger::~Logger() {

}

void Logger::debug(const std::string &value) {
    // creates a new logger debug record
    LoggerRecord record(value, DEBUG);

    // handles the logger record
    this->handle(&record);
}

void Logger::info(const std::string &value) {
    // creates a new logger info record
    LoggerRecord record(value, INFO);

    // handles the logger record
    this->handle(&record);
}

void Logger::warning(const std::string &value) {
    // creates a new logger warning record
    LoggerRecord record(value, WARNING);

    // handles the logger record
    this->handle(&record);
}

void Logger::fault(const std::string &value) {
    // creates a new logger fault record
    LoggerRecord record(value, FAULT);

    // handles the logger record
    this->handle(&record);
}

void Logger::critical(const std::string &value) {
    // creates a new logger critical record
    LoggerRecord record(value, CRITICAL);

    // handles the logger record
    this->handle(&record);
}

void Logger::handle(LoggerRecord *record) {
    // in case the log level is not suficient ignores it
    if(record->getLevel() < this->logLevel)
        return;

    // retrieves the handlers list iterator
    handlersListType::iterator handlersListIterator;

    // calls the logger format to format the record
    this->loggerFormatter->format(record);

    // iterates over all the handlers to handle the record
    for(handlersListIterator = this->handlersList.begin(); handlersListIterator != this->handlersList.end(); handlersListIterator++) {
        // retrieves the current logger handler
        LoggerHandler *loggerHandler = *handlersListIterator;

        // sends the record to the logger handler for handling
        loggerHandler->handle(record);
    }
}

/**
 * Sets the log level.
 *
 * @param level The log level.
 */
void Logger::setLevel(unsigned int level) {
    this->logLevel = level;
}

void Logger::addHandler(LoggerHandler *loggerHandler) {
    this->handlersList.push_back(loggerHandler);
}

void Logger::setDefaultHandler(LoggerHandler *defaultLoggerHandler) {

}

void Logger::setFormatter(LoggerFormatter *loggerFormatter) {
    this->loggerFormatter = loggerFormatter;
}

Logger *Logger::getLogger(const std::string &loggerName) {
    // allocates the logger reference
    Logger *logger;

    // retrieves the logger iterator finding the logger name
    loggersMapType::iterator loggerIterator = Logger::loggersMap.find(loggerName);

    // in case the result is valid
    if(loggerIterator != loggersMap.end()) {
        // retrieves the logger from the iterator
        logger = loggerIterator->second;
    }
    // in case the result is invalid
    else {
        // creates a new logger
        logger = new Logger();

        // sets the logger in the loggers map
        Logger::loggersMap[loggerName] = logger;
    }

    // returns the logger
    return logger;
}

Logger *Logger::getLogger(char *loggerName) {
    return Logger::getLogger(std::string(loggerName));
}

Logger *Logger::getLogger() {
    return Logger::getLogger(std::string(DEFAULT_LOGGER_NAME));
}

loggersMapType Logger::loggersMap = loggersMapType();

LoggerHandler::LoggerHandler() {
}

LoggerHandler::~LoggerHandler() {
}

LoggerStreamHandler::LoggerStreamHandler() : LoggerHandler() {
    this->streamOpen = false;
}

LoggerStreamHandler::~LoggerStreamHandler() {
}

void LoggerStreamHandler::handle(LoggerRecord *record) {
    // in case the stream is not open
    if(!this->streamOpen) {
        // opesn the stream
        this->openStream();
    }

    // retrieves the formatted value and puts it into the stream
    *(this->stream) << record->getFormattedValue() << std::endl;

    // flushes the stream
    this->flushStream();
}

LoggerStandardOutHandler::LoggerStandardOutHandler() : LoggerStreamHandler() {
}

LoggerStandardOutHandler::~LoggerStandardOutHandler() {
}

void LoggerStandardOutHandler::openStream() {
    LoggerStreamHandler::openStream();
    this->stream = &std::cout;
}

LoggerFileHandler::LoggerFileHandler() : LoggerStreamHandler() {
}

LoggerFileHandler::LoggerFileHandler(const std::string &fileName) : LoggerStreamHandler() {
    this->setFileName(fileName);
    this->openStream();
}

LoggerFileHandler::~LoggerFileHandler() {
}

void LoggerFileHandler::openStream() {
    LoggerStreamHandler::openStream();
    std::ofstream *stream = new std::ofstream();
    stream->open(this->fileName.c_str(), DEFAULT_FILE_STREAM_OPEN_MODE);
    this->stream = stream;
}

void LoggerFileHandler::closeStream() {
    LoggerStreamHandler::closeStream();
    std::ofstream *stream = (std::ofstream *) this->stream;
    stream->close();
}

void LoggerFileHandler::setFileName(const std::string &fileName) {
    this->fileName = fileName;
}

LoggerFormatter::LoggerFormatter() {
}


LoggerFormatter::~LoggerFormatter() {
}

void LoggerFormatter::format(LoggerRecord *record) {
    // retrieves the record level
    unsigned int recordLevel = record->getLevel();

    // retrieves the record value
    std::string &recordValue = record->getValue();

    // creates the record level string
    std::string recordLevelString;

    // switches in the record level
    switch(recordLevel) {
        case DEBUG:
            recordLevelString += "DEBUG";
            break;
        case INFO:
            recordLevelString += "INFO";
            break;
        case WARNING:
            recordLevelString += "WARNING";
            break;
        case FAULT:
            recordLevelString += "FAULT";
            break;
        case CRITICAL:
            recordLevelString += "CRITICAL";
            break;
    }

    // creates the record formatted value from the
    // record level string and the record value
    std::string recordFormattedValue = recordLevelString + " " + recordValue;

    // sets the record formatted value
    record->setFormattedValue(recordFormattedValue);
}

LoggerTimeFormatter::LoggerTimeFormatter() : LoggerFormatter() {
}

LoggerTimeFormatter::~LoggerTimeFormatter() {
}

void LoggerTimeFormatter::format(LoggerRecord *record) {
    LoggerFormatter::format(record);

    // retrieves the formatted value
    std::string &formattedValue = record->getFormattedValue();

    // creates the time buffer
    char timeBuffer[1024];

    // creates the local time value
    tm *localTimeValue;

    // retrieves the current time value
    time_t timeValue = time(NULL);

    // converts the time to local time
    LOCAL_TIME(localTimeValue, &timeValue);

    // formats the time
    strftime(timeBuffer, 1024, "%Y-%m-%d %H:%M:%S", localTimeValue);

    // creates the time string
    const std::string &timeString(timeBuffer);

    // creates the new formatted value
    const std::string &newFormattedValue(timeString + " " + formattedValue);

    // sets the new formatted value in the record
    record->setFormattedValue(newFormattedValue);
}

LoggerStringFormatter::LoggerStringFormatter() : LoggerFormatter() {
}

LoggerStringFormatter::LoggerStringFormatter(const std::string &formatString) : LoggerFormatter() {
    this->setFormatString(formatString);
}

LoggerStringFormatter::~LoggerStringFormatter() {
}

void LoggerStringFormatter::setFormatString(const std::string &formatString) {
    this->formatString = formatString;
}

/**
 * Constructor of the class.
 */
LoggerRecord::LoggerRecord() {
}

LoggerRecord::LoggerRecord(const std::string &value, unsigned int level) {
    this->setValue(value);
    this->setLevel(level);
}

/**
 * Destructor of the class.
 */
LoggerRecord::~LoggerRecord() {
}

std::string &LoggerRecord::getValue() {
    return this->value;
}

void LoggerRecord::setValue(const std::string &value) {
    this->value = value;
}

unsigned int LoggerRecord::getLevel() {
    return this->level;
}

void LoggerRecord::setLevel(unsigned int level) {
    this->level = level;
}

std::string &LoggerRecord::getFormattedValue() {
    return this->formattedValue;
}

void LoggerRecord::setFormattedValue(const std::string &formattedValue) {
    this->formattedValue = formattedValue;
}
