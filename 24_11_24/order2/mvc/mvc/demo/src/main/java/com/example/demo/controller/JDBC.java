package com.example.demo.controller;

import com.example.demo.entity.Customer;
import com.example.demo.entity.UserSex;
import com.example.demo.entity.UserType;
import com.fasterxml.jackson.databind.deser.std.DateDeserializers;
import org.apache.catalina.User;
import org.apache.ibatis.jdbc.Null;
import com.example.demo.controller.CustomerThing;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.sql.*;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import com.example.demo.entity.score;
import com.example.demo.controller.storeThing;

import javax.imageio.ImageIO;

public class JDBC {
    public static void main(String[] args) {
        CustomerThing customerThing = new CustomerThing();
//        try {
//            String x=customerThing.getProductInfoByName("猫");
//            System.out.println(x);
//        } catch (ClassNotFoundException e) {
//            throw new RuntimeException(e);
//        } catch (SQLException e) {
//            throw new RuntimeException(e);
//        }
    }
//        storeThing ScoreThing = new storeThing();
//        UserType userType = UserType.s;
//              try {
//                  ScoreThing.addstore(6, "345","wang", 188,"2024-10-29", userType,1);
//            } catch (ClassNotFoundException e) {
//                throw new RuntimeException(e);
//            } catch (SQLException e) {
//                throw new RuntimeException(e);
//            }
//
//    }

//        CommentThing commentThing = new CommentThing();
//        try {
//            commentThing.AddFire(1);
//        } catch (SQLException e) {
//            throw new RuntimeException(e);
//        } catch (ClassNotFoundException e) {
//            throw new RuntimeException(e);
//        }
//    }

//        public static void main(String[] args) throws ClassNotFoundException,SQLException{
//        Class.forName("com.mysql.cj.jdbc.Driver");
//        String url = "jdbc:mysql://localhost:3306/easy_test";
//        String user = "root";
//        String password = "123456";
//
//        Connection connection = DriverManager.getConnection(url,user,password);
//        Statement statement = connection.createStatement();
//
//        if(connection == null) {
//            System.out.println("连接失败");
//        } else {
//            System.out.println("连接成功");
//        }
//        String what = "whats";
//        int is = 100;
//        String sql = "INSERT INTO customer (" +
//                     "UserID, " +
//                     "UserPassword, " +
//                     "UserName, " +
//                     "UserPhone, " +
//                     "UserAddress, " +
//                     "RegisterTime, " +
//                     "UserType, " +
//                     "UserSex, " +
//                     "UserBirth, " +
//                     "IsVerified, " +
//                     "UserScore" +
//                     ") VALUES (" +
//                     "4, '345', '"+what+"', '188', 'Handan', '2024-10-29', 'c', 'M', '2024-5-29', 1, 200)";
//
//        if(statement.executeUpdate(sql) >= 1){
//            System.out.println("添加成功");
//        }else {
//            System.out.println("添加失败");
//        }
//
//        connection.close();
//    }

}
