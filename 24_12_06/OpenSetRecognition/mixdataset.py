import os
import shutil
import random


def split_dataset(source_dir, target_dir, split_ratio=1):
    # 创建目标文件夹
    if not os.path.exists(target_dir):
        os.makedirs(target_dir)

    # 遍历源目录下的每个文件夹
    for folder in os.listdir(source_dir):
        folder_path = os.path.join(source_dir, folder)

        if os.path.isdir(folder_path):
            # 获取所有图片文件
            images = [f for f in os.listdir(folder_path) if
                      f.lower().endswith(('.png', '.jpg', '.jpeg', '.bmp', '.gif'))]
            random.shuffle(images)  # 随机打乱图片顺序

            # 计算需要选择的图片数量
            num_to_select = int(len(images) * split_ratio)
            selected_images = images[:num_to_select]

            # 移动选中的图片到目标文件夹
            for image in selected_images:
                shutil.move(os.path.join(folder_path, image), os.path.join(target_dir, image))
            print(f"Moved {num_to_select} images from {folder} to {target_dir}")


# 使用示例
source_directory = r'.\dataset1\train'  # 源目录
target_directory = r'.\mixed_dataset1'  # 目标目录
split_dataset(source_directory, target_directory)
