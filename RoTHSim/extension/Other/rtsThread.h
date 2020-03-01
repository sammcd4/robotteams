//===========================================================================
/*
    This file is part of the CHAI 3D visualization and haptics libraries.
    Copyright (C) 2003-2009 by CHAI 3D. All rights reserved.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License("GPL") version 2
    as published by the Free Software Foundation.

    For using the CHAI 3D libraries with software that can not be combined
    with the GNU GPL, and for taking advantage of the additional benefits
    of our support services, please contact CHAI 3D about acquiring a
    Professional Edition License.

    \author    <http://www.chai3d.org>
    \author    Francois Conti
    \version   2.0.0 $Rev: 251 $
*/
//===========================================================================

//---------------------------------------------------------------------------
#ifndef RTSThreadH
#define RTSThreadH
//---------------------------------------------------------------------------
#include "system/CGlobals.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
    \file       rtsThread.h

    \brief
    <b> Timers </b> \n 
    Threads.
*/
//===========================================================================

//---------------------------------------------------------------------------
/*!
    Defines thread priority
*/
//---------------------------------------------------------------------------
enum rtsThreadPriority
{
	RT_THREAD_PRIORITY_LOWEST = -2,
	RT_THREAD_PRIORITY_BELOW_NORMAL = -1,
    RT_THREAD_PRIORITY_NORMAL = 0,
    RT_THREAD_PRIORITY_ABOVE_NORMAL = 1,
	RT_THREAD_PRIORITY_HIGHEST = 2,
};


//===========================================================================
/*!
    \class	    rtsThread
    \ingroup    timers  

    \brief	
    rtsThread provides a class to manage threads.
*/
//===========================================================================
class rtsThread
{
  public:

    //-----------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR:
    //-----------------------------------------------------------------------

    //! Constructor of rtsThread.
    rtsThread();

    //! Destructor of rtsThread.
    ~rtsThread();


    //-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------

    //! Set the thread parameters.
    void set(void (*a_function)(void), rtsThreadPriority a_level);

    //! Set the thread priority level.
    void setPriority(rtsThreadPriority a_level);

    //! Get the current thread priority level.
    rtsThreadPriority getPriority() { return (m_priorityLevel); }

  protected:

#if defined(_WIN32)
    //! Thread handle
    DWORD m_threadId;
#endif

    //! Pointer to thread function.
    void* m_function;

    //! Thread priority level.
    rtsThreadPriority m_priorityLevel;
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
