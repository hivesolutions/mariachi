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

#include "observable.h"

using namespace mariachi::patterns;

/**
 * Constructor of the class.
 */
Observable::Observable() {
}

/**
 * Destructor of the class.
 */
Observable::~Observable() {
    //@todo destroy all the created lists in the eventHandlersMap
}

/**
 * Registers a callback function (with the given arguments) for the given event.
 *
 * @param eventName The name of the event to "listen".
 * @param callbackFunction The callback function to be called when a event is fired.
 * @param callbackArguments The arguments to be used in the callback function call.
 * @return The event registration handle, that can be used to identifie univocally
 * the event registration. It can be used to unregister for the event, etc.
 */
void Observable::registerForEvent(const std::string &eventName, void (*callbackFunction)(void *), void *callbackArguments) {
    // creates the event callback information list
    std::list<CallbackInformation_t> *eventCallbackInformationList;

    // in case the given event does not exist in the event
    // handlers map
    if((eventCallbackInformationList = this->eventHandlersMap[eventName]) == NULL) {
        // creates a new list for the callback functions of the event
        this->eventHandlersMap[eventName] = new std::list<CallbackInformation_t>();

        // sets the event callback information list
        eventCallbackInformationList = this->eventHandlersMap[eventName];
    }

    // creates the callback information
    CallbackInformation_t callbackInformation;

    // sets the callback function in the callback information
    callbackInformation.callbackFunction = callbackFunction;

    // sets the callback arguments in the callback information
    callbackInformation.callbackArguments = callbackArguments;

    // adds the callback information to the event handlers map
    eventCallbackInformationList->push_back(callbackInformation);
}

/**
 * Registers a callback function (with the given arguments) for
 * the given event (with properties).
 *
 * @param eventName The name of the event to "listen".
 * @param properties The properties of the event registration.
 * @param callbackFunction The callback function to be called when a event is fired.
 * @param callbackArguments The arguments to be used in the callback function call.
 * @return The event registration handle, that can be used to identifie univocally
 * the event registration. It can be used to unregister for the event, etc.
 */
void Observable::registerForEventProperties(const std::string &eventName, void *properties, void (*callbackFunction)(void *), void *callbackArguments) {
    // creates the event callback information list
    std::list<CallbackInformation_t> *eventCallbackInformationList;

    // in case the given event does not exist in the event
    // handlers map
    if((eventCallbackInformationList = this->eventHandlersMap[eventName]) == NULL) {
        // creates a new list for the callback functions of the event
        this->eventHandlersMap[eventName] = new std::list<CallbackInformation_t>();

        // sets the event callback information list
        eventCallbackInformationList = this->eventHandlersMap[eventName];
    }

    // creates the callback information
    CallbackInformation_t callbackInformation;

    // sets the callback function in the callback information
    callbackInformation.callbackFunction = callbackFunction;

    // sets the callback arguments in the callback information
    callbackInformation.callbackArguments = callbackArguments;

    // adds the callback information to the event handlers map
    eventCallbackInformationList->push_back(callbackInformation);
}

void Observable::unregisterForEvent(const std::string &eventName, unsigned int eventRegistrationHandle) {

}

/**
 * Fires the given event, triggering the call of all the callback functions
 * for the given event.
 *
 * @param eventName The name of the event to be fired.
 */
void Observable::fireEvent(const std::string &eventName) {
    // creates the event callback information list
    std::list<CallbackInformation_t> *eventCallbackInformationList;

    // in case the given event does not exist in the event
    // handlers map
    if((eventCallbackInformationList = this->eventHandlersMap[eventName]) == NULL) {
        return;
    }

    // retrieves the event callback information iterator
    std::list<CallbackInformation_t>::iterator eventCallbackInformationListIterator = eventCallbackInformationList->begin();

    // iterates over all the event callback information
    while(eventCallbackInformationListIterator != eventCallbackInformationList->end()) {
        // retrieves the callback information
        CallbackInformation_t callbackInformation = *eventCallbackInformationListIterator;

        // notifies the callback
        this->notifyCallback(callbackInformation);
    }
}

/**
 * Notifies the callback (about the event), calling the callback function with the defined
 * callback arguments.
 *
 * @param callbackInformation The callback information to be used in the notofication.
 */
inline void Observable::notifyCallback(CallbackInformation_t callbackInformation) {
    callbackInformation.callbackFunction(callbackInformation.callbackArguments);
}
