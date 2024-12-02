package pacmangame;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;

public class FrightFruit implements GameObject {
    private int x, y;

    public FrightFruit(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public void draw(Graphics g) {
        BufferedImage imageToDraw = null;
        try {
            imageToDraw = ImageIO.read(getClass().getClassLoader().getResource("images/FrightFruit_m.png"));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        if (imageToDraw != null) {
            g.drawImage(imageToDraw, x, y, 22, 22, null);
        } else {
            // 如果图像未加载成功，使用颜色绘制占位符
            g.setColor(Color.ORANGE);
            g.fillRect(x, y, 22, 22);
        }
    }

    public Rectangle getBounds() {
        return new Rectangle(x, y, 22, 22);
    }
}
