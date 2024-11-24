package com.example.demo.controller;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.SimpleDateFormat;
import java.util.Date;

import com.example.demo.entity.UserType;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;

import static com.example.demo.entity.UserType.s;
public class storeThing {
    //商家注册，注册成功返回1，注册失败返回0;
    public int addStore(int UserID,String UserPassword, String UserName,int UserPhone) throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";

        Connection connection = DriverManager.getConnection(url,user,password);
        Statement statement = connection.createStatement();
        if(connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        java.util.Date now = new Date();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        String RegisterTime = sdf.format(now);
        UserType UserType = s;
        int IsVerified = 1;
        String sql = "INSERT INTO store (" +
                "UserID, " +
                "UserPassword, " +
                "UserName, " +
                "UserPhone, " +
                "RegisterTime, " +
                "UserType, " +
                "IsVerified " +
                ") VALUES (" +
                "" + UserID + ", " +
                "'" + UserPassword + "', " +
                "'" + UserName + "', " +
                "'" + UserPhone + "', " +
                "'" + RegisterTime + "', " +
                "'" + UserType + "', " +
                "" + IsVerified+  ")";

        if(statement.executeUpdate(sql) >= 1){
            System.out.println("用户注册成功");
            connection.close();
            return 1;
        }else {
            System.out.println("用户注册失败");
            connection.close();
            return 0;
        }
    }

    //商家登录,返回0表示密码错误，返回1表示登录成功，返回2表示用户名不存在
    public int loadStore(int userID,String userPassword) throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }
        //登录成功
        String sql = "SELECT COUNT(*) FROM store WHERE UserID = ? AND UserPassword = ?";
        PreparedStatement statement = connection.prepareStatement(sql);
        statement.setInt(1, userID);
        statement.setString(2, userPassword);
        ResultSet resultSet = statement.executeQuery();
        if (resultSet.next()) {
            int count = resultSet.getInt(1);
            if (count > 0) {
                System.out.println("商家登录成功");
                connection.close();
                return 1;
            }
        }
        //密码错误或用户名不存在
        sql = "SELECT COUNT(*) FROM store WHERE UserID = ? ";
        statement = connection.prepareStatement(sql);
        statement.setInt(1, userID);
        resultSet = statement.executeQuery();
        if (resultSet.next()) {
            int count = resultSet.getInt(1);
            if (count > 0) {
                System.out.println("用户密码错误");
                connection.close();
                return 0;
            }
        }
        System.out.println("用户名不存在");
        connection.close();
        return 2;
    }

    // 获取商家信息
    public String getStoreInfo(int userID) throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        
        Connection connection = DriverManager.getConnection(url, user, password);
        String sql = "SELECT * FROM store WHERE UserID = ?";
        PreparedStatement statement = connection.prepareStatement(sql);
        statement.setInt(1, userID);
        ResultSet resultSet = statement.executeQuery();
        
        if (resultSet.next()) {
            StringBuilder storeInfo = new StringBuilder();
            storeInfo.append("UserID: ").append(resultSet.getInt("UserID"))
                     .append(", UserName: ").append(resultSet.getString("UserName"))
                     .append(", UserPhone: ").append(resultSet.getInt("UserPhone"))
                     .append(", RegisterTime: ").append(resultSet.getString("RegisterTime"))
                     .append(", UserType: ").append(resultSet.getString("UserType"));
            connection.close();
            return storeInfo.toString();
        } else {
            connection.close();
            return "Store not found";
        }
    }
    

}


