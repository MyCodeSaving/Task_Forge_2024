import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import silhouette_score

# 加载数据，读取客户流失数据集
data = pd.read_csv('Customer Churn Dataset.csv')

# 数据预处理
# 将'CreditScore'（信用评分）分为低、中、高三个组，并创建新列'CreditScoreGroup'表示信用评分组
data['CreditScoreGroup'] = pd.cut(data['CreditScore'], bins=[300, 600, 800, 900], labels=['低', '中', '高'])

# 将'Age'（年龄）分组，创建'AgeGroup'列，表示不同的年龄段
data['AgeGroup'] = pd.cut(data['Age'], bins=[18, 30, 40, 50, 60, 100], labels=['18-30', '31-40', '41-50', '51-60', '60+'])

# 将'EstimatedSalary'（估计薪资）分组，创建'SalaryGroup'列，表示不同薪资区间
data['SalaryGroup'] = pd.cut(data['EstimatedSalary'], bins=[0, 50000, 100000, 200000], labels=['低薪', '中薪', '高薪'])

# 子分析与可视化

# 1. 按国家的流失率
# 按'Geography'（国家）分组，计算每个国家的流失率（'Exited'列的均值），并按流失率从高到低排序
country_loss_rate = data.groupby('Geography')['Exited'].mean().sort_values(ascending=False)

# 绘制条形图显示按国家的流失率
country_loss_rate.plot(kind='bar', color='skyblue', title='按国家的流失率')
plt.ylabel('流失率')  # 设置y轴标签
plt.show()  # 显示图表

# 2. 活跃与非活跃成员的流失率比较
# 计算活跃成员（'IsActiveMember' == 1）和非活跃成员（'IsActiveMember' == 0）的流失率
active_loss_rate = data[data['IsActiveMember'] == 1]['Exited'].mean()
inactive_loss_rate = data[data['IsActiveMember'] == 0]['Exited'].mean()

# 使用seaborn绘制柱状图，比较活跃与非活跃成员的流失率
sns.barplot(x=['活跃成员', '非活跃成员'], y=[active_loss_rate, inactive_loss_rate], palette='viridis')
plt.title('流失率：活跃与非活跃成员比较')  # 设置标题
plt.ylabel('流失率')  # 设置y轴标签
plt.show()  # 显示图表

# 3. 按信用评分组的流失率
# 按'CreditScoreGroup'（信用评分组）分组，计算每个组的流失率
credit_score_loss_rate = data.groupby('CreditScoreGroup')['Exited'].mean()

# 绘制条形图显示按信用评分组的流失率
credit_score_loss_rate.plot(kind='bar', color='teal', title='按信用评分组的流失率')
plt.ylabel('流失率')  # 设置y轴标签
plt.show()  # 显示图表

# 4. 零余额与非零余额的流失率
# 计算余额为零（'Balance' == 0）和非零余额（'Balance' > 0）客户的流失率
zero_balance_loss_rate = data[data['Balance'] == 0]['Exited'].mean()
non_zero_balance_loss_rate = data[data['Balance'] > 0]['Exited'].mean()

# 使用seaborn绘制柱状图，比较零余额与非零余额客户的流失率
sns.barplot(x=['零余额', '非零余额'], y=[zero_balance_loss_rate, non_zero_balance_loss_rate], palette='coolwarm')
plt.title('流失率：余额比较')  # 设置标题
plt.ylabel('流失率')  # 设置y轴标签
plt.show()  # 显示图表

# 5. 按产品数量的流失率
# 按'NumOfProducts'（产品数量）分组，计算每个组的流失率
products_loss_rate = data.groupby('NumOfProducts')['Exited'].mean()

# 绘制条形图显示按产品数量的流失率
products_loss_rate.plot(kind='bar', color='coral', title='按产品数量的流失率')
plt.ylabel('流失率')  # 设置y轴标签
plt.show()  # 显示图表

# 6. 按年龄组的流失率
# 按'AgeGroup'（年龄组）分组，计算每个组的流失率
age_group_loss_rate = data.groupby('AgeGroup')['Exited'].mean()

# 绘制条形图显示按年龄组的流失率
age_group_loss_rate.plot(kind='bar', color='purple', title='按年龄组的流失率')
plt.ylabel('流失率')  # 设置y轴标签
plt.show()  # 显示图表

# 7. 按性别的流失率
# 按'Gender'（性别）分组，计算每个性别的流失率
gender_loss_rate = data.groupby('Gender')['Exited'].mean()

# 绘制条形图显示按性别的流失率
gender_loss_rate.plot(kind='bar', color='gold', title='按性别的流失率')
plt.ylabel('流失率')  # 设置y轴标签
plt.show()  # 显示图表

# 8. 按薪资组的流失率
# 按'SalaryGroup'（薪资组）分组，计算每个薪资组的流失率
salary_loss_rate = data.groupby('SalaryGroup')['Exited'].mean()

# 绘制条形图显示按薪资组的流失率
salary_loss_rate.plot(kind='bar', color='green', title='按薪资组的流失率')
plt.ylabel('流失率')  # 设置y轴标签
plt.show()  # 显示图表

# 聚类分析
# 定义用于聚类分析的特征列
features = ['CreditScore', 'Age', 'Balance', 'NumOfProducts', 'EstimatedSalary']

# 标准化数据（将每个特征缩放到相同的尺度）
scaler = StandardScaler()
data_scaled = scaler.fit_transform(data[features])

# 使用KMeans算法进行聚类分析，设置聚类数为3
kmeans = KMeans(n_clusters=3, random_state=42, n_init=10)

# 将聚类结果分配给数据集中的新列'Cluster'
data['Cluster'] = kmeans.fit_predict(data_scaled)

# 聚类结果可视化
# 绘制年龄与余额的散点图，根据聚类结果的不同类别着色
plt.figure(figsize=(10, 6))
sns.scatterplot(data=data, x='Age', y='Balance', hue='Cluster', palette='Set1')
plt.title('客户聚类分析')  # 设置标题
plt.xlabel('年龄')  # 设置x轴标签
plt.ylabel('余额')  # 设置y轴标签
plt.legend(title='聚类')  # 设置图例标题
plt.show()  # 显示图表

# 评估聚类效果
# 计算并输出轮廓系数（Silhouette Score），该值用于评估聚类结果的质量
silhouette_avg = silhouette_score(data_scaled, data['Cluster'])
print("\n轮廓系数:", silhouette_avg)
