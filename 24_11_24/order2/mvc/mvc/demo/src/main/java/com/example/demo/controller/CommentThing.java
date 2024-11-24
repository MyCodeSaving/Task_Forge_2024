package com.example.demo.controller;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import com.example.demo.entity.Comment;

public class CommentThing {
    private static final String URL = "jdbc:mysql://localhost:3306/easy_test";
    private static final String USER = "root";
    private static final String PASSWORD = "123456";

    private Connection getConnection() throws SQLException {
        return DriverManager.getConnection(URL, USER, PASSWORD);
    }

    // 用户发表评论
    public int addComment(int userID, int psID, String commentContent) {


        String sql = "INSERT INTO comment (UserID, PsID, CommentContent, CommentTime, Fire) VALUES (?, ?, ?, NOW(), 0)";
        try (Connection connection = getConnection();
             PreparedStatement preparedStatement = connection.prepareStatement(sql)) {
            preparedStatement.setInt(1, userID);
            preparedStatement.setInt(2, psID);
            preparedStatement.setString(3, commentContent);
            return preparedStatement.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
            return 0;
        }
    }

    // 用户删除自己的评论
    public int deleteComment(int userID, int psID) {
        String sql = "DELETE FROM comment WHERE UserID = ? AND PsID = ?";
        try (Connection connection = getConnection();
             PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setInt(1, userID);
            statement.setInt(2, psID);
            return statement.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
            return 0;
        }
    }

    // 用户编辑评论
    public int updateComment(int commentID, String newContent) {
        String sql = "UPDATE comment SET CommentContent = ?, CommentTime = NOW() WHERE CommentID = ?";
        try (Connection connection = getConnection();
             PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setString(1, newContent);
            statement.setInt(2, commentID);
            return statement.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
            return 0;
        }
    }

    // 分页查看商品评论
    public List<Comment> searchCommentsWithPagination(int psID, int page, int pageSize) {
        List<Comment> comments = new ArrayList<>();
        String sql = "SELECT * FROM comment WHERE PsID = ? ORDER BY CommentTime DESC LIMIT ?, ?";
        try (Connection connection = getConnection();
             PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setInt(1, psID);
            statement.setInt(2, (page - 1) * pageSize);
            statement.setInt(3, pageSize);
            ResultSet rs = statement.executeQuery();
            while (rs.next()) {
                Comment comment = new Comment(
                        rs.getInt("CommentID"),
                        rs.getInt("UserID"),
                        rs.getInt("PsID"),
                        rs.getString("CommentContent"),
                        rs.getString("CommentTime"),
                        rs.getInt("Fire"));
                comments.add(comment);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return comments;
    }
}
