#include <QThread>
#include <QDebug>
#include "PythonLocker.h"

static int gil_init = 0;

//On initialization, register the thread with the interpretter and acquire the GUI 
PythonLocker::PythonLocker()
{
    //If the GIL is not initialized...initialize it
    if (!gil_init)
    {
        gil_init = 1;
        //Initialize and acquire the global interpreter lock.
        PyEval_InitThreads();
        //Release the global interpreter lock (if it has been created and thread support is enabled) and reset the thread state to NULL
        PyEval_SaveThread();
    }

    //Ensure that the current thread is ready to call the Python C API regardless of the current state of Python.
    m_gstate = PyGILState_Ensure();
}

//When finished, reset the thread state pointer, release the GIL, 
//and finally free the thread state data structure. Through the 
//PyGILState_Release call.
PythonLocker::~PythonLocker(void)
{
        PyGILState_Release(m_gstate);
}
