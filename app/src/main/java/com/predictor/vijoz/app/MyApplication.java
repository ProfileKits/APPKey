package com.predictor.vijoz.app;

import com.predictor.vijoz.base.KeyApplication;
import com.predictor.vijoz.base.KeySet;

//-------------------------APP加密处理方法，用于APP打包测试的使用限制---------------------------------------
//两种加密方式 1、打包时使用配套的keystore的key密钥加密 2、限制APP的使用时间
public class MyApplication extends KeyApplication {

    @Override
    public KeySet setKey() {
        //TODO 使用说明。
        // 第一个参数是截止时间，要转换成时间戳，例如：1696089600（2023-10-1）。
        // 第二个参数是加密方法，ALL为两种加密方式同时使用 KEY为只使用签名方式加密 DATE只使用日期方式加密 NONE不加密。
        // 第三个参数是获取当前时间的方式，LOC为获取手机时间，NET为获取网络时间（注意这种方式需要联网且会有延时）
        return new KeySet(1700000000, KeySet.Type.ALL, KeySet.TimeSource.NET);
    }

    @Override
    public void initData() {
        //TODO 无需重写onCreate方法，初始化内容写在这个方法里。
        // 之前有onCreate方法的可以删掉，把里面的内容复制到这个初始化方法中来。

    }

}
