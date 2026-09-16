import sys
from heapq import heappush, heappop

class PuzzleState:
    def __init__(self, state, parent=None, move=None, depth=0):
        self.state = state
        self.parent = parent
        self.move = move
        self.depth = depth
        self.cost = self.calculate_cost()

    def __lt__(self, other):
        return self.cost < other.cost

    def calculate_cost(self):
        return self.depth + self.manhattan_distance()

    def manhattan_distance(self):
        distance = 0
        for i in range(9):
            if self.state[i] == 0:
                continue
            x, y = i % 3, i // 3
            target_x, target_y = (self.state[i] - 1) % 3, (self.state[i] - 1) // 3
            distance += abs(x - target_x) + abs(y - target_y)
        return distance

    def is_goal(self):
        return self.state == (1, 2, 3, 4, 5, 6, 7, 8, 0)

    def get_children(self):
        children = []
        zero_index = self.state.index(0)
        x, y = zero_index % 3, zero_index // 3

        for dx, dy, move in [(-1, 0, 'left'), (1, 0, 'right'), (0, -1, 'up'), (0, 1, 'down')]:
            new_x, new_y = x + dx, y + dy
            if 0 <= new_x < 3 and 0 <= new_y < 3:
                new_index = new_y * 3 + new_x
                new_state = list(self.state)
                new_state[zero_index], new_state[new_index] = new_state[new_index], new_state[zero_index]
                children.append(PuzzleState(tuple(new_state), self, move, self.depth + 1))
        return children

    def get_path(self):
        path = []
        current = self
        while current:
            path.append(current)
            current = current.parent
        return path[::-1]

def solve_puzzle(initial_state):
    initial_state = tuple(map(int, initial_state))
    open_list = []
    closed_set = set()

    start_state = PuzzleState(initial_state)
    heappush(open_list, start_state)

    while open_list:
        current_state = heappop(open_list)

        if current_state.is_goal():
            return current_state.get_path()

        closed_set.add(current_state.state)

        for child in current_state.get_children():
            if child.state not in closed_set:
                heappush(open_list, child)

    return None

def print_solution(path):
    for i, state in enumerate(path):
        print(f"Step {i}: Move {state.move}" if state.move else "Initial State")
        print_state(state.state)
        print()

def print_state(state):
    for i in range(0, 9, 3):
        print(state[i:i+3])

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python 8puzzle.py <initial_configuration>")
        sys.exit(1)

    initial_config = sys.argv[1]
    if len(initial_config) != 9 or not initial_config.isdigit():
        print("Invalid input. Please provide a 9-digit string representing the puzzle configuration.")
        sys.exit(1)

    solution_path = solve_puzzle(initial_config)

    if solution_path:
        print("Solution found!")
        print_solution(solution_path)
    else:
        print("No solution found.")
