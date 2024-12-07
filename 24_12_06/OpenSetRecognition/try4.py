# 简单CNN+K聚类，效果太差

import numpy as np
import os
import matplotlib.pyplot as plt
from skimage import color, io, transform
from skimage.feature import hog
from sklearn.cluster import KMeans
from sklearn.decomposition import PCA
from sklearn.metrics import silhouette_score
import pandas as pd


# 卷积层（包含卷积和激活函数）
class ConvLayer:
    def __init__(self, input_channels, output_channels, kernel_size, stride=1, padding=0):
        self.input_channels = input_channels
        self.output_channels = output_channels
        self.kernel_size = kernel_size
        self.stride = stride
        self.padding = padding

        # 初始化卷积核
        self.filters = np.random.randn(output_channels, input_channels, kernel_size, kernel_size) * 0.1

    def forward(self, input):
        self.input = input
        batch_size, input_channels, height, width = input.shape
        kernel_size = self.kernel_size
        stride = self.stride
        padding = self.padding

        # 对输入进行零填充
        if padding > 0:
            input = np.pad(input, ((0,), (0,), (padding,), (padding,)), mode='constant', constant_values=0)

        # 输出尺寸
        output_height = (height - kernel_size + 2 * padding) // stride + 1
        output_width = (width - kernel_size + 2 * padding) // stride + 1

        output = np.zeros((batch_size, self.output_channels, output_height, output_width))

        # 进行卷积操作
        for i in range(output_height):
            for j in range(output_width):
                input_patch = input[:, :, i * stride:i * stride + kernel_size, j * stride:j * stride + kernel_size]
                output[:, :, i, j] = np.tensordot(input_patch, self.filters, axes=((1, 2, 3), (1, 2, 3)))

        return output


# 池化层（最大池化）
class MaxPoolLayer:
    def __init__(self, pool_size=2, stride=2):
        self.pool_size = pool_size
        self.stride = stride

    def forward(self, input):
        self.input = input
        batch_size, channels, height, width = input.shape
        pool_size = self.pool_size
        stride = self.stride

        # 计算输出尺寸
        output_height = (height - pool_size) // stride + 1
        output_width = (width - pool_size) // stride + 1

        output = np.zeros((batch_size, channels, output_height, output_width))

        # 进行最大池化操作
        for i in range(output_height):
            for j in range(output_width):
                input_patch = input[:, :, i * stride:i * stride + pool_size, j * stride:j * stride + pool_size]
                output[:, :, i, j] = np.max(input_patch, axis=(2, 3))

        return output


# 全连接层
class FullyConnectedLayer:
    def __init__(self, input_size, output_size):
        self.weights = np.random.randn(input_size, output_size) * 0.1
        self.bias = np.zeros((1, output_size))

    def forward(self, input):
        self.input = input
        return np.dot(input, self.weights) + self.bias


# 激活函数：ReLU
def relu(x):
    return np.maximum(0, x)

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

        # 将图像扩展成 (1, C, H, W) 的形状，C=1，因为是灰度图像
        img = img[np.newaxis, np.newaxis, :, :]

        # 卷积层、池化层、全连接层的初始化
        conv1 = ConvLayer(input_channels=1, output_channels=16, kernel_size=3)
        pool1 = MaxPoolLayer(pool_size=2)
        fc1 = FullyConnectedLayer(input_size=16 * 31 * 31, output_size=128)  # 计算输入大小（假设图像是 64x64）

        # 前向传播
        x = conv1.forward(img)
        x = relu(x)
        x = pool1.forward(x)
        x = x.flatten()  # 展平为一维数组
        x = fc1.forward(x)

        features.append(x)

    # 将特征矩阵转为二维
    return np.array(features).reshape(len(features), -1)  # 变为二维数据，行数为图像数量，列数为每张图像的特征维度


# 使用 K-means 聚类
def perform_kmeans(X_features, n_clusters=5):
    kmeans = KMeans(n_clusters=n_clusters, random_state=42)
    kmeans.fit(X_features)
    return kmeans


# 可视化聚类结果
def plot_clusters(labels, X_features):
    plt.scatter(X_features[:, 0], X_features[:, 1], c=labels, cmap='viridis', marker='o')
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

# 使用 K-means 聚类
kmeans = perform_kmeans(X_features, n_clusters=5)

# 聚类结果
labels = kmeans.labels_

# 降维到2D
pca = PCA(n_components=2)
X_pca = pca.fit_transform(X_features)

# 可视化
plot_clusters(labels, X_pca)

# 获取图像路径和特征
df = create_cluster_relation_table(dir, X_features, labels)

# 打印出关系表
print(df)

# 保存为 CSV 文件
df.to_csv('image_cluster_relation_cnn.csv', index=False)
