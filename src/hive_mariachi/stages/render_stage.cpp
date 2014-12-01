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

#include "../render_adapters/render_adapters.h"
#include "definitions/render_stage_definitions.h"

#include "render_stage.h"

using namespace mariachi;
using namespace mariachi::render;
using namespace mariachi::stages;
using namespace mariachi::render_adapters;

THREAD_RETURN mariachi::stages::renderRunnerThread(THREAD_ARGUMENTS parameters) {
    // retrieves the render adapter
    RenderAdapter *renderAdapter = ((RenderAdapter **) parameters)[3];

    // starts the render adapter
    renderAdapter->start(parameters);

    // returns valid value
    return THREAD_VALID_RETURN_VALUE;
}

/**
 * Constructor of the class.
 */
RenderStage::RenderStage() : Stage() {
    this->initThread();
    this->initRenderInformation();
}

/**
 * Constructor of the class.
 *
 * @param engine The currently used engine.
 */
RenderStage::RenderStage(Engine *engine) : Stage(engine) {
    this->initThread();
    this->initRenderInformation();
}

/**
 * Constructor of the class.
 *
 * @param engine The currently used engine.
 * @param name The name of the stage.
 */
RenderStage::RenderStage(Engine *engine, const std::string &name) : Stage(engine, name) {
    this->initThread();
    this->initRenderInformation();
}

/**
 * Constructor of the class.
 *
 * @param engine The currently used engine.
 * @param name The name of the stage.
 * @param renderSystem The render system to be used.
 */
RenderStage::RenderStage(Engine *engine, const std::string &name, RenderSystem renderSystem) : Stage(engine, name) {
    this->initThread();
    this->initRenderInformation();
    this->renderSystem = renderSystem;
}

/**
 * Destructor of the class.
 */
RenderStage::~RenderStage() {
    delete this->renderInformation;
}

inline void RenderStage::initThread() {
    this->thread = true;
}

inline void RenderStage::initRenderInformation() {
    this->renderInformation = new RenderInformation();
}

void RenderStage::start(void *arguments) {
    Stage::start(arguments);

    // TENHO DE ADICIONAR TODOS OS ADAPTERS DISPONIVEIS
    // E DEPOIS FAZER SET SO DO MEU PRINCIAPL (por defined)

    // creates the render adapter
    this->renderAdapter = new DEFAULT_RENDER_ADAPTER_CLASS();

    // allocates space for the render arguments
    char *renderArguments = (char *) malloc(sizeof(int) + sizeof(char **) + sizeof(RenderInformation *) + sizeof(RenderAdapter *) + sizeof(Engine *));

    // sets the argc in the render arguments
    ((int *) renderArguments)[0] = this->engine->getArgc();

    // sets the argv in the render arguments
    ((char ***) renderArguments)[1] = this->engine->getArgv();

    // sets the render information in the render arguments
    ((RenderInformation **) renderArguments)[2] = this->renderInformation;

    // sets the render adapter in the render arguments
    ((RenderAdapter **) renderArguments)[3] = this->renderAdapter;

    // sets the engine in the render arguments
    ((Engine **) renderArguments)[4] = this->engine;

    // creates the engine runnner thread
    this->renderAdapterThreadHandle = THREAD_CREATE_BASE(this->renderAdapterThreadIdentifier, renderRunnerThread, renderArguments);
}

void RenderStage::stop(void *arguments) {
    Stage::stop(arguments);

    // stops the render adapter
    this->renderAdapter->stop(NULL);

    // joins the render adapter thread
    THREAD_JOIN_BASE(this->renderAdapterThreadHandle, this->renderAdapterThreadIdentifier);

    // closes the render adapter thread handle
    THREAD_CLOSE(this->renderAdapterThreadHandle);
}

void RenderStage::update(void *arguments) {
    Stage::update(arguments);

    // waits for the render information mutex
    MUTEX_LOCK(this->renderInformation->getMutex());

    // updates the render information
    this->updateRenderInformation(this->renderInformation);

    // releases the render information mutex
    MUTEX_UNLOCK(this->renderInformation->getMutex());
}

void RenderStage::updateRenderInformation(RenderInformation *renderInformation) {
    renderInformation->setRender(this->engine->getRender());
    renderInformation->setRender2d(this->engine->getRender2d());
    renderInformation->setActiveCamera(this->engine->getActiveCamera());
}
