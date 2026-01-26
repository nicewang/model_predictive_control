## Model Predictive Control
Basic Model Predictive Control Template
#### MPC Components
- Prediction Model
- Cost Function (Optimization Target) & Constraints
- Receding Horizon Optimization
- Feedback Correction

https://nicewang.github.io/model_predictive_control/

#### Package Structure
```
${project_workspace}/
├── CMakeLists.txt
└── basic_template/
    ├── CMakeLists.txt
    ├── src/
    │   ├── mpc.cpp
    │   └── main.cpp
    └── include/
        └── mpc.h
```

### Appendix: Locally Build & Run

- Manually:
```bash
# Build
mkdir out/ && g++ -std=c++11 -I/usr/local/include/eigen3 -Iinclude src/mpc.cpp src/main.cpp -o out/mpc-demo

# Run
./out/mpc-demo

# Clean
rm -rf out/
```
- Use Make:
```bash
# Build
make

# Run
./bin/mpc

# Clean
make clean

- Use CMakeLists
```bash
# Build
mkdir build && cd build
cmake ..
make

# Run
./bin/mpc

# Or Build and Run
make run

# Clean
cmake --build . --target clean_all
```
