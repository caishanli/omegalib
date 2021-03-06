/******************************************************************************
 * THE OMEGA LIB PROJECT
 *-----------------------------------------------------------------------------
 * Copyright 2010-2015		Electronic Visualization Laboratory, 
 *							University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-----------------------------------------------------------------------------
 * Copyright (c) 2010-2015, Electronic Visualization Laboratory,  
 * University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this 
 * list of conditions and the following disclaimer. Redistributions in binary 
 * form must reproduce the above copyright notice, this list of conditions and 
 * the following disclaimer in the documentation and/or other materials provided 
 * with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *-----------------------------------------------------------------------------
 * What's in this file
 *	The abstract base class used by display system implementations.
 ******************************************************************************/
#ifndef __DISPLAY_SYSTEM_H__
#define __DISPLAY_SYSTEM_H__

#include "osystem.h"
#include "ApplicationBase.h"
#include "DisplayConfig.h"
#include "Color.h"

namespace omega
{

///////////////////////////////////////////////////////////////////////////////
// Forward declarations
class SystemManager;

///////////////////////////////////////////////////////////////////////////////
class OMEGA_API DisplaySystem: public ReferenceType
{
public:
    enum DisplaySystemType { Invalid, Equalizer, Glut, Null };

public:
    virtual ~DisplaySystem() {}

    // sets up the display system. Called before initalize.
    virtual void setup(Setting& setting) 
    {
        DisplayConfig::LoadConfig(setting, myDisplayConfig);
    }

    // initializes the display system
    virtual void initialize(SystemManager* sys) {}

    //! Starts display system rendering. This call does not return until the 
    //! current omegalib application sends an exit request to the system manager.
    virtual void run() = 0;

    virtual void cleanup() {}

    virtual DisplaySystemType getId() { return Invalid; }

    virtual void killCluster() {}

    //! @deprecated (use DisplayUtils) Returns a view ray given a pointer 
    //! position in pixel coordinates
    Ray getViewRay(Vector2i position) 
    {
        return myDisplayConfig.getViewRay(position);
    }

    //! @deprecated (use DIsplayUtils) Computes a view ray from a pointer or 
    //! wand event. Returns true if the ray has been generated succesfully, 
    //! false otherwise (i.e. because the event is not a wand or pointer event)
    bool getViewRayFromEvent(const Event& evt, Ray& ray, bool normalizedPointerCoords = false)
    {
        return myDisplayConfig.getViewRayFromEvent(evt, ray, normalizedPointerCoords);
    }

    virtual DisplayConfig& getDisplayConfig() { return myDisplayConfig; }

    //! @internal 
    //! Called internally by derived classes to signal the beginning of a new
    //! frame.
    void frameStarted() 
    {
        // Do nothing for now, but this space may be useful in the future.
    }

    //! internal 
    //! Called internally by derived classes to signal the end of a frame.
    void frameFinished()
    {
        // Reset bring to front flag
        myDisplayConfig._bringToFrontRequested = false;
    }

protected:

    DisplaySystem()
    {
        // Increase the display config reference count: this is done because 
        // DisplayConfig may be accessed by reference (for instance through the
        // getDIsplayConfig python API call), and releasing that reference would
        // trigger an unwanted deallocation of this object.
        myDisplayConfig.ref();
    }

    DisplayConfig myDisplayConfig;
};

}; // namespace omega

#endif