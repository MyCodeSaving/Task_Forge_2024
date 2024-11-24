package com.example.demo.controller;

import com.example.demo.entity.petsupply;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class petsupplyThing {
    //商家添加宠物商品信息，返回1表示添加成功，返回0表示添加失败，其中使用的图片为.jpg形式
    public int addPetSupplies(int PsID, String PsName, BufferedImage PsPicture, int PsPrice, String PsIntroduction, String PsType, int PsGetScore, int PsNeedScore, int PsInventory)throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        try {
            final var jpg = ImageIO.write(PsPicture, "jpg", baos);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        byte[] imageBytes = baos.toByteArray();

        String sql = "INSERT INTO petsupply (PsID, PsName, PsPicture, PsPrice, PsIntroduction, PsType, PsGetScore, PsNeedScore, PsInventory) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
        try (PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setInt(1, PsID);
            statement.setString(2, PsName);
            statement.setBytes(3, imageBytes); // 设置BLOB类型的字段
            statement.setInt(4, PsPrice);
            statement.setString(5,PsIntroduction);
            statement.setString(6, PsType);
            statement.setInt(7, PsGetScore);
            statement.setInt(8, PsNeedScore);
            statement.setInt(9, PsInventory);

            int rowsAffected = statement.executeUpdate();
            if(rowsAffected > 0){
                System.out.println("商品信息添加成功");
                connection.close();
                return 1;
            }

        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
        System.out.println("商品信息添加失败");
        connection.close();
        return 0;
    }

    //商家修改宠物商品信息，返回1表示修改成功，返回0表示修改失败
    //PsID是改之前的，其他的都是改之后的，PsID不可以改,修改的信息不包括图片PsPicture，因为修改图片比较麻烦
    public int changePetSupplies(int PsID, String PsName, int PsPrice, String PsIntroduction, String PsType, int PsGetScore, int PsNeedScore, int PsInventory)throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        String sql = "UPDATE petsupply SET PsName=?, PsPrice=?, PsIntroduction=?, PsType=?, PsGetScore=?, PsNeedScore=?, PsInventory=? WHERE PsID=?";
        try (PreparedStatement preparedStatement = connection.prepareStatement(sql)) {

            preparedStatement.setString(1, PsName);
            preparedStatement.setInt(2, PsPrice);
            preparedStatement.setString(3, PsIntroduction);
            preparedStatement.setString(4, PsType);
            preparedStatement.setInt(5, PsGetScore);
            preparedStatement.setInt(6, PsNeedScore);
            preparedStatement.setInt(7, PsInventory);
            preparedStatement.setInt(8, PsID);

            int rowsUpdated = preparedStatement.executeUpdate();
            if(rowsUpdated > 0)
            {
                System.out.println("商品信息修改成功");
                connection.close();
                return 1;
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        System.out.println("商品信息修改失败");
        connection.close();
        return 0;
    }

    //商家下架宠物信息，成功返回1，错误返回0，根据PsID进行下架
    public int cancePetsupplies(int PsID) throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        String sql = "DELETE FROM petsupply WHERE PsID=?";
        try (PreparedStatement preparedStatement = connection.prepareStatement(sql)){
            preparedStatement.setInt(1, PsID);
            int rowsDeleted = preparedStatement.executeUpdate();
            if(rowsDeleted > 0)
            {
                System.out.println("商品下架成功");
                connection.close();
                return 1;
            }
        }catch (SQLException e) {
            e.printStackTrace();
        }
        System.out.println("商品下架失败");
        connection.close();
        return 0;
    }

    //搜索宠物商品信息，分类搜索;
    public List<petsupply> searchPetSuppliesByType(String PsType) throws SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        List<petsupply> petSupplies = new ArrayList<>();
        String sql = "SELECT * FROM petsupply WHERE PsType=?";
        try (PreparedStatement preparedStatement = connection.prepareStatement(sql)){
            preparedStatement.setString(1, PsType);
            ResultSet rs = preparedStatement.executeQuery();
            while (rs.next()) {
                int PsID = rs.getInt("PsID");
                String PsName = rs.getString("PsName");
                int PsPrice = rs.getInt("PsPrice");
                String PsIntroduction = rs.getString("PsIntroduction");
                String type = rs.getString("PsType"); // 尽管我们已经通过WHERE子句限制了类型，但这里还是获取它
                int PsGetScore = rs.getInt("PsGetScore");
                int PsNeedScore = rs.getInt("PsNeedScore");
                int PsInventory = rs.getInt("PsInventory");
                Blob blob = rs.getBlob("PsPicture");
                byte[] imageBytes = blob.getBytes(1, (int) blob.length());
                BufferedImage PsPicture = null;
                try (ByteArrayInputStream bis = new ByteArrayInputStream(imageBytes)) {
                    PsPicture = ImageIO.read(bis);
                } catch (IOException e) {
                    e.printStackTrace();
                }
                petsupply petSupply = new petsupply(PsID, PsName, PsPicture, PsPrice, PsIntroduction, PsType, PsGetScore, PsNeedScore, PsInventory);
                petSupplies.add(petSupply);
            }
        }catch (SQLException e) {
            e.printStackTrace();
        }
        connection.close();
        return petSupplies;
    }

    //返回购买商品时获得的积分
    //这个函数会被其他函数调用，不必去管
    public int GetScore(int PsID) throws ClassNotFoundException, SQLException {
        int PsGetScore = 0;
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }
        String sql = "SELECT PsGetScore FROM petsupply WHERE PsID = ?";
        try (PreparedStatement preparedStatement = connection.prepareStatement(sql)){
            preparedStatement.setInt(1, PsID);
            ResultSet rs = preparedStatement.executeQuery();
            if (rs.next()) {
                PsGetScore = rs.getInt("PsGetScore");
            }
        }catch (SQLException e) {
            e.printStackTrace();
        }
        return PsGetScore;
    }

    //返回兑换商品时消耗的积分
    //这个函数会被其他函数调用，不必去管
    public int decreaseScore(int PsID) throws ClassNotFoundException, SQLException {
        int PsNeedScore = 0;
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }
        String sql = "SELECT PsNeedScore FROM petsupply WHERE PsID = ?";
        try (PreparedStatement preparedStatement = connection.prepareStatement(sql)){
            preparedStatement.setInt(1, PsID);
            ResultSet rs = preparedStatement.executeQuery();
            if (rs.next()) {
                PsNeedScore = rs.getInt("PsNeedScore");
            }
        }catch (SQLException e) {
            e.printStackTrace();
        }
        return PsNeedScore;
    }
}
