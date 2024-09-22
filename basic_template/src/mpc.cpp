#include "../include/mpc.h"

// Constructor
MPC::MPC(const Eigen::MatrixXd& A, const Eigen::MatrixXd& B, const Eigen::MatrixXd& Q, const Eigen::MatrixXd& R, double u_min, double u_max, int N)
    : A_(A), B_(B), Q_(Q), R_(R), u_min_(u_min), u_max_(u_max), N_(N) {}

// Solve the MPC problem for a given initial state x0 and reference state x_ref
Eigen::VectorXd MPC::solve(const Eigen::VectorXd& x0, const Eigen::VectorXd& x_ref) {
    int nx = A_.rows();  // Number of states
    int nu = B_.cols();  // Number of inputs
    std::cout << "Number of states: " << nx << std::endl;
    std::cout << "Number of inputs: " << nu << std::endl;

    Eigen::VectorXd u_opt = Eigen::VectorXd::Zero(nu);  // Optimal control input (initial guess)
    Eigen::VectorXd x = x0;  // Current state

    // Iterate over the prediction horizon N
    for (int k = 0; k < N_; ++k) {
        // Control law: u = -Kx (simplified here, in practice we would use an optimizer)
        Eigen::VectorXd u = -R_.inverse() * B_.transpose() * (x - x_ref);  // Simplified control calculation

        // Apply input constraints
        u = u.cwiseMax(u_min_).cwiseMin(u_max_);

        // Update the state
        x = A_ * x + B_ * u;

        // Save the first control input to be applied
        if (k == 0) {
            u_opt = u;
        }
    }
    return u_opt;
}
