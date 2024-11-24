package com.example.demo.servlet;

import java.io.IOException;
import java.sql.SQLException;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.example.demo.controller.scoreThing;
import com.example.demo.entity.score;

@WebServlet("/score")
public class ScoreServlet extends HttpServlet {

    // 处理 GET 请求：显示积分记录和剩余积分
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        // 获取当前用户的 ID（假设从 session 或请求中获取）
        int userID = Integer.parseInt(request.getParameter("userID"));  // 根据实际情况获取用户ID
        
        try {
            // 获取积分记录
            List<score> scoreList = new scoreThing().searchAll(userID);
            
            // 获取用户剩余积分
            int remainingScore = new scoreThing().searchscore(userID);
            
            // 将数据传递给前端
            request.setAttribute("scoreList", scoreList);
            request.setAttribute("remainingScore", remainingScore);
            
            // 转发请求到 getscore.html 页面
            request.getRequestDispatcher("/getscore.html").forward(request, response);
        } catch (SQLException e) {
            e.printStackTrace();
            response.sendError(HttpServletResponse.SC_INTERNAL_SERVER_ERROR, "Error while fetching score data.");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
            response.sendError(HttpServletResponse.SC_INTERNAL_SERVER_ERROR, "Class not found.");
        }
    }

    // 处理 POST 请求：积分兑换
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        // 获取前端请求中的数据（假设传递的是 JSON 格式数据）
        int userId = Integer.parseInt(request.getParameter("userId"));
        int psId = Integer.parseInt(request.getParameter("psId")); // 商品ID

        // 创建scoreThing实例，调用兑换积分的方法
        scoreThing scoreService = new scoreThing();
        try {
            // 使用积分兑换商品
            int result = scoreService.usescore(psId, userId);
            
            if (result == 1) {
                // 积分兑换成功
                response.setContentType("application/json");
                response.getWriter().write("{\"success\": true, \"message\": \"积分兑换成功\"}");
            } else {
                // 积分不足，兑换失败
                response.setContentType("application/json");
                response.getWriter().write("{\"success\": false, \"message\": \"积分不足，兑换失败\"}");
            }
        } catch (SQLException | ClassNotFoundException e) {
            e.printStackTrace();
            response.setContentType("application/json");
            response.getWriter().write("{\"success\": false, \"message\": \"系统错误，兑换失败\"}");
        }
    }
}
