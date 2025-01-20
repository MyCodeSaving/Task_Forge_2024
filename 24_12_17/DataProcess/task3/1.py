import requests
import re

# 目标网址（大连市未来7天天气）
url = "https://www.weather.com.cn/weather15d/101070201.shtml"

# 发送请求获取网页内容，并确保使用UTF-8编码
response = requests.get(url)
response.encoding = 'utf-8'  # 明确设置编码为UTF-8
if response.status_code == 200:
    html_content = response.text
else:
    print(f"请求失败，状态码：{response.status_code}")
    exit()

# 使用正则表达式提取天气信息
# 1. 提取日期
dates_pattern = r'<span class="time">(.*?)</span>'
dates = re.findall(dates_pattern, html_content)

# 2. 提取天气状况
weather_pattern = r'<span class="wea">(.*?)</span>'
weather_conditions = re.findall(weather_pattern, html_content)

# 3. 提取温度（白天/夜间温度）
temperature_pattern = r'<span class="tem"><em>(.*?)</em>/(.*?)</span>'
temperatures = re.findall(temperature_pattern, html_content)

# 4. 提取风力
wind_pattern = r'<span class="wind">(.*?)</span>'
winds = re.findall(wind_pattern, html_content)

# 输出提取的数据
for i in range(len(dates)):
    print(f"日期: {dates[i]}")
    print(f"天气情况: {weather_conditions[i]}")
    print(f"白天气温: {temperatures[i][0]} | 夜间气温: {temperatures[i][1]}")
    print(f"风力: {winds[i]}")
    print('-' * 40)
