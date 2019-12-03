import pySnake
import time
import numpy as np

ACTORS = 1
env_wrapper = []

for i in range(ACTORS):
    env1 = pySnake.make()
    env_wrapper.append(env1)

pySnake.render(True)

while True:
    for i, env in enumerate(env_wrapper):
        env.reset()
    
    done = False
    while not done:
        time.sleep(0.2)
        for i, env in enumerate(env_wrapper):
            value = np.random.randint(4, size=1)
            obs, reward, done = env.step(value[0])
            print(value)
            obs = np.array(obs)
            print(obs.shape)
            print('reward: ', reward)
        pass
