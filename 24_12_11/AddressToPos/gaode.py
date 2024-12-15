import time
import requests

def get_coordinates_amap(address, api_key):

    time.sleep(0.5)

    """通过高德地图API获取经纬度"""
    url = "https://restapi.amap.com/v3/geocode/geo"
    params = {
        "address": address,  # 地址
        "key": api_key,      # 你的高德API密钥
    }

    try:
        response = requests.get(url, params=params)
        response.raise_for_status()  # 检查请求是否成功
        data = response.json()
        if data["status"] == "1" and data["geocodes"]:
            latitude = data["geocodes"][0]["location"].split(",")[1]
            longitude = data["geocodes"][0]["location"].split(",")[0]
            return float(latitude), float(longitude)
        else:
            return None, None
    except Exception as e:
        print(f"获取经纬度失败: {e}")
        return None, None

def process_addresses(input_file, output_file, api_key):
    """从文件读取地址，批量获取经纬度并保存结果到文件"""
    with open(input_file, 'r', encoding='utf-8') as infile:
        addresses = infile.readlines()  # 读取所有地址

    with open(output_file, 'w', encoding='utf-8') as outfile:
        for address in addresses:
            address = address.strip()  # 去除两端的空格和换行符
            print(f"正在处理地址: {address}")
            lat, lng = get_coordinates_amap(address, api_key)
            if lat and lng:
                result = f"{address} -> 纬度: {lat}, 经度: {lng}\n"
            else:
                result = f"{address} -> 未找到对应的经纬度信息\n"
            outfile.write(result)  # 写入文件

# 测试代码
if __name__ == "__main__":
    input_file = "standardized_address.txt"  # 输入地址文件路径
    output_file = "result.txt"  # 输出结果文件路径
    api_key = ""  # 你的高德API密钥
    process_addresses(input_file, output_file, api_key)
