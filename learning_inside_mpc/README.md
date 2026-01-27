<!--<script src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.7/MathJax.js?config=TeX-AMS_HTML"></script>-->

##  Learning _inside_ of model predictive control
E.g. learn a neural network model of robotic dynamics:
> 	- (learning) dynamics
> 	- cost function
> 	- sampling distribution
> 	- how to optimize
> 	- etc.


| Component | Learning Phase | Inference / Planning Phase (Execution) | **Online Meta-Learning Support** |
| :--- | :--- | :--- | :--- |
| **Dynamics Learning** | **Offline**: Training dynamics prediction model. | **Fixed**: Used as a simulator/world model to rollout trajectories. | **Supported**: Adjusts model weights online to adapt to physical changes (e.g., changing friction). |
| **Cost Function Learning** | **Offline**: Learning user preferences or safety constraints via Inverse RL. | **Fixed**: Acts as a critic to score the sampled trajectories. | **Rare**: Usually stays fixed to ensure consistent task objectives. |
| **Sampling Distribution (Prior)** | **Offline**: Learning a general "motion bank" (unconditioned). | **Fixed**: Provides a broad starting point for sampling. | **N/A**: Usually absorbed into the Posterior or Context Net. |
| **Sampling Distribution (Posterior)** | **Offline**: Training a Conditional Flow. | **Dynamic**: Generates task-specific samples. | **Highly Supported**: Uses online gradients ($dL/dh$) to refine the distribution for the specific instance. |
