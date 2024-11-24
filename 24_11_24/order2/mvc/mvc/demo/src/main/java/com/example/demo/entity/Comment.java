package com.example.demo.entity;

import lombok.Data;
import org.w3c.dom.Text;

import java.util.Date;

@Data
public class Comment {
    public int CommentID;
    public int UserID;
    public int PsID;
    public String CommentContent;
    public String CommentTime;
    public int Fire;
    public Comment(int commentID, int userID, int psID, String commentContent, String commentTime, int fire) {
        this.CommentID = commentID;
        this.UserID = userID;
        this.PsID = psID;
        this.CommentContent = commentContent;
        this.CommentTime = commentTime; // 由于您声明它是public，这里直接赋值
        this.Fire = fire;
    }

    public int getCommentID() {
        return CommentID;
    }

    public void setCommentID(int commentID) {
        CommentID = commentID;
    }

    public int getUserID() {
        return UserID;
    }

    public void setUserID(int userID) {
        UserID = userID;
    }

    public int getPsID() {
        return PsID;
    }

    public void setPsID(int psID) {
        PsID = psID;
    }

    public String getCommentContent() {
        return CommentContent;
    }

    public void setCommentContent(String commentContent) {
        CommentContent = commentContent;
    }

    public String getCommentTime() {
        return CommentTime;
    }

    public void setCommentTime(String commentTime) {
        CommentTime = commentTime;
    }

    public int getFire() {
        return Fire;
    }

    public void setFire(int fire) {
        Fire = fire;
    }
}
