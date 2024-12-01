import java.util.ArrayList;
import java.util.List;
import java.util.UUID;
import java.util.Scanner;

public class app {
    // articalSystem as = new articalSystem();

    // public static void main(String[] args) {
    // // 文章管理
    // // articalSystem.startArticleSystem();

    // // 评论管理
    // commentSystem.startCommentSystem();
    // }
    public static void start(User user) {
        System.out.println("欢迎使用博客系统");
        loop: while (true) {
            System.out.print("1:文章管理\t");
            System.out.println("2:评论管理");
            System.out.println("3:退出");
            System.out.print("请输入您的选择：");
            Scanner sc = new Scanner(System.in, "GBK");
            String choose = sc.next();
            switch (choose) {
                // 文章管理，支持增删改查
                case "1" -> articalSystem.startArticleSystem(user);
                // 评论管理，支持增，查，回复，形成评论链条
                case "2" -> commentSystem.startCommentSystem(user);
                // 评论，然后直接保存进评论列表，然后返回

                case "3" -> {
                    // 退出主界面，就是结束
                    System.out.println("退出成功");
                    break loop;
                }
            }
        }
    }

}
