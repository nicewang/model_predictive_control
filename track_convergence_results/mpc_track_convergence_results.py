import numpy as np
import time
from mpc_python import MPCController

from track_results import TrackResults
from convergence_logger import LoggerStatistics, CountMinMaxMeanVarStd

def run_mpc_experiment(Q_weight, R_weight, N_horizon):
    """
    Run MPC simulation, log convergence steps, and return final tracking metrics.
    """
    # 1. System Dynamics Setup
    nx, nu = 2, 1
    A = np.array([[1.0, 1.0], 
                  [0.0, 1.0]])
    B = np.array([[0.5], 
                  [1.0]])
    
    # Convert weights to numpy arrays
    Q = np.array(Q_weight, dtype=np.float64)
    R = np.eye(nu) * R_weight

    u_min, u_max = np.array([-1.0]), np.array([1.0])
    
    # Initialize Controller
    mpc = MPCController(A, B, Q, R, u_min, u_max, N_horizon)
    
    x_current = np.array([0.0, 0.0])
    x_ref = np.array([10.0, 0.0])
    
    # ===========================================
    # Initialize Convergence Logger
    # Create a unique name for this specific run
    # ===========================================
    conv_logger = LoggerStatistics(
        statistics=CountMinMaxMeanVarStd(),
        n_intervals=100,
        n_values=4,
        min_time_range=0.0,
        max_time_range=100.0
    )
    
    total_error = 0.0
    start_time = time.time()
    
    # 2. Simulation Loop
    for k in range(100):
        mpc.solve(x_current, x_ref)
        u_first = mpc.get_first_input()

        error = np.linalg.norm(x_current - x_ref)
        total_error += error

        # =============================================
        # Log Microscopic Step Data
        # Record the internal state at every time step
        # =============================================
        conv_logger.add_value(
            t=float(k),
            values=[
                float(x_current[0]),
                float(x_current[1]),
                float(u_first[0]),
                float(error)
            ]
        )

        # Propagate
        x_current = A @ x_current + B @ u_first

        if error < 0.01:
            print(f"  -> Converged at step {k}!")
            break
        
    end_time = time.time()
    
    # Return Macroscopic Data
    return {
        "total_tracking_error": total_error,
        "execution_time_seconds": end_time - start_time,
        "final_state_distance": np.linalg.norm(x_current - x_ref),
        "convergence_step": k  # Good practice to also track how fast it converged
    }

# ===============================================
# Main: Automated parameter tuning and recording
# ===============================================
if __name__ == "__main__":
    # Initialize the Macro-Logger
    tracker = TrackResults(collection="mpc_tuning_experiments")
    
    # MPC Meta-parameters (Using standard lists for BSON serialization safety)
    test_parameters = [
        {"Q_w": [[100.0, 0.0], [0.0, 100.0]], "R_w": 0.01, "N": 2},
        {"Q_w": [[100.0, 0.0], [0.0, 100.0]], "R_w": 0.01, "N": 5},
        {"Q_w": [[100.0, 0.0], [0.0, 100.0]], "R_w": 0.01, "N": 10},
        {"Q_w": [[100.0, 0.0], [0.0, 100.0]], "R_w": 0.01, "N": 20},
        
        {"Q_w": [[100.0, 0.0], [0.0, 100.0]], "R_w": 0.1, "N": 2},
        {"Q_w": [[100.0, 0.0], [0.0, 100.0]], "R_w": 0.1, "N": 5},
        {"Q_w": [[100.0, 0.0], [0.0, 100.0]], "R_w": 0.1, "N": 10},
        {"Q_w": [[100.0, 0.0], [0.0, 100.0]], "R_w": 0.1, "N": 20},

        {"Q_w": [[10.0, 0.0], [0.0, 10.0]], "R_w": 0.01, "N": 2},
        {"Q_w": [[10.0, 0.0], [0.0, 10.0]], "R_w": 0.01, "N": 5},
        {"Q_w": [[10.0, 0.0], [0.0, 10.0]], "R_w": 0.01, "N": 10},
        {"Q_w": [[10.0, 0.0], [0.0, 10.0]], "R_w": 0.01, "N": 20},
        
        {"Q_w": [[10.0, 0.0], [0.0, 10.0]], "R_w": 0.1, "N": 2},
        {"Q_w": [[10.0, 0.0], [0.0, 10.0]], "R_w": 0.1, "N": 5},
        {"Q_w": [[10.0, 0.0], [0.0, 10.0]], "R_w": 0.1, "N": 10},
        {"Q_w": [[10.0, 0.0], [0.0, 10.0]], "R_w": 0.1, "N": 20},
    ]
    
    print(f"Starting {len(test_parameters)} MPC Experiments...\n")
    
    for params in test_parameters:
        print(f"Testing: Q={params['Q_w'][0][0]}, R={params['R_w']}, N={params['N']}")
        
        # 1. Run simulation
        metrics = run_mpc_experiment(params["Q_w"], params["R_w"], params["N"])
        
        # 2. Track Macro-results
        tracker.add(parameters=params, results=metrics)
        print(f"  -> Error: {metrics['total_tracking_error']:.2f} | Time: {metrics['execution_time_seconds']:.4f}s\n")

    # 3. Print Summary
    results_df = tracker.get()
    print("=========================================")
    print("       All Experiments Summary")
    print("=========================================")
    # Flatten the DataFrame slightly for better display
    display_df = results_df[['parameters_Q_w', 'parameters_R_w', 'parameters_N', 'results_total_tracking_error', 'results_convergence_step']]
    print(display_df.sort_values('results_total_tracking_error'))