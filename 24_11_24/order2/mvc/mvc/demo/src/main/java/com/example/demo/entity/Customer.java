package com.example.demo.entity;
import lombok.Data;

import java.util.Date;

@Data
public class Customer {
    public int UserID;
    public String UserPassword;
    public String UserName;
    public String UserPhone;
    public String UserAddress;
    public String RegisterTime;
    public UserType UserType;
    public UserSex UserSex;
    public String UserBirth;
    public int IsVerified;
    public int UserScore;

}
