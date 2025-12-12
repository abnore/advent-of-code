from z3 import *
import sys
import re

# Parse a "(1,3,4)" string into a list of ints [1,3,4]
def parse_button(s):
    inside = s.strip()[1:-1]   # remove parentheses
    if inside == "":
        return []
    return [int(x) for x in inside.split(",")]

# Parse "{3,5,4,7}" into [3,5,4,7]
def parse_req(s):
    inside = s.strip()[1:-1]
    return [int(x) for x in inside.split(",")]


def solve_line(line):
    # Example line structure:
    #   [.#..#] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}

    parts = line.split()
    # Buttons start after the "[...]" part
    buttons = []
    i = 1
    while i < len(parts) and parts[i][0] == '(':
        buttons.append(parse_button(parts[i]))
        i += 1

    # Next token is the { ... } vector
    req = parse_req(parts[i])

    num_lights = len(req)
    num_buttons = len(buttons)

    # Matrix A[row][col] = 1 if button col hits light row
    A = [[0] * num_buttons for _ in range(num_lights)]
    for j, hits in enumerate(buttons):
        for h in hits:
            A[h][j] = 1

    # Create Z3 variables x0..xN
    xs = [Int(f"x{j}") for j in range(num_buttons)]

    solver = Optimize()

    # All xi >= 0
    for x in xs:
        solver.add(x >= 0)

    # Linear constraints A*x = req
    for r in range(num_lights):
        solver.add(Sum(A[r][c] * xs[c] for c in range(num_buttons)) == req[r])

    # Objective: minimize sum(xs)
    total_presses = Sum(xs)
    solver.minimize(total_presses)

    # Solve
    if solver.check() != sat:
        raise Exception("No solution for line!")

    model = solver.model()
    sol = [model[x].as_long() for x in xs]
    presses = sum(sol)
    return presses


def solve_file(filename):
    total = 0
    with open(filename) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            total += solve_line(line)
    return total


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python solve_day10_part2.py input.txt")
        sys.exit(1)

    print(solve_file(sys.argv[1]))
