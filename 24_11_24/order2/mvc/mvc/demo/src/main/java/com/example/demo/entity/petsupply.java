package com.example.demo.entity;

import lombok.Data;

import java.awt.image.BufferedImage;

@Data
public class petsupply {
    public int PsID;
    public String PsName;
    public BufferedImage PsPicture;
    public int PsPrice;
    public String PsIntroduction;
    public String PsType;
    public int PsGetScore;
    public int PsNeedScore;
    public int PsInventory;

    public petsupply(int PsID, String PsName, BufferedImage PsPicture, int PsPrice, String PsIntroduction, String PsType, int PsGetScore, int PsNeedScore, int PsInventory) {
        this.PsID = PsID;
        this.PsName = PsName;
        this.PsPicture = PsPicture;
        this.PsPrice = PsPrice;
        this.PsIntroduction = PsIntroduction;
        this.PsType = PsType;
        this.PsGetScore = PsGetScore;
        this.PsNeedScore = PsNeedScore;
        this.PsInventory = PsInventory;
    }
    public int getPsID() {
        return PsID;
    }

    public void setPsID(int psID) {
        PsID = psID;
    }

    public String getPsName() {
        return PsName;
    }

    public void setPsName(String psName) {
        PsName = psName;
    }

    public BufferedImage getPsPicture() {
        return PsPicture;
    }

    public void setPsPicture(BufferedImage psPicture) {
        PsPicture = psPicture;
    }

    public int getPsPrice() {
        return PsPrice;
    }

    public void setPsPrice(int psPrice) {
        PsPrice = psPrice;
    }

    public String getPsIntroduction() {
        return PsIntroduction;
    }

    public void setPsIntroduction(String psIntroduction) {
        PsIntroduction = psIntroduction;
    }

    public String getPsType() {
        return PsType;
    }

    public void setPsType(String psType) {
        PsType = psType;
    }

    public int getPsGetScore() {
        return PsGetScore;
    }

    public void setPsGetScore(int psGetScore) {
        PsGetScore = psGetScore;
    }

    public int getPsNeedScore() {
        return PsNeedScore;
    }

    public void setPsNeedScore(int psNeedScore) {
        PsNeedScore = psNeedScore;
    }

    public int getPsInventory() {
        return PsInventory;
    }

    public void setPsInventory(int psInventory) {
        PsInventory = psInventory;
    }
}
