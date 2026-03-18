/*
 * @file            model_predictive_control/mpc_python/cpp/utils/PgdUtils.h
 * @description     
 * @author          nicewang <wangxiaonannice@gmail.com>
 * @createTime      2026-03-18
 * @lastModified    2026-03-18
 * Copyright © Xiaonan (Nice) Wang. All rights reserved
*/

#ifndef PGDUTILS_H
#define PGDUTILS_H

#include <Eigen/Dense>

/**
 * @class PgdUtils
 * @brief Projected Gradient Descent (PGD) Util Class
 */
class PgdUtils{
public:
    /**
     * @brief Project vector onto box constraints [u_min, u_max]
     * @param u Input vector
     * @param B Control input matrix (nx × nu)
     * @param low Lower bound
     * @param high Upper bound
     * @param N Prediction horizon length
     * @return Clipped vector
     */
    static Eigen::VectorXd projectToBoxConstraints(const Eigen::VectorXd& u, 
                                        const Eigen::MatrixXd& B,
                                        const Eigen::VectorXd& low, 
                                        const Eigen::VectorXd& high, 
                                        int N);

    /**
     * @brief Optimized Project vector onto box constraints [u_min, u_max]
     * @param u Input vector
     * @param low Lower bound
     * @param high Upper bound
     * @param N Prediction horizon length
     * @return Clipped vector
     */
    static Eigen::VectorXd projectToBoxConstraints(const Eigen::VectorXd& u, 
                                        const Eigen::VectorXd& low, 
                                        const Eigen::VectorXd& high, 
                                        int N);
};

#endif // PGDUTILS_H