package pacmangame;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;

public class Man {
    private int x, y;
    private int dx, dy;
    private int lives;
    private ArrayList<BufferedImage> images_left;
    private ArrayList<BufferedImage> images_up;
    private ArrayList<BufferedImage> images_down;
    private ArrayList<BufferedImage> images_right;
    private Timer timer;
    private int CurrentFrameIndex = 0;
    private int direction;
    private ArrayList<ArrayList> images;

    public Man(int x, int y) {
        this.x = x;
        this.y = y;
        this.lives = 3;
        this.direction = 0;

        images_left = new ArrayList<>(4);
        images_right = new ArrayList<>(4);
        images_up = new ArrayList<>(4);
        images_down = new ArrayList<>(4);
        images = new ArrayList<>(4);

        // 加载图像资源
        try {
            ClassLoader classLoader = getClass().getClassLoader();
            URL normalImageUrl_1 = classLoader.getResource("images/Ghost1.gif");
            URL normalImageUrl_2 = classLoader.getResource("images/Ghost2.gif");
            URL frightImageUrl_1 = classLoader.getResource("images/GhostScared1.gif");
            URL frightImageUrl_2 = classLoader.getResource("images/GhostScared2.gif");

            images_left.add(0, ImageIO.read(classLoader.getResource("images/PacMan1.gif")));
            images_left.add(1, ImageIO.read(classLoader.getResource("images/PacMan2left.gif")));
            images_left.add(2, ImageIO.read(classLoader.getResource("images/PacMan3left.gif")));
            images_left.add(3, ImageIO.read(classLoader.getResource("images/PacMan4left.gif")));

            images_right.add(0, ImageIO.read(classLoader.getResource("images/PacMan1.gif")));
            images_right.add(1, ImageIO.read(classLoader.getResource("images/PacMan2right.gif")));
            images_right.add(2, ImageIO.read(classLoader.getResource("images/PacMan3right.gif")));
            images_right.add(3, ImageIO.read(classLoader.getResource("images/PacMan4right.gif")));

            images_up.add(0, ImageIO.read(classLoader.getResource("images/PacMan1.gif")));
            images_up.add(1, ImageIO.read(classLoader.getResource("images/PacMan2up.gif")));
            images_up.add(2, ImageIO.read(classLoader.getResource("images/PacMan3up.gif")));
            images_up.add(3, ImageIO.read(classLoader.getResource("images/PacMan4up.gif")));

            images_down.add(0, ImageIO.read(classLoader.getResource("images/PacMan1.gif")));
            images_down.add(1, ImageIO.read(classLoader.getResource("images/PacMan2down.gif")));
            images_down.add(2, ImageIO.read(classLoader.getResource("images/PacMan3down.gif")));
            images_down.add(3, ImageIO.read(classLoader.getResource("images/PacMan4down.gif")));

            images.add(0, images_up);
            images.add(1, images_down);
            images.add(2, images_left);
            images.add(3, images_right);
        } catch (IOException e) {
            e.printStackTrace();
        }

        // 初始化计时器
        this.timer = new Timer();
        // 每隔 200 毫秒切换帧
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                CurrentFrameIndex = (++CurrentFrameIndex)%4;
            }
        }, 0, 200);  // 初始延迟 0ms，每 200ms 切换一次

    }

    public void reset() {
        x = 176;
        y = 176;
        direction = 2;
    }
    public void setDirection(int dx, int dy) {
        this.dx = dx;
        this.dy = dy;
    }

    public void move(int[][] boardpoints) {
        // Change direction
        if (dx>0) direction = 3;
        if (dx<0) direction = 2;
        if (dy>0) direction = 1;
        if (dy<0) direction = 0;

        // 计算在网格中的坐标
        int px = x/22-1;
        int py = y/22-1;
        int pxdx = (x+dx)/22-1;
        int pxdx_c = (x+dx+11)/22-1;
        int pydy = (y+dy)/22-1;
        int pydy_c = (y+dy+11)/22-1;

        if (dx>0) {
            if (px==7) {
                if (x+dx<=8*22) {
                    x += dx;
                }
                return;
            }
        } else if (dx<0) {
            if (px==0) {
                if (x+dx>=22) {
                    x += dx;
                }
                return;
            }
        } else if (dy>0) {
            if (py==7) {
                if (y+dy<=8*22) {
                    y += dy;
                }
                return;
            }
        } else if (dy<0) {
            if (py==0) {
                if (y+dy>=22) {
                    y += dy;
                }
                return;
            }
        }
        if (boardpoints[pydy][pxdx]==0 && boardpoints[pydy_c][pxdx_c]==0 && boardpoints[pydy_c][pxdx]==0 && boardpoints[pydy][pxdx_c]==0) {
            x += dx;
            y += dy;
        }
    }

    public void loseLife() {
        lives--;
    }

    public int getX() {
        return this.x;
    }

    public int getY() {
        return this.y;
    }
    public int getLives() {
        return lives;
    }

    public void draw(Graphics g) {
        BufferedImage imageToDraw = (BufferedImage) images.get(direction).get(CurrentFrameIndex);
        if (imageToDraw != null) {
            g.drawImage(imageToDraw, x, y, 22, 22, null);
        } else {
            // 如果图像未加载成功，使用颜色绘制占位符
            g.setColor(Color.YELLOW);
            g.fillRect(x, y, 22, 22);
        }
    }

    public boolean intersects(GameObject obj) {
        return new Rectangle(x, y, 22, 22).intersects(obj.getBounds());
    }
}
