import time

import requests

def get_coordinates(address):
    # Nominatim API URL
    url = "https://nominatim.openstreetmap.org/search"
    params = {
        "q": address,         # 要查询的地址
        "format": "json",     # 返回格式
        "limit": 1            # 最多返回 1 个结果
    }
    headers = {
        "User-Agent": "AddressToPosApp/1.0 (your_email@example.com)"  # 替换为你的邮箱或应用描述
    }

    try:
        response = requests.get(url, params=params, headers=headers, timeout=10)
        response.raise_for_status()  # 检查请求是否成功
        data = response.json()
        if data:
            latitude = data[0]['lat']
            longitude = data[0]['lon']
            return float(latitude), float(longitude)
        else:
            return None, None
    except Exception as e:
        print(f"获取经纬度失败: {e}")
        return None, None

def read_addresses_from_file(file_path):
    """从文件中读取地址"""
    with open(file_path, 'r', encoding='utf-8') as file:
        addresses = file.readlines()
    # 去除每个地址的多余空格和换行符
    addresses = [address.strip() for address in addresses]
    return addresses

def process_addresses(file_path):
    """处理地址文件，获取每个地址的经纬度"""
    addresses = read_addresses_from_file(file_path)
    for address in addresses:
        time.sleep(2)
        print(f"正在处理地址: {address}")
        lat, lng = get_coordinates(address)
        if lat and lng:
            print(f"地址: {address}\n经纬度: {lat}, {lng}")
        else:
            print(f"地址: {address} 未找到对应的经纬度信息")

# 测试代码
if __name__ == "__main__":
    # 文件路径
    file_path = "standardized_address.txt"  # 请替换为你的地址文件路径
    process_addresses(file_path)
