package com.predictor.vijoz.base;

import android.app.Application;
import android.content.Context;
import android.util.Log;
import android.widget.Toast;

import java.net.URL;
import java.net.URLConnection;


public abstract class KeyApplication extends Application {
    public abstract KeySet setKey();
    public abstract void initData();

    @Override
    public void onCreate() {
        super.onCreate();
        initParams();
        initData();
    }

    private void getNetTime(Context context, KeySet set) {
        new Thread(() -> {
            try {
                int type = (set.getType() == KeySet.Type.KEY) ? 1 : ((set.getType() == KeySet.Type.DATE) ? 2 : ((set.getType() == KeySet.Type.NONE) ? 3 : 0));
                URL url = new URL("http://www.baidu.com");
                URLConnection uc = url.openConnection();//生成连接对象
                uc.connect(); //发出连接
                int time = (int) (uc.getDate() / 1000); //取得网站日期时间
                boolean key = VIJOZInvoke.getInstance().init(context, set.getTime(), type, time);
                Log.i("key:", key + "");
            } catch (Exception e) {
                e.printStackTrace();
            }
        }).start();
    }

    private void initParams() {
        KeySet set = setKey();
        if (set.timeSource == KeySet.TimeSource.LOC) {
            int type = (set.getType() == KeySet.Type.KEY) ? 1 : ((set.getType() == KeySet.Type.DATE) ? 2 : ((set.getType() == KeySet.Type.NONE) ? 3 : 0));
            boolean key = VIJOZInvoke.getInstance().init(this, set.getTime(), type, -1);
            Log.i("key--is:", key + "");
        } else {
            if (HttpUtil.isNetConnected(this)) {
                getNetTime(this, set);
            }else{
                Toast.makeText(this,"请打开网络！！",Toast.LENGTH_LONG).show();
                VIJOZInvoke.getInstance().exit();
            }
        }
    }
}