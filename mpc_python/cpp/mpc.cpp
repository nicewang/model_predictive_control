/*
 * @file            model_predictive_control/mpc_python/cpp/mpc.cpp
 * @description     
 * @author          nicewang <wangxiaonannice@gmail.com>
 * @createTime      2026-03-16
 * @lastModified    2026-03-18
 * Copyright © Xiaonan (Nice) Wang. All rights reserved
*/

#include "mpc.h"
#include "utils/PgdUtils.h"

// ================================================================================
// CONSTRUCTOR: Initialize MPC controller with system parameters
// ================================================================================
MPC::MPC(const Eigen::MatrixXd& A, const Eigen::MatrixXd& B,
         const Eigen::MatrixXd& Q, const Eigen::MatrixXd& R,
         const Eigen::VectorXd& u_min, const Eigen::VectorXd& u_max, int N)
    : A_(A), B_(B), Q_(Q), R_(R), u_min_(u_min), u_max_(u_max), N_(N) {
    
    U_opt_ = Eigen::VectorXd::Zero(N_ * B_.cols());
    
    std::cout << "[MPC] Initialized with:" << std::endl;
    std::cout << "  - States (nx): " << A_.rows() << std::endl;
    std::cout << "  - Inputs (nu): " << B_.cols() << std::endl;
    std::cout << "  - Prediction horizon (N): " << N_ << std::endl;
}

// ================================================================================
// MAIN SOLVE FUNCTION: Implements the MPC control cycle
// ================================================================================
Eigen::VectorXd MPC::solve(const Eigen::VectorXd& x0, const Eigen::VectorXd& x_ref) {
    
    // ========== Component 1: Prediction Model ==========
    // Build expanded matrices that predict all future states given current state and control inputs
    // X = Sx * x0 + Su * U
    // where X = [x(1); x(2); ...; x(N)] is the predicted state sequence, and U = [u(0); u(1); ...; u(N-1)] is the control sequence
    Eigen::MatrixXd Sx, Su;
    buildPredictionMatrices(Sx, Su);
    
    // ========== Component 2: Cost Function & Constraints ==========
    // Build Hessian matrix and gradient vector for quadratic objective function:
    // J = Σ_{i=0}^{N-1} [||x(k+i) - x_ref||²_Q + ||u(k+i)||²_R]
    Eigen::MatrixXd H;
    Eigen::VectorXd g;
    buildHessianMatrix(Su, H);
    buildGradientVector(x0, x_ref, Sx, Su, g);
    
    // ========== Component 3: Receding Horizon Optimization ==========
    // Solve the global quadratic programming problem over the entire prediction horizon
    // min (1/2) * U^T * H * U + g^T * U, subject to u_min ≤ U[i] ≤ u_max for all i
    solveQP(H, g, U_opt_);
    
    // ========== Component 4: Feedback Correction ==========
    // In MPC, only the first control input u(0) is applied to the system
    // At the next time step, the entire optimization problem is re-solved with:
    // - Updated state measurement x(k+1) from the system
    // - Shifted prediction horizon (receding horizon) (sliding window)
    // This creates a closed feedback loop and corrects the modeling error.
    return U_opt_.head(B_.cols());
}

// ================================================================================
// Component 1: PREDICTION MODEL BUILDER
// Constructs matrices relating future states to current state and control inputs
// ================================================================================
void MPC::buildPredictionMatrices(Eigen::MatrixXd& Sx, Eigen::MatrixXd& Su) {
    Sx = Eigen::MatrixXd::Zero(N_ * A_.rows(), A_.rows());
    Su = Eigen::MatrixXd::Zero(N_ * A_.rows(), N_ * B_.cols());
    
    // Compute powers of A: A, A², A³, ..., A^N
    Eigen::MatrixXd A_power = A_;
    
    for (int k = 0; k < N_; ++k) {
        // x(k+1) = A^(k+1) * x0 + A^k * B * u(0) + ... + B * u(k)
        // i.e. x(k+1) = A * x(k) + B * u(k)
        
        // Fill Sx: coefficients of x0
        Sx.block(k * A_.rows(), 0, A_.rows(), A_.rows()) = A_power;
        
        // Fill Su: coefficients of control inputs
        Eigen::MatrixXd A_power_temp = Eigen::MatrixXd::Identity(A_.rows(), A_.rows());
        for (int j = 0; j <= k; ++j) {
            Su.block(k * A_.rows(), j * B_.cols(), A_.rows(), B_.cols()) = A_power_temp * B_;
            if (j < k) {
                A_power_temp = A_ * A_power_temp;
            }
        }
        
        A_power = A_ * A_power;
    }
}

// ================================================================================
// Component 2: COST FUNCTION BUILDER - Hessian Matrix
// ================================================================================
void MPC::buildHessianMatrix(const Eigen::MatrixXd& Su, Eigen::MatrixXd& H) {
    // Build the Q_bar matrix (block diagonal of Q repeated N times)
    Eigen::MatrixXd Q_bar = Eigen::MatrixXd::Zero(N_ * A_.rows(), N_ * A_.rows());
    for (int i = 0; i < N_; ++i) {
        Q_bar.block(i * A_.rows(), i * A_.rows(), A_.rows(), A_.rows()) = Q_;
    }
    
    // Build the R_bar matrix (block diagonal of R repeated N times)
    Eigen::MatrixXd R_bar = Eigen::MatrixXd::Zero(N_ * B_.cols(), N_ * B_.cols());
    for (int i = 0; i < N_; ++i) {
        R_bar.block(i * B_.cols(), i * B_.cols(), B_.cols(), B_.cols()) = R_;
    }
    
    // Hessian: H = 2 * (Su^T * Q_bar * Su + R_bar)
    // The factor of 2 comes from expanding the quadratic cost ||x||²
    H = 2.0 * (Su.transpose() * Q_bar * Su + R_bar);
}

// ================================================================================
// Component 2: COST FUNCTION BUILDER - Gradient Vector
// ================================================================================
void MPC::buildGradientVector(const Eigen::VectorXd& x0, const Eigen::VectorXd& x_ref,
                             const Eigen::MatrixXd& Sx, const Eigen::MatrixXd& Su,
                             Eigen::VectorXd& g) {
    // Build Q_bar matrix
    Eigen::MatrixXd Q_bar = Eigen::MatrixXd::Zero(N_ * A_.rows(), N_ * A_.rows());
    for (int i = 0; i < N_; ++i) {
        Q_bar.block(i * A_.rows(), i * A_.rows(), A_.rows(), A_.rows()) = Q_;
    }
    
    // Construct the reference state vector: X_ref = [x_ref; x_ref; ...; x_ref]
    Eigen::VectorXd X_ref = Eigen::VectorXd::Zero(N_ * A_.rows());
    for (int i = 0; i < N_; ++i) {
        X_ref.segment(i * A_.rows(), A_.rows()) = x_ref;
    }
    
    // Predicted states given initial state and zero input: X = Sx * x0
    Eigen::VectorXd X_free = Sx * x0;
    
    // Gradient: g = 2 * Su^T * Q * (X_free - X_ref)
    // We want to minimize ||X - X_ref||²_Q to let gradient point toward X_ref
    g = 2.0 * Su.transpose() * Q_bar * (X_free - X_ref);
}

// ================================================================================
// Component 3: RECEDING HORIZON OPTIMIZATION - QP Solver
// Solves the global optimization problem using projected gradient descent
// ================================================================================
void MPC::solveQP(const Eigen::MatrixXd& H, const Eigen::VectorXd& g,
                 Eigen::VectorXd& U_opt, int max_iter, double tolerance) {
    
    // Adaptive step size - use Lipschitz constant of gradient
    // For quadratic problem, step_size = 1 / (2 * largest_eigenvalue(H))
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(H);
    double L = es.eigenvalues().maxCoeff();
    double step_size = 0.5 / (L + 1e-6);
    
    Eigen::VectorXd gradient;
    Eigen::VectorXd U_prev;
    
    for (int iter = 0; iter < max_iter; ++iter) {
        U_prev = U_opt;
        
        // Compute gradient: grad = H * U + g
        gradient = H * U_opt + g;
        
        // Gradient descent step with adaptive step size
        U_opt = U_opt - step_size * gradient;
        
        // Project onto box constraints
        // U_opt = projectToBoxConstraints(U_opt);
        U_opt = PgdUtils::projectToBoxConstraints(U_opt, B_, u_min_, u_max_, N_);
        
        // Check convergence
        double error = (U_opt - U_prev).norm();
        if (error < tolerance) {
            break;
        }
    }
}

// ================================================================================
// UTILITY: Box Constraint Projection
// ================================================================================
Eigen::VectorXd MPC::projectToBoxConstraints(const Eigen::VectorXd& u) const {
    Eigen::VectorXd u_proj = u;
    
    // Project each control input to its bounds
    for (int i = 0; i < N_; ++i) {
        for (int j = 0; j < B_.cols(); ++j) {
            int idx = i * B_.cols() + j;
            u_proj(idx) = std::max(u_min_(j), std::min(u_max_(j), u_proj(idx)));
        }
    }
    
    return u_proj;
}
