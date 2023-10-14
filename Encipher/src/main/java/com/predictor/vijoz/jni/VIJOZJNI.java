package com.predictor.vijoz.jni;

import android.content.Context;

public class VIJOZJNI {
    static {
        System.loadLibrary("VIJOZ");
    }

    public static native boolean getToken(Context context, int date, int type, int time);

    public static native boolean getPermission();

    public static native void exit();
}
