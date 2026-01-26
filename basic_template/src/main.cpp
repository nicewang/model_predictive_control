#include "../include/mpc.h"
#include <iomanip>

/**
 * @brief Example: MPC control of a simple 2D system
 * 
 * System dynamics: Simple position-velocity system
 * State: x = [position, velocity]^T
 * Control: u = [acceleration]
 * 
 * Standard MPC Framework with 4 Components:
 * 1. Prediction Model: Predicts N steps ahead using system dynamics
 * 2. Cost Function & Constraints: Defines optimization objective and bounds
 * 3. Receding Horizon Optimization: Globally optimizes all N control inputs
 * 4. Feedback Correction: Applies first input, re-optimizes next step
 */
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   Standard MPC Template (Basic) Demo" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    // ========== System Setup ==========
    std::cout << "[1] Setting up system dynamics..." << std::endl;
    
    // State transition matrix A (2×2)
    // Represents: position(k+1) = position(k) + velocity(k)
    //             velocity(k+1) = velocity(k)
    Eigen::MatrixXd A(2, 2);
    A << 1.0, 1.0,
         0.0, 1.0;
    std::cout << "A matrix:\n" << A << "\n" << std::endl;
    
    // Control input matrix B (2×1)
    // Represents effect of acceleration on position and velocity
    Eigen::MatrixXd B(2, 1);
    B << 0.5,
         1.0;
    std::cout << "B matrix:\n" << B << "\n" << std::endl;
    
    // ========== Cost Function Setup ==========
    std::cout << "[2] Setting up cost function..." << std::endl;
    
    // State cost weight Q (2×2)
    // Higher values penalize deviation from reference state more strongly
    // Q = 100.0: Strong tracking to reference position and velocity
    Eigen::MatrixXd Q(2, 2);
    Q << 100.0, 0.0,    // Strong position tracking
         0.0, 100.0;    // Strong velocity tracking
    std::cout << "Q matrix (state weight):\n" << Q << "\n" << std::endl;
    
    // Input cost weight R (1×1)
    // Penalizes control effort (energy consumption)
    // R = 0.01: Low cost allows more aggressive control
    Eigen::MatrixXd R(1, 1);
    R << 0.01;  // Low control cost to allow aggressive control
    std::cout << "R matrix (input weight): " << R(0, 0) << "\n" << std::endl;
    
    // ========== Constraints Setup ==========
    std::cout << "[3] Setting up constraints..." << std::endl;
    
    // Control input bounds
    Eigen::VectorXd u_min(1), u_max(1);
    u_min << -1.0;  // Minimum acceleration
    u_max << 1.0;   // Maximum acceleration
    std::cout << "Input constraints: [" << u_min.transpose() << ", " 
              << u_max.transpose() << "]\n" << std::endl;
    
    // ========== MPC Parameters ==========
    std::cout << "[4] Setting up MPC parameters..." << std::endl;
    
    // Prediction horizon: N = 20 steps ahead
    // Longer horizon = better future planning, more computation
    int N = 20;
    std::cout << "Prediction horizon N: " << N << " steps\n" << std::endl;
    
    // ========== Initial and Reference States ==========
    std::cout << "[5] Defining initial and reference states..." << std::endl;
    
    // Initial state: at origin with zero velocity
    Eigen::VectorXd x0(2);
    x0 << 0.0, 0.0;
    std::cout << "Initial state x0: " << x0.transpose() << std::endl;
    
    // Reference state: position = 10, velocity = 0 (at rest at target position)
    Eigen::VectorXd x_ref(2);
    x_ref << 10.0, 0.0;
    std::cout << "Reference state x_ref: " << x_ref.transpose() << "\n" << std::endl;
    
    // ========== MPC Instantiation ==========
    std::cout << "========================================" << std::endl;
    std::cout << "   Instantiating MPC Controller" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    // Create MPC controller with all components
    MPC mpc(A, B, Q, R, u_min, u_max, N);
    
    // ========== Simulation Loop ==========
    std::cout << "========================================" << std::endl;
    std::cout << "   Running Control Simulation" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    int num_steps = 100;  // Simulate 100 time steps
    Eigen::VectorXd x = x0;
    
    std::cout << std::setw(5) << "Step" 
              << std::setw(15) << "Position" 
              << std::setw(15) << "Velocity"
              << std::setw(15) << "Control u"
              << std::setw(15) << "Error" << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    
    for (int k = 0; k < num_steps; ++k) {
        // Solve MPC problem at current state
        // 4 MPC components:
        // 1. Prediction Model: Predicts system behavior N steps ahead
        // 2. Cost Function & Constraints: Defines optimization objective
        // 3. Receding Horizon Optimization: Globally optimizes all N inputs
        // 4. Feedback Correction: Returns only u(0), will re-optimize next step
        Eigen::VectorXd u = mpc.solve(x, x_ref);
        
        // Clamp input to ensure it respects bounds
        u = u.cwiseMax(u_min).cwiseMin(u_max);
        
        // Apply control input and propagate system dynamics
        x = A * x + B * u;
        
        // Calculate tracking error (distance from reference)
        double error = (x - x_ref).norm();
        
        // Print current state and control
        std::cout << std::setw(5) << k
                  << std::setw(15) << std::fixed << std::setprecision(4) << x(0)
                  << std::setw(15) << std::fixed << std::setprecision(4) << x(1)
                  << std::setw(15) << std::fixed << std::setprecision(4) << u(0)
                  << std::setw(15) << std::fixed << std::setprecision(4) << error << std::endl;
        
        // Check if converged to reference state
        if (error < 0.01) {
            std::cout << "\n[Success] System converged to reference state!" << std::endl;
            break;
        }
    }
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "   Simulation Complete" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    std::cout << "Final state: " << x.transpose() << std::endl;
    std::cout << "Reference state: " << x_ref.transpose() << std::endl;
    std::cout << "Final error: " << (x - x_ref).norm() << std::endl;
    
    return 0;
}
