import requests

# 目标网址
url = "https://news.baidu.com/"

# 获取网页源代码
response = requests.get(url)

# 检查请求是否成功
if response.status_code == 200:
    # 输出网页源代码
    with open("baidu_news.html", "w", encoding="utf-8") as f:
        f.write(response.text)
    print("百度新闻网页源代码已保存。")
else:
    print(f"请求失败，状态码：{response.status_code}")
