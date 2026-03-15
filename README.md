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
mpc_python_project/
├── mpc_python/
│   ├── __init__.py
│   └── cpp/
│       ├── mpc.h          <-- copy fr. main:basic_template/include/mpc.h
│       ├── mpc.cpp        <-- copy fr. main:basic_template/src/mpc.cpp
│       └── bindings.cpp   
├── CMakeLists.txt         
├── pyproject.toml         
└── setup.py              
```

### Install

- Manually:
```bash

```
