import numpy as np
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D  # 确保引入 3D 图功能
import scipy.optimize  # 导入 scipy

# 工具函数
def error_plot(errors, yscale='log'):
    """
    绘制误差曲线。

    参数:
        errors: 一个列表，表示每次迭代的误差或目标函数值。
    """
    plt.figure(figsize=(8, 5))
    plt.plot(errors, marker='o', linestyle='-', color='b')
    plt.title("Error Plot")
    plt.xlabel("Iteration")
    plt.ylabel("Error / Objective Value")
    plt.grid(True)
    plt.yscale(yscale)  # 支持设置y轴比例
    plt.show()


def gss(f, a, b, tol=1e-5):
    """Golden section search

    Find the minimum of f on [a,b]
    f: a strictly unimodal function on [a,b]
    """
    gr = (np.sqrt(5) + 1) / 2
    c = b - (b - a) / gr
    d = a + (b - a) / gr
    while abs(c - d) > tol:
        if f(c) < f(d):
            b = d
        else:
            a = c
        # we recompute both c and d here to avoid loss of precision
        c = b - (b - a) / gr
        d = a + (b - a) / gr
    return (b + a) / 2


def random_search(oracle, init, num_steps, line_search=gss):
    """Implements random search.

    oracle: Function.
    init: Point in domain of oracle.
    num_steps: Number of iterations.
    line_search: Line search method (defaults to golden section.)

    Returns: List of iterates."""
    iterates = [init]
    for _ in range(num_steps):
        d = np.random.normal(0, 100, init.shape)
        d /= np.linalg.norm(d)
        x = iterates[-1]
        eta = line_search(lambda step: oracle(x + step * d), -1, 1)
        iterates.append(x + eta * d)
    return iterates


A = np.random.normal(0, 1, (100, 100))
b = np.random.normal(0, 1, (100,))


def least_squares(x):
    return 0.5 * np.linalg.norm(A.dot(x) - b) ** 2


xs = random_search(least_squares, b, 1000, gss)
error_plot([least_squares(x) for x in xs])


def nelder_mead(oracle, init, num_steps):
    """Wrapper around scipy Nelder-Mead search algorithm."""
    iterates = [init]
    scipy.optimize.minimize(
        oracle,
        init,
        callback=lambda x: iterates.append(x),
        options={'maxiter': num_steps},
        method='Nelder-Mead'
    )
    return iterates


def himmelblau(x):
    """Himmelblau's function."""
    return (x[0] ** 2 + x[1] - 11) ** 2 + (x[0] + x[1] ** 2 - 7) ** 2


# 绘制 3D Himmelblau 函数
points = np.linspace(-5, 5, 1000)
X, Y = np.meshgrid(points, points)
Z = himmelblau([X, Y])

fig = plt.figure(figsize=(15, 15))
ax = fig.add_subplot(111, projection='3d')  # 使用 add_subplot 添加三维轴
ax.plot_surface(X, Y, Z, cmap=plt.cm.coolwarm)
ax.set_xlabel("X-axis")
ax.set_ylabel("Y-axis")
ax.set_zlabel("Z-axis")
plt.show()

# 使用 Nelder-Mead 优化 Himmelblau 函数
xs = nelder_mead(himmelblau, [1, 1], 50)
error_plot([himmelblau(x) for x in xs])

# 使用随机搜索优化 Himmelblau 函数
xs = random_search(himmelblau, np.array([1, 1]), 50)
error_plot([himmelblau(x) for x in xs])


def flappy_bird(state, action):
    """Flappy bird dynamical system.

    state: pair of downward velocity and height
    action: upward acceleration

    Returns new state."""
    v, h = state
    g = 1.  # acceleration due to gravity
    k = 0.01  # drag coefficient
    m = 1  # mass
    return (v + g - (k / m) * v - action, h - v)


def unroll(system, inits, policy, num_steps):
    """Unroll a dynamical system."""
    states = [inits[0]]
    actions = [inits[1]]
    for _ in range(num_steps):
        action = policy(states, actions)
        actions.append(action)
        states.append(system(states[-1], action))
    return states, actions


def example_policy(states, actions):
    """Accelerate upwards every 5 steps."""
    if len(states) % 5 == 0:
        return 7
    else:
        return 0.


flappy_inits = [(0, 10.), 0]
states, actions = unroll(flappy_bird, flappy_inits, example_policy, 10)
heights = [h for (v, h) in states]

plt.figure(figsize=(15, 5))
plt.ylabel('Height')
plt.xlabel('Time')
plt.plot(range(len(heights)), heights, color='blue', linewidth=2)
plt.show()

states, actions = unroll(flappy_bird, flappy_inits, example_policy, 20)
heights = [h for (v, h) in states]
plt.figure(figsize=(15, 5))
plt.ylabel('Height')
plt.xlabel('Time')
plt.plot(range(len(heights)), heights, color='green', linewidth=2)
plt.show()


def flappy_reward(states, actions):
    """Rewards for flappy bird game."""
    reward = 0.0
    for (state, action) in zip(states, actions):
        _, height = state
        if height < 0:
            reward += 10 * height
        reward -= action
    return reward


def flappy_policy(theta, states, actions):
    """Linear map of last two states, ignoring previous actions."""
    if len(states) == 1:
        states = states + states
    return np.dot(theta, np.reshape(np.array(states[-2:]), (4,)))


def flappy_simulator(theta, num_steps):
    """Compute rewards of policy for given number of steps."""
    policy = lambda sts, ats: flappy_policy(theta, sts, ats)
    states, actions = unroll(flappy_bird, flappy_inits, policy, num_steps)
    return flappy_reward(states, actions)


def objective(theta, horizon=20):
    """Flappy bird search objective."""
    return -1.0 * flappy_simulator(theta, horizon)


xs = random_search(objective, np.zeros(4), 10, gss)
policy = lambda states, actions: flappy_policy(xs[-1], states, actions)
states, actions = unroll(flappy_bird, flappy_inits, policy, 20)
heights = [h for (v, h) in states]

plt.figure(figsize=(15, 5))
plt.ylabel('Height')
plt.xlabel('Time')
plt.plot(range(len(heights)), heights, color='red', linewidth=2)
plt.show()
