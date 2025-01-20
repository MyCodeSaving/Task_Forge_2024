from selenium import webdriver
from selenium.webdriver.edge.service import Service
from selenium.webdriver.common.by import By
import time
import requests
from webdriver_manager.microsoft import EdgeChromiumDriverManager


url = "https://image.baidu.com/search/index?tn=baiduimage&word=大连财经学院"
# 使用 WebDriverManager 获取 Edge WebDriver
driver = webdriver.Edge(service=Service(EdgeChromiumDriverManager().install()))

# 启动浏览器，访问目标页面
driver.get(url)

# 等待页面加载
time.sleep(5)  # 可以增加延时等待页面加载完全

# 查找所有图片元素
img_elements = driver.find_elements(By.TAG_NAME, "img")
img_urls = [img.get_attribute("src") for img in img_elements if img.get_attribute("src")]

if img_urls:
    # 选择第几张图片
    img_url = img_urls[30]
    print(f"找到图片: {img_url}")

    # 使用 requests 下载图片
    img_data = requests.get(img_url).content
    with open("img.jpg", "wb") as f:
        f.write(img_data)
    print("图片已保存。")
else:
    print("未找到图片。")

# 关闭浏览器
driver.quit()
