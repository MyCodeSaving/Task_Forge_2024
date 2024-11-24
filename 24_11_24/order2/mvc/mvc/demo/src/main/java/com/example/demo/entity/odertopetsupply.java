package com.example.demo.entity;

import lombok.Data;

@Data
public class odertopetsupply {
    private int OrderID;
    private int PsID;

    public int getOrderID() {
        return OrderID;
    }

    public void setOrderID(int orderID) {
        OrderID = orderID;
    }

    public int getPsID() {
        return PsID;
    }

    public void setPsID(int psID) {
        PsID = psID;
    }
}
