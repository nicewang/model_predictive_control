#include <pybind11/pybind11.h>
#include <pybind11/eigen.h> 
#include "mpc.h"

namespace py = pybind11;

PYBIND11_MODULE(mpc_core, m) {
    m.doc() = "Pybind11 wrapper for basic Model Predictive Control template";

    py::class_<MPC>(m, "MPC")
        .def(py::init<const Eigen::MatrixXd&, const Eigen::MatrixXd&, 
                      const Eigen::MatrixXd&, const Eigen::MatrixXd&,
                      const Eigen::VectorXd&, const Eigen::VectorXd&, int>(),
             py::arg("A"), py::arg("B"), py::arg("Q"), py::arg("R"),
             py::arg("u_min"), py::arg("u_max"), py::arg("N"))
        
        .def("solve", &MPC::solve, 
             py::arg("x0"), py::arg("x_ref"),
             "Solve the MPC optimization problem for the current state.")
        
        .def("get_first_input", &MPC::getFirstInput,
             "Get only the first control input to apply at current time step.");
}