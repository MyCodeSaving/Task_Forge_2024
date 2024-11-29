import numpy as np
import matplotlib.pyplot as plt

# 数据表
t = np.array([0.10, 0.24, 0.38, 0.52, 0.66, 0.80, 0.94, 1.08, 1.22, 1.36,
              1.50, 1.65, 1.79, 1.93, 3.26, 3.53, 3.80, 4.07, 4.34, 4.61,
              15.0, 25.0, 34.0, 53.0, 62.0])
r = np.array([11.1, 19.9, 25.4, 28.8, 31.9, 34.2, 36.3, 38.9, 41.0, 42.8,
              44.4, 46.0, 46.9, 48.7, 59.0, 61.1, 62.9, 64.3, 65.6, 97.3,
              106.5, 130.0, 145.0, 175.0, 185.0])

# 转化为线性关系
y = 5 * np.log(r)
x = 2 * np.log(t)

# 线性拟合
coefficients = np.polyfit(x, y, 1)  # 一次多项式拟合
slope, intercept = coefficients

# 计算E值
rho = 1.25  # 空气密度
E = np.exp(intercept + np.log(rho))

# 打印结果
print(f"拟合得到的斜率: {slope}")
print(f"拟合得到的截距: {intercept}")
print(f"拟合得到的E值: {E}")

# 可视化
y_fit = slope * x + intercept  # 拟合线
plt.scatter(x, y, color='blue', label='Data Points')
plt.plot(x, y_fit, color='red', label='Fitted Line')
plt.xlabel('2 * ln(t)')
plt.ylabel('5 * ln(r)')
plt.legend()
plt.title('Linear Fitting')
plt.grid()
plt.show()
