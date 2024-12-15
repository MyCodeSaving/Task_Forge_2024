from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.action_chains import ActionChains
import time
import os
import requests

# 创建文件夹
os.makedirs("images/cats", exist_ok=True)
os.makedirs("images/dogs", exist_ok=True)

def download_image(url, folder, count):
    try:
        response = requests.get(url, stream=True)
        if response.status_code == 200:
            file_path = os.path.join(folder, f"image_{count}.jpg")
            with open(file_path, 'wb') as file:
                for chunk in response.iter_content(1024):
                    file.write(chunk)
            print(f"下载成功: {file_path}")
        else:
            print(f"无法下载图片，HTTP状态码: {response.status_code}")
    except Exception as e:
        print(f"下载失败: {url}, 错误: {e}")

def scrape_images(query, folder, num_images):
    # 设置 Selenium 驱动（以 Chrome 为例）
    options = webdriver.ChromeOptions()
    options.add_argument("--headless")  # 无界面模式
    options.add_argument("--disable-gpu")
    options.add_argument("--no-sandbox")
    driver = webdriver.Chrome(options=options)

    search_url = f"https://www.google.com/search?q={query}&source=lnms&tbm=isch"
    driver.get(search_url)
    time.sleep(2)  # 等待页面加载

    img_urls = set()
    count = 0

    while count < num_images:
        # 向下滚动加载更多图片
        driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")
        time.sleep(2)

        images = driver.find_elements(By.CSS_SELECTOR, "img")
        for img in images:
            if count >= num_images:
                break
            try:
                src = img.get_attribute("src")
                if src and src.startswith("http") and src not in img_urls:
                    img_urls.add(src)
                    download_image(src, folder, count)
                    count += 1
            except Exception as e:
                print(f"图片处理失败: {e}")

    driver.quit()

# 爬取猫的图片
scrape_images("cat", "images/cats", 100)

# 爬取狗的图片
scrape_images("dog", "images/dogs", 100)

print("图片下载完成！")
