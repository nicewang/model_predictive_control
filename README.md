## Model Predictive Control Python Lib
Python Library of Basic Model Predictive Control Template
#### MPC Components
- Prediction Model
- Cost Function (Optimization Target) & Constraints
- Receding Horizon Optimization
- Feedback Correction

https://nicewang.github.io/model_predictive_control/

#### Package Structure
```
${project_workspace}/
├── mpc_python/
│   ├── __init__.py
│   └── cpp/
│       ├── mpc.h          <-- corresponding to main:basic_template/include/mpc.h
│       ├── mpc.cpp        <-- corresponding to main:basic_template/src/mpc.cpp
│       └── bindings.cpp   
├── CMakeLists.txt         
├── pyproject.toml         
└── setup.py              
```

### Install

#### Local Install:
```bash
cd model_predictive_control/
pip install -e .
```

#### Remote Install
```bash
pip install git+https://github.com/nicewang/model_predictive_control.git@basic_template_py
```
