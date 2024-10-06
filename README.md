## Model Predictive Control
> [Model Predictive Control Template](basic_template/README.md)

And, there are several ways for combining learning and model predictive control:
> 1. [Learning __inside__ of model predictive control](learning_inside_mpc/README.md):
> 
> 	(e.g. learn a neural network model of robotic dynamics)
> 
> 	- (learning) dynamics
> 	- cost function
> 	- sampling distribution
> 	- how to optimize
> 	- etc.
> 
> 2. Model predictive control __is__ __itself__ a learning algorithm
> 
> 	(tune optimization for better performance, i.e., MPC as a learner for parameter tuning when training)
> 
> 3. Model predictive control __inside__ of a learning algorithm
> 
> 	(e.g. improving MPC with Q-Learning)
> 
> 4. Learning __a__ __policy__ of model predictive controllers
> 
> 	(e.g. learning state-dependent MPC parameters)

