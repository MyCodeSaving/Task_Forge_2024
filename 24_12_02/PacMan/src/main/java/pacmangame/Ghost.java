package pacmangame;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;
import javax.imageio.ImageIO;

public class Ghost implements GameObject {
    private int x, y;
    private Timer timer;
    private ArrayList<BufferedImage> normalImage;
    private ArrayList<BufferedImage> frightImage;
    private int CurrentFrameIndex;
    private int speed = 2;

    public Ghost(int x, int y) {
        this.x = x;
        this.y = y;
        normalImage = new ArrayList<>(2);
        frightImage = new ArrayList<>(2);

        // 加载图像资源
        try {
            ClassLoader classLoader = getClass().getClassLoader();
            URL normalImageUrl_1 = classLoader.getResource("images/Ghost1.gif");
            URL normalImageUrl_2 = classLoader.getResource("images/Ghost2.gif");
            URL frightImageUrl_1 = classLoader.getResource("images/GhostScared1.gif");
            URL frightImageUrl_2 = classLoader.getResource("images/GhostScared2.gif");

            normalImage.add(0, ImageIO.read(normalImageUrl_1));
            normalImage.add(1, ImageIO.read(normalImageUrl_2));
            frightImage.add(0, ImageIO.read(frightImageUrl_1));
            frightImage.add(1, ImageIO.read(frightImageUrl_2));
        } catch (IOException e) {
            e.printStackTrace();
        }
        // 初始化计时器
        this.timer = new Timer();
        // 每隔 1000 毫秒切换帧
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                CurrentFrameIndex = 1 - CurrentFrameIndex;
            }
        }, 0, 1000);  // 初始延迟 0ms，每 1000ms 切换一次帧
    }

    public void moveTowards(Man pacman, int[][] boardpoints) {
        if (x%22!=0) {
            if (x < pacman.getX()) x += speed;
            else if (x > pacman.getX()) x -= speed;
        } else if (y%22!=0) {
            if (y < pacman.getY()) y += speed;
            else if (y > pacman.getY()) y -= speed;
        } else {
            // 这种情况为Ghost正好在网格内
            // 计算在网格中的坐标
            int px = x/22-1;
            int py = y/22-1;
            if (y < pacman.getY() && y < 22*8) {
                if (boardpoints[py+1][px] == 0){
                    y += speed;
                    return;
                }
            } else if (y > pacman.getY() && y > 22) {
                if (boardpoints[py-1][px] == 0){
                    y -= speed;
                    return;
                }
            }
            if (x < pacman.getX() && x < 22*8) {
                if (boardpoints[py][px+1] == 0){
                    x += speed;
                    return;
                }
            } else if (x > pacman.getX() && x > 22) {
                if (boardpoints[py][px-1] == 0){
                    x -= speed;
                    return;
                }
            }
        }
    }

    public void draw(Graphics g, boolean frightMode) {
        BufferedImage imageToDraw = frightMode ? frightImage.get(CurrentFrameIndex) : normalImage.get(CurrentFrameIndex);
        if (imageToDraw != null) {
            g.drawImage(imageToDraw, x, y, 22, 22, null);
        } else {
            // 如果图像未加载成功，使用颜色绘制占位符
            g.setColor(frightMode ? Color.BLUE : Color.RED);
            g.fillRect(x, y, 22, 22);
        }
    }

    public Rectangle getBounds() {
        return new Rectangle(x, y, 22, 22);
    }

    public int getX() {
        return x;
    }
    public int getY() {
        return y;
    }
}
