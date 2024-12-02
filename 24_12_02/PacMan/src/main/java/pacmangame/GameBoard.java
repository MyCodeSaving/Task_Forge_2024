package pacmangame;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.ArrayList;

public class GameBoard {
    private ArrayList<BufferedImage> Images;
    private ArrayList<int[][]> BoardPoints;

    public GameBoard () {
        Images = new ArrayList<>(2);
        BoardPoints = new ArrayList<int[][]>(2);
        try {
            Images.add(0, ImageIO.read(getClass().getClassLoader().getResource("images/board0.png")));
            Images.add(1, ImageIO.read(getClass().getClassLoader().getResource("images/board1.png")));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        int[][] boardpoint1 = {
                {0,1,1,1,1,0,1,1},
                {0,0,0,0,0,0,0,0},
                {0,1,0,1,0,1,0,0},
                {0,0,0,0,0,0,0,0},
                {0,1,0,1,1,1,0,0},
                {0,0,0,0,0,0,0,0},
                {0,1,0,1,1,1,0,0},
                {0,0,0,0,0,0,0,0}};
        int[][] boardpoint2 = {
                {0,1,1,1,0,0,1,0},
                {0,1,0,0,0,0,1,0},
                {0,1,0,1,1,0,1,0},
                {0,0,0,0,0,0,1,0},
                {0,1,0,1,1,0,0,0},
                {0,1,0,0,0,0,0,0},
                {0,1,1,0,1,1,0,0},
                {0,0,0,0,0,0,0,0}};
        BoardPoints.add(0, boardpoint1);
        BoardPoints.add(1, boardpoint2);
    }

    public void draw(Graphics g, int level) {
        BufferedImage imageToDraw = Images.get(level);
        g.drawImage(imageToDraw, 0, 0, 220, 220, null);
    }

    public int[][] getBoardPoints(int level) {
        return BoardPoints.get(level);
    }

}
