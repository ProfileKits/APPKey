package com.predictor.vijoz.jni;

import android.content.Context;

public class VIJOZJNI {
    static {
        System.loadLibrary("VIJOZ");
    }

    public static native boolean getToken(Context context, String key, int date, int type, int time);

    public static native boolean getPermission();

    public static native String getKey(Context context);

    public static native String getStartCode();

    public static native void exit();
}
