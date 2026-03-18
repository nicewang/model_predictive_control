/*
 * @file            model_predictive_control/mpc_python/cpp/mpc.h
 * @description     
 * @author          nicewang <wangxiaonannice@gmail.com>
 * @createTime      2026-03-16
 * @lastModified    2026-03-18
 * Copyright © Xiaonan (Nice) Wang. All rights reserved
*/

#ifndef MPC_H
#define MPC_H

#include <Eigen/Dense>
#include <iostream>
#include <cmath>

/**
 * @class MPC
 * @brief Model Predictive Control (MPC) implementation following standard framework
 * 
 * Standard MPC Framework with 4 Components:
 * 1. Prediction Model: Discrete-time linear system dynamics
 * 2. Cost Function & Constraints: Quadratic objective with input constraints
 * 3. Receding Horizon Optimization: Global optimization over prediction horizon
 * 4. Feedback Correction: Apply first optimal input and re-optimize at next step
 */
class MPC {
public:
    /**
     * @brief Constructor for MPC controller
     * @param A State transition matrix (nx × nx)
     * @param B Control input matrix (nx × nu)
     * @param Q State cost weight matrix (nx × nx)
     * @param R Input cost weight matrix (nu × nu)
     * @param u_min Lower bound for control input (nu × 1)
     * @param u_max Upper bound for control input (nu × 1)
     * @param N Prediction horizon length (number of steps)
     */
    MPC(const Eigen::MatrixXd& A, const Eigen::MatrixXd& B, 
        const Eigen::MatrixXd& Q, const Eigen::MatrixXd& R,
        const Eigen::VectorXd& u_min, const Eigen::VectorXd& u_max, int N);

    /**
     * @brief Solve the MPC optimization problem
     * 
     * This function implements the complete MPC cycle:
     * - Component 1: Build prediction model matrices (Sx, Su)
     * - Component 2: Construct cost function (Hessian H, Gradient g)
     * - Component 3: Solve global optimization problem
     * - Component 4: Return first optimal control input
     * 
     * @param x0 Current state measurement (nx × 1)
     * @param x_ref Reference/desired state (nx × 1)
     * @return First optimal control input u(0) (nu × 1)
     */
    Eigen::VectorXd solve(const Eigen::VectorXd& x0, const Eigen::VectorXd& x_ref);

    /**
     * @brief Get only the first control input to apply at current time step
     * @return First optimal control input u(0) (nu × 1)
     */
    Eigen::VectorXd getFirstInput() const { return U_opt_.head(B_.cols()); }

private:
    // ========== System Parameters ==========
    Eigen::MatrixXd A_;      // State transition matrix (nx × nx)
    Eigen::MatrixXd B_;      // Control input matrix (nx × nu)
    Eigen::MatrixXd Q_;      // State cost weight matrix (nx × nx)
    Eigen::MatrixXd R_;      // Input cost weight matrix (nu × nu)
    Eigen::VectorXd u_min_;  // Control input lower bounds (nu × 1)
    Eigen::VectorXd u_max_;  // Control input upper bounds (nu × 1)
    int N_;                  // Prediction horizon length
    
    // ========== Optimization Variables ==========
    Eigen::VectorXd U_opt_;  // Optimal control sequence [u(0); u(1); ...; u(N-1)]
    
    // ========== Functions of Core Components ==========

    /**
     * @brief Component 1: Build the expanded prediction matrices
     * 
     * Constructs matrices that relate all future states to current state and inputs:
     * X = Sx * x0 + Su * U
     * where X = [x(1); x(2); ...; x(N)] and U = [u(0); u(1); ...; u(N-1)]
     * 
     * @param Sx Output matrix relating states to initial state (N*nx × nx)
     * @param Su Output matrix relating states to control inputs (N*nx × N*nu)
     */
    void buildPredictionMatrices(Eigen::MatrixXd& Sx, Eigen::MatrixXd& Su);
    
    /**
     * @brief Component 2: Build Hessian matrix for quadratic optimization
     * 
     * The optimization problem is formulated as:
     * min (1/2) * U^T * H * U + g^T * U
     * s.t. u_min ≤ U[i] ≤ u_max (box constraints)
     * 
     * @param Su Prediction matrix relating future states to control inputs
     * @param H Output Hessian matrix (N*nu × N*nu)
     */
    void buildHessianMatrix(const Eigen::MatrixXd& Su, Eigen::MatrixXd& H);
    
    /**
     * @brief Component 2: Build gradient vector for quadratic optimization
     * 
     * @param x0 Current state
     * @param x_ref Reference state
     * @param Sx Prediction matrix relating future states to initial state
     * @param Su Prediction matrix relating future states to control inputs
     * @param g Output gradient vector (N*nu × 1)
     */
    void buildGradientVector(const Eigen::VectorXd& x0, const Eigen::VectorXd& x_ref,
                            const Eigen::MatrixXd& Sx, const Eigen::MatrixXd& Su,
                            Eigen::VectorXd& g);
    
    /**
     * @brief Component 3: Solve quadratic programming problem
     * 
     * Minimizes: (1/2) * U^T * H * U + g^T * U
     * Subject to: u_min ≤ U[i] ≤ u_max for all i
     * 
     * Uses projected gradient descent with adaptive step size
     * 
     * @param H Hessian matrix
     * @param g Gradient vector
     * @param U_opt Output optimal control sequence
     * @param max_iter Maximum number of iterations
     * @param tolerance Convergence tolerance
     */
    void solveQP(const Eigen::MatrixXd& H, const Eigen::VectorXd& g,
                Eigen::VectorXd& U_opt, int max_iter = 200, double tolerance = 1e-6);
};

#endif // MPC_H
