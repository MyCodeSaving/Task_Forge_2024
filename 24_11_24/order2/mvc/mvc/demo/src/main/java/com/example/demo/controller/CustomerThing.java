package com.example.demo.controller;

import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.sql.*;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Base64;
import java.util.Date;
import java.util.List;

import com.example.demo.entity.*;
import org.springframework.web.bind.annotation.*;

import javax.imageio.ImageIO;

import static com.example.demo.entity.UserSex.*;
import static com.example.demo.entity.UserType.s;
import static com.example.demo.entity.UserType.u;

@RestController
@CrossOrigin(origins = "http://localhost:63342", allowedHeaders = "*", methods = {RequestMethod.POST})
public class CustomerThing {


    //1.用户类
    //用户注册,用户注册成功返回1，注册失败返回0;
    @PostMapping ("/addUser")
    public int addUser(@RequestParam("userID") int UserID, @RequestParam("UserPassword") String UserPassword, @RequestParam("UserName") String UserName, @RequestParam("UserPhone") int UserPhone, @RequestParam("UserAddress") String UserAddress, @RequestParam("sex") String sex, @RequestParam("UserBirth") String UserBirth) throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";

        Connection connection = DriverManager.getConnection(url, user, password);
        Statement statement = connection.createStatement();

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        java.util.Date now = new Date();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        String RegisterTime = sdf.format(now);
        UserType UserType = u;
        UserSex Usersex;
        if(sex.equals("男")){
            Usersex = M;
        }else if(sex.equals("女")){
            Usersex = W;
        }else {
            Usersex = N;
        }

        int IsVerified = 1;
        int UserScore = 0;
        String sql = "INSERT INTO customer (" +
                "UserID, " +
                "UserPassword, " +
                "UserName, " +
                "UserPhone, " +
                "UserAddress, " +
                "RegisterTime, " +
                "UserType, " +
                "UserSex, " +
                "UserBirth, " +
                "IsVerified, " +
                "UserScore" +
                ") VALUES (" +
                "" + UserID + ", " +
                "'" + UserPassword + "', " +
                "'" + UserName + "', " +
                "'" + UserPhone + "', " +
                "'" + UserAddress + "', " +
                "'" + RegisterTime + "', " +
                "'" + UserType + "', " +
                "'" + Usersex + "', " +
                "'" + UserBirth + "', " +
                "" + IsVerified + ", " +
                "" + UserScore + ")";

        if (statement.executeUpdate(sql) >= 1) {
            System.out.println("用户注册成功");
            connection.close();
            return 1;
        } else {
            System.out.println("用户注册失败");
            connection.close();
            return 0;
        }
    }

    //用户登录,返回0表示密码错误，返回1表示登录成功，返回2表示用户名不存在
    @PostMapping ("/loadUser")
    public int loadUser(@RequestParam("userID") int userID,@RequestParam("password") String userPassword) throws ClassNotFoundException, SQLException {
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
        String sql = "SELECT COUNT(*) FROM customer WHERE UserID = ? AND UserPassword = ?";
        PreparedStatement statement = connection.prepareStatement(sql);
        statement.setInt(1, userID);
        statement.setString(2, userPassword);
        ResultSet resultSet = statement.executeQuery();
        if (resultSet.next()) {
            int count = resultSet.getInt(1);
            if (count > 0) {
                System.out.println("用户登录成功");
                connection.close();
                return 1;
            }
        }
        //密码错误或用户名不存在
        sql = "SELECT COUNT(*) FROM customer WHERE UserID = ? ";
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

    //修改用户信息，给出改之前的用户ID,以及改之后的其他信息，返回1修改信息成功，返回0失败,其中用户ID不可修改
    //UserID是改之前的，其他的都是改之后的，UserID不可以改
    @PostMapping ("/changeUser")
    public int changeUser(@RequestParam("userID") int UserID, @RequestParam("name") String UserName,@RequestParam("phone") int UserPhone,@RequestParam("address") String UserAddress,@RequestParam("gender") String sex,@RequestParam("birthDate") String UserBirth) throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        UserType UserType = u;
        int IsVerified = 1;
        int UserScore = 0;
        UserSex Usersex;
        if(sex.equals("男")){
            Usersex = M;
        }else if(sex.equals("女")){
            Usersex = W;
        }else {
            Usersex = N;
        }
        String sql = "UPDATE customer SET " +
                     "UserName = ?, " +
                     "UserPhone = ?, " +
                     "UserAddress = ?, " +
                     "UserType = ?, " +
                     "UserSex = ?, " +
                     "UserBirth = ?, " +
                     "IsVerified = ?, " +
                     "UserScore = ? " +
                     "WHERE UserID = ?";

        try (PreparedStatement preparedStatement = connection.prepareStatement(sql)) {
            preparedStatement.setString(1, UserName);
            preparedStatement.setInt(2, UserPhone);
            preparedStatement.setString(3, UserAddress);
            preparedStatement.setString(4, UserType.toString());
            preparedStatement.setString(5, Usersex.toString());
            preparedStatement.setString(6, UserBirth);
            preparedStatement.setInt(7, IsVerified);
            preparedStatement.setInt(8, UserScore);
            preparedStatement.setInt(9, UserID);

            int rowsUpdated = preparedStatement.executeUpdate();
            if(rowsUpdated > 0)
            {
                System.out.println("用户信息修改成功");
                connection.close();
                return 1;
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        System.out.println("用户信息修改失败");
        connection.close();
        return 0;

    }

    //修改用户密码
    //传用户ID,原来的密码，和现在的密码
    //返回0表示用户原来密码输入错误，返回1表示用户密码修改成功
    @PostMapping ("/changeUserPassword")
    public int changeUserPassword(@RequestParam("userID") int UserID, @RequestParam("currentPassword") String PastUserPassword, @RequestParam("newPassword") String NowUserPassword) throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        String sql = "SELECT UserPassword FROM Customer WHERE UserID = ?";

        PreparedStatement preparedStatement = connection.prepareStatement(sql);
        preparedStatement.setInt(1, UserID);
        ResultSet rs = preparedStatement.executeQuery();
        String Password = "1234";
        if (rs.next()) {
            Password = rs.getString("UserPassword");
        }


        if(Password.equals(PastUserPassword)){
            String sql1 =  "UPDATE Customer SET UserPassword = ? WHERE UserID = ?";
            PreparedStatement preparedStatement1 = connection.prepareStatement(sql1);
            preparedStatement1.setString(1, NowUserPassword);
            preparedStatement1.setInt(2, UserID);
            int affectedRows = preparedStatement1.executeUpdate();

            System.out.println("用户密码修改成功");
            connection.close();
            return 1;

        }

        System.out.println("密码输入错误!");
        connection.close();
        return 0;

    }

    //注销账号，注销成功返回1，注销失败返回0;
    @PostMapping ("/cancelUser")
    public int cancelUser(@RequestParam("userID") int userID, @RequestParam("password") String userPassword) throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        String sql = "DELETE FROM customer WHERE UserID = ? AND UserPassword = ?";
        try (PreparedStatement preparedStatement = connection.prepareStatement(sql)){
            preparedStatement.setInt(1, userID);
            preparedStatement.setString(2, userPassword);
            int rowsDeleted = preparedStatement.executeUpdate();
            if(rowsDeleted > 0)
            {
                System.out.println("用户注销成功");
                connection.close();
                return 1;
            }
        }catch (SQLException e) {
            e.printStackTrace();
        }
        System.out.println("用户注销失败");
        connection.close();
        return 0;
    }

    //用户积分增加，用户由于购买

    // 获取用户信息
    @PostMapping ("/getUserInfo")
    public String getUserInfo(@RequestParam("userID") int userID) throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        
        Connection connection = DriverManager.getConnection(url, user, password);
        String sql = "SELECT * FROM customer WHERE UserID = ?";
        PreparedStatement statement = connection.prepareStatement(sql);
        statement.setInt(1, userID);
        ResultSet resultSet = statement.executeQuery();
        
        if (resultSet.next()) {
            StringBuilder userInfo = new StringBuilder();
            userInfo.append("用户ID: ").append(resultSet.getInt("UserID")).append("\n");
            userInfo.append("姓名: ").append(resultSet.getString("UserName")).append("\n");
            userInfo.append("手机号: ").append(resultSet.getInt("UserPhone")).append("\n");
            userInfo.append("地址: ").append(resultSet.getString("UserAddress")).append("\n");
            userInfo.append("性别: ").append(resultSet.getString("UserSex")).append("\n");
            userInfo.append("出生日期: ").append(resultSet.getString("UserBirth")).append("\n");

            connection.close();
            return userInfo.toString();
        } else {
            connection.close();
            return "User not found";
        }
    }





    //2.商家类

    //商家注册，注册成功返回1，注册失败返回0;
    @PostMapping("/addStore")
    public int addStore(@RequestParam("userID") int UserID, @RequestParam("password") String UserPassword, @RequestParam("username") String UserName, @RequestParam("phone") int UserPhone) throws ClassNotFoundException, SQLException {
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
    @PostMapping( "/loadStore")
    public int loadStore(@RequestParam("userID") int userID, @RequestParam("password") String userPassword) throws ClassNotFoundException, SQLException {
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
    @PostMapping("/getStoreInfo")
    public String getStoreInfo(@RequestParam("userID") int userID) throws ClassNotFoundException, SQLException {
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



    //3.积分类
    //用户积分增加，用户由于购买
    @PostMapping("/addCustomerScore")
    public int addCustomerScore(@RequestParam("userID") int UserID,@RequestParam("ScoreValue") int ScoreValue)throws ClassNotFoundException, SQLException {
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
    @PostMapping("/decreaseCustomerScore")
    public int decreaseCustomerScore(@RequestParam("userID") int UserID, @RequestParam("ScoreValue") int ScoreValue)throws ClassNotFoundException, SQLException {
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
    @PostMapping("/addScore")
    public int addScore(@RequestParam("PsID") int PsID, @RequestParam("userID") int UserID, @RequestParam("ScoreValue") int ScoreValue,@RequestParam("RecordTime") String RecordTime)throws ClassNotFoundException, SQLException {
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
    @PostMapping("/useScore")
    public int useScore(@RequestParam("PsID") int PsID,@RequestParam("userID") int UserID)throws ClassNotFoundException, SQLException {
        petsupplyThing petsupplything = new petsupplyThing();
        int PsNeedScore = petsupplything.decreaseScore(PsID);
        decreaseCustomerScore(UserID,PsNeedScore);

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
    @PostMapping("/searchScore")
    public int searchScore(@RequestParam("userID") int UserID)throws ClassNotFoundException, SQLException {
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
    @PostMapping("/searchAll")
    public List<score> searchAll(@RequestParam("userID") int UserID) throws SQLException {
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




    //4.商品类
    //商家添加宠物商品信息，返回1表示添加成功，返回0表示添加失败，其中使用的图片为.jpg形式
    @PostMapping("/addPetSupplies")
    public int addPetSupplies(@RequestParam("PsID")int PsID, @RequestParam("PsName") String PsName,@RequestParam("PsPrice") int PsPrice,@RequestParam("PsIntroduction") String PsIntroduction,@RequestParam("PsType") String PsType,@RequestParam("PsGetScore") int PsGetScore,@RequestParam("PsNeedScore") int PsNeedScore,@RequestParam("PsInventory") int PsInventory) throws ClassNotFoundException, SQLException, IOException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        String sql = "INSERT INTO petsupply (PsID, PsName, PsPrice, PsIntroduction, PsType, PsGetScore, PsNeedScore, PsInventory) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
        try (PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setInt(1, PsID);
            statement.setString(2, PsName);
            statement.setInt(3, PsPrice);
            statement.setString(4,PsIntroduction);
            statement.setString(5, PsType);
            statement.setInt(6, PsGetScore);
            statement.setInt(7, PsNeedScore);
            statement.setInt(8, PsInventory);

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
    @PostMapping("/changePetSupplies")
    public int changePetSupplies(@RequestParam("PsID") int PsID, @RequestParam("PsName") String PsName,@RequestParam("PsPrice") int PsPrice,@RequestParam("PsIntroduction") String PsIntroduction,@RequestParam("PsType") String PsType,@RequestParam("PsGetScore") int PsGetScore,@RequestParam("PsNeedScore") int PsNeedScore,@RequestParam("PsInventory") int PsInventory)throws ClassNotFoundException, SQLException {
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
    @PostMapping("/cancePetsupplies")
    public int cancePetsupplies(@RequestParam("PsID") int PsID) throws ClassNotFoundException, SQLException {
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
    @PostMapping("/searchPetSuppliesByType")
    public List<petsupply> searchPetSuppliesByType(@RequestParam("PsType") String PsType) throws SQLException {
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


    @PostMapping("/getProductInfoByName")
    public String getProductInfoByName(@RequestParam("PsName") String psName) throws SQLException, ClassNotFoundException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";

        Connection connection = DriverManager.getConnection(url, user, password);
        String sql = "SELECT PsID, PsName, PsPrice, PsIntroduction, Pstype, PsGetScore, PsNeedScore, PsInventory FROM `petsupply` WHERE PsName LIKE?";
        PreparedStatement statement = connection.prepareStatement(sql);
        statement.setString(1, "%" + psName + "%");
        ResultSet resultSet = statement.executeQuery();

        if (resultSet.next()) {
            StringBuilder productInfo = new StringBuilder();
            productInfo.append("商品ID: ").append(resultSet.getInt("PsID")).append("\n");
            productInfo.append("商品名称: ").append(resultSet.getString("PsName")).append("\n");
            productInfo.append("价格: ").append(resultSet.getInt("PsPrice")).append("\n");
            productInfo.append("介绍: ").append(resultSet.getString("PsIntroduction")).append("\n");
            productInfo.append("商品类型 ").append(resultSet.getString("Pstype")).append("\n");
            productInfo.append("获得积分: ").append(resultSet.getInt("PsGetScore")).append("\n");
            productInfo.append("需要积分: ").append(resultSet.getInt("PsNeedScore")).append("\n");
            productInfo.append("库存量: ").append(resultSet.getInt("PsInventory")).append("\n");

            connection.close();
            return productInfo.toString();
        } else {
            connection.close();
            return "Product not found";
        }
    }

    @PostMapping("/getProductInfoByType")
    public String getProductInfoByType(@RequestParam("PsType") String psType) throws SQLException, ClassNotFoundException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";

        Connection connection = DriverManager.getConnection(url, user, password);
        String sql = "SELECT PsID, PsName, PsPrice, PsIntroduction, Pstype, PsGetScore, PsNeedScore, PsInventory FROM `petsupply` WHERE PsType LIKE?";
        PreparedStatement statement = connection.prepareStatement(sql);
        statement.setString(1, "%" + psType + "%");
        ResultSet resultSet = statement.executeQuery();

        if (resultSet.next()) {
            StringBuilder productInfo = new StringBuilder();
            productInfo.append("商品ID: ").append(resultSet.getInt("PsID")).append("\n");
            productInfo.append("商品名称: ").append(resultSet.getString("PsName")).append("\n");
            productInfo.append("价格: ").append(resultSet.getInt("PsPrice")).append("\n");
            productInfo.append("介绍: ").append(resultSet.getString("PsIntroduction")).append("\n");
            productInfo.append("商品类型 ").append(resultSet.getString("Pstype")).append("\n");
            productInfo.append("获得积分: ").append(resultSet.getInt("PsGetScore")).append("\n");
            productInfo.append("需要积分: ").append(resultSet.getInt("PsNeedScore")).append("\n");
            productInfo.append("库存量: ").append(resultSet.getInt("PsInventory")).append("\n");

            connection.close();
            return productInfo.toString();
        } else {
            connection.close();
            return "Product not found";
        }
    }

    //5.订单类
    //添加订单，返回0表示商品库存不足，返回1表示添加成功，返回2表示添加失败
    //添加订单后会获得相应的积分
    @PostMapping("/createOrder")
    public int createOrder(@RequestParam("userID") int UserID,@RequestParam("psID") int PsID) throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }
        String sql2 = "SELECT UserAddress FROM Customer WHERE UserID = ?";
        PreparedStatement pstmt = connection.prepareStatement(sql2);
        pstmt.setInt(1, UserID); // 假设userId是您要查询的用户ID
        ResultSet rs1 = pstmt.executeQuery();
        String UserAddress = "Hebei";
        if (rs1.next()) {
            UserAddress = rs1.getString("UserAddress");
        }
        String OrderAddress = UserAddress;
        //查询库存
        String sql1 = "SELECT PsInventory FROM petsupply WHERE PsID = ?";
        int inventory = 0;
        PreparedStatement statement = connection.prepareStatement(sql1);
        statement.setInt(1, PsID);
        ResultSet rs = statement.executeQuery();
        if(rs.next()){
            inventory = rs.getInt("PsInventory");
        }
        //库存量不足处理
        if(inventory > 0){
            String updateInventorySql = "UPDATE petsupply SET PsInventory = PsInventory - 1 WHERE PsID = ?";
            statement = connection.prepareStatement(updateInventorySql);
            statement.setInt(1, PsID);
            statement.executeUpdate();
        } else{
            System.out.println("商品库存不足");
            connection.close();
            return 0;
        }

        Date now = new Date();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        String OrderTime = sdf.format(now);
        //添加订单
        String sql = "INSERT INTO `order` (UserID, OrderTime, OrderAddress, OrderState, PsID) VALUES (?, ?, ?, ?, ?)";
        statement = connection.prepareStatement(sql);
        statement.setInt(1, UserID);
        statement.setString(2, OrderTime);
        statement.setString(3, OrderAddress);
        statement.setInt(4, 1);
        statement.setInt(5, PsID);

        int rowsAffected = statement.executeUpdate();

        if(rowsAffected > 0) {
            System.out.println("订单信息添加成功");
            connection.close();
            petsupplyThing petsupplything = new petsupplyThing();
            int PsGetScore = petsupplything.GetScore(PsID);
            scoreThing scorething = new scoreThing();
            scorething.addCustomerScore(UserID,PsGetScore);
            return 1;
        }
        System.out.println("订单信息添加失败");
        connection.close();
        petsupplyThing petsupplything = new petsupplyThing();
        int PsGetScore = petsupplything.GetScore(PsID);
        scoreThing scorething = new scoreThing();
        scorething.addCustomerScore(PsID,PsGetScore);
        return 2;
    }

    //取消订单，输入要取消订单的ID,即OrderID,未发货取消成功返回1,如果已发送不能取消订单返回0
    //删除订单后会取消相应的积分
    @PostMapping("/cancelOrder")
    public int cancelOrder(@RequestParam("orderID") int OrderID)throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        String sql = "DELETE FROM `order` WHERE OrderID = ? AND OrderState <= 1";
        try (PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setInt(1, OrderID);
            int rowsDeleted = statement.executeUpdate();
            if (rowsDeleted > 0) {
                System.out.println("订单删除成功");
                connection.close();
                return 1;
            }
        }catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
        System.out.println("商品已发货，不能取消订单");
        connection.close();
        return 0;
    }

    //修改订单信息，由于与订单ID等有关的因素大多不可修改，几乎只能修改地址
    //返回1修改成功，返回0修改失败
    @PostMapping("/changeOrder")
    public int changeOrder(@RequestParam("orderID") int OrderID,@RequestParam("address") String OrderAddress)throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        String sql = "UPDATE `order` SET OrderAddress = ? WHERE OrderID = ?";
        try (PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setString(1, OrderAddress); // 设置新地址
            statement.setInt(2, OrderID); // 设置订单ID
            int rowsUpdated = statement.executeUpdate();
            if (rowsUpdated > 0) {
                System.out.println("订单信息修改成功！");
                connection.close();
                return 1;
            }
        }catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
        System.out.println("订单信息修改失败");
        connection.close();
        return 0;
    }
    //订单状态的修改，无论是商家将状态从待发货改为已发货，还是用户将状态从已发货改为待评价，都是让状态位OrderState加一
    @PostMapping("/changeOrderState")
    public int changeOrderState(@RequestParam("OrderID") int OrderID)throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        String sql = "UPDATE `order` SET OrderState = OrderState + 1 WHERE OrderID = ?";
        try (PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setInt(1, OrderID); // 设置订单ID

            // 执行更新操作
            int rowsUpdated = statement.executeUpdate();
            if (rowsUpdated > 0) {
                System.out.println("订单状态更新成功！");
                connection.close();
                return 1;
            }

        }catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
        System.out.println("订单状态更新失败");
        connection.close();
        return 0;

    }

    @PostMapping("/searchOrder")
    public List<Order> searchOrder(@RequestParam("userID") int UserID) throws SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        List<Order> orders = new ArrayList<>();
        String sql = "SELECT * FROM order WHERE UserID=?";
        try (PreparedStatement preparedStatement = connection.prepareStatement(sql)) {
            preparedStatement.setInt(1, UserID);
            ResultSet rs = preparedStatement.executeQuery();
            while (rs.next()) {
                int orderId = rs.getInt("OrderID");
                int orderState = rs.getInt("OrderState");
                String orderAddress = rs.getString("OrderAddress");
                String orderTime = rs.getString("OrderTime");
                int PsID = rs.getInt("PsID");

                Order order = new Order(orderId, UserID, orderTime, orderAddress, orderState, PsID);
                orders.add(order);
            }
        }catch (SQLException e) {
            e.printStackTrace();
        }
        connection.close();
        return orders;
    }

    @PostMapping("/getOrderById")
    public String getOrderById(@RequestParam("orderID") int orderID) throws SQLException, ClassNotFoundException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";


        Connection connection = DriverManager.getConnection(url, user, password);
        String sql = "SELECT * FROM `order` WHERE OrderID =?";
        PreparedStatement statement = connection.prepareStatement(sql);
        statement.setInt(1, orderID);
        ResultSet resultSet = statement.executeQuery();

        if (resultSet.next()) {
            StringBuilder orderInfo = new StringBuilder();
            int orderState = resultSet.getInt("OrderState");
            String now;
            if(orderState == 1){
                now = "代发货";
            }else if(orderState == 2){
                now = "待收货";
            }else if(orderState == 3){
                now = "待评价";
            }else{
                now = "完成";
            }
            orderInfo.append("订单ID: ").append(resultSet.getInt("OrderID")).append("\n");
            orderInfo.append("用户ID: ").append(resultSet.getInt("UserID")).append("\n");
            orderInfo.append("商品ID: ").append(resultSet.getInt("PsID")).append("\n");
            orderInfo.append("下单时间: ").append(resultSet.getString("OrderTime")).append("\n");
            orderInfo.append("下单地址: ").append(resultSet.getString("OrderAddress")).append("\n");
            orderInfo.append("订单状态: ").append(now).append("\n");

            connection.close();
            return orderInfo.toString();
        } else {
            connection.close();
            return "Order not found";
        }
    }

    //用户增加评论，成功返回1，失败0
    @PostMapping("/addComment")
    public int addComment(@RequestParam("userID") int UserID,@RequestParam("PsID") int PsID,@RequestParam("CommentContent") String CommentContent) throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        Date now = new Date();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        String CommentTime = sdf.format(now);
        int Fire = 0;
        String sql = "INSERT INTO comment (UserID, PsID, CommentContent, CommentTime, Fire) VALUES (?, ?, ?, ?, ?)";
        try (PreparedStatement preparedStatement = connection.prepareStatement(sql)){
            preparedStatement.setInt(1, UserID);
            preparedStatement.setInt(2, PsID);
            preparedStatement.setString(3, CommentContent);
            preparedStatement.setString(4, CommentTime);
            preparedStatement.setInt(5, Fire);
            int rowsAffected = preparedStatement.executeUpdate();
            if (rowsAffected > 0) {
                System.out.println("评论发布成功");
                connection.close();
                return 1;
            }
        }catch (SQLException e) {
            e.printStackTrace();
        }

        System.out.println("评论发布失败");
        connection.close();
        return 0;
    }


    //用户删除自己的评论，成功返回1，失败0
    @PostMapping("/deleteComment")
    public int deleteComment(@RequestParam("userID") int UserID,@RequestParam("PsID") int PsID) throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }
        String sql = "DELETE FROM `comment` WHERE UserID = ?  AND PsID = ?";
        try (PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setInt(1, UserID);
            statement.setInt(2, PsID);
            int rowsDeleted = statement.executeUpdate();
            if (rowsDeleted > 0) {
                System.out.println("评论删除成功");
                connection.close();
                return 1;
            }
        }catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
        System.out.println("评论删除失败");
        connection.close();
        return 0;
    }



    //审核评论，改为管理员对不好的评论进行删除，成功返回1，失败返回0
    //管理员使用这个函数删除评论
    @PostMapping("/deleteAllComment")
    public int deleteAllComment(@RequestParam("CommentID") int CommentID) throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }
        String sql = "DELETE FROM `comment` WHERE CommentID = ?";
        try (PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setInt(1, CommentID);
            int rowsDeleted = statement.executeUpdate();
            if (rowsDeleted > 0) {
                System.out.println("评论删除成功");
                connection.close();
                return 1;
            }
        }catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
        System.out.println("评论删除失败");
        connection.close();
        return 0;
    }


    //用户查看评论，点击参考，评论热度增加Fire加一
    //返回1表示评论热度增加，返回0不变
    @PostMapping("/AddFire")
    public int AddFire(@RequestParam("CommentID") int CommentID)throws ClassNotFoundException, SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        String sql = "UPDATE `comment` SET Fire = Fire + 1 WHERE CommentID = ?";
        try (PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setInt(1, CommentID); // 设置订单ID

            // 执行更新操作
            int rowsUpdated = statement.executeUpdate();
            if (rowsUpdated > 0) {
                System.out.println("评论热度增加");
                connection.close();
                return 0;
            }

        }catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
        System.out.println("评论热度不变");
        connection.close();
        return 0;
    }

    @PostMapping("/searchComment")
    //查看某个商品的评论，返回为包含该评论的集合
    public List<Comment> searchComment(@RequestParam("PsID") int PsID) throws SQLException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url, user, password);

        if (connection == null) {
            System.out.println("连接失败");
        } else {
            System.out.println("连接成功");
        }

        List<Comment> comments = new ArrayList<>();
        String sql = "SELECT * FROM comment WHERE PsID = ?";
        try (PreparedStatement preparedStatement = connection.prepareStatement(sql)) {
            preparedStatement.setInt(1, PsID);
            ResultSet rs = preparedStatement.executeQuery();
            while (rs.next()) {
                int CommentID = rs.getInt("CommentID");
                int UserID = rs.getInt("UserID");
                int PSID = rs.getInt("PSID");
                String CommentContent = rs.getString("CommentContent");
                String CommentTime = rs.getString("CommentTime");
                int Fire = rs.getInt("Fire");

                Comment comment = new Comment(CommentID, UserID, PSID, CommentContent, CommentTime, Fire);
                comments.add(comment);
            }
        }catch (SQLException e) {
            e.printStackTrace();
        }
        connection.close();
        return comments;
    }


    @PostMapping("/getCommentByIdAndPsId")
    public String getCommentByIdAndPsId(@RequestParam("CommentID") int commentID, @RequestParam("PsID") int psID) throws SQLException, ClassNotFoundException {
        String url = "jdbc:mysql://localhost:3306/easy_test";
        String user = "root";
        String password = "123456";


        Connection connection = DriverManager.getConnection(url, user, password);
        String sql = "SELECT * FROM `comment` WHERE CommentID =? AND PsID =?";
        PreparedStatement statement = connection.prepareStatement(sql);
        statement.setInt(1, commentID);
        statement.setInt(2, psID);
        ResultSet resultSet = statement.executeQuery();

        if (resultSet.next()) {
            StringBuilder commentInfo = new StringBuilder();
            commentInfo.append("评论ID: ").append(resultSet.getInt("CommentID")).append("\n");
            commentInfo.append("评价人ID: ").append(resultSet.getInt("UserID")).append("\n");
            commentInfo.append("商品ID: ").append(resultSet.getInt("PsID")).append("\n");
            commentInfo.append("评论内容: ").append(resultSet.getString("CommentContent")).append("\n");
            commentInfo.append("评论时间: ").append(resultSet.getString("CommentTime")).append("\n");
            commentInfo.append("评论热度: ").append(resultSet.getInt("Fire")).append("\n");

            AddFire(commentID);
            connection.close();
            return commentInfo.toString();
        } else {
            connection.close();
            return "Comment not found";
        }
    }


}

