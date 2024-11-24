package com.example.demo.entity;

import lombok.Data;

import java.util.Date;
@Data
public class score {
    private int RecordID;
    private int PsID;
    private int UserID;
    private int ScoreValue;
    private String RecordTime;

    public score(int recordID, int psID, int userID, int scoreValue, String recordTime) {
        this.RecordID = recordID;
        this.PsID = psID;
        this.UserID = userID;
        this.ScoreValue = scoreValue;
        this.RecordTime = recordTime;
    }

    public int getRecordID() {
        return RecordID;
    }

    public void setRecordID(int recordID) {
        RecordID = recordID;
    }

    public int getPsID() {
        return PsID;
    }

    public void setPsID(int psID) {
        PsID = psID;
    }

    public int getUserID() {
        return UserID;
    }

    public void setUserID(int userID) {
        UserID = userID;
    }

    public int getScoreValue() {
        return ScoreValue;
    }

    public void setScoreValue(int scoreValue) {
        ScoreValue = scoreValue;
    }

    public String getRecordTime() {
        return RecordTime;
    }

    public void setRecordTime(String recordTime) {
        RecordTime = recordTime;
    }

}
