import pandas as pd
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.cluster import KMeans
from sklearn.preprocessing import LabelEncoder
from mlxtend.frequent_patterns import apriori, association_rules
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error


# 设置中文字体
matplotlib.rcParams['font.family'] = 'SimHei'  # 设置为黑体，如果需要其他字体可修改为其他支持中文的字体
matplotlib.rcParams['axes.unicode_minus'] = False  # 解决负号显示问题

# # 示例图形，测试中文显示
# plt.figure(figsize=(10, 6))
# plt.plot([1, 2, 3], [1, 4, 9])
# plt.title('测试中文标题')  # 标题包含中文
# plt.xlabel('横坐标')  # 横坐标标签包含中文
# plt.ylabel('纵坐标')  # 纵坐标标签包含中文
# plt.show()


# 加载数据
data = pd.read_csv('order.csv')

# 数据预处理

# 1. 删除空值
data.dropna(inplace=True)

# 2. 删除重复值
data.drop_duplicates(inplace=True)

# 3. 格式化订单日期字段
data['Order_Date'] = pd.to_datetime(data['订单日期'])

# 4. 拆分日期字段
data['Order_Year'] = data['Order_Date'].dt.year
data['Order_Month'] = data['Order_Date'].dt.month
data['Order_Day'] = data['Order_Date'].dt.day

# 5. 生成新的字段
# 例如将销售金额按产品计算
data['Profit_Per_Product'] = data['利润'] / data['订单数量']

# 6. 类别编码
le = LabelEncoder()
data['Region_Encoded'] = le.fit_transform(data['销售区域ID'])

# --------------- 八个子分析 ---------------

# 1. 体育用品在各个区域的销售情况
sales_by_category_region = data.groupby(['产品类别', '销售区域ID'])['销售金额'].sum().reset_index()

# 可视化
plt.figure(figsize=(12, 8))
sns.barplot(x='销售金额', y='产品类别', hue='销售区域ID', data=sales_by_category_region)
plt.title('Sales Amount by Product Category and Region')
plt.show()

# 2. 不同区域的销售金额分布
sales_by_region = data.groupby('销售区域ID')['销售金额'].sum().reset_index()

# 可视化
plt.figure(figsize=(10, 6))
sns.barplot(x='销售区域ID', y='销售金额', data=sales_by_region)
plt.title('Total Sales Amount by Region')
plt.show()

# 3. 产品类别与销售金额的关系
sales_by_product_category = data.groupby('产品类别')['销售金额'].sum().reset_index()

# 可视化
plt.figure(figsize=(10, 6))
sns.barplot(x='销售金额', y='产品类别', data=sales_by_product_category)
plt.title('Sales Amount by Product Category')
plt.show()

# 4. 按年份统计销售金额
sales_by_year = data.groupby('Order_Year')['销售金额'].sum().reset_index()

# 可视化
plt.figure(figsize=(10, 6))
sns.lineplot(x='Order_Year', y='销售金额', data=sales_by_year, marker='o')
plt.title('Sales Amount by Year')
plt.show()

# 5. 关联分析：体育用品间的关联关系
# 转换数据格式
basket = data.groupby(['客户ID', '产品名称'])['销售金额'].sum().unstack().reset_index().fillna(0).set_index('客户ID')
basket = basket.applymap(lambda x: 1 if x > 0 else 0)

# 执行Apriori算法
frequent_itemsets = apriori(basket, min_support=0.05, use_colnames=True)

# 生成关联规则
rules = association_rules(frequent_itemsets, metric="lift", min_threshold=1.0)

# 显示关联规则
print("Association Rules:")
print(rules)

# 6. 客户聚类分析
features = data[['销售金额', '产品成本', '利润', '单价']].dropna()

# 使用KMeans进行聚类
kmeans = KMeans(n_clusters=3, random_state=42)
data['Customer_Cluster'] = kmeans.fit_predict(features)

# 可视化聚类结果
plt.figure(figsize=(10, 6))
sns.scatterplot(x='销售金额', y='单价', hue='Customer_Cluster', data=data, palette='viridis')
plt.title('Customer Clustering')
plt.show()

# 7. 预测销售金额：线性回归模型
# 特征选择
features = data[['产品成本', '利润', '单价']]
target = data['销售金额']

# 数据集拆分
X_train, X_test, y_train, y_test = train_test_split(features, target, test_size=0.2, random_state=42)

# 训练线性回归模型
model = LinearRegression()
model.fit(X_train, y_train)

# 预测
y_pred = model.predict(X_test)

# 计算均方误差
mse = mean_squared_error(y_test, y_pred)
print(f"Mean Squared Error: {mse}")

# 8. 销售金额与利润的关系
profit_vs_sales = data[['销售金额', '利润']].dropna()

# 可视化
plt.figure(figsize=(10, 6))
sns.scatterplot(x='销售金额', y='利润', data=profit_vs_sales)
plt.title('Sales Amount vs Profit')
plt.show()

# --------------- 可视化表格 ---------------

# 按产品类别和销售区域生成的销售金额表格
sales_by_category_region_pivot = sales_by_category_region.pivot('产品类别', '销售区域ID', '销售金额')

# 可视化热图
plt.figure(figsize=(12, 8))
sns.heatmap(sales_by_category_region_pivot, annot=True, cmap='YlGnBu')
plt.title('Sales Amount by Product Category and Region')
plt.show()