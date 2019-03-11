#pragma once

#include <QObject>
#include <QMutex>

#undef slots
#include <Python.h>
#define slots

#include "frameobject.h"
#include "PythonLocker.h"

//The PythonRunner class is an interface to execute a python class method calls.
//The public function "runLoop" and "getCounter" are specific to "pauseAbortDemo" python
//class, but these could be replaced with the real test driver functions.  The main functionality
//of the Python runner is to initialize the Python intrepretter, python class module passed, and give
//UI a mechanism to start, pause, abort, and resume a test/task.

class PythonRunner : public QObject
{
    Q_OBJECT

public:
    PythonRunner(QString moduleName, QString path, QString name, QObject *parent = nullptr);
    ~PythonRunner();

    //The tracing callback calls this member function to handle "Pause" and "Abort" functionality
    void tracer(void);

    //Public functions to execute C / Python methods in the pauseAbortDemo Python class
    void runLoop(void);
    int getCounter(void);

    //Helper function to notify the Python runner when "Pause", "Resume", and "Abort have been called
    void pause(void);
    void resume(void);
    void abort(void);

private:
    //Python module name i.e. filename
    QString m_strModuleName;
    //Path to python module
    QString m_modulePath;
    //Class name
    QString m_className;

    // The Python objects to facilitate creating the C / Python class object
    PyObject * m_pyModule, * m_pyObject, * m_pyDict, * m_pyClass;

    QMutex* m_pControlMutex;

    bool m_pyInitialized;
    bool m_blnPausePending;
    bool m_blnResumePending;
    bool m_interruptRequest;

    //Function to initialize and import the python demo object so it call be called
    void initializeAndImportPythonDemo();
    
    //Getter functions
    bool pauseRequested(void);
    bool resumeRequested(void);
    bool interruptRequested(void);

    //Static functions that will be registered with the PyEval_SetProfile call.  
    
    //The calls out to the demo are blocking functions, but we can register a callback that will get called every ~100 python lines.
    //The demo sleeps every second so the traceFunction is frequently called.  This gives us a hook to "Pause"
    //or "Abort".  This callback function is necessarily a static function.  So it gets the one and only one 
    //instance of PythonRunner and calls the member tracer function.

    static int tracer(PyObject *, struct _frame *, int, PyObject *);

    
};
