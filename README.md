<!--<script src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.7/MathJax.js?config=TeX-AMS_HTML"></script>
Sample:
$$
E = mc^2
$$
-->
## Model Predictive Control
### 1. Basic Template
> [Model Predictive Control Template](basic_template/README.md)

### 2. Machine Learning and Model Predictive Control<sup>[1]</sup>
And, there are several ways for combining learning and model predictive control:
> 1. [Learning __inside__ of model predictive control](learning_inside_mpc/README.md):
> 
> 	(e.g. learn a neural network model of robotic dynamics)
> 
> 	- (learning) dynamics
> 	- cost function
> 	- sampling distribution<sup>[2]</sup>
> 	- how to optimize<sup>[3]</sup>
> 	- etc.<sup>[4]</sup>
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

## Reference
[1] This part is summarized from Prof. Byron Boots' talk at CMU at 2022: [Machine Learning and Model Predictive Control for Adaptive Robotic Systems](https://www.youtube.com/watch?v=1PObAZkeQeY&t=187s)


[2] Some related work:
> 1. Sacks, Jacob, and Byron Boots. "Learning sampling distributions for model predictive control." _Conference_ _on_ _Robot_ _Learning_. PMLR, 2023.
> 2. Power, Thomas, and Dmitry Berenson. "Learning a Generalizable Trajectory Sampling Distribution for Model Predictive Control." _IEEE_ _Transactions_ _on_ _Robotics_ (2024). ([Video Link](https://www.youtube.com/watch?v=AT0FmJlWRSI))

[3] Some related work:
> 1. Sacks, Jacob, and Byron Boots. "Learning to optimize in model predictive control." _2022_ _International_ _Conference_ _on_ _Robotics_ _and_ _Automation_ _(ICRA)_. IEEE, 2022.

[4] Some other ideas for `Learning inside of MPC`:

- Learn __subgoals__ for MPC
	
	> 1. Subgoal Diffuser:
		- Huang, Zixuan, et al. "Subgoal Diffuser: Coarse-to-fine Subgoal Generation to Guide Model Predictive Control for Robot Manipulation." _arXiv_ _preprint_ _arXiv:2403.13085_ (2024).
		
		- [Video Link](https://www.youtube.com/watch?v=M0gmBtzZKwM)

