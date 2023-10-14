package com.predictor.vijoz.base;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkInfo;
import android.os.Build;
import android.text.TextUtils;
import android.util.Log;

import androidx.annotation.WorkerThread;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.concurrent.TimeUnit;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

//网络判断权限
public class HttpUtil {
    public static final String REGEX_URL = "((((ht|f)tp(s?))\\:\\/\\/)([\\w\\-]{1,63})(\\.[\\w\\-]{1,63})+|([\\w\\-]{1,63}\\.)+(com|cn|cc|top|xyz|edu|gov|mil|net|org|biz|info|name|museum|us|ca|uk))(\\:\\d+)?(\\/([\\w_\\-\\.~!*\\'()\\;\\:@&=+&$,/?#%]*))*";
    private static final String TAG = "HttpUtil";

    public static boolean isURL(String text) {
        return getURLList(text).size() > 0;
    }

    /**
     * url 解码
     * @param schemeUrl url
     * @return 解码url
     */
    public static String decodeURL(String schemeUrl) {
        try {
            return URLDecoder.decode(schemeUrl, "UTF-8");
        } catch (UnsupportedEncodingException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return schemeUrl;
    }

    /**
     * url 编码
     * @param schemeUrl url
     * @return 编码url
     */
    public static String encodeURL(String schemeUrl) {
        try {
            return URLEncoder.encode(schemeUrl, "UTF-8");
        } catch (UnsupportedEncodingException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        ;
        return schemeUrl;
    }

    /**
     * 返回带参数的get请求url地址
     * @param url url
     * @param params 参数
     * @return 带参数的get请求url地址
     */
    public static String getURLWithParams(String url, Map<String, String> params){
        return url+"?"+joinParam(params);
    }

    /**
     * 连接参数
     * @param params 参数
     * @return 连接结果
     */
    private static StringBuffer joinParam(Map<String, String> params) {
        StringBuffer result = new StringBuffer();
        Iterator<Map.Entry<String, String>> iterator = params.entrySet().iterator();
        while (iterator.hasNext()) {
            Map.Entry<String, String> param = iterator.next();
            String key = param.getKey();
            String value = param.getValue();
            result.append(key).append('=').append(value);
            if (iterator.hasNext()) {
                result.append('&');
            }
        }
        return result;
    }

    public static List<String> getURLList(String str) {
        List<String> URLListStr = new ArrayList<>();
        // Pattern pattern = Patterns.WEB_URL;// 系统检测URL的正则
        Pattern pattern = Pattern.compile(REGEX_URL);
        Matcher matcher = pattern.matcher(str);
        StringBuilder stringBuffer = new StringBuilder();
        String s;
        while (matcher.find()) {
            s = matcher.group();
            stringBuffer.append(s);
        }
        if (TextUtils.isEmpty(stringBuffer.toString()))
            return URLListStr;

        String[] split = stringBuffer.toString().split("(http|https)://");
        for (String aSplit : split) {
            Log.e("html", aSplit);
            String mFilterChineseStr = filterChinese(aSplit);
            if (!TextUtils.isEmpty(mFilterChineseStr)) {
                URLListStr.add("http://" + mFilterChineseStr);
            }
        }
        return URLListStr;
    }

    // 过滤掉中文
    private static String filterChinese(String str) {
        String REGEX_CHINESE = "[\u4e00-\u9fa5]";
        Pattern pattern = Pattern.compile(REGEX_CHINESE);
        Matcher matcher = pattern.matcher(str);
        return matcher.replaceAll("");
    }

    /************************************************************/

    // 是否连接了网络
    public static boolean isNetConnected(Context context) {
        ConnectivityManager mConnectivityManager =
                (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        @SuppressLint("MissingPermission") NetworkInfo gprs = mConnectivityManager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
        @SuppressLint("MissingPermission") NetworkInfo wifi = mConnectivityManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
        boolean isConnectedGprs = gprs != null && gprs.isConnected();
        boolean isConnectedWifi = wifi != null && wifi.isConnected();
        return isConnectedGprs || isConnectedWifi;
    }

    // 是否连接了网络
    public static boolean isGprsOrWifiConnected(Context context) {
        ConnectivityManager mConnectivityManager =
                (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        @SuppressLint("MissingPermission") NetworkInfo gprs = mConnectivityManager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
        @SuppressLint("MissingPermission") NetworkInfo wifi = mConnectivityManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
        boolean isConnectedGprs = gprs != null && gprs.isConnected();
        boolean isConnectedWifi = wifi != null && wifi.isConnected();
        return isConnectedGprs || isConnectedWifi;
    }

    // 是否使用的4G网络
    public static boolean isConnectedGprs(Context context) {
        ConnectivityManager mConnectivityManager =
                (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        @SuppressLint("MissingPermission") NetworkInfo gprs = mConnectivityManager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
        boolean isConnectedGprs = gprs != null && gprs.isConnected();
        return isConnectedGprs;
    }

    /**
     * 判断网络是否连接
     *
     * @param mContext
     * @return
     */
    public static boolean checkNetworkInfo(Context mContext) {
        ConnectivityManager conMan = (ConnectivityManager) mContext.getSystemService(Context.CONNECTIVITY_SERVICE);
        @SuppressLint("MissingPermission") NetworkInfo.State mobile = conMan.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).getState();
        @SuppressLint("MissingPermission") NetworkInfo.State wifi = conMan.getNetworkInfo(ConnectivityManager.TYPE_WIFI).getState();
        if (mobile == NetworkInfo.State.CONNECTED || mobile == NetworkInfo.State.CONNECTING)
            return true;
        return wifi == NetworkInfo.State.CONNECTED || wifi == NetworkInfo.State.CONNECTING;
    }


    /**
     * 获取连接的网络状态
     *
     * @param context
     * @return
     */
    @SuppressLint("MissingPermission")
    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    public static int getNetworkState(Context context) {
        int result = NetWork.NONE;
        ConnectivityManager connManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);

        Network[] networks = new Network[0];
        networks = connManager.getAllNetworks();
        for (Network network : networks) {
            NetworkInfo networkInfo = connManager.getNetworkInfo(network);
            NetworkInfo.State state = networkInfo.getState();

            if (state == NetworkInfo.State.CONNECTED || state == NetworkInfo.State.CONNECTING) {
                if (networkInfo.getType() == ConnectivityManager.TYPE_WIFI) {
                    result = NetWork.WIFI;
                    break;
                } else if (networkInfo.getType() == ConnectivityManager.TYPE_MOBILE) {
                    result = NetWork.MOBILE;
                }
            }
        }
        return result;
    }


    public static class NetWork {
        public static final int NONE = 100;
        public static final int WIFI = 200;
        public static final int MOBILE = 300;
    }


    public static boolean isConnectedWifi(Context context) {
        ConnectivityManager mConnectivityManager =
                (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        @SuppressLint("MissingPermission") NetworkInfo wifi = mConnectivityManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
        boolean isConnectedWifi = wifi != null && wifi.isConnected();
        return isConnectedWifi;
    }

    // 判断是否有外网连接（普通方法不能判断外网的网络是否连接，比如连接上局域网）

    /**
     * ip == www.google.com 可以用来检测手机是否翻墙
     *
     * @param ip
     * @return
     */
    public static boolean ping(String ip) {
        String result = null;
        try {
            /**
             * -c 次数
             * -w 超时时长
             */
            Process p = Runtime.getRuntime().exec("ping -c 5 -w 2 " + ip);
            int status = p.waitFor();
            if (status == 0) {
                result = "success";
                return true;
            } else {
                result = "failed";
            }
        } catch (IOException e) {
            result = "IOException";
        } catch (InterruptedException e) {
            result = "InterruptedException";
        } finally {
            Log.d("----result---", "result = " + result);
        }
        return false;
    }

    /**
     * 通过204 No Content测试网络连接状态，
     * 比起ping测试，http受代理影响，
     * 204测试可以排除网络需要登录的干扰，
     * 墙内可用测试服务器 https://www.google.cn/generate_204
     * https://captive.v2ex.co/generate_204
     * https://httpbin.org/status/204
     * 墙外可用测试服务器 https://www.google.com/generate_204
     */
    @WorkerThread
    public static boolean test204(String url) {
        Log.i(TAG, "test204() called with: url = [" + url + "]");
        boolean ret;
        try {
            HttpURLConnection connection = (HttpURLConnection) (new URL(url).openConnection());
            // 5秒超时，墙内访问谷歌被墙会一直阻塞直到timeout,
            connection.setConnectTimeout((int) TimeUnit.SECONDS.toMillis(5));
            connection.setReadTimeout((int) TimeUnit.SECONDS.toMillis(5));
            ret = connection.getResponseCode() == 204;
            Log.i(TAG, "test204: " + url + " responseCode = " + connection.getResponseCode());
        } catch (Exception e) {
            Log.e(TAG, "test204 error: " + url, e);
            ret = false;
        }
        return ret;
    }

    @WorkerThread
    public static boolean testGoogle() {
        return test204("https://www.google.com/generate_204");
    }
}
