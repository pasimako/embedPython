#ifndef PYRUN_H
#define PYRUN_H

#include <Python.h>
#include <marshal.h>

#include <QString>

class PyRun
{
public:
    PyRun(QString);
    ~PyRun();
    QString cssmin(QString);

private:
    std::wstring execFile;
    std::wstring pythonPath;
    bool hasError();
    PyObject* importModule(const QByteArray&, const QString&);
    PyObject* callFunction(PyObject*, QString, PyObject*);
    QString ObjectToString(PyObject*);
};

#endif // PYRUN_H
