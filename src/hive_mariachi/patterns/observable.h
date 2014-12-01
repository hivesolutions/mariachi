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

namespace mariachi {
    namespace patterns {
        typedef struct CallbackInformation_t {
            void (*callbackFunction)(void *);
            void *callbackArguments;
        } CallbackInformation;

        /**
         * Abstract class that implements the observable pattern.
         * It is used in event driven programming, where a client (user), registers
         * for a given event and is notified about any changes.
         */
        class Observable {
            private:
                /**
                 * The event registration handle event handler map, that associates
                 * a given event registration handle (a single number that identifies a registration)
                 * with the pointer to the callback information.
                 */
                std::map<unsigned int, CallbackInformation_t> eventRegistrationHandleEventHandlersMap;

                /**
                 * The event handlers map that associates the event name with
                 * a list containing pointers to the callback informations for the given
                 * event name.
                 */
                std::map<std::string, std::list<CallbackInformation_t> *> eventHandlersMap;

            public:
                Observable();
                ~Observable();
                void registerForEvent(const std::string &eventName, void (*callbackFunction)(void *), void *callbackArguments);
                void registerForEventProperties(const std::string &eventName, void *properties, void (*callbackFunction)(void *), void *callbackArguments);
                void unregisterForEvent(const std::string &eventName, unsigned int eventRegistrationHandle);
                void fireEvent(const std::string &eventName);
                inline void notifyCallback(CallbackInformation_t callbackInformation);
        };
    }
}
