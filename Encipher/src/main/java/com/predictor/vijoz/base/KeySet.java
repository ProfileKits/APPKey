package com.predictor.vijoz.base;

public class KeySet {
    String key;
    int date;
    Type type;
    TimeSource timeSource;
    //加密方式
    public enum Type {ALL,KEY,DATE,NONE}
    //获取时间方式
    public enum  TimeSource{LOC,NET}

    public KeySet(String key, int date,Type type,TimeSource timeSource) {
        this.key = key;
        this.date = date;
        this.type = type;
        this.timeSource = timeSource;
    }

    public String getKey() {
        return key;
    }

    public void setKey(String key) {
        this.key = key;
    }

    public int getDate() {
        return date;
    }

    public void setDate(int date) {
        this.date = date;
    }

    public Type getType() {
        return type;
    }

    public void setType(Type type) {
        this.type = type;
    }
}
