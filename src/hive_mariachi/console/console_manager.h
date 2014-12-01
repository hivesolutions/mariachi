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

#include "../main/engine.h"
#include "../script/script_engine.h"

/**
 * The console help text.
 */
#define CONSOLE_MANAGER_HELP_TEXT "### MARIACHI ENGINE HELP ###\n\
help [extension-id]  - shows this message or the referred console extension help message\n\
helpall              - shows the help message of all the loaded console extensions\n\
extensions           - shows the list of loaded console extensions\n\
show <plugin-id>     - shows the status of the plugin with the defined id\n\
showall              - shows the status of all the loaded plugins\n\
info <plugin-id>     - shows information about the plugin with the defined id\n\
infoall              - shows information about all the loaded plugins\n\
add <plugin-path>    - adds a new plugin\n\
remove <plugin-id>   - removes a plugin\n\
load <plugin-id>     - loads a plugin\n\
unload <plugin-id>   - unloads a plugin\n\
script <engine-name> - enters in script execution with the given engine name\n\
exit                 - exits the system"

/**
 * The invalid command message.
 */
#define CONSOLE_MANAGER_INVALID_COMMAND_MESSAGE "invalid command"

/**
 * The invalid number arguments message.
 */
#define CONSOLE_INVALID_NUMBER_ARGUMENTS_MESSAGE "invalid number of arguments"

/**
 * The carret identifier.
 */
#define CONSOLE_MANAGER_CARRET ">>"

/**
 * The commands list, mapping the
 */
#define COMMANDS_LIST { { "help", ConsoleManager::processHelp }, { "script", ConsoleManager::processScript }, { "exit", ConsoleManager::processExit }, { NULL, NULL } }

namespace mariachi {
    namespace console {
        typedef void (*WriteOuputFunction_t)(const char *text, bool newline);
        typedef void (*ConsoleProcessFunction_t)(std::vector<std::string> &commandTokens, WriteOuputFunction_t outputFunction, ConsoleManager *consoleManager);

        typedef struct CommandProcessInformation_t {
            const char *name;
            ConsoleProcessFunction_t processFunction;
        } CommandProcessInformation;

        class ConsoleManager {
            private:
                Engine *engine;
                std::map<std::string, CommandProcessInformation_t> processInformationMap;
                script::ScriptEngine *currentScriptEngine;
                std::string currentScriptEngineName;
                std::string currentScriptString;
                bool extraCarret;

                inline void initProcessInformationMap();
                inline void initCurrentScriptEngine();
                inline void initExtraCarret();
                inline void initEngine(Engine *engine);

            public:
                ConsoleManager();
                ConsoleManager(Engine *engine);
                ~ConsoleManager();
                void load(void *arguments);
                void unload(void *arguments);
                void processCommandLine(const char *commandLine, WriteOuputFunction_t outputFunction = NULL);
                void processCommandLineConsole(const char *commandLine, WriteOuputFunction_t outputFunction);
                void processCommandLineScript(const char *commandLine, WriteOuputFunction_t outputFunction);
                std::string getCarretValue();
                Engine *getEngine();
                void setEngine(Engine *engine);
                static void write(const char *text, bool newline = true);
                static void processHelp(std::vector<std::string> &commandTokens, WriteOuputFunction_t outputFunction, ConsoleManager *consoleManager);
                static void processScript(std::vector<std::string> &commandTokens, WriteOuputFunction_t outputFunction, ConsoleManager *consoleManager);
                static void processExit(std::vector<std::string> &commandTokens, WriteOuputFunction_t outputFunction, ConsoleManager *consoleManager);
        };

        static const CommandProcessInformation_t processInformationList[] = COMMANDS_LIST;
    }
}
