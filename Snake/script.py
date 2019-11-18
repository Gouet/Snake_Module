import pySnake
import time
import numpy as np

ACTORS = 1
env_wrapper = []

for i in range(ACTORS):
    env1 = pySnake.make()
    env_wrapper.append(env1)

pySnake.render(True)
size = 4

while True:
    print(size)
    pySnake.reduce_wall(size)
    for i, env in enumerate(env_wrapper):
        env.reset()
    
    done = False
    size -= 1
    while not done:
        time.sleep(0.2)
        #value = np.random.randint(4, size=1)
        #print(value)
        for i, env in enumerate(env_wrapper):
            value = np.random.randint(4, size=1)
            obs, reward, done = env.step(value[0])
            print(value)
            obs = np.array(obs)
            print(obs.shape)
            print('reward: ', reward)
            #print(i, reward)
            #obs = np.reshape(obs, (40, 40))
            #plt.show()
            #print(obs.shape)
            #time.sleep(5)
        #obs, reward2, done = env2.step(value[0])
        #print(reward, reward2)
        #print(obs.shape)
        pass