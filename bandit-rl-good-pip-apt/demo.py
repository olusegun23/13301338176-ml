# Convenience functions
def ind_max(x):
  m = max(x)
  return x.index(m)

# Need access to random numbers
import random

# Definitions of bandit arms
from arms.bernoulli import *
from arms.normal import *
# Definitions of bandit algorithms
from algorithms.epsilon_greedy.standard import *
from algorithms.softmax.standard import *

arm1 = BernoulliArm(0.7)
arm1.draw()
arm1.draw()


arm2 = NormalArm(10.0, 1.0)
arm2.draw()
arm2.draw()



arm3 = BernoulliArm(0.2)
arm3.draw()
arm3.draw()

arms = [arm1,arm2,arm3]

n_arms = len(arms)

algo1 = EpsilonGreedy(0.1, [], [])
algo2 = Softmax(1.0, [], [])

algos = [algo1, algo2]

for algo in algos:
  algo.initialize(n_arms)

for t in range(1000):
  for algo in algos:
    chosen_arm = algo.select_arm()
    reward = arms[chosen_arm].draw()
    algo.update(chosen_arm, reward)

print algo1.counts
print algo1.values

print algo2.counts
print algo2.values

