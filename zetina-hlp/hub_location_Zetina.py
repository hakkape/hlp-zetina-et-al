#!/usr/bin/env python3
import ctypes
from pathlib import Path
from collections import defaultdict

class Solution(ctypes.Structure):
    _fields_ = [
        ("assigned_hubs", ctypes.POINTER(ctypes.c_int)),
        ("n_customers", ctypes.c_int),
        ("sol_val", ctypes.c_float),
        ("cputime", ctypes.c_float),
    ]


class HubLocationZetina():
    def __init__(self):
        # get the shared library
        project_root_path = Path(__file__).parent.parent
        lib_path = project_root_path / "build/libhlp.so"
        self._solverlib = ctypes.CDLL(lib_path)
        self._solverlib.solve.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int]
        self._solverlib.solve.restype = ctypes.c_void_p
        self._solverlib.freeSolution.argtypes = [ctypes.POINTER(Solution)]
        self.hlp_path = None
        self.hlps_path = None        
        
        # store solution
        self.solution = None

    def __del__(self):
        if self.solution is not None:
            self._solverlib.freeSolution(ctypes.byref(self.solution))

    # create model to solve specific instance
    @classmethod
    def from_file(cls, hlp_path,hlps_path): 
        model = HubLocationZetina()
        model.hlp_path = hlp_path
        model.hlps_path = hlps_path

    def solve(self, timelimit=None):
        if timelimit is None:
            timelimit = 10000000
        self.solution = Solution.from_address(self._solverlib.solve(self.hlp_path.encode(), self.hlps_path.encode(), timelimit))

    @property
    def solution_dict(self) -> dict[int, list[int]]:
        """Returns a dictionary where the keys are the opened facilities and the entries are lists of assigned customers."""
        solution = defaultdict(list)
        for customer in range(self.solution.n_customers):
            solution[self.solution.assigned_hubs[customer]].append(customer)
        return solution

    @property
    def solution_value(self):
        return self.solution.sol_val

    @property
    def runtime(self):
        return self.solution.cpu_time

if __name__ == "__main__":
    print("Solving CAB instance")
    instances = [
        # ("hlpython/data/10ll.hlp", "hlpython/data/zetina_p5.hlps"),
        # ("hlpython/data/100ll.hlp", "hlpython/data/zetina_p5.hlps"),
        # ("hlpython/data/ap20_c.hlp", "hlpython/data/p4_a05.hlps"),
        # ("hlpython/data/cab20_c.hlp", "hlpython/data/p4_a05.hlps"),
        ("hlpython/data/tr20_c.hlp", "hlpython/data/p4_a05.hlps"),
    ]

    project_root_path = Path(__file__).parent.parent
    for hlp, hlps in instances:
        hlp_file = str(project_root_path / hlp)
        hlps_file = str(project_root_path / hlps)
        solver = HubLocationZetina()
        solver.solve(hlp_file, hlps_file)
        print("Finished solving")

        sol = solver.solution
        print(sol.n_customers)
        print(sol.sol_val)
        for i in range(sol.n_customers):
            print(f"customer {i} was assigned hub {sol.assigned_hubs[i]}")