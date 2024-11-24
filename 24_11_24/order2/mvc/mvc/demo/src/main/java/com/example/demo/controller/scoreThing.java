package com.example.demo.controller;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import com.example.demo.entity.score;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;

public class scoreThing {
    //用户积分增加，成功返回1，失败返回0
    public int addCustomerScore(int UserID,int ScoreValue)throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        String sql = "UPDATE `customer` SET UserScore = UserScore + ? WHERE UserID = ?";
        try (PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setInt(1, ScoreValue);
            statement.setInt(2, UserID);
            int rowsUpdated = statement.executeUpdate();
            if(rowsUpdated > 0){
                System.out.println("用户积分添加成功");
                connection.close();
                return 1;
            }
        }catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }

        System.out.println("用户积分添加失败");
        connection.close();
        return 0;
    }

    //用户积分减少，返回1表示用户积分兑换成功，返回0表示用户积分不够兑换
    //会被其他函数调用不用管
    public int decreaseCustomerscore(int UserID, int ScoreValue)throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        String sql = "UPDATE `customer` SET UserScore = UserScore - ? WHERE UserID = ? AND UserScore >= ?";
        try (PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setInt(1, ScoreValue);
            statement.setInt(2, UserID);
            statement.setInt(3,ScoreValue);
            int rowsUpdated = statement.executeUpdate();
            if(rowsUpdated > 0){
                System.out.println("用户积分兑换成功");
                connection.close();
                return 1;
            }
        }catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }

        System.out.println("用户积分不够兑换");
        connection.close();
        return 0;
    }
    //添加积分记录，成功返回1，失败返回0
    //这个增加是增加积分记录
    public int addscore(int PsID, int UserID, int ScoreValue, String RecordTime)throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        String sql = "INSERT INTO score (PsID, UserID, ScoreValue, RecordTime) VALUES (?, ?, ?, ?)";
        try (PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setInt(1, PsID);
            statement.setInt(2, UserID);
            statement.setInt(3, ScoreValue);
            statement.setString(4, RecordTime);

            int rowsAffected = statement.executeUpdate();
            if(rowsAffected > 0){
                System.out.println("积分记录添加成功");
                connection.close();
                return 1;
            }

        }catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
        System.out.println("积分记录添加失败");
        connection.close();
        return 0;
    }

    //使用积分,会在积分表中留下记录，并且用户的积分会相应的减少
    public int usescore(int PsID, int UserID)throws ClassNotFoundException, SQLException {
        petsupplyThing petsupplything = new petsupplyThing();
        int PsNeedScore = petsupplything.decreaseScore(PsID);
        decreaseCustomerscore(UserID,PsNeedScore);

        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        String sql = "INSERT INTO `score` (PsID, UserID, ScoreValue, RecordTime) VALUES (?, ?, ?, ?)";
        PreparedStatement statement = connection.prepareStatement(sql);
        statement.setInt(1, PsID);
        statement.setInt(2, UserID);
        statement.setInt(3, PsNeedScore);
        Date now = new Date();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        String RecordTime = sdf.format(now);
        statement.setString(4, RecordTime);

        int rowsAffected = statement.executeUpdate();
        if(rowsAffected > 0){
            System.out.println("积分兑换成功");
            connection.close();
            return 1;
        }
        System.out.println("积分不够兑换");
        connection.close();
        return 0;
    }

    //用户查询自己拥有的积分数,返回用户拥有的积分数
    public int searchscore(int UserID)throws ClassNotFoundException, SQLException {
        int Score = 0;
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        String sql = "SELECT UserScore FROM customer WHERE UserID = ?";
        try (PreparedStatement preparedStatement = connection.prepareStatement(sql)) {
            preparedStatement.setInt(1, UserID);
            ResultSet rs = preparedStatement.executeQuery();
            if (rs.next()) {
                Score = rs.getInt("UserScore");
            }
        }catch (SQLException e) {
            e.printStackTrace();
        }
        return Score;
    }

    //查询用户的积分兑换记录
    public List<score> searchAll(int UserID) throws SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        List<score> scores = new ArrayList<>();
        String sql = "SELECT * FROM score WHERE UserID=?";
        try (PreparedStatement preparedStatement = connection.prepareStatement(sql)) {
            preparedStatement.setInt(1, UserID);
            ResultSet rs = preparedStatement.executeQuery();
            while (rs.next()) {
                int RecordId = rs.getInt("RecordId");
                int PsID = rs.getInt("PsID");
                int UserIDs = rs.getInt("UserID");
                int ScoreValue = rs.getInt("ScoreValue");
                String RecordTime = rs.getString("RecordTime");

                score Score = new score(RecordId, PsID, UserIDs, ScoreValue, RecordTime);
                scores.add(Score);
            }
        }catch (SQLException e) {
            e.printStackTrace();
        }
        connection.close();
        return scores;

    }

}
