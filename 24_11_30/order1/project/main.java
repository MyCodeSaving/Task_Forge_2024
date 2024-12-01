import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Scanner;

public class main {
    // 定义保存用户的文件
    private static final String USERS_FILE = "users.txt";
    private static Map<String, String> users = new HashMap<>();

    public static void main(String[] args) {
        loadUsers();
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.println("请选择操作：1. 注册 2. 登录 3. 发布评论 4. 退出");
            int choice = scanner.nextInt();
            scanner.nextLine(); // 消耗换行符

            switch (choice) {
                case 1:
                    // 进入注册函数
                    register(scanner);
                    saveUsers();
                    break;
                case 2:
                    // 进入登录函数
                    User user = login(scanner);
                    // 传递登录的用户信息
                    app.start(user);
                    break;
                case 3:
                    // 发表评论函数
                    setComment();
                    continue;
                case 4:
                    // 终止程序
                    System.out.println("退出程序");

                    return;
                default:
                    System.out.println("无效的选择，请重新选择");

            }
        }
    }

    private static ArrayList<comment> comments = new ArrayList<>();
    private static String comments_path = ".\\commnets.txt";

    // 发表评论
    public static void setComment() {
        // 获取文章目录下的文件名
        String path = ".\\article\\";
        List<String> articleName = fileUtile.listFileNames(path);

        System.err.println(articleName);
        System.err.println("请输入评论的文章：");
        Scanner sc = new Scanner(System.in, "GBK");
        String article = sc.nextLine();
        if (!articleName.contains(article)) {
            System.out.println("文章不存在");
            return;
        }

        comment newComment = new comment();
        newComment.setId(fileUtile.commentsSize(comments_path) + 1);
        newComment.setTitle(article);
        try {
            String info = fileUtile.readFileToString(path + article + ".txt");
            String author = info.split("author")[1].replace(":", "").split("\n")[0];
            newComment.setAuthor(author);
            System.err.println("请输入评论内容：");
            String content = sc.nextLine();
            newComment.setContent(content);
            comments.add(newComment);
            // 将评论放在comments.txt中
            fileUtile.downComments(comments_path, comments);
            comments.clear();
        } catch (Exception e) {
            System.err.println("读取文件时发生错误: " + e.getMessage());
        }

    }

    // 读取用户文件里的信息
    private static void loadUsers() {
        try {
            if (Files.exists(Paths.get(USERS_FILE))) {
                Files.lines(Paths.get(USERS_FILE)).forEach(line -> {
                    String[] parts = line.split(",");
                    if (parts.length == 3) {
                        users.put(parts[0], parts[1] + ':' + parts[2]);
                    }
                });
            }
        } catch (IOException e) {
            System.out.println("读取用户文件时发生错误: " + e.getMessage());
        }
    }

    // 将用户信息保存到文件
    private static void saveUsers() {
        try (BufferedWriter writer = Files.newBufferedWriter(Paths.get(USERS_FILE))) {
            for (Map.Entry<String, String> entry : users.entrySet()) {
                String[] parts = entry.getValue().split(":");
                writer.write(entry.getKey() + "," + parts[0] + "," + parts[1]);
                writer.newLine();
            }
        } catch (IOException e) {
            System.out.println("保存用户文件时发生错误: " + e.getMessage());
        }
    }

    // 注册新用户
    private static void register(Scanner scanner) {
        System.out.println("请输入用户名：");
        String username = scanner.nextLine();
        if (users.containsKey(username)) {
            System.out.println("用户名已存在，请重新注册");
            return;
        }
        System.out.println("请输入密码：");
        String password = scanner.nextLine();
        System.out.println("请输入电话：");
        String phone = scanner.nextLine();
        users.put(username, password + ':' + phone);
        saveUsers(); // 注册成功后立即保存用户信息到文件
        System.out.println("注册成功！");
    }

    // 登录
    private static User login(Scanner scanner) {
        System.out.println("请输入用户名：");
        String username = scanner.nextLine();
        System.out.println("请输入密码：");
        String password = scanner.nextLine();
        if (users.containsKey(username)) {
            String[] parts = users.get(username).split(":");
            if (parts[0].equals(password)) {
                // 登录成功，创建user对象，返回user对象，将其传递到app的start函数
                User user = new User();
                user.setName(username);
                user.setPassword(password);
                user.setPhone(parts[1]);
                return user;
            } else {
                System.out.println("密码错误，请重新登录");
            }
        } else {
            System.out.println("用户名不存在，请重新登录");
        }
        return null;
    }
}