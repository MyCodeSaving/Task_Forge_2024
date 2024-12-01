import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Scanner;

public class articalSystem {
    // 文章管理界面
    public static void startArticleSystem(User user) {
        ArrayList<article> articles = new ArrayList<>();
        loop: while (true) {
            System.out.println("-------------------文章管理界面--------------------");
            System.out.print("1:发布文章\t");
            System.out.print("2:查看文章列表\t");
            System.out.print("3:编辑文章\t");
            System.out.print("4:删除文章\t");
            System.out.println("5:返回主页面");
            System.out.print("请输入您的选择：");
            Scanner sc = new Scanner(System.in);
            String choose = sc.next();
            switch (choose) {
                case "1" -> addArticle(articles, user);
                case "4" -> deleteArticle(articles);
                case "3" -> updateArticle(articles);
                case "2" -> queryArticle(articles);
                case "5" -> {
                    // 退出主界面，就是结束
                    System.out.println("即将退出");
                    app.start(user);
                    break loop;
                }
                default -> System.out.println("没有这个选项");
            }
        }
    }

    // 创建文章
    public static void addArticle(ArrayList<article> list, User user) {
        // 需要看一下剪辑器是什么编码格式，chcp是936则是gbk编码，则需要转换一下，否则用户输入有中文传入的是乱码
        article a = new article();
        Scanner sc = new Scanner(System.in, "gbk");
        while (true) {
            System.out.println("请输入文章标题：");
            String title = sc.nextLine();
            a.setTitle(title);
            System.out.println("请输入文章内容：");
            String content = sc.nextLine();
            a.setContent(content);
            // System.out.println("请输入文章作者：");
            // String author = sc.nextLine();
            a.setAuthor(user.getName());
            System.out.println("是否输入完毕？(y or n)");
            String s = sc.nextLine();
            if (s.equals("y")) {
                break;
            }
        }
        Map<String, String> articleMap = new LinkedHashMap<>();
        articleMap.put("title", a.getTitle() + "\n\n");
        articleMap.put("content", a.getContent() + "\n\n");
        articleMap.put("author", a.getAuthor() + "\n\n");
        // 以文章名作为文件名
        String fileName = String.format("article/%s.txt", a.getTitle());
        fileUtile.down(fileName, articleMap);

    }

    // 删除文章
    public static void deleteArticle(ArrayList<article> list) {
        Scanner sc = new Scanner(System.in, "GBK");
        String title;
        while (true) {
            System.out.println("请输入文章标题：");
            title = sc.nextLine();
            if (title.equals("")) {
                System.out.println("输入错误，请重新输入");
                continue;
            } else {
                break;
            }

        }
        String path = ".\\article\\" + title + ".txt";
        // 进入文件工具类中，删除文件
        fileUtile.deleteFile(path);
    }

    // 修改文章
    public static void updateArticle(ArrayList<article> list) {
        Scanner sc = new Scanner(System.in, "GBK");
        System.out.println("请输入要修改的文章标题：");
        String title = sc.nextLine();
        // 传入文章路径
        String article_path = ".\\article\\" + title + ".txt";
        String content = null;
        try {
            content = fileUtile.readFileToString(article_path);
            // System.out.println(content);
        } catch (Exception e) {
            System.err.println("写入失败");
        }
        // 提取原文的题目，作者，因为是修改内容，所以内容不需要提取，等用户输入完直接覆盖
        Map<String, String> content_to_Map = new LinkedHashMap();

        content_to_Map.put("title", content.split("content")[0].split(":")[1]);
        content_to_Map.put("author", content.split("author")[1].split(":")[1]);

        System.out.println("请输入新的文章内容：");
        String new_content = sc.nextLine();
        content_to_Map.put("content", new_content);
        fileUtile.down(article_path, content_to_Map);
    }

    public static void queryArticle(ArrayList<article> list) {
        String directoryPath = ".\\article";
        // 遍历该路径下的文件
        List<String> fileNames = fileUtile.listFileNames(directoryPath);
        System.out.println("所有文章为：");
        fileNames.forEach(System.out::println);
    }
}
