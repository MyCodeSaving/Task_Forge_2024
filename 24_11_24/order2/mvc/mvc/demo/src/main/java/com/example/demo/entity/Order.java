package com.example.demo.entity;

import lombok.Data;

import java.util.Date;

@Data
public class Order {
    public int OrderID;
    public int UserID;
    public String OrderTime;
    public String OrderAddress;
    public int OrderState;
    public int PsID;

    public Order(int orderID, int userID, String orderTime, String orderAddress, int orderState, int psID) {
        this.OrderID = orderID;
        this.UserID = userID;
        this.OrderTime = orderTime;
        this.OrderAddress = orderAddress;
        this.OrderState = orderState;
        this.PsID = psID;
    }

    public int getOrderID() {
        return OrderID;
    }

    public void setOrderID(int orderID) {
        OrderID = orderID;
    }

    public int getUserID() {
        return UserID;
    }

    public void setUserID(int userID) {
        UserID = userID;
    }

    public String getOrderTime() {
        return OrderTime;
    }

    public void setOrderTime(String orderTime) {
        OrderTime = orderTime;
    }

    public String getOrderAddress() {
        return OrderAddress;
    }

    public void setOrderAddress(String orderAddress) {
        OrderAddress = orderAddress;
    }

    public int getOrderState() {
        return OrderState;
    }

    public void setOrderState(int orderState) {
        OrderState = orderState;
    }

    public int getPsID() {
        return PsID;
    }

    public void setPsID(int psID) {
        PsID = psID;
    }
}


