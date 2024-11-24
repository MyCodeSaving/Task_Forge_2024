//package com.example.demo.jiekou;
//
//import com.example.demo.controller.CustomerThing;
//import com.example.demo.controller.storeThing;
//import org.springframework.web.bind.annotation.*;
//import org.springframework.web.bind.annotation.CrossOrigin;
//import org.springframework.web.bind.annotation.GetMapping;
//import org.springframework.web.bind.annotation.RequestMethod;
//import org.springframework.web.bind.annotation.RestController;
//
//import java.sql.SQLException;
//
//@RestController
//@CrossOrigin(origins = "http://localhost:63342", allowedHeaders = "*", methods = {RequestMethod.POST})
//public class LoginTodo {
//    //@PostMapping ("/loginuser")
////    public int loginuser(@RequestParam("userID") int userID, @RequestParam("password") String userPassword) {
////        CustomerThing customerThing = new CustomerThing();
////        int result = 5;
////        try {
////            result = customerThing.loadUser(userID,userPassword);
////            return result;
////        } catch (ClassNotFoundException e) {
////            throw new RuntimeException(e);
////        } catch (SQLException e) {
////            throw new RuntimeException(e);
////        }
////    }
//
//    @PostMapping ("/loginstore")
//    public int loginstore(@RequestParam("userID") int userID, @RequestParam("password") String userPassword) {
//        storeThing storething = new storeThing();
//        int result = 5;
//        try {
//            result = storething.loadstore(userID,userPassword);
//            return result;
//        } catch (ClassNotFoundException e) {
//            throw new RuntimeException(e);
//        } catch (SQLException e) {
//            throw new RuntimeException(e);
//        }
//    }
//
//    @PostMapping ("/addstore")
//    public int addstore(@RequestParam("userID") int UserID, @RequestParam("password") String UserPassword, @RequestParam("username") String UserName, @RequestParam("phone") int UserPhone){
//        storeThing storething = new storeThing();
//        int result = 5;
//        try {
//            result = storething.addstore(UserID,UserPassword,UserName,UserPhone);
//            return result;
//        } catch (ClassNotFoundException | SQLException e) {
//            throw new RuntimeException(e);
//        }
//
//    }
//    @GetMapping("/success")
//    public String returnSuccess() {
//        return "wrong_password";
//    }
//
//}
