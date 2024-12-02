package pacmangame;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;
import java.util.Random;
import java.net.URL;
import javax.imageio.ImageIO;

public class GamePanel extends JPanel implements KeyListener, ActionListener {
    public int Level = 0;
    private Timer timer;
    private Man pacman;
    private ArrayList<Ghost> ghosts;
    private ArrayList<FrightFruit> fruits;
    private boolean frightMode = false;
    private long frightStartTime;
    private GameBoard gameboard;

    public GamePanel() {
        this.setFocusable(true);
        this.addKeyListener(this);
        gameboard = new GameBoard();
        pacman = new Man(176, 176); // 初始化PacMan位置
        ghosts = new ArrayList<>();
        fruits = new ArrayList<>();
        generateGhosts();
        generateFruit();

        timer = new Timer(50, this); // 每50毫秒更新一次
    }

    public void startGame() {
        timer.start();
    }

    private void generateGhosts() {
        int count = 1;
        switch (Level) {
            case 0 -> {
                ghosts.add(new Ghost(22, 22));
            }
            case 1 -> {
                ghosts.add(new Ghost(22, 22));
                ghosts.add(new Ghost(22, 100));
            }
        }
    }

    private void generateFruit() {
        int fx,fy;
        Random random = new Random();
        fx = random.nextInt(6);
        fy = random.nextInt(6);
        while (gameboard.getBoardPoints(Level)[fy][fx]!=0) {
            fx = random.nextInt(6);
            fy = random.nextInt(6);
        }
//        System.out.println(String.format("(fx,fy):(%d,%d)",fx,fy));
        fruits.add(new FrightFruit((fx+1)*22,(fy+1)*22));
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        // 绘制背景
        gameboard.draw(g, Level);

        // 绘制PacMan
        pacman.draw(g);

        // 绘制Ghosts
        for (Ghost ghost : ghosts) {
            ghost.draw(g, frightMode);
        }

        // 绘制FrightFruits
        for (FrightFruit fruit : fruits) {
            fruit.draw(g);
        }

        // 游戏信息
        g.setColor(Color.WHITE);
        g.drawString("PacMan Lives: " + pacman.getLives(), 10, 10);

//        System.out.println(String.format("PacMan:(%d,%d)", pacman.getX(), pacman.getY()));
//        for (Ghost ghost : ghosts) {
//            System.out.println(String.format("Ghost:(%d,%d)", ghost.getX(), ghost.getY()));
//        }
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        // 更新PacMan位置
        pacman.move(gameboard.getBoardPoints(Level));

        // 更新Ghost位置
        for (Ghost ghost : ghosts) {
            ghost.moveTowards(pacman, gameboard.getBoardPoints(Level));
        }

        // 检测碰撞
        checkCollisions();

        // 检查Fright模式时间
        if (frightMode && System.currentTimeMillis() - frightStartTime > 3000) {
            frightMode = false; // 恢复正常模式
        }

        repaint();
    }

    private void checkCollisions() {
        // 检查PacMan与FrightFruit的碰撞
        fruits.removeIf(fruit -> {
            if (pacman.intersects(fruit)) {
                frightMode = true;
                frightStartTime = System.currentTimeMillis();
                return true;
            }
            return false;
        });

        // 检查PacMan与Ghost的碰撞
        ghosts.removeIf(ghost -> {
            if (pacman.intersects(ghost)) {
                if (frightMode) {
                    return true; // 消灭Ghost
                } else {
                    pacman.loseLife();
                    if (pacman.getLives() <= 0) {
                        timer.stop();
                        JOptionPane.showMessageDialog(this, "Game Over!");
                        System.exit(0);
                    }
                }
            }
            return false;
        });

        // 如果所有Ghost被消灭，进入下一关
        if (ghosts.isEmpty()) {
            Level ++;
            if (Level >= 2) {
                Win();
            }
/*            for (Ghost g : ghosts) {
                ghosts.remove(g);
            }*/
            fruits.removeIf(f -> true);
            pacman.reset();
            generateGhosts();
            generateFruit();
        }
        // 如果地图中没有Fruit，则生成一个
        if (fruits.isEmpty()) {
            generateFruit();
        }
    }

    @Override
    public void keyPressed(KeyEvent e) {
        switch (e.getKeyCode()) {
            case KeyEvent.VK_UP -> pacman.setDirection(0, -11);
            case KeyEvent.VK_DOWN -> pacman.setDirection(0, 11);
            case KeyEvent.VK_LEFT -> pacman.setDirection(-11, 0);
            case KeyEvent.VK_RIGHT -> pacman.setDirection(11, 0);
        }
    }

    @Override
    public void keyReleased(KeyEvent e) {
        pacman.setDirection(0, 0); // 停止移动
    }

    @Override
    public void keyTyped(KeyEvent e) {}

    public void Win() {
        timer.stop();
        JOptionPane.showMessageDialog(this, "You Win!");
        System.exit(0);
    }
}
