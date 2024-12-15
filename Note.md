### Python plt图表显示中文

```python
import matplotlib.pyplot as plt
import matplotlib

# 设置中文字体
matplotlib.rcParams['font.family'] = 'SimHei'  # 设置为黑体，如果需要其他字体可修改为其他支持中文的字体
matplotlib.rcParams['axes.unicode_minus'] = False  # 解决负号显示问题

# 示例图形，测试中文显示
plt.figure(figsize=(10, 6))
plt.plot([1, 2, 3], [1, 4, 9])
plt.title('测试中文标题')  # 标题包含中文
plt.xlabel('横坐标')  # 横坐标标签包含中文
plt.ylabel('纵坐标')  # 纵坐标标签包含中文
plt.show()
```

