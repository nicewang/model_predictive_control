#include "../include/mpc.h"

int main() {
    // Define system dynamics for a simple system
    Eigen::MatrixXd A(2, 2);  // State transition matrix
    A << 1, 1,
         0, 1;

    Eigen::MatrixXd B(2, 1);  // Input matrix
    B << 0.5, 1;

    // Cost matrices
    Eigen::MatrixXd Q(2, 2);  // State cost
    Q << 1, 0,
         0, 1;

    Eigen::MatrixXd R(1, 1);  // Input cost
    R << 0.1;

    // Input constraints
    double u_min = -1.0;
    double u_max = 1.0;

    // Prediction horizon
    int N = 10;

    // Initial state and reference state
    Eigen::VectorXd x0(2);    // Initial state
    x0 << 0, 0;

    Eigen::VectorXd x_ref(2); // Reference state
    x_ref << 10, 0;

    // Instantiate MPC controller
    MPC mpc(A, B, Q, R, u_min, u_max, N);

    // Solve MPC for the current state
    Eigen::VectorXd u = mpc.solve(x0, x_ref);

    // Output the optimal control input
    std::cout << "Optimal control input: " << u.transpose() << std::endl;

    return 0;
}
