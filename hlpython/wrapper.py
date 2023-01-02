import ctypes
from pathlib import Path

project_root_path = Path(__file__).parent.parent
lib_path = project_root_path / "build/libhlp.so"
_solverlib = ctypes.CDLL(lib_path)


class Solution(ctypes.Structure):
    global _solverlib
    _fields_ = [
        ("assigned_hubs", ctypes.POINTER(ctypes.c_int)),
        ("n_customers", ctypes.c_int),
        ("sol_val", ctypes.c_float),
        ("cputime", ctypes.c_float),
    ]

    # def __del__(self):
    #     _solverlib.freeSolution(ctypes.byref(self))


_solverlib.solve.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
_solverlib.solve.restype = ctypes.c_void_p
_solverlib.freeSolution.argtypes = [ctypes.POINTER(Solution)]


def solve(hlp_path, hlps_path):
    global _solverlib
    sol = Solution.from_address(_solverlib.solve(hlp_path.encode(), hlps_path.encode()))
    print(sol.n_customers)
    print(sol.sol_val)
    for i in range(sol.n_customers):
        print(f"customer {i} was assigned hub {sol.assigned_hubs[i]}")


if __name__ == "__main__":
    print("Solving CAB instance")
    instances = [
        ("hlpython/data/10ll.hlp", "hlpython/data/c10_p4_zetina.hlps"),
        ("hlpython/data/10lt.hlp", "hlpython/data/c10_p4_zetina.hlps"),
        ("hlpython/data/100ll.hlp", "hlpython/data/c100_p10_zetina.hlps"),
    ]

    for hlp, hlps in instances:
        hlp_file = str(project_root_path / hlp)
        hlps_file = str(project_root_path / hlps)

        solve(hlp_file, hlps_file)
        print("Finished solving")
