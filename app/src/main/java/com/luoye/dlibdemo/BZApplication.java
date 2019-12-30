package com.luoye.dlibdemo;

import android.app.Application;

/**
 * Created by zhandalin on 2019-12-30 16:57.
 * description:
 */
public class BZApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        System.loadLibrary("bzfacetrack");
    }
}
