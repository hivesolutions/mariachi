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

#ifdef MARIACHI_PLATFORM_DIRECT3D

#include "../system/system.h"
#include "../render_utils/direct3d_win32_window.h"
#include "direct3d9_adapter.h"

#define CUSTOMFVF ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE )

struct CUSTOMVERTEX { FLOAT X, Y, Z, RHW; DWORD COLOR; };

using namespace mariachi::render;
using namespace mariachi::render_utils;
using namespace mariachi::render_adapters;

Direct3d9Adapter::Direct3d9Adapter() : Direct3dAdapter() {
    this->triangleVertexBuffer = NULL;
}

Direct3d9Adapter::~Direct3d9Adapter() {
}

void Direct3d9Adapter::start(void *arguments) {
    int argc = ((int *) arguments)[0];
    char **argv = ((char ***) arguments)[1];
    RenderInformation *renderInformation = ((RenderInformation **) arguments)[2];
    RenderAdapter *renderAdapter = ((RenderAdapter **) arguments)[3];

    // creates the arguments map
    std::map<std::string, void *> argumentsMap;

    // sets the arguments in the arguments map
    argumentsMap["argc"] = (void *) argc;
    argumentsMap["argv"] = (void *) argv;
    argumentsMap["render_information"] = (void *) renderInformation;
    argumentsMap["adapter"] = (void *) renderAdapter;

    // resets the frame count
    this->frameCount = 0;

    // retrieves the current clock and sets it as
    // the base clock
    this->baseClock = this->clockSeconds();

    // sets the render information
    this->renderInformation = renderInformation;

    // creates the direct3d window
    this->window = new Direct3dWin32Window();

    // starts the direct3d window
    this->window->start(&argumentsMap);

    // starts the loop in the opengl window
    this->window->loop(&argumentsMap);

    // cleans the open gl system
    this->clean();
}

void Direct3d9Adapter::stop(void *arguments) {
}

void Direct3d9Adapter::init() {
    // retrieves the handler window
    HWND handlerWindow = this->window->getHandlerWindow();

    // creates the direct3d interface
    this->direct3d = Direct3DCreate9(D3D_SDK_VERSION);

    // creates the struct to hold various device information
    D3DPRESENT_PARAMETERS direct3DevicePresentParameters;

    // resets the structure for use
    ZeroMemory(&direct3DevicePresentParameters, sizeof(direct3DevicePresentParameters));

    direct3DevicePresentParameters.Windowed = true;
    direct3DevicePresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    direct3DevicePresentParameters.hDeviceWindow = handlerWindow;

    // creates the direct3d device
    this->direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, handlerWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &direct3DevicePresentParameters, &this->direct3dDevice);

    // !!! CARREGA O MATERIAL PARA O VERTEX BUFFER !!!!

    // create the vertices using the CUSTOMVERTEX struct
    CUSTOMVERTEX vertices[] = { { 40.0f, 62.5f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255), },
                                { 65.0f, 500.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 255, 0), },
                                { 15.0f, 500.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 0, 0), }, };

    // creates a vertex buffer interface and stores it in the triangle vertex buffers
    this->direct3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 0, CUSTOMFVF, D3DPOOL_MANAGED, &this->triangleVertexBuffer, NULL);

    // allocates a void pointer
    void *pointerVoid;

    // locks the vertex buffer to load the vertices into it
    this->triangleVertexBuffer->Lock(0, 0, (void **) &pointerVoid, 0);

    // loads the the vertices into the vertex buffer
    memcpy(pointerVoid, vertices, sizeof(vertices));

    // unlocks the vertex buffer
    this->triangleVertexBuffer->Unlock();
}

void Direct3d9Adapter::clean() {
    // closes and releases the vertex buffer
    this->triangleVertexBuffer->Release();

    // closes and releases the 3d device
    this->direct3dDevice->Release();

    // closes and releases direct3d
    this->direct3d->Release();
}

void Direct3d9Adapter::display() {
    // updates the frame rate
    this->updateFrameRate();

    // clears the window to a deep blue
    this->direct3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    // begins the scene
    this->direct3dDevice->BeginScene();

    // selects which vertex format to use
    this->direct3dDevice->SetFVF(CUSTOMFVF);

    // selects the vertex buffer to display
    this->direct3dDevice->SetStreamSource(0, this->triangleVertexBuffer, 0, sizeof(CUSTOMVERTEX));

    // copy the vertex buffer to the back buffer
    this->direct3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

    // ends the scene
    this->direct3dDevice->EndScene();

    // displays the created frame on the screen
    this->direct3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Direct3d9Adapter::resizeScene(int windowWidth, int windowHeight) {
}

void Direct3d9Adapter::keyPressed(unsigned char key, int x, int y) {
}

inline time_t Direct3d9Adapter::clockSeconds() {
    // allocates space for the current clock
    time_t currentClock;

    // sets the current clock
    time(&currentClock);

    // returns the current clock
    return currentClock;
}

inline void Direct3d9Adapter::updateFrameRate() {
    // increments the frame count
    this->frameCount++;

    // retrieves the current clock
    time_t currentClock = this->clockSeconds();

    // calculates the delta clock
    float deltaClock = (float) difftime(currentClock, this->baseClock);

    // checks if the delta clock is greater than the frame
    // sampling limit
    if(deltaClock > FRAME_SAMPLING_LIMIT) {
        // calculates the frame rate
        float frameRate = (float) frameCount / deltaClock;

        // prints the frame rate
        //printf("Frame rate: %.2f\n", frameRate);

        // resets the frame count
        this->frameCount = 0;

        // sets the base clock as the current clock
        this->baseClock = currentClock;
    }
}

#endif
