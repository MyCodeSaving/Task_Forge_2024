package com.example.demo.servlet;

import java.io.IOException; 
import java.io.PrintWriter;
import java.util.List;

import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.example.demo.controller.CommentThing;
import com.example.demo.entity.Comment;

@WebServlet("/CommentServlet")
public class CommentServlet extends HttpServlet {
    private final CommentThing commentThing = new CommentThing();

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException {
        response.setContentType("application/json");
        response.setCharacterEncoding("UTF-8");
        PrintWriter out = response.getWriter();

        String action = request.getParameter("action");
        try {
            switch (action) {
                case "addComment":
                    int userId = Integer.parseInt(request.getParameter("userId"));
                    int psId = Integer.parseInt(request.getParameter("psId"));
                    String content = request.getParameter("content");
                    out.write("{\"success\": " + (commentThing.addComment(userId, psId, content) > 0) + "}");
                    break;
                case "updateComment":
                    int commentId = Integer.parseInt(request.getParameter("commentId"));
                    String newContent = request.getParameter("newContent");
                    out.write("{\"success\": " + (commentThing.updateComment(commentId, newContent) > 0) + "}");
                    break;
                case "searchComments":
                    psId = Integer.parseInt(request.getParameter("psId"));
                    int page = Integer.parseInt(request.getParameter("page"));
                    int pageSize = Integer.parseInt(request.getParameter("pageSize"));
                    List<Comment> comments = commentThing.searchCommentsWithPagination(psId, page, pageSize);
                    out.write(commentsToJson(comments));
                    break;
                default:
                    out.write("{\"error\": \"Invalid action\"}");
            }
        } catch (Exception e) {
            e.printStackTrace();
            out.write("{\"error\": \"An error occurred\"}");
        }
    }

    private String commentsToJson(List<Comment> comments) {
        StringBuilder json = new StringBuilder("[");
        for (Comment comment : comments) {
            json.append("{")
                .append("\"commentId\": ").append(comment.getCommentID()).append(",")
                .append("\"userId\": ").append(comment.getUserID()).append(",")
                .append("\"psId\": ").append(comment.getPsID()).append(",")
                .append("\"content\": \"").append(comment.getCommentContent()).append("\",")
                .append("\"time\": \"").append(comment.getCommentTime()).append("\",")
                .append("\"fire\": ").append(comment.getFire())
                .append("},");
        }
        if (!comments.isEmpty()) {
            json.deleteCharAt(json.length() - 1);
        }
        json.append("]");
        return json.toString();
    }
}
