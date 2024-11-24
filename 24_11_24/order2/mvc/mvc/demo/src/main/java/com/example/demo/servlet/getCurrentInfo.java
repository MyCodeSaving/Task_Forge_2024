package com.example.demo.servlet;

import java.io.IOException;
import java.sql.SQLException;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.example.demo.controller.CustomerThing;
import com.example.demo.controller.storeThing;

@WebServlet("/getCurrentInfo")
public class getCurrentInfo extends HttpServlet {

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        String userType = request.getParameter("userType"); // "customer" 或 "store"
        int userID = Integer.parseInt(request.getParameter("userID"));
        
        try {
            if ("customer".equalsIgnoreCase(userType)) {
                CustomerThing customerThing = new CustomerThing();
                // 获取用户信息
                String userInfo = customerThing.getUserInfo(userID);
                response.getWriter().write(userInfo);
            } else if ("store".equalsIgnoreCase(userType)) {
                storeThing storeThing = new storeThing();
                // 获取商家信息
                String storeInfo = storeThing.getStoreInfo(userID);
                response.getWriter().write(storeInfo);
            } else {
                response.getWriter().write("Invalid user type");
            }
        } catch (ClassNotFoundException | SQLException e) {
            e.printStackTrace();
            response.getWriter().write("Error retrieving information");
        }
    }
}
