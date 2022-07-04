import sys
import itertools
import math

import clingo
from clingo.control import Control

from .program import Program
from . import choices
from .utils import new_list, undef_atom_ignore, start_timer, end_timer

"""
Construct target rules from a Program and add them to a .

Suppose we have a query ℙ(Q|E) where Q = {a, not b} and E = {not c, d}. The target rules `r` and
`t` of ℙ(Q|E) will be:

```
e  :- not c, d.
q  :- a, not b.
r  :- q, e.
t  :- not q, e.
```

Target rule `r` corresponds to the query assignments in conditions 1 and 2, while `t` codifies the
truth values in conditions 3 and 4 in [1].

[1] - On the Semantics and Complexity of Probabilistic Logic Programs. Fabio Cozman and Denis Mauá.
Journal of Artificial Intelligence Research. 2017.
"""
def add_target_rules(P: Program, B: clingo.backend.Backend, T: list[tuple[clingo.symbol.Function, clingo.symbol.Function]]):
  for i, query in enumerate(P.Q):
    Q, E = query.Q, query.E
    r_f, t_f = clingo.parse_term(f"__query_r_{i}"), clingo.parse_term(f"__query_t_{i}")
    e = B.add_atom(clingo.parse_term(f"__query_e_{i}"))
    q = B.add_atom(clingo.parse_term(f"__query_q_{i}"))
    r, t = B.add_atom(r_f), B.add_atom(t_f)
    B.add_rule((e,), [B.add_atom(x) if t else -B.add_atom(x) for x, t in E])
    B.add_rule((q,), [B.add_atom(x) if t else -B.add_atom(x) for x, t in Q])
    B.add_rule((r,), (q, e,))
    B.add_rule((t,), (-q, e,))
    B.add_project([r, t])
    T[i] = (r_f, t_f)
  return T

"""
Runs exact inference in order to answer the queries in `P`.
"""
def exact(P: Program) -> list[tuple[float, float]]:
  # Get all probabilistic facts.
  PF = P.PF
  # Get all queries.
  queries = P.Q
  # Query results.
  n_queries = len(queries)
  R = new_list(n_queries, None)

  # Condition 1: if every stable model in Γ(θ) satisfies Q and E.
  cond_1 = new_list(n_queries, False)
  # Condition 2: if some stable model in Γ(θ) satisfies Q and E.
  cond_2 = new_list(n_queries, False)
  # Condition 3: if every stable model in Γ(θ) satisfies E but fails Q.
  cond_3 = new_list(n_queries, False)
  # Condition 4: if some stable model in Γ(θ) satisfies E but fails Q.
  cond_4 = new_list(n_queries, False)
  # How many models satisfy Q and E, and how many models satisfy E.
  count_q_e, count_e = new_list(n_queries, 0), new_list(n_queries, 0)
  # How many models satisfy E but do not satisfy Q completely.
  count_partial_q_e = new_list(n_queries, 0)
  # Model counts
  a, b = new_list(n_queries, .0), new_list(n_queries, .0)
  c, d = new_list(n_queries, .0), new_list(n_queries, .0)

  # We iterate through each total choice θ (itertools.product is written in C, so this loop
  # somewhat efficient.)
  for theta in itertools.product([False, True], repeat = len(PF)):
    # Initialize a clingo Control.
    C = Control(logger = undef_atom_ignore)
    # Force solver to output all stable models.
    C.configuration.solve.models = 0
    # Input the logic program into the clingo Control.
    C.add("base", [], P.P)
    # Add probabilistic facts according to θ.
    with C.backend() as B:
      for x in [PF[i].cl_f for i, x in enumerate(theta) if x]: B.add_rule((B.add_atom(x),))
    # Ground atoms.
    C.ground([("base", [])])
    # m is the number of stable models according to <P,θ>, i.e. m = |Γ(θ)|.
    m = 0
    # Zero-initialize counters.
    for i in range(n_queries):
      cond_1[i] = cond_2[i] = cond_3[i] = cond_4[i] = False
      count_q_e[i] = count_e[i] = count_partial_q_e[i] = 0
    # Count which models satisfy Q and/or E.
    def count_sat(s: clingo.solving.Model):
      nonlocal m
      m += 1
      for i, query in enumerate(queries):
        Q, E = query.Q, query.E
        all_e = all(s.contains(e) if t else not s.contains(e) for e, t in E) # if e = true, check if e ∈ σ.
        if not all_e: continue
        all_q = all(s.contains(q) if t else not s.contains(q) for q, t in Q) # if q = true, check if q ∈ σ.
        count_e[i] += 1
        if all_q: cond_2[i] = True; count_q_e[i] += 1
        else: cond_4[i] = True; count_partial_q_e[i] += 1
    # Solve for <P,θ>, running on_model for every stable model σ found.
    C.solve(on_model = count_sat)
    p = choices.pr(PF, theta)
    for i in range(n_queries):
      # Evaluate counts to judge whether cond_1 and/or cond_3 are true.
      if count_e[i] == m or len(queries[i].E) == 0:
        # All stable models satisfy Q and E completely.
        if count_q_e[i] == m: cond_1[i] = True
        # All stable models satisfy E, but none satisfies Q completely.
        if count_partial_q_e[i] == m: cond_3[i] = True
      # Add probability ℙ(θ) according to model satisfiabilities.
      a[i] += cond_1[i]*p
      b[i] += cond_2[i]*p
      c[i] += cond_3[i]*p
      d[i] += cond_4[i]*p
  for i in range(n_queries):
    # Evaluate a, b, c, d values and return ℙ(Q|E) as a tuple of lower and upper probabilities.
    _a, _b, _c, _d = a[i], b[i], c[i], d[i]
    if len(queries[i].E) == 0: R[i] = [_a, _b]
    else:
      if _b + _d == 0:
        print("Fail: ℙ(E) = 0!")
        R[i] = [-math.inf, math.inf]
      else:
        if _b + _c == 0 and _d > 0: R[i] = [0, 0]
        elif _a + _d == 0 and _b > 0: R[i] = [1, 1]
        else: R[i] = [_a/(_a+_d), _b/(_b+_c)]
    print(f"{queries[i]} = {R[i]}")
  return R

"""
Runs exact inference using brave and cautious reasoning in order to answer the queries in `P`.
"""
def exact_bc(P: Program) -> list[tuple[float, float]]:
  # Get all probabilistic facts.
  PF = P.PF
  # Get all queries.
  queries = P.Q
  # Query results.
  n_queries = len(queries)
  R = new_list(n_queries, None)
  # Model counts.
  a, b = new_list(n_queries, .0), new_list(n_queries, .0)
  c, d = new_list(n_queries, .0), new_list(n_queries, .0)
  # Target rules.
  T = new_list(n_queries, None)

  # We iterate through each total choice θ (itertools.product is written in C, so this loop
  # somewhat efficient.)
  for theta in itertools.product([False, True], repeat = len(PF)):
    # Initialize a clingo Control.
    C = Control(logger = undef_atom_ignore)
    # Force solver to output all stable models.
    C.configuration.solve.models = 0
    # Force projection on target rules.
    C.configuration.solve.project = "auto"
    # Input the logic program into the clingo Control.
    C.add("base", [], P.P)
    with C.backend() as B:
      # Add probabilistic facts according to θ.
      for x in [PF[i].cl_f for i, x in enumerate(theta) if x]: B.add_rule((B.add_atom(x),))
      # Add query targets.
      add_target_rules(P, B, T)
    # Ground atoms.
    C.ground([("base", [])])
    # Record consequences of either brave or cautious reasoning.
    K = None
    def record_consequences(s: clingo.solving.Model):
      nonlocal K; K = s.symbols(atoms = True)
    # Probability ℙ(θ) of total choice θ.
    p = choices.pr(PF, theta)
    # Solve for the cautious consequences of <P, θ>.
    C.configuration.solve.enum_mode = "cautious"
    C.solve(on_model = record_consequences)
    K_map = set(K)
    for i in range(n_queries):
      r, t = T[i]
      # Condition 1: if every stable model in Γ(θ) satisfies Q and E.
      a[i] += (r in K_map)*p
      # Condition 3: if every stable model in Γ(θ) satisfies E but fails Q.
      c[i] += (t in K_map)*p
    # Solve for the brave consequences of <P,θ>.
    C.configuration.solve.enum_mode = "brave"
    C.solve(on_model = record_consequences)
    K_map = set(K)
    for i in range(n_queries):
      r, t = T[i]
      # Condition 2: if some stable model in Γ(θ) satisfies Q and E.
      b[i] += (r in K_map)*p
      # Condition 4: if some stable model in Γ(θ) satisfies E but fails Q.
      d[i] += (t in K_map)*p
  for i in range(n_queries):
    # Evaluate a, b, c, d values and return ℙ(Q|E) as a tuple of lower and upper probabilities.
    _a, _b, _c, _d = a[i], b[i], c[i], d[i]
    if len(queries[i].E) == 0: R[i] = [_a, _b]
    else:
      if _b + _d == 0:
        print("Fail: ℙ(E) = 0!")
        R[i] = [-math.inf, math.inf]
      else:
        if _b + _c == 0 and _d > 0: R[i] = [0, 0]
        elif _a + _d == 0 and _b > 0: R[i] = [1, 1]
        else: R[i] = [_a/(_a+_d), _b/(_b+_c)]
    print(f"{queries[i]} = {R[i]}")
  return R