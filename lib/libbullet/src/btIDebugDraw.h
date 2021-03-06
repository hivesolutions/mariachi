/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2009 Erwin Coumans  http://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/


#ifndef IDEBUG_DRAW__H
#define IDEBUG_DRAW__H

#include "btVector3.h"
#include "btTransform.h"


///The btIDebugDraw interface class allows hooking up a debug renderer to visually debug simulations.
///Typical use case: create a debug drawer object, and assign it to a btCollisionWorld or btDynamicsWorld using setDebugDrawer and call debugDrawWorld.
///A class that implements the btIDebugDraw interface has to implement the drawLine method at a minimum.
class    btIDebugDraw
{
    public:

    enum    DebugDrawModes
    {
        DBG_NoDebug=0,
        DBG_DrawWireframe = 1,
        DBG_DrawAabb=2,
        DBG_DrawFeaturesText=4,
        DBG_DrawContactPoints=8,
        DBG_NoDeactivation=16,
        DBG_NoHelpText = 32,
        DBG_DrawText=64,
        DBG_ProfileTimings = 128,
        DBG_EnableSatComparison = 256,
        DBG_DisableBulletLCP = 512,
        DBG_EnableCCD = 1024,
        DBG_DrawConstraints = (1 << 11),
        DBG_DrawConstraintLimits = (1 << 12),
        DBG_FastWireframe = (1<<13),
        DBG_MAX_DEBUG_DRAW_MODE
    };

    virtual ~btIDebugDraw() {};

    virtual void    drawLine(const btVector3& from,const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
    {
        drawLine (from, to, fromColor);
    }

    virtual void    drawBox (const btVector3& boxMin, const btVector3& boxMax, const btVector3& color, btScalar alpha)
    {
    }

    virtual void    drawSphere (const btVector3& p, btScalar radius, const btVector3& color)
    {
    }

    virtual void    drawLine(const btVector3& from,const btVector3& to,const btVector3& color)=0;

    virtual    void    drawTriangle(const btVector3& v0,const btVector3& v1,const btVector3& v2,const btVector3& /*n0*/,const btVector3& /*n1*/,const btVector3& /*n2*/,const btVector3& color, btScalar alpha)
    {
        drawTriangle(v0,v1,v2,color,alpha);
    }
    virtual    void    drawTriangle(const btVector3& v0,const btVector3& v1,const btVector3& v2,const btVector3& color, btScalar /*alpha*/)
    {
        drawLine(v0,v1,color);
        drawLine(v1,v2,color);
        drawLine(v2,v0,color);
    }

    virtual void    drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)=0;

    virtual void    reportErrorWarning(const char* warningString) = 0;

    virtual void    draw3dText(const btVector3& location,const char* textString) = 0;

    virtual void    setDebugMode(int debugMode) =0;

    virtual int        getDebugMode() const = 0;

    inline void drawAabb(const btVector3& from,const btVector3& to,const btVector3& color)
    {

        btVector3 halfExtents = (to-from)* 0.5f;
        btVector3 center = (to+from) *0.5f;
        int i,j;

        btVector3 edgecoord(1.f,1.f,1.f),pa,pb;
        for (i=0;i<4;i++)
        {
            for (j=0;j<3;j++)
            {
                pa = btVector3(edgecoord[0]*halfExtents[0], edgecoord[1]*halfExtents[1],
                    edgecoord[2]*halfExtents[2]);
                pa+=center;

                int othercoord = j%3;
                edgecoord[othercoord]*=-1.f;
                pb = btVector3(edgecoord[0]*halfExtents[0], edgecoord[1]*halfExtents[1],
                    edgecoord[2]*halfExtents[2]);
                pb+=center;

                drawLine(pa,pb,color);
            }
            edgecoord = btVector3(-1.f,-1.f,-1.f);
            if (i<3)
                edgecoord[i]*=-1.f;
        }
    }
    void drawTransform(const btTransform& transform, btScalar orthoLen)
    {
        btVector3 start = transform.getOrigin();
        drawLine(start, start+transform.getBasis() * btVector3(orthoLen, 0, 0), btVector3(0.7f,0,0));
        drawLine(start, start+transform.getBasis() * btVector3(0, orthoLen, 0), btVector3(0,0.7f,0));
        drawLine(start, start+transform.getBasis() * btVector3(0, 0, orthoLen), btVector3(0,0,0.7f));
    }

    void drawArc(const btVector3& center, const btVector3& normal, const btVector3& axis, btScalar radiusA, btScalar radiusB, btScalar minAngle, btScalar maxAngle,
                const btVector3& color, bool drawSect, btScalar stepDegrees = btScalar(10.f))
    {
        const btVector3& vx = axis;
        btVector3 vy = normal.cross(axis);
        btScalar step = stepDegrees * SIMD_RADS_PER_DEG;
        int nSteps = (int)((maxAngle - minAngle) / step);
        if(!nSteps) nSteps = 1;
        btVector3 prev = center + radiusA * vx * btCos(minAngle) + radiusB * vy * btSin(minAngle);
        if(drawSect)
        {
            drawLine(center, prev, color);
        }
        for(int i = 1; i <= nSteps; i++)
        {
            btScalar angle = minAngle + (maxAngle - minAngle) * btScalar(i) / btScalar(nSteps);
            btVector3 next = center + radiusA * vx * btCos(angle) + radiusB * vy * btSin(angle);
            drawLine(prev, next, color);
            prev = next;
        }
        if(drawSect)
        {
            drawLine(center, prev, color);
        }
    }
    void drawSpherePatch(const btVector3& center, const btVector3& up, const btVector3& axis, btScalar radius,
        btScalar minTh, btScalar maxTh, btScalar minPs, btScalar maxPs, const btVector3& color, btScalar stepDegrees = btScalar(10.f))
    {
        btVector3 vA[74];
        btVector3 vB[74];
        btVector3 *pvA = vA, *pvB = vB, *pT;
        btVector3 npole = center + up * radius;
        btVector3 spole = center - up * radius;
        btVector3 arcStart;
        btScalar step = stepDegrees * SIMD_RADS_PER_DEG;
        const btVector3& kv = up;
        const btVector3& iv = axis;
        btVector3 jv = kv.cross(iv);
        bool drawN = false;
        bool drawS = false;
        if(minTh <= -SIMD_HALF_PI)
        {
            minTh = -SIMD_HALF_PI + step;
            drawN = true;
        }
        if(maxTh >= SIMD_HALF_PI)
        {
            maxTh = SIMD_HALF_PI - step;
            drawS = true;
        }
        if(minTh > maxTh)
        {
            minTh = -SIMD_HALF_PI + step;
            maxTh =  SIMD_HALF_PI - step;
            drawN = drawS = true;
        }
        int n_hor = (int)((maxTh - minTh) / step) + 1;
        if(n_hor < 2) n_hor = 2;
        btScalar step_h = (maxTh - minTh) / btScalar(n_hor - 1);
        bool isClosed = false;
        if(minPs > maxPs)
        {
            minPs = -SIMD_PI + step;
            maxPs =  SIMD_PI;
            isClosed = true;
        }
        else if((maxPs - minPs) >= SIMD_PI * btScalar(2.f))
        {
            isClosed = true;
        }
        else
        {
            isClosed = false;
        }
        int n_vert = (int)((maxPs - minPs) / step) + 1;
        if(n_vert < 2) n_vert = 2;
        btScalar step_v = (maxPs - minPs) / btScalar(n_vert - 1);
        for(int i = 0; i < n_hor; i++)
        {
            btScalar th = minTh + btScalar(i) * step_h;
            btScalar sth = radius * btSin(th);
            btScalar cth = radius * btCos(th);
            for(int j = 0; j < n_vert; j++)
            {
                btScalar psi = minPs + btScalar(j) * step_v;
                btScalar sps = btSin(psi);
                btScalar cps = btCos(psi);
                pvB[j] = center + cth * cps * iv + cth * sps * jv + sth * kv;
                if(i)
                {
                    drawLine(pvA[j], pvB[j], color);
                }
                else if(drawS)
                {
                    drawLine(spole, pvB[j], color);
                }
                if(j)
                {
                    drawLine(pvB[j-1], pvB[j], color);
                }
                else
                {
                    arcStart = pvB[j];
                }
                if((i == (n_hor - 1)) && drawN)
                {
                    drawLine(npole, pvB[j], color);
                }
                if(isClosed)
                {
                    if(j == (n_vert-1))
                    {
                        drawLine(arcStart, pvB[j], color);
                    }
                }
                else
                {
                    if(((!i) || (i == (n_hor-1))) && ((!j) || (j == (n_vert-1))))
                    {
                        drawLine(center, pvB[j], color);
                    }
                }
            }
            pT = pvA; pvA = pvB; pvB = pT;
        }
    }

    void drawBox(const btVector3& bbMin, const btVector3& bbMax, const btVector3& color)
    {
        drawLine(btVector3(bbMin[0], bbMin[1], bbMin[2]), btVector3(bbMax[0], bbMin[1], bbMin[2]), color);
        drawLine(btVector3(bbMax[0], bbMin[1], bbMin[2]), btVector3(bbMax[0], bbMax[1], bbMin[2]), color);
        drawLine(btVector3(bbMax[0], bbMax[1], bbMin[2]), btVector3(bbMin[0], bbMax[1], bbMin[2]), color);
        drawLine(btVector3(bbMin[0], bbMax[1], bbMin[2]), btVector3(bbMin[0], bbMin[1], bbMin[2]), color);
        drawLine(btVector3(bbMin[0], bbMin[1], bbMin[2]), btVector3(bbMin[0], bbMin[1], bbMax[2]), color);
        drawLine(btVector3(bbMax[0], bbMin[1], bbMin[2]), btVector3(bbMax[0], bbMin[1], bbMax[2]), color);
        drawLine(btVector3(bbMax[0], bbMax[1], bbMin[2]), btVector3(bbMax[0], bbMax[1], bbMax[2]), color);
        drawLine(btVector3(bbMin[0], bbMax[1], bbMin[2]), btVector3(bbMin[0], bbMax[1], bbMax[2]), color);
        drawLine(btVector3(bbMin[0], bbMin[1], bbMax[2]), btVector3(bbMax[0], bbMin[1], bbMax[2]), color);
        drawLine(btVector3(bbMax[0], bbMin[1], bbMax[2]), btVector3(bbMax[0], bbMax[1], bbMax[2]), color);
        drawLine(btVector3(bbMax[0], bbMax[1], bbMax[2]), btVector3(bbMin[0], bbMax[1], bbMax[2]), color);
        drawLine(btVector3(bbMin[0], bbMax[1], bbMax[2]), btVector3(bbMin[0], bbMin[1], bbMax[2]), color);
    }
    void drawBox(const btVector3& bbMin, const btVector3& bbMax, const btTransform& trans, const btVector3& color)
    {
        drawLine(trans * btVector3(bbMin[0], bbMin[1], bbMin[2]), trans * btVector3(bbMax[0], bbMin[1], bbMin[2]), color);
        drawLine(trans * btVector3(bbMax[0], bbMin[1], bbMin[2]), trans * btVector3(bbMax[0], bbMax[1], bbMin[2]), color);
        drawLine(trans * btVector3(bbMax[0], bbMax[1], bbMin[2]), trans * btVector3(bbMin[0], bbMax[1], bbMin[2]), color);
        drawLine(trans * btVector3(bbMin[0], bbMax[1], bbMin[2]), trans * btVector3(bbMin[0], bbMin[1], bbMin[2]), color);
        drawLine(trans * btVector3(bbMin[0], bbMin[1], bbMin[2]), trans * btVector3(bbMin[0], bbMin[1], bbMax[2]), color);
        drawLine(trans * btVector3(bbMax[0], bbMin[1], bbMin[2]), trans * btVector3(bbMax[0], bbMin[1], bbMax[2]), color);
        drawLine(trans * btVector3(bbMax[0], bbMax[1], bbMin[2]), trans * btVector3(bbMax[0], bbMax[1], bbMax[2]), color);
        drawLine(trans * btVector3(bbMin[0], bbMax[1], bbMin[2]), trans * btVector3(bbMin[0], bbMax[1], bbMax[2]), color);
        drawLine(trans * btVector3(bbMin[0], bbMin[1], bbMax[2]), trans * btVector3(bbMax[0], bbMin[1], bbMax[2]), color);
        drawLine(trans * btVector3(bbMax[0], bbMin[1], bbMax[2]), trans * btVector3(bbMax[0], bbMax[1], bbMax[2]), color);
        drawLine(trans * btVector3(bbMax[0], bbMax[1], bbMax[2]), trans * btVector3(bbMin[0], bbMax[1], bbMax[2]), color);
        drawLine(trans * btVector3(bbMin[0], bbMax[1], bbMax[2]), trans * btVector3(bbMin[0], bbMin[1], bbMax[2]), color);
    }
};


#endif //IDEBUG_DRAW__H
