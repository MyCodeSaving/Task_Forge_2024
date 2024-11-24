package com.example.demo.servlet;

import java.io.BufferedReader;
import java.io.IOException;
import java.sql.SQLException;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.example.demo.controller.CustomerThing;
import com.example.demo.entity.UserSex;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

@WebServlet({"/resetPassword", "/resetUserInfo"})
public class ResetServlet extends HttpServlet {

//    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
//        String path = request.getServletPath(); // 获取访问路径
//
//        if ("/resetPassword".equals(path)) {
//            handlePasswordReset(request, response);
//        } else if ("/resetUserInfo".equals(path)) {
//            handleUserInfoReset(request, response);
//        } else {
//            response.setStatus(HttpServletResponse.SC_NOT_FOUND);
//            response.getWriter().write("Invalid endpoint");
//        }
//    }
//
//    private void handlePasswordReset(HttpServletRequest request, HttpServletResponse response) throws IOException {
//        int userID = Integer.parseInt(request.getParameter("userID")); // 替换为实际登录用户的 ID 获取逻辑
//        String currentPassword = request.getParameter("currentPassword");
//        String newPassword = request.getParameter("newPassword");
//
//        CustomerThing customerThing = new CustomerThing();
//
//        try {
//            int result = customerThing.changeUserPassword(userID, currentPassword, newPassword);
//            if (result == 1) {
//                response.getWriter().write("success"); // 密码修改成功
//            } else if (result == 0) {
//                response.getWriter().write("wrong_password"); // 原密码错误
//            } else {
//                response.getWriter().write("error"); // 其他错误
//            }
//        } catch (ClassNotFoundException | SQLException e) {
//            e.printStackTrace();
//            response.getWriter().write("error");
//        }
//    }
//
//    private void handleUserInfoReset(HttpServletRequest request, HttpServletResponse response) throws IOException {
//        BufferedReader reader = request.getReader();
//        StringBuilder sb = new StringBuilder();
//        String line;
//        while ((line = reader.readLine()) != null) {
//            sb.append(line);
//        }
//        String requestData = sb.toString();
//
//        JsonObject jsonObject = JsonParser.parseString(requestData).getAsJsonObject();
//        int userID = jsonObject.get("userID").getAsInt();
//        String userName = jsonObject.get("userName").getAsString();
//        int userPhone = jsonObject.get("userPhone").getAsInt();
//        String userAddress = jsonObject.get("userAddress").getAsString();
//        UserSex userSex = UserSex.valueOf(jsonObject.get("userSex").getAsString());
//        String userBirth = jsonObject.get("userBirth").getAsString();
//
//        CustomerThing customerThing = new CustomerThing();
//
//        try {
//            int result = customerThing.changeUser(userID, userName, userPhone, userAddress, userSex, userBirth);
//            if (result == 1) {
//                response.getWriter().write("success");
//            } else {
//                response.getWriter().write("failure");
//            }
//        } catch (ClassNotFoundException | SQLException e) {
//            e.printStackTrace();
//            response.getWriter().write("error");
//        }
//    }
}

