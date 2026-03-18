/*
 * @file            model_predictive_control/mpc_python/cpp/utils/PgdUtils.cpp
 * @description     
 * @author          nicewang <wangxiaonannice@gmail.com>
 * @createTime      2026-03-18
 * @lastModified    2026-03-18
 * Copyright © Xiaonan (Nice) Wang. All rights reserved
*/

#include "PgdUtils.h"


Eigen::VectorXd PgdUtils::projectToBox(const Eigen::VectorXd& u, 
                                            const Eigen::MatrixXd B,
                                            const Eigen::VectorXd& u_min, 
                                            const Eigen::VectorXd& u_max, 
                                            int N) {
    Eigen::VectorXd u_proj = u;
    int nu = u_min.size();

    // Project each control input to its bounds
    for (int i = 0; i < N; ++i) {
        // u_proj.segment(i * nu, nu) = u_proj.segment(i * nu, nu)
        //                                    .cwiseMax(u_min)
        //                                    .cwiseMin(u_max);
        for (int j = 0; j < B.cols(); ++j) {
            int idx = i * B.cols() + j;
            u_proj(idx) = std::max(u_min(j), std::min(u_max(j), u_proj(idx)));
        }
    }
    
    return u_proj;
}

Eigen::VectorXd PgdUtils::projectToBox(const Eigen::VectorXd& u, 
                                            const Eigen::VectorXd& u_min, 
                                            const Eigen::VectorXd& u_max, 
                                            int N) {
    Eigen::VectorXd u_proj = u;
    int nu = u_min.size();

    // Project each control input to its bounds
    for (int i = 0; i < N; ++i) {
        u_proj.segment(i * nu, nu) = u_proj.segment(i * nu, nu)
                                        .cwiseMax(u_min)
                                        .cwiseMin(u_max);
    }
    
    return u_proj;
}
