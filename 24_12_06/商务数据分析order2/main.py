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
# 使用默认的Seaborn设置
sns.set(font="SimHei", style="whitegrid")

# 1. 数据预处理

# 读取CSV数据
data = pd.read_csv('dangdang_data.csv')

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

# 将折扣比例从字符串转化为数值
data['折扣比例'] = data['折扣比例'].str.replace('折', '', regex=False).astype(float) / 10

# 编码分类变量
le = LabelEncoder()
categorical_features = ['排行榜类型']  # 这里只选了一个分类变量 '排行榜类型'

for feature in categorical_features:
    data[feature] = le.fit_transform(data[feature])

# 特征缩放
scaler = StandardScaler()
numeric_features = ['原价', '售价', '评论数', '电子书价格', '折扣比例']
data[numeric_features] = scaler.fit_transform(data[numeric_features])

# 查看预处理后的数据
print("\n预处理后的数据：")
print(data.head())

# 2. 数据分析与挖掘

# 2.1 书籍折扣分析
discount_analysis = data[['书名', '折扣比例']].sort_values(by='折扣比例', ascending=False)
print("\n书籍折扣分析：")
print(discount_analysis)

# 2.2 价格分析
price_analysis = data[['书名', '原价', '售价']].sort_values(by='售价', ascending=True)
print("\n书籍价格分析：")
print(price_analysis)

# 2.3 电子书价格与评论数关系分析
plt.figure(figsize=(10, 6))
sns.scatterplot(x='电子书价格', y='评论数', data=data, hue='折扣比例', palette='viridis')
plt.title('电子书价格与评论数关系分析')
plt.xlabel('电子书价格（标准化）')
plt.ylabel('评论数（标准化）')
plt.legend()
plt.show()

# 2.4 顾客画像分类（聚类分析）
# 使用KMeans进行聚类
X_cluster = data[['原价', '售价', '折扣比例', '评论数']]
kmeans = KMeans(n_clusters=3, n_init=10, random_state=42)
data['Cluster'] = kmeans.fit_predict(X_cluster)

# 评估聚类效果
score = silhouette_score(X_cluster, data['Cluster'])
print("\n聚类轮廓系数：", score)

# 3. 数据可视化

# # 3.1 折扣比例可视化
# plt.figure(figsize=(10,6))
# sns.barplot(x='折扣比例', y='书名', data=discount_analysis, palette="coolwarm")
# plt.title('书籍折扣比例分析')
# plt.xlabel('折扣比例')
# plt.ylabel('书名')
# plt.show()

# 3.2 价格分析可视化
plt.figure(figsize=(10,6))
sns.scatterplot(x='原价', y='售价', data=data, hue='折扣比例', palette='coolwarm')
plt.title('书籍原价与售价关系分析')
plt.xlabel('原价（标准化）')
plt.ylabel('售价（标准化）')
plt.legend()
plt.show()

# 3.3 书籍评论数分布
plt.figure(figsize=(8,5))
sns.histplot(data['评论数'], bins=10, kde=True, color='blue')
plt.title('书籍评论数分布')
plt.xlabel('评论数')
plt.ylabel('数量')
plt.show()

# 3.4 聚类结果可视化
plt.figure(figsize=(10,6))
sns.scatterplot(x='原价', y='售价', hue='Cluster', data=data, palette='Set1')
plt.title('书籍聚类分析')
plt.xlabel('原价（标准化）')
plt.ylabel('售价（标准化）')
plt.legend(title='Cluster')
plt.show()


# 新增子分析

# 新增01
# 热销书籍分析
# 评论数最多的书籍
top_rated_books = data[['书名', '评论数']].sort_values(by='评论数', ascending=False).head(10)
print("\n评论数最高的书籍：")
print(top_rated_books)

# 可视化
plt.figure(figsize=(12, 6))
sns.barplot(x='评论数', y='书名', data=top_rated_books, palette='cool')
plt.title('评论数最多的书籍')
plt.xlabel('评论数（标准化）')
plt.ylabel('书名')
plt.show()

# 新增02
# 不同排行榜类型的价格分布
# 绘制箱线图
plt.figure(figsize=(12, 6))
sns.boxplot(x='排行榜类型', y='售价', data=data, palette='Set3')
plt.title('不同排行榜类型的售价分布')
plt.xlabel('排行榜类型')
plt.ylabel('售价（标准化）')
plt.show()

# 新增03
# 折扣比例与售价关系分析
# 绘制散点图
plt.figure(figsize=(10, 6))
sns.scatterplot(x='折扣比例', y='售价', data=data, palette='viridis')
plt.title('折扣比例与售价关系分析')
plt.xlabel('折扣比例')
plt.ylabel('售价（标准化）')
plt.show()

# 新增04
# 评论数与折扣比例的关系
# 绘制散点图
plt.figure(figsize=(10, 6))
sns.scatterplot(x='折扣比例', y='评论数', data=data, palette='plasma')
plt.title('评论数与折扣比例关系分析')
plt.xlabel('折扣比例')
plt.ylabel('评论数（标准化）')
plt.show()

# 新增05
# 热门价格段的分析
# 绘制价格分布直方图
plt.figure(figsize=(10, 6))
sns.histplot(data['售价'], bins=20, kde=True, color='green', label='售价')
sns.histplot(data['原价'], bins=20, kde=True, color='orange', label='原价')
plt.title('书籍售价和原价分布')
plt.xlabel('价格（标准化）')
plt.ylabel('数量')
plt.legend()
plt.show()

# 新增06
# 聚类结果深入分析
# 聚类结果均值
cluster_means = data.groupby('Cluster')[['原价', '售价', '折扣比例', '评论数']].mean()
print("\n聚类结果特征均值：")
print(cluster_means)

# 可视化
cluster_means.plot(kind='bar', figsize=(10, 6), colormap='Set2')
plt.title('聚类结果特征均值分析')
plt.ylabel('均值（标准化）')
plt.xlabel('Cluster')
plt.legend(title='特征')
plt.show()
