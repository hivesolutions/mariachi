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

#include "../exceptions/exception.h"
#include "../system/thread.h"
#include "../devices/device.h"
#include "../logging/logger.h"
#include "../nodes/nodes.h"
#include "../user_interface/user_interface.h"
#include "../tasks/task.h"
#include "../debugging/debugging.h"
#include "../structures/fifo.h"

#ifdef MARIACHI_PLATFORM_WIN32
#define threadHandleStageMapPut(threadHandle, threadId, currentStage) this->threadHandleStageMap[threadHandle] = currentStage
#elif MARIACHI_PLATFORM_UNIX
#define threadHandleStageMapPut(threadHandle, threadId, currentStage) this->threadHandleStageMap[threadId] = currentStage
#endif

/**
 * The branding text value.
 */
#define MARIACHI_BRANDING_TEXT "Hive Mariachi Engine"

/**
 * The version value.
 */
#define MARIACHI_VERSION "1.0.0"

/**
 * The date value.
 */
#define MARIACHI_DATE __DATE__

/**
 * The time value.
 */
#define MARIACHI_TIME __TIME__

/**
 * The realse information value.
 */
#define MARIACHI_RELEASE_INFO "Hive Solutions Lda. r1: "

/**
 * The help text value.
 */
#define MARIACHI_HELP_TEXT "Type \"help\" for more information."

namespace mariachi {
    namespace camera {
        class CameraManager;
    }

    namespace configuration {
        class ConfigurationManager;
    }

    namespace console {
        class ConsoleManager;
    }

    namespace physics {
        class PhysicsEngine;
    }

    namespace script {
        class ScriptEngine;
    }

    namespace stages {
        class StageRunner;
        class Stage;
    }

    class Engine {
        private:
            /**
             * The number of arguments sent to the program.
             */
            int argc;

            /**
             * The value of the arguments sent to the program.
             */
            char **argv;

            /**
             * The flag that controls the running of the engine.
             */
            bool runningFlag;

            /**
             * The list of paths to be used in file localization.
             */
            std::list<std::string> pathsList;

            /**
             * The logger used in the message logging.
             */
            logging::Logger *logger;

            /**
             * The manager used to provide configuration options.
             */
            configuration::ConfigurationManager *configurationManager;

            /**
             * The manager used to control the camera.
             */
            camera::CameraManager *cameraManager;

            /**
             * The manager used to control the console.
             */
            console::ConsoleManager *consoleManager;

            /**
             * The top level 3d scene render node.
             */
            nodes::SceneNode *render;

            /**
             * The top level 2d scene render node.
             */
            nodes::Scene2dNode *render2d;

            /**
             * The engine used for debuging provisioning.
             */
            debugging::DebugEngine *debugEngine;

            /**
             * The mutex that controls the task list access.
             */
            MUTEX_HANDLE taskListMutex;

            /**
             * The task list ready event.
             */
            EVENT_HANDLE taskListReadyEvent;

            /**
             * The map associating the stage with the
             * stage runner.
             */
            std::map<stages::Stage *, stages::StageRunner *> stageRunnersMap;

            /**
             * The list containing all the stages to be used.
             */
            std::list<stages::Stage *> stagesList;

            /**
             * The map associating the stage name with the
             * stage reference.
             */
            std::map<std::string, stages::Stage *> stagesMap;

            /**
             * The map associating the device name with the
             * device reference.
             */
            std::map<std::string, devices::Device *> devicesMap;

            /**
             * The map associating the script engine name with the
             * script engine reference.
             */
            std::map<std::string, script::ScriptEngine *> scriptEnginesMap;

            /**
             * The map associating the physics engine name with the
             * physics engine reference.
             */
            std::map<std::string, physics::PhysicsEngine *> physicsEnginesMap;

            /**
             * The list of currently available tasks.
             */
            std::list<tasks::Task *> taskList;

            /**
             * The list of stages to be run in the main thread.
             */
            std::list<stages::Stage *> mainThreadStagesList;

            /**
             * The map associating the thread handle with the associated stage.
             */
            std::map<THREAD_REFERENCE, stages::Stage *> threadHandleStageMap;

            /**
             * The currently active physics engine, to be used in physics operations.
             */
            physics::PhysicsEngine *activePhysicsEngine;

            inline void initRunningFlag();
            inline void initLogger();
            inline void initRenders();
            inline void initArgs(int argc, char** argv);

        public:
            structures::Fifo<bool> *fifo;
            void startFifos();

            Engine();
            Engine(int argc, char** argv);
            ~Engine();
            void start(void *arguments);
            void stop(void *arguments);
            void update();
            void printInformation();
            void handleException(exceptions::Exception *exception);
            void startPathsList();
            void startConfigurationManager();
            void stopConfigurationManager();
            void startCameraManager();
            void stopCameraManager();
            void startConsoleManager();
            void stopConsoleManager();
            void startLogger(int level, bool pidFile);
            void startInputDevices();
            void startScriptEngines();
            void startPhysicsEngines();
            void startStages();
            void stopStages();
            void startDebugEngine();
            void stopDebugEngine();
            void startRunLoop();
            void updatePathsListEnvironment();
            void updatePathsListConfiguration();
            void addPath(const std::string &path);
            void removePath(const std::string &path);
            void addPaths(std::vector<std::string *> &pathsList);
            void removePaths(std::vector<std::string *> &pathsList);
            std::string getAbsolutePath(const std::string &relativePath);
            void addStage(stages::Stage *stage);
            void removeStage(stages::Stage *stage);
            void addMainThreadStage(stages::Stage *stage);
            void removeMainThreadStage(stages::Stage *stage);
            void addTask(tasks::Task *task);
            void removeTask(tasks::Task *task);
            void startCameraTransition(const std::string &cameraName, int duration);
            void cancelCameraTransition();
            void getCurrentProcessIdString(std::string &currentProcessIdString);
            stages::StageRunner *getStageRunner(stages::Stage *stage);
            void setStageRunner(stages::Stage *stage, stages::StageRunner *stageRunner);
            stages::Stage *getStage(const std::string &stageName);
            void setStage(const std::string &stageName, stages::Stage *stage);
            devices::Device *getDevice(const std::string &deviceName);
            void setDevice(const std::string &deviceName, devices::Device *device);
            script::ScriptEngine *getScriptEngine(const std::string &scriptEngineName);
            void setScriptEngine(const std::string &scriptEngineName, script::ScriptEngine *scriptEngine);
            physics::PhysicsEngine *getPhysicsEngine(const std::string &physicsEngineName);
            void setPhysicsEngine(const std::string &physicsEngineName, physics::PhysicsEngine *physicsEngine);
            nodes::CameraNode *getCamera(const std::string &cameraName);
            void setCamera(const std::string &cameraName, nodes::CameraNode *camera);
            configuration::ConfigurationManager *getConfigurationManager();
            void setConfigurationManager(configuration::ConfigurationManager *configurationManager);
            camera::CameraManager *getCameraManager();
            void setCameraManager(camera::CameraManager *cameraManager);
            console::ConsoleManager *getConsoleManager();
            void setConsoleManager(console::ConsoleManager *consoleManager);
            std::list<std::string> *getPathsList();
            void setPathsList(std::list<std::string> *pathsList);
            logging::Logger *getLogger();
            void setLogger(logging::Logger *logger);
            nodes::SceneNode *getRender();
            void setRender(nodes::SceneNode *render);
            nodes::Scene2dNode *getRender2d();
            void setRender2d(nodes::Scene2dNode *render2d);
            nodes::CameraNode *getActiveCamera();
            void setActiveCamera(nodes::CameraNode *activeCamera);
            physics::PhysicsEngine *getActivePhysicsEngine();
            void setActivePhysicsEngine(physics::PhysicsEngine *activePhysicsEngine);
            int getArgc();
            char **getArgv();
    };

    THREAD_RETURN mainRunnerThread(THREAD_ARGUMENTS parameters);
    THREAD_RETURN stageRunnerThread(THREAD_ARGUMENTS parameters);
}
