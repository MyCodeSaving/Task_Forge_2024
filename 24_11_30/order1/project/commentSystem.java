// 评论系统

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class commentSystem {
    // 评论系统
    public static void startCommentSystem(User user) {
        ArrayList<comment> comments = new ArrayList<>();
        loop: while (true) {
            System.out.println("-------------------评论界面--------------------");
            System.out.print("1:发布评论(你可以选择是否回复某个评论)\t");
            System.out.println("2:查看文章下所有评论");
            System.out.print("3:查看评论链条");
            System.out.println("4:退到主页面");
            System.out.print("请输入您的选择：");
            Scanner sc = new Scanner(System.in);
            String choose = sc.next();
            switch (choose) {
                case "1" -> addComment(comments, user);
                case "2" -> queryComment(comments, user);
                // 查看评论链条
                case "3" -> replyChain.checkchain(user.getName().toString());
                case "4" -> {
                    // 退出主界面，就是结束
                    System.out.println("退出成功");
                    app.start(user);
                    break loop;
                }
                default -> System.out.println("没有这个选项");
            }
        }
    }

    // 一个包含所有评论的列表
    private static List<comment> comments = new ArrayList<>();

    // 存放评论的txt文件
    private static String comments_path = ".\\commnets.txt";

    // 查看所有评论，-1是没有子评论，有的话是有对应的评论id
    public static void queryComment(ArrayList<comment> list, User user) {
        String filePath = comments_path; // 替换为你的文件路径
        String targetUser = user.getName();

        try {
            List<String> lines = Files.readAllLines(Paths.get(filePath));
            // 过滤该用户的评论
            for (String line : lines) {
                String[] parts = line.split(",", 5); // 分割成最多4个部分
                if (parts.length == 5 && parts[2].equals(targetUser)) {
                    System.out.println("评论ID: " + parts[0]);
                    System.out.println("原文作者: " + parts[1]);
                    System.out.println("内容: " + parts[3]);
                    System.out.println("父评论ID: " + parts[4]);
                    System.out.println("--------------------");
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void addComment(ArrayList<comment> list, User user) {
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

            // 询问是否回复某条评论
            System.err.println("是否回复某条评论？(y/n)");
            String isReply = sc.next();
            if (isReply.equalsIgnoreCase("y")) {
                System.err.println("请输入父评论的ID：");
                int parentId = sc.nextInt();
                newComment.setParentId(parentId);
            } else {
                newComment.setParentId(-1); // -1 表示不是回复
            }

            comments.add(newComment);
            // 将评论放在comments.txt中
            fileUtile.downComments(comments_path, comments);
            comments.clear();
        } catch (Exception e) {
            System.err.println("读取文件时发生错误: " + e.getMessage());
        }
    }
}
