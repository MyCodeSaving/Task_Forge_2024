# 此文件使用DBSCN聚类分析，但难以找到合适的超参数来分簇，故废弃

from sklearn.cluster import DBSCAN
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

# 使用 DBSCAN 聚类
def perform_dbscan(X_features, eps=0.5, min_samples=5):
    dbscan = DBSCAN(eps=eps, min_samples=min_samples)
    dbscan.fit(X_features)
    return dbscan

# 可视化聚类结果
def plot_clusters(labels, X_features):
    plt.scatter(X_features[:, 0], X_features[:, 1], c=labels, cmap='viridis', marker='o')
    plt.title('DBSCAN Clustering')
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

# 使用 DBSCAN 聚类
eps = 0.01  # 邻域的大小（可以调整）
min_samples = 5  # 每个簇的最小样本数（可以调整）
dbscan = perform_dbscan(X_features, eps=eps, min_samples=min_samples)

# 聚类结果
labels = dbscan.labels_

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
df.to_csv('image_cluster_relation_dbscan.csv', index=False)
