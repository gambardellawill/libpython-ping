#include <Python.h>

/*static PyObject *
spam_system(PyObject *self, PyObject *args)
{
  const char *command;
  int sts;

  if(!PyArg_ParseTuple(args, "s", &command))
    return NULL;

  sts = system(command);
  return PyLong_FromLong(sts);
}


static PyObject *SpamError
PyMODINIT_FUNC PyInit_spam(void)
{
  PyObject *m;

  m = PyModule_Create(&spammodule);
  if (m == NULL)
    return NULL;

  SpamError = PyErr_NewException("spam.error", NULL, NULL);
  Py_INCREF(SpamError);
  PyModule_AddObject(m, "error", SpamError);
  return m;
}*/

int Cfib(int n)
{
  if (n < 2)
    return n;
  else
    return Cfib(n-1)+Cfib(n-2);
}

static PyObject* fib(PyObject* self, PyObject* args)
{
  int n;

  if(!PyArg_ParseTuple(args, "i", &n))
    return NULL;

  return Py_BuildValue("i", Cfib(n));
}

static PyObject* version(PyObject* self)
{
  return Py_BuildValue("s", "Version 1.0");
}

static PyMethodDef myMethods[] = {
  {"fib", fib, METH_VARARGS, "Calculate the Fibonacci numbers."},
  {"version", (PyCFunction)version, METH_NOARGS, "Returns the version."},
  {NULL, NULL, 0, NULL}
};

static struct PyModuleDef myModule = {
  PyModuleDef_HEAD_INIT,
  "myModule",
  "Fibonacci Module",
  -1,
  myMethods
};

PyMODINIT_FUNC PyInit_myModule(void) {
  return PyModule_Create(&myModule);
}
