# C++ Template for Metaheuristics

C++ Template of practices in the course Metaheuristics in the course of Computer Science degree at the University of Granada.

This is a skeleton of the C++ for doing the practices, 

The main design criteria applied were: 

- To clearly separate the problem part from the algorithm 
- To make easier to change the algorithm maintaining the same API, see the main code.
- To identify the files parts to maintain fixed (in **/common** directory) for the files to be adapted.
- To facilitate the creation of automatic tests.

The C++ template for metaheuristics follows a clean, modular design with clear separation of concerns:

## Core Components

### 1. **Problem Interface (`Problem`)**
Abstract base class defining the problem's API.

*Example:* `ProblemIncrem` implements a binary problem where fitness = count of even-indexed 1s minus odd-indexed 1s.

The `Problem` interface (abstract base class) defines these key methods:

- `tFitness fitness(const tSolution &solution)`
  → Computes fitness of a given solution.
  *Example: In `ProblemIncrem`*, it Counts +1 for each `1` at even index, -1 for each `1` at odd index.

- `tSolution createSolution()`
  → Generates a random valid solution (it must fulfill all constraints).
  *Example: In `ProblemIncrem`*, it returns a binary vector of given size, each bit randomly `true`/`false`.

- `size_t getSolutionSize()`
  → Returns the fixed size of solutions (e.g., number of bits).

- `std::pair<tDomain, tDomain> getSolutionDomainRange()`
  → Returns domain bounds (min, max) for solution components.
  *In `ProblemIncrem`*: Returns `(false, true)` — binary domain.
  
- `bool isValid(const tSolution<tDomain> &solution)`
  → Checks if a solution meets all problem constraints.
  *Example: In `ProblemIncrem`, always returns `true` as any binary vector is valid.*

- `void fix(tSolution<tDomain> &solution)`
  → Modifies an invalid solution to meet constraints.
  *Example: In `ProblemIncrem`, no fixing is needed, so method may be left empty.*
```
All metaheuristics (`RandomSearch`, `GreedySearch`) rely on these methods to evaluate, generate, and explore solutions without knowing internal structure.

### 2. **Metaheuristic Interface (`MH`)**

The `MH` (Metaheuristic) interface defines a single abstract method:

```cpp
virtual ResultMH<tDomain> optimize(Problem<tDomain> &problem, int maxevals) = 0;
```

### Key Details:
- **Template**: `MH<tDomain>` — generic over solution domain type (e.g., `int`, `bool`).
- **Return Type**: `ResultMH<tDomain>` — a struct containing:
  - Best solution found (`tSolution<tDomain>`)
  - Its fitness value (`tFitness`)
  - Number of evaluations performed (`int`)
- **Parameters**:
  - `problem`: Instance of `Problem<tDomain>` to optimize.
  - `maxevals`: Maximum allowed fitness evaluations.

### Implemented by:
- `RandomSearch<tDomain>`: Generates random solutions, returns best.
- `GreedySearch<tDomain>`: Builds solution incrementally using heuristic (e.g., `ProblemIncrem`).

### Example Usage:
```cpp
ProblemInt p(10);
GreedySearch gs;
auto result = gs.optimize(p, 100); // Uses 1 eval (greedy), not 100
```

> **Note**: `GreedySearch::optimize` ignores `maxevals` in practice — it performs only *one* construction step, not `maxevals` evaluations. This is a design flaw unless intentional.

### 3. **Solution & Fitness Types**
- `tSolution`: `vector<tOption>` (it can be anyone: `bool`, `int`, `double`, ...)
- `tFitness`: `double` the type of the fitness function.

### 4. **Result Structure (`ResultMH`)**
Return type from `optimize()`: `{solution, fitness, evaluations}`

---

## Example Implementations

### Random Search (`RandomSearch`)
```cpp
ResultMH RandomSearch::optimize(Problem<tDomain> &p, int maxevals) {
  tSolution<tDomain> best; float best_fit = -1;
  for(int i=0; i<maxevals; i++) {
    auto sol = p->createSolution(); // Random binary vector
    auto fit = p->fitness(sol);
    if(fit < best_fit || best_fit < 0) { best = sol; best_fit = fit; }
  }
  return {best, best_fit, maxevals};
}
```

This algorithm is generic, because it could be applied without changes to different representations.

### Greedy Search (`GreedySearch`)
```cpp
using ProblemInt = Problem<int>;
using ResultMHInt = ResultMH<int>;

ResultMHInt GreedySearch::optimize(ProblemInt &p, int maxevals) {
  auto size = p.getSolutionSize();
  vector<int> values(size); iota(values.begin(), values.end(), 0);
  tSolution<int> sol(size, 0);
  
  for(int r = 0; r < size/2; r++) {
    // Heuristic: prefer even indices
    auto best_idx = min_element(heuristics.begin(), heuristics.end());
    sol[values[best_idx]] = 1;
    values.erase(values.begin() + best_idx);
  }
  return {sol, p->fitness(sol), 1}; // Only 1 evaluation
}
```

This algorithm is not generic, so it does not a template.

---

## Design Benefits

- ✅ **Swap algorithms easily**: Just change the `MH` instance in `main()`.
- ✅ **Testable**: Each `Problem` and `MH` can be unit-tested independently.
- ✅ **Extensible**: Add new problems (e.g., `TSPProblem`) or heuristics (e.g., `HillClimbing`) without breaking existing code.
- ✅ **Fixed API**: `/common/` files remain untouched — only `/src/` and `/inc/` are modified.

This design enables students to focus on algorithm logic, not boilerplate.
