# Madule Snake Python


Made in C++ Qt 5.9.7.

Windows 10.

This environment can be reuse in python like a module.

## Installation

The file deploy.bat install the environment on an custom anaconda environment.

### WARNING : Be sure to install on YOUR environment.

Open deploy.bat and change the 2 firsts lines with our setup :

```
call {PATH_TO_ANACONDA}Scripts\activate.bat

call conda activate {ENVIRONMENT_NAME}
```

### Deploy

```
deploy.bat
```

## Use it like OpenAI Gym

```
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
```
