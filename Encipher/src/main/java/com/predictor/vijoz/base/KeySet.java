package com.predictor.vijoz.base;

public class KeySet {
    int time;
    Type type;
    TimeSource timeSource;
    //加密方式
    public enum Type {ALL,KEY,DATE,NONE}
    //获取时间方式
    public enum  TimeSource{LOC,NET}

    public KeySet(int time,Type type,TimeSource timeSource) {
        this.time = time;
        this.type = type;
        this.timeSource = timeSource;
    }

    public int getTime() {
        return time;
    }

    public void setTime(int time) {
        this.time = time;
    }

    public Type getType() {
        return type;
    }

    public void setType(Type type) {
        this.type = type;
    }
}
