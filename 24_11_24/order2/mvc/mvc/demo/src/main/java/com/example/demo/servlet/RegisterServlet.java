//package com.example.demo.servlet;
//
//import java.io.IOException;
//
//import javax.servlet.ServletException;
//import javax.servlet.annotation.WebServlet;
//import javax.servlet.http.HttpServlet;
//import javax.servlet.http.HttpServletRequest;
//import javax.servlet.http.HttpServletResponse;
//
//import com.example.demo.controller.CustomerThing;
//import com.example.demo.controller.storeThing;
//import com.example.demo.entity.UserSex;
//
//@WebServlet({"/register", "/registerStore"})
//public class RegisterServlet extends HttpServlet {
//    @Override
//    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
//        String path = request.getServletPath();
//        if (path.equals("/register")) {
//            // 普通用户注册逻辑
//            int userID = Integer.parseInt(request.getParameter("userID"));
//            String password = request.getParameter("password");
//            String username = request.getParameter("username");
//            int phone = Integer.parseInt(request.getParameter("phone"));
//            String address = request.getParameter("address");
//            String userSex = request.getParameter("userSex");
//            String birth = request.getParameter("birth");
//
//            CustomerThing customerThing = new CustomerThing();
//            try {
//                //int result = customerThing.addUser(userID, password, username, phone, address, UserSex.valueOf(userSex), birth);
//                //response.getWriter().write(result == 1 ? "success" : "failure");
//            } catch (Exception e) {
//                e.printStackTrace();
//                response.getWriter().write("error");
//            }
//        } else if (path.equals("/registerStore")) {
//            // 商家注册逻辑
//            int userID = Integer.parseInt(request.getParameter("userID"));
//            String password = request.getParameter("password");
//            String username = request.getParameter("username");
//            int phone = Integer.parseInt(request.getParameter("phone"));
//
//            storeThing storeThing = new storeThing();
//            try {
//                int result = storeThing.addstore(userID, password, username, phone);
//                response.getWriter().write(result == 1 ? "success" : "failure");
//            } catch (Exception e) {
//                e.printStackTrace();
//                response.getWriter().write("error");
//            }
//        }
//    }
//}

