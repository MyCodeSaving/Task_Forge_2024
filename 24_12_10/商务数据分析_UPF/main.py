import pandas as pd  # 导入用于数据操作和分析的pandas库
import numpy as np  # 导入用于数值计算的numpy库
import matplotlib  # 导入用于绘图的matplotlib库
import matplotlib.pyplot as plt  # 导入matplotlib的pyplot模块，用于生成图形
import seaborn as sns  # 导入seaborn库，用于生成高级统计图形
from sklearn.cluster import KMeans  # 导入KMeans算法，用于聚类分析
from sklearn.preprocessing import StandardScaler  # 导入StandardScaler模块，用于数据标准化

# 设置中文字体
matplotlib.rcParams['font.family'] = 'SimHei'  # 设置字体为黑体，支持中文显示
matplotlib.rcParams['axes.unicode_minus'] = False  # 避免负号显示问题

# 加载数据
data = pd.read_csv('user_personalized_features.csv')  # 从CSV文件中读取用户个性化特征数据

# 数据预处理
# 删除空值，避免后续分析出错
data = data.dropna()  # 删除数据中的空值行
# 去除重复值，避免重复数据对结果产生影响
data = data.drop_duplicates()  # 删除重复的行

# 确保数据类型正确，以便后续分析
columns_to_convert = {  # 定义需要转换的数据列及其目标类型
    'Age': int,
    'Income': int,
    'Last_Login_Days_Ago': int,
    'Purchase_Frequency': int,
    'Average_Order_Value': float,
    'Total_Spending': float,
    'Time_Spent_on_Site_Minutes': int,
    'Pages_Viewed': int
}
for col, dtype in columns_to_convert.items():  # 遍历每一列并进行类型转换
    data[col] = data[col].astype(dtype)  # 将列转换为指定类型

# 添加描述性统计信息以了解数据分布
data_summary = data.describe()  # 生成数据的统计信息概览
print("数据概览:")  # 打印标题
print(data_summary)  # 输出统计信息

# ------------------------------
# 1. 购买行为分析：Total_Spending与其他变量的关系
# 数据处理：生成消费分段，便于观察不同消费群体的行为
bins = [0, 100, 500, 1000, 5000, np.inf]  # 定义消费分段区间
labels = ['非常低', '低', '中', '高', '非常高']  # 为每个分段定义标签
data['Spending_Category'] = pd.cut(data['Total_Spending'], bins=bins, labels=labels)  # 将总消费按区间分段

# 散点图前进行分组统计，获取消费类别的中心趋势
spending_summary = data.groupby('Spending_Category')[['Age', 'Total_Spending']].mean().reset_index()  # 按消费类别分组计算平均值
print("消费类别中心趋势:")  # 打印标题
print(spending_summary)  # 输出消费类别的统计信息

# 可视化消费类别与年龄的关系
plt.figure(figsize=(10, 6))  # 设置图形大小
sns.scatterplot(data=spending_summary, x='Age', y='Total_Spending', hue='Spending_Category', palette='cool')  # 绘制散点图
plt.title('消费类别与年龄的关系')  # 设置图形标题
plt.show()  # 显示图形

# 消费类别与年龄的关系 - 改为箱线图
plt.figure(figsize=(10, 6))
sns.boxplot(data=data, x='Spending_Category', y='Age', palette='cool')
plt.title('消费类别与年龄的关系（箱线图）')
plt.xlabel('消费类别')
plt.ylabel('年龄')
plt.show()

# ------------------------------
# 2. 用户分群：基于收入和消费进行用户分群
# 数据处理：对Income和Total_Spending标准化
scaler = StandardScaler()  # 初始化标准化工具
data_scaled = scaler.fit_transform(data[['Income', 'Total_Spending']])  # 对收入和总消费进行标准化

# 使用KMeans进行聚类，分为3组
kmeans = KMeans(n_clusters=3, random_state=42)  # 初始化KMeans聚类模型，设置3个簇
data['Cluster'] = kmeans.fit_predict(data_scaled)  # 预测聚类结果并添加到数据中

# 统计每个簇的中心点
cluster_centers = kmeans.cluster_centers_  # 获取聚类中心
print("聚类中心:")  # 打印标题
print(cluster_centers)  # 输出聚类中心

# 可视化聚类结果
plt.figure(figsize=(10, 6))  # 设置图形大小
sns.scatterplot(data=data, x='Income', y='Total_Spending', hue='Cluster', palette='viridis')  # 绘制聚类结果散点图
plt.title('基于收入和消费的用户分群')  # 设置图形标题
plt.show()  # 显示图形

# ------------------------------
# 3. 用户活跃度分析：Last_Login_Days_Ago与Time_Spent_on_Site_Minutes的关系
# 数据处理：对登录天数分段，以更直观地观察行为模式
bins = [0, 7, 30, np.inf]  # 定义登录天数分段区间
labels = ['1周内', '1个月内', '1个月以上']  # 定义分段标签
data['Login_Category'] = pd.cut(data['Last_Login_Days_Ago'], bins=bins, labels=labels)  # 按区间分段登录天数

# 统计每个登录类别的平均浏览时间
login_summary = data.groupby('Login_Category')['Time_Spent_on_Site_Minutes'].mean().reset_index()  # 按登录类别分组计算平均浏览时间
print("登录类别活跃度:")  # 打印标题
print(login_summary)  # 输出登录类别的统计信息

# 可视化登录类别与活跃时间的关系
plt.figure(figsize=(10, 6))  # 设置图形大小
sns.barplot(data=login_summary, x='Login_Category', y='Time_Spent_on_Site_Minutes', palette='Blues')  # 绘制条形图
plt.title('登录类别与活跃时间的关系')  # 设置图形标题
plt.show()  # 显示图形

# 登录类别与活跃时间的关系 - 改为箱线图
plt.figure(figsize=(10, 6))
sns.boxplot(data=data, x='Login_Category', y='Time_Spent_on_Site_Minutes', palette='Blues')
plt.title('登录类别与活跃时间的关系（箱线图）')
plt.xlabel('登录类别')
plt.ylabel('活跃时间（分钟）')
plt.show()

# ------------------------------
# 4. 个性化推荐预测：基于兴趣分类进行购买频率分析
# 数据处理：对兴趣分组并统计平均购买频率
interest_purchase = data.groupby('Interests')['Purchase_Frequency'].mean()  # 按兴趣分组计算平均购买频率
interest_purchase = interest_purchase.sort_values(ascending=False)  # 按频率降序排序

# 可视化兴趣与购买频率的关系
interest_purchase.plot(kind='bar', figsize=(10, 6))  # 绘制条形图
plt.title('基于兴趣的平均购买频率')  # 设置图形标题
plt.ylabel('平均购买频率')  # 设置y轴标签
plt.show()  # 显示图形

# ------------------------------
# 5. 年龄与购买行为分析：查看不同年龄段的购买频率
# 数据处理：对年龄进行分段处理，便于观察不同年龄组的购买行为
bins = [0, 18, 25, 35, 45, 60, np.inf]  # 定义年龄分段区间
labels = ['18以下', '18-25', '26-35', '36-45', '46-60', '60以上']  # 定义分段标签
data['Age_Group'] = pd.cut(data['Age'], bins=bins, labels=labels)  # 按区间分段年龄

# 统计每个年龄组的购买频率
age_purchase = data.groupby('Age_Group')['Purchase_Frequency'].mean()  # 按年龄组分组计算平均购买频率

# 可视化年龄组与购买频率的关系
age_purchase.plot(kind='bar', figsize=(10, 6))  # 绘制条形图
plt.title('不同年龄组的平均购买频率')  # 设置图形标题
plt.ylabel('平均购买频率')  # 设置y轴标签
plt.show()  # 显示图形

# ------------------------------
# 6. 地区差异分析：不同地区用户的总消费对比
# 数据处理：统计每个地区的平均消费总额
location_spending = data.groupby('Location')['Total_Spending'].mean().sort_values(ascending=False)  # 按地区分组计算平均消费

# 可视化地区与消费的关系
location_spending.plot(kind='bar', figsize=(10, 6))  # 绘制条形图
plt.title('不同地区用户的平均消费')  # 设置图形标题
plt.ylabel('平均消费总额')  # 设置y轴标签
plt.show()  # 显示图形

# ------------------------------
# 7. 用户活跃度分析：Time_Spent_on_Site_Minutes与Pages_Viewed的关系
# 数据处理：对浏览时间分段，便于观察趋势
bins = [0, 5, 15, 30, 60, 120, np.inf]  # 定义浏览时间分段区间
labels = ['5分钟内', '15分钟内', '30分钟内', '1小时内', '2小时内', '2小时以上']  # 定义分段标签
data['Time_Spent_Category'] = pd.cut(data['Time_Spent_on_Site_Minutes'], bins=bins, labels=labels)  # 按区间分段浏览时间

# 统计不同时间段的页面浏览情况
page_view_summary = data.groupby('Time_Spent_Category')['Pages_Viewed'].mean().reset_index()  # 按时间段分组计算平均页面访问数

# 可视化浏览时间与页面访问数的关系
plt.figure(figsize=(10, 6))  # 设置图形大小
sns.barplot(data=page_view_summary, x='Time_Spent_Category', y='Pages_Viewed', palette='cool')  # 绘制条形图
plt.title('浏览时间与页面访问数的关系')  # 设置图形标题
plt.show()  # 显示图形

# ------------------------------
# 8. 营销活动与用户互动：是否订阅新闻通讯与消费的关系
# 数据处理：统计订阅与未订阅用户的平均消费
newsletter_spending = data.groupby('Newsletter_Subscription')['Total_Spending'].mean()  # 按订阅状态分组计算平均消费

# 可视化新闻订阅与消费的关系
newsletter_spending.plot(kind='bar', figsize=(10, 6))  # 绘制条形图
plt.title('新闻订阅与平均消费的关系')  # 设置图形标题
plt.ylabel('平均消费总额')  # 设置y轴标签
plt.show()  # 显示图形