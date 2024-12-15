import numpy as np
import matplotlib.pyplot as plt
from autograd import grad

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

# 梯度下降
def gradient_descent(init, steps, grad, proj=lambda x: x):
    """Projected gradient descent.
    Inputs:
        initial: starting point
        steps: list of scalar step sizes
        grad: function mapping points to gradients
        proj (optional): function mapping points to points
    Returns:
        List of all points computed by projected gradient descent.
    """
    xs = [init]
    for step in steps:
        xs.append(proj(xs[-1] - step * grad(xs[-1])))
    return xs

# 优化二次方程
def quadratic(x):
    return 0.5*x.dot(x)

def quadratic_gradient(x):
    return x

x0 = np.random.normal(0, 1, (1000))  # 随机生成一个初始点
_, x1 = gradient_descent(x0, [1.0], quadratic_gradient)  # 使用步长 1.0 执行梯度下降
print(x1.all() == 0)  # 验证最优解

xs = gradient_descent(x0, [0.1]*50, quadratic_gradient)
error_plot([quadratic(x) for x in xs])

# 约束优化
# U is an orthonormal basis of a random 100-dimensional subspace.
U = np.linalg.qr(np.random.normal(0, 1, (1000, 100)))[0]
b = np.random.normal(0, 1, 1000)

def proj(x):
    """Projection of x onto an affine subspace"""
    return b + U.dot(U.T).dot(x-b)

x0 = np.random.normal(0, 1, (1000))
xs = gradient_descent(x0, [0.1]*50, quadratic_gradient, proj)
# the optimal solution is the projection of the origin
x_opt = proj(0)
error_plot([quadratic(x) for x in xs])
plt.plot(range(len(xs)), [quadratic(x_opt)]*len(xs),
        label=r'$\frac{1}{2}|\!|x_{\mathrm{opt}}|\!|^2$')
plt.legend()
error_plot([np.linalg.norm(x_opt-x)**2 for x in xs])

# 最小二乘
def least_squares(A, b, x):
    """Least squares objective."""
    m, _ = A.shape  # 获取样本数
    return (0.5/m) * np.linalg.norm(A.dot(x)-b)**2

def least_squares_gradient(A, b, x):
    """Gradient of least squares objective at x."""
    m, _ = A.shape  # 获取样本数
    return A.T.dot(A.dot(x)-b)/m

m, n = 1000, 100
A = np.random.normal(0, 1, (m, n))
x_opt = np.random.normal(0, 1, n)
noise = np.random.normal(0, 0.1, m)
b = A.dot(x_opt) + noise
objective = lambda x: least_squares(A, b, x)
gradient = lambda x: least_squares_gradient(A, b, x)

kwargs = {'color': 'blue', 'linewidth': 2}

x0 = np.random.normal(0, 1, n)
xs = gradient_descent(x0, [0.1]*100, gradient)
error_plot([objective(x) for x in xs])
plt.plot(range(len(xs)), [np.linalg.norm(noise)**2]*len(xs),
        label='noise level', color='blue', linewidth=2)
plt.plot(range(len(xs)), [least_squares(A,b,x_opt)]*len(xs),
        label='optimal', color='blue', linewidth=2)
_ = plt.legend()

error_plot([np.linalg.norm(x-x_opt)**2 for x in xs])

m, n = 100, 1000
A = np.random.normal(0, 1, (m, n))
b = np.random.normal(0, 1, m)
# The least norm solution is given by the pseudo-inverse
x_opt = np.linalg.pinv(A).dot(b)
objective = lambda x: least_squares(A, b, x)
gradient = lambda x: least_squares_gradient(A, b, x)
x0 = np.random.normal(0, 1, n)
xs = gradient_descent(x0, [0.1]*100, gradient)
error_plot([objective(x) for x in xs])
plt.plot(range(len(xs)), [least_squares(A,b,x_opt)]*len(xs),
        label='optimal', color='blue', linewidth=2)
_ = plt.legend()

error_plot([np.linalg.norm(x-x_opt)**2 for x in xs], yscale='linear')
plt.plot(range(len(xs)), [np.linalg.norm(x_opt)**2]*len(xs),
         label=r'$|\!|x_{\mathrm{opt}}|\!|^2$', color='blue', linewidth=2)
plt.legend()

# Tikhonov正则化
def least_squares_l2(A, b, x, alpha=0.1):
    return least_squares(A, b, x) + (alpha/2) * x.dot(x)

def least_squares_l2_gradient(A, b, x, alpha=0.1):
    return least_squares_gradient(A, b, x) + alpha * x

m, n = 100, 1000
A = np.random.normal(0, 1, (m, n))
b = A.dot(np.random.normal(0, 1, n))
objective = lambda x: least_squares_l2(A, b, x)
gradient = lambda x: least_squares_l2_gradient(A, b, x)

x_opt = np.linalg.inv(A.T.dot(A) + 0.1*np.eye(1000)).dot(A.T).dot(b)

x0 = np.random.normal(0, 1, n)
xs = gradient_descent(x0, [0.1]*500, gradient)
error_plot([objective(x) for x in xs])
plt.plot(range(len(xs)), [least_squares_l2(A,b,x_opt)]*len(xs),
        label='optimal', color='blue', linewidth=2)
_ = plt.legend()

xs = gradient_descent(x0, [0.1]*500, gradient)
error_plot([np.linalg.norm(x-x_opt)**2 for x in xs])
plt.plot(range(len(xs)), [np.linalg.norm(x_opt)**2]*len(xs),
        label=r'squared norm of $x_{\mathrm{opt}}$', color='blue', linewidth=2)
_ = plt.legend()

# We initialize from 0
x0 = np.zeros(n)
# Note this is the gradient w.r.t. the unregularized objective!
gradient = lambda x: least_squares_gradient(A, b, x)
xs = gradient_descent(x0, [0.1]*50, gradient)
error_plot([np.linalg.norm(x_opt-x)**2 for x in xs], yscale='linear')
plt.plot(range(len(xs)), [np.linalg.norm(x_opt)**2]*len(xs),
         label=r'$|\!|x_{\mathrm{opt}}|\!|^2$', color='blue', linewidth=2)
plt.legend()

# 套索
def lasso(A, b, x, alpha=0.1):
    return least_squares(A, b, x) + alpha * np.linalg.norm(x, 1)

def ell1_subgradient(x):
    """Subgradient of the ell1-norm at x."""
    g = np.ones(x.shape)
    g[x < 0.] = -1.0
    return g

def lasso_subgradient(A, b, x, alpha=0.1):
    """Subgradient of the lasso objective at x"""
    return least_squares_gradient(A, b, x) + alpha*ell1_subgradient(x)

m, n = 100, 1000
A = np.random.normal(0, 1, (m, n))
x_opt = np.zeros(n)
x_opt[:10] = 1.0
b = A.dot(x_opt)
x0 = np.random.normal(0, 1, n)
xs = gradient_descent(x0, [0.1]*500, lambda x: lasso_subgradient(A, b, x))
error_plot([lasso(A, b, x) for x in xs])

plt.figure(figsize=(14,10))
plt.title('Comparison of initial, optimal, and computed point')
idxs = range(50)
plt.plot(idxs, x0[idxs], '--', label='initial', color='blue', linewidth=2)
plt.plot(idxs, x_opt[idxs], 'r-', label='optimal', color='blue', linewidth=2)
plt.plot(idxs, xs[-1][idxs], 'g-', label='final', color='blue', linewidth=2)
plt.xlabel('Coordinate')
plt.ylabel('Value')
plt.legend()

# 支持向量机
def hinge_loss(z):
    return np.maximum(1.-z, np.zeros(z.shape))

def svm_objective(A, y, x, alpha=0.1):
    """SVM objective."""
    m, _ = A.shape
    return np.mean(hinge_loss(np.diag(y).dot(A.dot(x))))+(alpha/2)*x.dot(x)

def svm_subgradient(A, y, x, alpha=0.1):
    """Subgradient of the SVM objective."""
    m, _ = A.shape
    z = np.diag(y).dot(A.dot(x))
    subgrad = -A.T.dot(np.diag(y).dot(hinge_loss(z) > 0).astype(float)*y)/m + alpha*x
    return subgrad

m, n = 1000, 100
A = np.random.normal(0, 1, (m, n))
y = 2*(np.random.rand(m) > 0.5) - 1
x_opt = np.linalg.pinv(A).dot(y)
objective = lambda x: svm_objective(A, y, x)
gradient = lambda x: svm_subgradient(A, y, x)

x0 = np.random.normal(0, 1, n)
xs = gradient_descent(x0, [0.1]*1000, gradient)
error_plot([objective(x) for x in xs])
plt.plot(range(len(xs)), [svm_objective(A, y, x_opt)]*len(xs),
        label='optimal', color='blue', linewidth=2)
_ = plt.legend()

error_plot([np.linalg.norm(x-x_opt)**2 for x in xs])

# X轴线性显示
plt.yscale('linear')
plt.plot(range(len(xs)), [np.linalg.norm(x_opt)**2]*len(xs),
         label=r'$|\!|x_{\mathrm{opt}}|\!|^2$', color='blue', linewidth=2)
plt.legend()
