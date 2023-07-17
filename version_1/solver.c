#include <Python.h>
#include <Math.h>

//sudoku struct
typedef struct {
    int size;
    int *sudoku;
} Sudoku;

Sudoku main_sudoku;

//allocates sudoku
void allocate_sudoku(Sudoku *sudoku, PyObject* py_sudoku, int size) {
    sudoku->size = size;
    sudoku->sudoku = malloc(sizeof(int) * (int)pow(size, 2));

    for (int i = 0; i < size*size; i++) {

        //checks if item exists in a list and adds it
        //if it doesnt exist adds empty cell
        if (i < PyList_Size(py_sudoku)) {
            sudoku->sudoku[i] = PyLong_AsLong(PyList_GetItem(py_sudoku, i));
        }
        else {
            sudoku->sudoku[i] = 0;
        }
    }
}

//deallocates sudoku
void deallocate_sudoku(Sudoku *sudoku) {
    free(sudoku->sudoku);
    sudoku->sudoku = NULL;
    free(sudoku);
    sudoku = NULL;
}

//copies sudoku (make sure target sudoku is deallocated)
void copy_sudoku(Sudoku *target, Sudoku *sudoku) {
    int size = sudoku->size;
    target->size = size;
    target->sudoku = malloc(sizeof(int) * size * size);

    for (int i = 0; i < size * size; i++) {

        //copies each value individually
        target->sudoku[i] = sudoku->sudoku[i];
    }
}

//gets values from python code
static PyObject*
set_sudoku(PyObject* self, PyObject* args) {
    PyObject* temp_sudoku;
    int temp_size;
    //value parsing
    if (!PyArg_ParseTuple(args, "Oi", &temp_sudoku, &temp_size)) {
        Py_RETURN_FALSE;
    }

    //makes sure sudoku is square type
    if ((int)pow((int)sqrt(temp_size), 2) != temp_size) {
        Py_RETURN_FALSE;
    }

    //allocates main_sudoku
    allocate_sudoku(&main_sudoku, temp_sudoku, temp_size);

    Py_RETURN_TRUE;
}

//returns sudoku
static PyObject*
return_sudoku() {
    //creates new py_list for sudoku
    PyObject* py_sudoku = PyList_New(pow(main_sudoku.size, 2));
    //sets each value in py_list
    for (int i = 0; i < pow(main_sudoku.size, 2); i++) {
        PyList_SetItem(py_sudoku, i, PyLong_FromLong(main_sudoku.sudoku[i]));
    }
    return py_sudoku;
}

static PyMethodDef SomeMethods[] = {
    {"set_sudoku", set_sudoku, METH_VARARGS, NULL},
    {"return_sudoku", return_sudoku, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef solver = {
    PyModuleDef_HEAD_INIT,
    "solver",
    "Some lib",
    -1,
    SomeMethods
};


PyMODINIT_FUNC PyInit_solver(void) {
    return PyModule_Create(&solver);
}