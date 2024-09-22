#ifndef MPC_H
#define MPC_H

#include <Eigen/Dense>
#include <iostream>
#include <vector>

class MPC {
public:
    // Constructor
    MPC(const Eigen::MatrixXd& A, const Eigen::MatrixXd& B, const Eigen::MatrixXd& Q, const Eigen::MatrixXd& R, double u_min, double u_max, int N);

    // Solve MPC problem
    Eigen::VectorXd solve(const Eigen::VectorXd& x0, const Eigen::VectorXd& x_ref);

private:
    Eigen::MatrixXd A_;     // System dynamics matrix
    Eigen::MatrixXd B_;     // Control input matrix
    Eigen::MatrixXd Q_;     // State cost weight matrix
    Eigen::MatrixXd R_;     // Input cost weight matrix
    double u_min_;          // Control input lower bound
    double u_max_;          // Control input upper bound
    int N_;                 // Prediction horizon
};

#endif // MPC_H
