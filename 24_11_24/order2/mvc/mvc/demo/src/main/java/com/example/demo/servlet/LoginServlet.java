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
//import org.jetbrains.annotations.NotNull;
//import org.springframework.web.bind.annotation.PostMapping;
//import org.springframework.web.bind.annotation.RequestMapping;
//import org.springframework.web.bind.annotation.RequestParam;
//import org.springframework.web.bind.annotation.RestController;
//
////@RestController
////@WebServlet({"/login", "/storeLogin"})
////@WebServlet({"/login"})
//public class LoginServlet extends HttpServlet {
//
//    @Override
//    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
//        String path = request.getServletPath();
//        if (path.equals("/login")) {
//            handleCustomerLogin(request, response);
//        } else if (path.equals("/storeLogin")) {
//            handleStoreLogin(request, response);
//        }
//    }
//
//
//    private void handleCustomerLogin(HttpServletRequest request, HttpServletResponse response) throws IOException {
//        int userID = Integer.parseInt(request.getParameter("userID"));
//        String password = request.getParameter("password");
//        CustomerThing customerThing = new CustomerThing();
//
//        try {
//            int result = customerThing.loadUser(userID, password);
//            if (result == 1) {
//                response.getWriter().write("success");
//            } else if (result == 0) {
//                response.getWriter().write("wrong_password");
//            } else {
//                response.getWriter().write("user_not_exist");
//            }
//        } catch (Exception e) {
//            e.printStackTrace();
//            response.getWriter().write("error");
//        }
//    }
//
//    private void handleStoreLogin(HttpServletRequest request, HttpServletResponse response) throws IOException {
//        int userID = Integer.parseInt(request.getParameter("userID"));
//        String password = request.getParameter("password");
//        storeThing storeThing = new storeThing();
//
//        try {
//            int result = storeThing.loadstore(userID, password);
//            if (result == 1) {
//                response.getWriter().write("success");
//            } else if (result == 0) {
//                response.getWriter().write("wrong_password");
//            } else {
//                response.getWriter().write("user_not_exist");
//            }
//        } catch (Exception e) {
//            e.printStackTrace();
//            response.getWriter().write("error");
//        }
//    }
//}
