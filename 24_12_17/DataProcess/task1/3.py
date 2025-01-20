from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By
from webdriver_manager.microsoft import EdgeChromiumDriverManager  # 正确导入
from selenium.webdriver.edge.service import Service
from bs4 import BeautifulSoup
import time
import pandas as pd

# 使用 WebDriverManager 获取 Edge WebDriver
driver = webdriver.Edge(service=Service(EdgeChromiumDriverManager().install()))

# 访问新浪微博首页
url = "https://weibo.com/"
driver.get(url)

# 等待页面加载
time.sleep(5)


# 定义爬取函数
def scrape_weibo():
    data = []

    for _ in range(10):  # 循环10次，滚动10次
        # 获取页面源代码
        soup = BeautifulSoup(driver.page_source, "html.parser")

        # 查找所有微博条目
        weibo_items = soup.find_all("div", class_="wbpro-scroller-item")  # 需要根据页面实际结构进行调整

        for item in weibo_items:
            try:
                # 用户名
                user_name = item.find("a", class_="ALink_default_2ibt1 head_cut_2Zcft head_name_24eEB").get_text(strip=True)

                # 发布时间
                pub_time = item.find("div", class_="woo-box-flex woo-box-alignCenter woo-box-justifyCenter head-info_info_2AspQ").get_text(strip=True)

                # 发布内容
                content = item.find("div", class_="detail_wbtext_4CRf9").get_text(strip=True)

                data.append({
                    "用户名": user_name,
                    "发布时间": pub_time,
                    "发布内容": content
                })
            except Exception as e:
                print(f"提取数据时出错：{e}")
                continue

        # 模拟滚动
        body = driver.find_element(By.TAG_NAME, "body")
        body.send_keys(Keys.PAGE_DOWN)  # 向下滚动
        time.sleep(2)  # 每次滚动后暂停2秒，以确保加载完新内容

    # 返回爬取的数据
    return data


# 主函数
def main():
    # 爬取数据
    weibo_data = scrape_weibo()

    # 保存数据到 CSV 文件
    if weibo_data:
        df = pd.DataFrame(weibo_data)
        df.to_csv("weibo_data.csv", index=False, encoding="utf-8-sig")
        print("数据已保存到 weibo_data.csv")
    else:
        print("未获取到任何数据！")

    # 关闭浏览器
    driver.quit()


if __name__ == "__main__":
    main()
