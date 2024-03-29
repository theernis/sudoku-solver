import solver
import math

sudoku_size = 9
sudoku = [7, 0, 0, 1, 0, 0, 5, 0, 0,
          9, 3, 5, 4, 0, 0, 0, 0, 0,
          4, 0, 0, 8, 7, 0, 6, 0, 0,
          0, 0, 0, 3, 0, 0, 0, 6, 0,
          0, 0, 0, 0, 0, 7, 3, 0, 0,
          1, 0, 0, 0, 0, 0, 8, 0, 0,
          0, 8, 7, 0, 0, 6, 0, 0, 0,
          0, 0, 0, 0, 2, 0, 0, 0, 0,
          6, 0, 0, 0, 3, 0, 1, 0, 5]

def print_sudoku(temp_sudoku, temp_size):
    output = ""
    for x in range(temp_size):
        if (x != 0 and x%math.sqrt(temp_size) == 0):
            output += "-"
            for y in range(temp_size):
                if (y != 0 and y%math.sqrt(temp_size) == 0):
                    output += "+-"
                output += "--"
            output += "\n "
        else:
            output += " "
        for y in range(temp_size):
            if (y != 0 and y%math.sqrt(temp_size) == 0):
                output += "| "
            output += str(temp_sudoku[x * temp_size + y]) + " "
        output += "\n"
    print(output)


print(solver.set_sudoku(sudoku, sudoku_size))
print_sudoku(sudoku, sudoku_size)
print(solver.backtrack())
print_sudoku(solver.return_sudoku(), sudoku_size)
print(solver.check_sudoku())
