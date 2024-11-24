package com.example.demo.entity;

import lombok.Data;

import java.util.Date;

@Data
public class store {
    private int UserID;
    private String UserPassword;
    private int UserPhone;
    private Date RegisterTime;
    private UserType userType;
    private int isVerified;
    private String UserName;

    public int getUserID() {
        return UserID;
    }

    public void setUserID(int userID) {
        UserID = userID;
    }

    public String getUserPassword() {
        return UserPassword;
    }

    public void setUserPassword(String userPassword) {
        UserPassword = userPassword;
    }

    public int getUserPhone() {
        return UserPhone;
    }

    public void setUserPhone(int userPhone) {
        UserPhone = userPhone;
    }

    public Date getRegisterTime() {
        return RegisterTime;
    }

    public void setRegisterTime(Date registerTime) {
        RegisterTime = registerTime;
    }

    public UserType getUserType() {
        return userType;
    }

    public void setUserType(UserType userType) {
        this.userType = userType;
    }

    public int getIsVerified() {
        return isVerified;
    }

    public void setIsVerified(int isVerified) {
        this.isVerified = isVerified;
    }

    public String getUserName() {
        return UserName;
    }

    public void setUserName(String userName) {
        UserName = userName;
    }
}
