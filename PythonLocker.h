#ifndef __PYTHONLOCKER_H__
#define __PYTHONLOCKER_H__

#undef slots
#include <Python.h>
#define slots

//Threads are created from C don’t hold the GIL, nor is there a thread state structure for them.
//If you need to call Python code from these threads, you must first register these threads with
//the interpreter by creating a thread state data structure, then acquiring the GIL, and finally 
//storing their thread state pointer, before you can start using the Python / C API.  This PythonLocker
//class allows Python/C API in C created threads to be wrapped with this functionality.

class PythonLocker
{
public:
    PythonLocker();
    ~PythonLocker();

private:

    PyGILState_STATE m_gstate;
};

#endif
