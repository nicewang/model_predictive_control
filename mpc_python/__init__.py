"""
 * @file            model_predictive_control/mpc_python/__init__.py
 * @description     
 * @author          nicewang <wangxiaonannice@gmail.com>
 * @createTime      2026-03-16
 * @lastModified    2026-03-17
 * Copyright © Xiaonan (Nice) Wang. All rights reserved
"""

import numpy as np
import sys

try:
    from . import mpc_core
except ImportError as e:
    print(f"Failed to import mpc_core: {e}", file=sys.stderr)
    raise

class MPCController:
    """
    Python wrapper class for the C++ MPC backend.
    """
    def __init__(self, A: np.ndarray, B: np.ndarray, Q: np.ndarray, R: np.ndarray, 
                 u_min: np.ndarray, u_max: np.ndarray, N: int):
        A = np.asarray(A, dtype=np.float64)
        B = np.asarray(B, dtype=np.float64)
        Q = np.asarray(Q, dtype=np.float64)
        R = np.asarray(R, dtype=np.float64)
        u_min = np.asarray(u_min, dtype=np.float64)
        u_max = np.asarray(u_max, dtype=np.float64)
        
        self._mpc = mpc_core.MPC(A, B, Q, R, u_min, u_max, N)
        
    def solve(self, x0: np.ndarray, x_ref: np.ndarray) -> np.ndarray:
        """
        Solve the MPC problem.
        Returns the optimal control sequence.
        """
        x0 = np.asarray(x0, dtype=np.float64)
        x_ref = np.asarray(x_ref, dtype=np.float64)
        return self._mpc.solve(x0, x_ref)

    def get_first_input(self) -> np.ndarray:
        """
        Get the first control action to apply to the system.
        """
        return self._mpc.get_first_input()
    