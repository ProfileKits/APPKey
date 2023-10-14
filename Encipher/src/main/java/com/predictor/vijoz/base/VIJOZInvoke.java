package com.predictor.vijoz.base;

import android.annotation.SuppressLint;
import android.content.Context;

import com.predictor.vijoz.jni.VIJOZJNI;

/**
 * 工具类初始化 主要识初始化CNBaseTools 和 CNImageTools
 */
public class VIJOZInvoke {
    private static VIJOZInvoke invoke;
    private static final Object LockThis = new Object();
    @SuppressLint("StaticFieldLeak")
    private static Context context;

    public synchronized static VIJOZInvoke getInstance() {
        synchronized (LockThis) {
            if (null == invoke) {
                invoke = new VIJOZInvoke();
            }
        }
        return invoke;
    }


    public boolean init(Context context,  int date, int type, int time) {
        this.context = context;
        boolean k = VIJOZJNI.getToken(context,  date, type, time);
        return k;
    }

    public void exit(){
        VIJOZJNI.exit();
    }


    /**
     * 获取ApplicationContext
     *
     * @return ApplicationContext
     */
    public static Context getContext() {
        if (context != null) return context;
        throw new NullPointerException("u should init first");
    }
}
