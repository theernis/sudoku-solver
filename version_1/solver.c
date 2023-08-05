#include <Python.h>
#include <Math.h>
#include <stdbool.h>

//sudoku struct
typedef struct {
    int size;
    int *sudoku;
} Sudoku;

Sudoku main_sudoku;

//allocates sudoku
void allocate_sudoku(Sudoku *sudoku, PyObject* py_sudoku, int size) {
    sudoku->size = size;
    sudoku->sudoku = (int*)malloc(sizeof(int) * (int)pow(size, 2));

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
}

//checks if sudokus is solved
bool check_sudoku_is_solved(Sudoku *sudoku) {

    int s = sudoku->size;

    for (int i = 0; i < s; i++)
    {
        int n = i * (s + (int)sqrt(s)) - (int)floor(i / (int)sqrt(s)) * (s - 1);

        //never freed could possibly lead to memory leaks
        bool *check_list = (bool*)malloc(sizeof(bool) * s);

        bool result = true;

        //resets checklist
        for (int j = 0; j < s; j++)
        {
            check_list[j] = false;
        }
        //loop trough row
        for (int j = 0; j < s; j++)
        {
            check_list[sudoku->sudoku[n - n % s + j] - 1] = true;
        }

        //check and resets checklist
        for (int j = 0; j < s; j++)
        {
            if (!check_list[j]) {
                result = false;
                break;
            }
            check_list[j] = false;
        }
        //loop trough column
        for (int j = 0; j < s; j++)
        {
            check_list[sudoku->sudoku[n % s + s * j] - 1] = true;
        }

        //check and resets checklist
        for (int j = 0; j < s; j++)
        {
            if (!check_list[j]) {
                result = false;
                break;
            }
            check_list[j] = false;
        }
        //loop trough cell
        for (int j = 0; j < s; j++)
        {
            check_list[sudoku->sudoku[n - n % (s * (int)sqrt(s)) + n % s - n % (int)sqrt(s) + j % (int)sqrt(s) + s * (int)floor(j / (int)sqrt(s))] - 1] = true;
        }
        //check checklist
        for (int j = 0; j < s; j++)
        {
            if (!check_list[j]) {
                result = false;
                break;
            }
        }

        //free(check_list);
        if (!result) {
            return false;
        }
    }

    return true;
}

//copies sudoku (make sure target sudoku is deallocated)
void copy_sudoku(Sudoku *target, Sudoku *sudoku) {
    int size = sudoku->size;
    target->size = size;
    target->sudoku = (int*)malloc(sizeof(int) * size * size);

    for (int i = 0; i < size * size; i++) {

        //copies each value individually
        target->sudoku[i] = sudoku->sudoku[i];
    }
}


//check if possible to place given number in given place
bool check_single(Sudoku* temp, int n, int num) {
    int s = temp->size;
    for (int j = 0; j < s; j++) {
        if (temp->sudoku[n - n % s + j] == num ||
            temp->sudoku[n % s + s * j] == num ||
            temp->sudoku[n - n % (s * (int)sqrt(s)) + n % s - n % (int)sqrt(s) + j % (int)sqrt(s) + s * (int)floor(j / (int)sqrt(s))] == num) {
            return 0;
        }
    }
    return 1;
}

//backtracking to solve sudoku
bool backtrack_solve(Sudoku *temp, int size, int i) {
    if (i >= pow(size, 2)) {
        return 1;
    }
    if (main_sudoku.sudoku[i] == 0) {
        for (int a = 1; a <= size; a++) {
            if (check_single(temp, i, a)) {
                temp->sudoku[i] = a;
                if (backtrack_solve(temp, size, i + 1)) {
                    return 1;
                }
            }
        }
        temp->sudoku[i] = 0;
    }
    else {
        if (backtrack_solve(temp, size, i + 1)) {
            return 1;
        }
    }
    return 0;
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

//returns to pythone code if sudoku is solved
static PyObject*
check_sudoku() {
    if (check_sudoku_is_solved(&main_sudoku)) {
        Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

//backtracking to solve sudoku and return if solved to python script
static PyObject*
backtrack() {
    Sudoku temp;
    copy_sudoku(&temp, &main_sudoku);
    if (backtrack_solve(&temp, main_sudoku.size, 0)) {
        copy_sudoku(&main_sudoku, &temp);
        deallocate_sudoku(&temp);
        Py_RETURN_TRUE;
    }
    deallocate_sudoku(&temp);
    Py_RETURN_FALSE;
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
    {"check_sudoku", check_sudoku, METH_NOARGS, NULL},
    {"backtrack", backtrack, METH_NOARGS, NULL},
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