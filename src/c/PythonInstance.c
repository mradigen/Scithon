#include "PythonInstance.h"

void Initialize() {
    char const *code ="\
import sys\n\
class StdoutCatcher:\n\
    def __init__(self):\n\
        self.data = ''\n\
    def write(self, val):\n\
        self.data += val\n\
stdout_catcher = StdoutCatcher()\n\
stderr_catcher = StdoutCatcher()\n\
sys.stdout = stdout_catcher\n\
sys.stderr = stderr_catcher\n\
    ";
    Py_Initialize();
    Main = PyImport_AddModule("__main__");
    PyRun_SimpleString(code);
    
}

void Finalize() {
    Main = NULL;
    Py_Finalize();
}

void GetStdOut(char *op) {
    PyObject *StdOut = PyObject_GetAttrString(Main, "stdout_catcher");
    PyObject *output = PyObject_GetAttrString(StdOut, "data");
    strcpy(op, PyUnicode_AsUTF8(output));
    PyRun_SimpleString("stdout_catcher.data = ''");
}

long GetStdOutSize() {
    PyObject *StdOut = PyObject_GetAttrString(Main, "stdout_catcher");
    PyObject *data = PyObject_GetAttrString(StdOut, "data");
    return PyObject_Size(data);
}

void GetStdErr(char *op) {
    PyObject *StdErr = PyObject_GetAttrString(Main, "stderr_catcher");
    PyObject *output = PyObject_GetAttrString(StdErr, "data");
    strcpy(op, PyUnicode_AsUTF8(output));
    PyRun_SimpleString("stderr_catcher.data = ''");
}

long GetStdErrSize() {
    PyObject *StdErr = PyObject_GetAttrString(Main, "stderr_catcher");
    PyObject *data = PyObject_GetAttrString(StdErr, "data");
    return PyObject_Size(data);
}

PyObject* GetPyObject(char *var) {
    if (!PyObject_HasAttrString(Main, var)) {
        return NULL;
    }

    PyObject *pvar = PyObject_GetAttrString(Main, var);
    return pvar;
}