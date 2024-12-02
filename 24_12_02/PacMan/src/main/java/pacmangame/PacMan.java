package pacmangame;

import javax.swing.*;

public class PacMan {
    public static void main(String[] args) {
        JFrame frame = new JFrame("PacMan Game");
        GamePanel panel = new GamePanel();

        frame.add(panel);
        frame.setSize(243, 262);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);

        panel.startGame(); // 启动游戏
    }
}

