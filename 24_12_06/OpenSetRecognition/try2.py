# 此文件用K-means聚类法进行无监督分类，但不适合本场景下细粒度的分类，故废弃

from sklearn.cluster import KMeans
from skimage.feature import hog
from skimage import color, io, transform
import numpy as np
import os
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA
from sklearn.metrics import silhouette_score
import pandas as pd

# 提取图像特征的函数
def extract_features(image_dir, image_size=(64, 64)):
    features = []
    for image_file in os.listdir(image_dir):
        img_path = os.path.join(image_dir, image_file)
        img = io.imread(img_path)

        # 检查图像是否为 RGB 格式，如果是灰度图像则跳过转换
        if len(img.shape) == 3 and img.shape[2] == 3:  # RGB 图像
            img = color.rgb2gray(img)  # 转为灰度图像
        elif len(img.shape) == 2:  # 已经是灰度图像
            pass
        else:
            print(f"Skipping image {img_path} due to unsupported format.")
            continue

        # 缩放图像至指定大小
        img = transform.resize(img, image_size)

        # 提取 HOG 特征（可以根据需要替换为其他特征提取方法）
        feature = hog(img, pixels_per_cell=(16, 16), cells_per_block=(2, 2), visualize=False)
        features.append(feature)

    return np.array(features)

# 使用 K-means 聚类
def perform_kmeans(X_features, n_clusters=5):
    kmeans = KMeans(n_clusters=n_clusters, random_state=42)
    kmeans.fit(X_features)
    return kmeans

# 肘部法则：自动选择最优的簇数
def determine_optimal_k(X_features, max_k=50):
    sse = []  # 存储每个簇数的误差平方和（SSE）
    for k in range(1, max_k + 1):
        kmeans = KMeans(n_clusters=k, random_state=42)
        kmeans.fit(X_features)
        sse.append(kmeans.inertia_)  # inertia_是KMeans模型中的误差平方和

    # 绘制肘部法则图
    plt.plot(range(1, max_k + 1), sse, marker='o')
    plt.title('Elbow Method for Optimal K')
    plt.xlabel('Number of Clusters')
    plt.ylabel('SSE')
    plt.show()

    # 根据肘部法则选择最优的簇数（通常选择曲线弯曲的位置）
    optimal_k = np.argmin(np.diff(sse)) + 2  # 假设弯曲的位置为最小增量的地方
    return optimal_k

# 可视化聚类结果
def plot_clusters(labels, X_features):
    plt.scatter(X_features[:, 0], X_features[:, 1], c=labels, cmap='viridis')
    plt.title('K-means Clustering')
    plt.show()

# 获取图像路径
def get_image_paths(image_dir):
    image_paths = []
    for image_file in os.listdir(image_dir):
        img_path = os.path.join(image_dir, image_file)
        if os.path.isfile(img_path):
            image_paths.append(img_path)
    return image_paths

# 生成类别与特征的关系表
def create_cluster_relation_table(image_dir, X_features, labels):
    # 获取图像路径
    image_paths = get_image_paths(image_dir)

    # 创建一个包含图像路径、类别标签和特征的表格
    data = {
        'Image Path': image_paths,
        'Cluster Label': labels
    }

    # 将特征数据加入表格（可以将特征降维为二维，以便于展示）
    feature_df = pd.DataFrame(X_features)
    data.update(feature_df.to_dict(orient='list'))

    # 将数据转换为 DataFrame
    df = pd.DataFrame(data)

    return df


# 主程序
dir = './mixed_dataset1'
# 提取图像特征
X_features = extract_features(dir)

# 使用肘部法则确定最优簇数
optimal_k = determine_optimal_k(X_features, max_k=50)
print(f"Optimal number of clusters: {optimal_k}")

# 训练 K-means 模型
kmeans = perform_kmeans(X_features, n_clusters=optimal_k)

# 聚类结果
labels = kmeans.labels_

# 降维到2D
pca = PCA(n_components=2)
X_pca = pca.fit_transform(X_features)

# 可视化
plot_clusters(labels, X_pca)

# 计算轮廓系数
score = silhouette_score(X_features, labels)
print(f'Silhouette Score: {score}')

# 获取图像路径和特征
df = create_cluster_relation_table(dir, X_features, labels)

# 打印出关系表
print(df)

# 保存为 CSV 文件
df.to_csv('image_cluster_relation.csv', index=False)
