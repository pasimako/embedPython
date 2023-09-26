#include "pyrun.h"

#include <QString>
#include <QStringList>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

PyRun::PyRun(QString execFile)
{
    QString pythonStdLib = "libpy311.zip";
    QString pythonStdLibPath = QDir::toNativeSeparators(QFileInfo(QFileInfo(execFile).absoluteDir(), pythonStdLib).canonicalFilePath());
    QString rcssminPath = "://res/rcssmin.py.codeobj";

    PyConfig config;
    PyConfig_InitPythonConfig(&config);

    // Path of our executable
    PyConfig_SetBytesString(&config, &config.program_name, execFile.toStdString().c_str());

    PyConfig_Read(&config);

    // Set module search path
    config.module_search_paths_set = 1;
    PyWideStringList_Append(&config.module_search_paths, pythonStdLibPath.toStdWString().c_str());

    Py_NoSiteFlag = 1;

    // Initialize the Python interpreter
    Py_InitializeFromConfig(&config);

    qDebug() << "Python interpreter version:" << QString(Py_GetVersion());
    qDebug() << "Python standard library path:" << QString::fromWCharArray(Py_GetPath());

    QFile f(rcssminPath);

    if(f.open(QIODevice::ReadOnly))
    {
        QByteArray codeObj = f.readAll();
        f.close();
        importModule(codeObj, "rcssmin");
    }
}

PyRun::~PyRun()
{
    Py_Finalize();
}

PyObject* PyRun::importModule(const QByteArray& codeObj, const QString& moduleName)
{
    PyObject *poModule = NULL;

    // Get reference to main module
    PyObject *mainModule = PyImport_AddModule("__main__");

    // De-serialize Python code object
    PyObject *poCodeObj = PyMarshal_ReadObjectFromString((char*)codeObj.data(), codeObj.size());

    if(!hasError())
    {
        // Load module from code object
        poModule = PyImport_ExecCodeModule(moduleName.toUtf8().data(), poCodeObj);

        if(!hasError())
        {
            // Add module to main module as moduleName
            PyModule_AddObject(mainModule, moduleName.toUtf8().data(), poModule);
        }

        // Release object reference (Python cannot track references automatically in C++!)
        Py_XDECREF(poCodeObj);
    }

    return poModule;
}

PyObject *PyRun::callFunction(PyObject *poModule, QString funcName, PyObject *poArgs)
{
    PyObject *poRet = NULL;

    // Get reference to function funcName in module poModule
    PyObject *poFunc = PyObject_GetAttrString(poModule, funcName.toUtf8().data());

    if(!hasError())
    {
        // Call function with arguments poArgs
        poRet = PyObject_CallObject(poFunc, poArgs);

        if(hasError())
        {
            poRet = NULL;
        }

        // Release reference to function
        Py_XDECREF(poFunc);
    }

    // Release reference to arguments
    Py_XDECREF(poArgs);

    return poRet;
}

QString PyRun::cssmin(QString css)
{
    QString res;

    // Get reference to rcssmin module
    PyObject *poModule = PyImport_AddModule("rcssmin");

    if(!hasError())
    {
        PyObject *poRet = callFunction(poModule, "cssmin", Py_BuildValue("(s)", css.toUtf8().data()));
        res = ObjectToString(poRet);
    }

    return res;
}

QString PyRun::ObjectToString(PyObject *poVal)
{
    QString val;

    if(poVal != NULL)
    {
        if(PyUnicode_Check(poVal))
        {
            // Convert Python Unicode object to UTF8 and return pointer to buffer
            const char *str = PyUnicode_AsUTF8(poVal);

            if(!hasError())
            {
                val = QString(str);
            }
        }

        // Release reference to object
        Py_XDECREF(poVal);
    }

    return val;
}

bool PyRun::hasError()
{
    bool error = false;

    if(PyErr_Occurred())
    {
        // Output error to stderr and clear error indicator
        PyErr_Print();
        error = true;
    }

    return error;
}
