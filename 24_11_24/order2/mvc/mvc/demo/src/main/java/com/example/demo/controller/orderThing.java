package com.example.demo.controller;

import com.example.demo.entity.Order;
import com.example.demo.entity.petsupply;
import org.apache.catalina.User;

import java.sql.*;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class orderThing {
    //添加订单，返回0表示商品库存不足，返回1表示添加成功，返回2表示添加失败
    //添加订单后会获得相应的积分
    public int createOrder(int UserID,int PsID) throws ClassNotFoundException, SQLException {
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
    public int cancelOrder(int OrderID)throws ClassNotFoundException, SQLException {
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
    public int changeOrder(int OrderID, String OrderAddress)throws ClassNotFoundException, SQLException {
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
    public int changeOrderState(int OrderID)throws ClassNotFoundException, SQLException {
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
                return 0;
            }

        }catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
        System.out.println("订单状态更新失败");
        connection.close();
        return 0;

    }

    public List<Order> searchorder(int UserID) throws SQLException {
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

}
