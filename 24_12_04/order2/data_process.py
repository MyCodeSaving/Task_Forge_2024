# 导入必要的库
import matplotlib
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from matplotlib import rcParams

from sklearn.preprocessing import LabelEncoder, StandardScaler
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score

# 设置字体，解决中文显示问题
matplotlib.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei', 'C:\\Windows\\Fonts\\simsun.ttc']
matplotlib.rcParams['axes.unicode_minus'] = False   # 解决负号显示问题

# 设置绘图风格
sns.set(style="whitegrid")
plt.rcParams['figure.figsize'] = (10, 6)

# 1. 数据预处理

# 读取CSV数据
data = pd.read_csv('mcdonalds.csv')

# 查看数据基本信息
print("数据概览：")
print(data.head())
print("\n数据描述：")
print(data.describe())
print("\n数据类型：")
print(data.dtypes)

# 处理缺失值（假设有缺失值）
# 检查缺失值
print("\n缺失值情况：")
print(data.isnull().sum())

# 填充或删除缺失值（根据具体情况选择）
data = data.dropna()  # 简单删除缺失值

# 处理异常字符
data['Like'] = data['Like'].replace('I love it!+5', '+5')
data["Like"] = data["Like"].replace('I hate it!-5', '-5')
# 将 'Like' 列中的字符串转换为数值（假设它们应该是数值型）
data['Like'] = pd.to_numeric(data['Like'], errors='coerce')

# 编码分类变量
le = LabelEncoder()
categorical_features = ['yummy', 'convenient', 'spicy', 'fattening',
                        'greasy', 'fast', 'cheap', 'tasty',
                        'expensive', 'healthy', 'disgusting', 'Gender', 'VisitFrequency']

for feature in categorical_features:
    data[feature] = le.fit_transform(data[feature])

# 特征缩放
scaler = StandardScaler()
numeric_features = ['Age', 'Like']
data[numeric_features] = scaler.fit_transform(data[numeric_features])

# 查看预处理后的数据
print("\n预处理后的数据：")
print(data.head())

# 2. 数据分析与挖掘

# 2.1 顾客口味偏好分析
taste_features = ['yummy', 'convenient', 'spicy', 'fattening',
                 'greasy', 'fast', 'cheap', 'tasty',
                 'expensive', 'healthy', 'disgusting']
taste_preferences = data[taste_features].mean().sort_values(ascending=False)
print("\n顾客口味偏好：")
print(taste_preferences)

# 2.2 价格敏感度分析
price_features = ['cheap', 'expensive']
price_sensitivity = data[price_features].mean()
print("\n价格敏感度：")
print(price_sensitivity)

# 2.3 消费频率预测
# 假设'VisitFrequency'是目标变量
X = data.drop(['Index', 'VisitFrequency'], axis=1)
y = data['VisitFrequency']

# 划分训练集和测试集
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# 建立线性回归模型
model = LinearRegression()
model.fit(X_train, y_train)

# 预测与评估
y_pred = model.predict(X_test)
print("\n消费频率预测模型系数：")
print(model.coef_)
print("\n模型评分（R^2）：", model.score(X_test, y_test))

# 2.4 顾客画像分类（聚类分析）
# 使用KMeans进行聚类
X_cluster = data[['Age', 'Like', 'VisitFrequency']]
kmeans = KMeans(n_clusters=3, random_state=42)
data['Cluster'] = kmeans.fit_predict(X_cluster)

# 评估聚类效果
score = silhouette_score(X_cluster, data['Cluster'])
print("\n聚类轮廓系数：", score)

# 3. 数据可视化

# 3.1 顾客口味偏好可视化
plt.figure(figsize=(10,6))
sns.barplot(x=taste_preferences.values, y=taste_preferences.index, palette="viridis")
plt.title('Customer Taste Preferences Analysis')
plt.xlabel('Average Rating')
plt.ylabel('Taste Attributes')
plt.show()

# 3.2 价格敏感度可视化
plt.figure(figsize=(6,4))
sns.barplot(x=price_sensitivity.index, y=price_sensitivity.values, palette="coolwarm")
plt.title('Price Sensitivity Analysis')
plt.xlabel('Price Attributes')
plt.ylabel('Average Rating')
plt.show()

# 3.3 消费频率分布
plt.figure(figsize=(8,5))
sns.histplot(data['VisitFrequency'], bins=10, kde=True, color='blue')
plt.title('Visit Frequency Distribution')
plt.xlabel('Visit Frequency')
plt.ylabel('Count')
plt.show()

# 3.4 聚类结果可视化
plt.figure(figsize=(10,6))
sns.scatterplot(x='Age', y='Like', hue='Cluster', data=data, palette='Set1')
plt.title('Customer Segmentation Analysis')
plt.xlabel('Normalized Age')
plt.ylabel('Normalized Like')
plt.legend(title='Cluster')
plt.show()

