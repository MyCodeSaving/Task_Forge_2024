import torch
import torch.nn as nn
import torchvision.transforms as transforms
import torchvision.datasets as datasets
import matplotlib.pyplot as plt
import numpy as np
from torch.utils.data import DataLoader

# Step 1: 设置设备 (GPU 优先)
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# Step 2: 加载 MNIST 数据集
transform = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize((0.5,), (0.5,))  # 对数据进行标准化处理
])

# 加载 MNIST 测试数据集
test_dataset = datasets.MNIST(root='./data', train=False, download=True, transform=transform)
test_loader = DataLoader(test_dataset, batch_size=1, shuffle=True)


# Step 3: 加载 LeNet-5 模型（假设你已经训练好模型并保存）
class LeNet5(nn.Module):
    def __init__(self):
        super(LeNet5, self).__init__()
        self.conv1 = nn.Conv2d(1, 6, kernel_size=5, stride=1, padding=2)
        self.conv2 = nn.Conv2d(6, 16, kernel_size=5)
        self.fc1 = nn.Linear(16 * 5 * 5, 120)  # 修改为 16 * 5 * 5
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 10)

    def forward(self, x):
        x = torch.relu(self.conv1(x))
        x = torch.max_pool2d(x, kernel_size=2, stride=2)
        x = torch.relu(self.conv2(x))
        x = torch.max_pool2d(x, kernel_size=2, stride=2)
        x = x.view(x.size(0), -1)
        x = torch.relu(self.fc1(x))
        x = torch.relu(self.fc2(x))
        x = self.fc3(x)
        return x



model = LeNet5().to(device)

# 加载训练好的模型权重
model.load_state_dict(torch.load('lenet5_mnist.pth'))
model.eval()  # 设置为评估模式

# 使用的损失函数
loss_fn = nn.CrossEntropyLoss()


# Step 4: FGSM 对抗攻击函数
def fgsm_attack(image, epsilon, data_grad):
    # 生成扰动方向
    sign_data_grad = data_grad.sign()
    # 生成对抗样本
    perturbed_image = image + epsilon * sign_data_grad
    # 对抗样本像素值范围约束在 [0,1]
    perturbed_image = torch.clamp(perturbed_image, 0, 1)
    return perturbed_image


# Step 5: 攻击流程
def attack_example(model, device, data, target, epsilon):
    data, target = data.to(device), target.to(device)

    # 确保计算图对输入的梯度
    data.requires_grad = True

    # 前向传播
    output = model(data)
    init_pred = output.max(1, keepdim=True)[1]  # 预测标签

    # 如果预测错误，则不攻击
    if init_pred.item() != target.item():
        return None, None, None

    # 计算损失
    loss = loss_fn(output, target)

    # 反向传播计算梯度
    model.zero_grad()
    loss.backward()

    # 提取梯度
    data_grad = data.grad.data

    # 执行 FGSM 攻击
    perturbed_data = fgsm_attack(data, epsilon, data_grad)

    # 再次进行预测
    output = model(perturbed_data)
    final_pred = output.max(1, keepdim=True)[1]  # 对抗样本的预测标签

    return data, perturbed_data, final_pred


# Step 6: 选择 epsilon 并进行测试
epsilon = 0.4  # 扰动强度

# 遍历数据集，找到预测正确的样本
for data, target in test_loader:
    orig_data, perturbed_data, final_pred = attack_example(model, device, data, target, epsilon)
    if orig_data is not None:
        break

# MNIST 类别
mnist_classes = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9']

if orig_data is not None:
    # 显示原始图像和对抗样本
    orig_img = orig_data.squeeze().detach().cpu().numpy()
    perturbed_img = perturbed_data.squeeze().detach().cpu().numpy()

    # 获取语义化标签
    original_label_name = mnist_classes[target.item()]
    adversarial_label_name = mnist_classes[final_pred.item()]

    # Step 7: 展示结果
    fig, (ax1, ax2) = plt.subplots(1, 2)
    ax1.imshow(orig_img, cmap='gray')
    ax1.set_title(f"Original Label: {original_label_name}")
    ax2.imshow(perturbed_img, cmap='gray')
    ax2.set_title(f"Adversarial Label: {adversarial_label_name}")
    plt.show()

    print(f"Original Label: {original_label_name}, Adversarial Label: {adversarial_label_name}")
else:
    print("Initial prediction was incorrect. No attack performed.")
