#include "PythonRunner.h"
#include <QThread>

static PythonRunner* _instance = NULL;

PythonRunner::PythonRunner(QString moduleName, QString path, QString name, QObject *parent)
    : QObject(parent),
    m_strModuleName(moduleName),
    m_modulePath(path),
    m_className(name)
{
    //Initialize variables  
    m_pyInitialized = false;
    m_blnPausePending = false;
    m_blnResumePending = false;
    m_interruptRequest = false;

    //Initialize mutex
    m_pControlMutex = new QMutex(QMutex::Recursive);

    //Initialize our instance so the static callback functions can access this intance
    _instance = this;

    Py_Initialize();
}

PythonRunner::~PythonRunner()
{
    Py_Finalize();

    if (m_pControlMutex)
    {
        delete m_pControlMutex;
        m_pControlMutex = nullptr;
    }
}

//Imports the python demo class, and creates it from the module path and module name (passed in from constructor)
void PythonRunner::initializeAndImportPythonDemo()
{
    PythonLocker locker;
       
    //Initializes the python module passed in
    PyRun_SimpleString("import sys");
    QString pathWithCommand = "sys.path.append(\"";
    pathWithCommand += m_modulePath + "\")";
    PyRun_SimpleString(pathWithCommand.toStdString().c_str());

    // Imports the pause abort module.
    PyObject * pStr = PyUnicode_DecodeFSDefault(m_strModuleName.toStdString().c_str());

    m_pyModule = PyImport_Import(pStr);
    Py_DECREF(pStr);

    if (!m_pyModule)
    {
        PyErr_Print();
        m_pyInitialized = false;
    }

    //Get the dictionary.  Dict is a borrowed reference.
    m_pyDict = PyModule_GetDict(m_pyModule);
    if (!m_pyDict)
    {
        PyErr_Print();
        m_pyInitialized = false;
    }
    Py_DECREF(m_pyModule);

    // Builds the name of a callable class
    m_pyClass = PyDict_GetItemString(m_pyDict, m_className.toStdString().c_str());
    if (!m_pyClass)
    {
        PyErr_Print();
        m_pyInitialized = false;
    }
    Py_DECREF(m_pyDict);


    // Creates an instance of the class
    if (PyCallable_Check(m_pyClass))
    {
        m_pyObject = PyObject_CallObject(m_pyClass, nullptr);
        Py_DECREF(m_pyClass);
    }
    else
    {
        //Log error
        Py_DECREF(m_pyClass);
        m_pyInitialized = false;
    }

    m_pyInitialized = true;

}

void PythonRunner::runLoop()
{
    //Initialize the python interpretter, imports the python demo, and initializes the PythonDemo class...
    initializeAndImportPythonDemo();

    //Python locker for any C Python API call
    PythonLocker locker;

    //We are initialized
    if (!m_pyInitialized)
    {
        return;  //TODO add logging
    }

    //Here we register traceFunction to give us a hook into the python execution
    PyEval_SetProfile(PythonRunner::tracer, NULL);
    
    //Abort interrupt reset to false
    m_interruptRequest = false;
    
    //Call the runLoop method
    const char *szName = "runLoop";

    PyObject_CallMethod(m_pyObject, szName, nullptr);

    int x =5;

}

int PythonRunner::getCounter()
{
    PythonLocker locker;

    //We are initialized
    if (!m_pyInitialized)
    {
        return -1;  //TODO add logging
    }

    //Calls the getCounter method and returns the value
    PyObject *value = PyObject_CallMethod(m_pyObject, "getCounter", nullptr);
    if (!PyLong_Check(value))
    {
        Py_DECREF(value);
    }

    int counter = (int)PyLong_AsLong(value);
    
    Py_DECREF(value);

    return counter;
}

//Help functions that allow the UI to let us know when the user would like to pause, resume, or abort
void PythonRunner::pause(void)
{
    QMutexLocker locker(m_pControlMutex);

    m_blnPausePending = true;
}

void PythonRunner::resume(void)
{
    QMutexLocker locker(m_pControlMutex);

    m_blnResumePending = true;
}

void PythonRunner::abort(void)
{
    QMutexLocker locker(m_pControlMutex);

    m_interruptRequest = true;
}

//Getter functions 
bool PythonRunner::interruptRequested(void)
{
    QMutexLocker locker(m_pControlMutex);

    return m_interruptRequest;
}

int PythonRunner::tracer(PyObject *, _frame *, int, PyObject *)
{
    PythonLocker locker;

    //Pause is requested...
    if (_instance->pauseRequested())
    {
        Py_BEGIN_ALLOW_THREADS;

        //If we are paused and no resume has been requested sleep
        while (!_instance->resumeRequested())
        {
            //spin here.
            QThread::msleep(500);
        }

        Py_END_ALLOW_THREADS;

    }
    //Abort requested
    else if (true == _instance->interruptRequested())
    {
        PyErr_SetString(PyExc_Exception, "ABORT");
        return -1;
    }

    // Otherwise proceed normally.
    return 0;
}

bool PythonRunner::pauseRequested(void)
{
    QMutexLocker locker(m_pControlMutex);

    bool blnReturn = m_blnPausePending;

    m_blnPausePending = false;

    return blnReturn;
}

bool PythonRunner::resumeRequested(void)
{
    QMutexLocker locker(m_pControlMutex);

    bool blnReturn = m_blnResumePending;

    m_blnResumePending = false;

    return blnReturn;
}
