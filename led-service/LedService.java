package com.android.server;

import android.os.ILedService;

public class LedService extends ILedService.Stub {
    private static final String TAG = "LedService";

    public LedService(){  //当new LedService类的时候，就调用了这个构造方法，调用到了本地的open函数

        native_ledOpen();
    }

    public int ledCtrl(int which, int status)throws android.os.RemoteException {
        return native_ledCtrl(which, status);   //调用本地的ledCtrl函数
    }

    public static native int native_ledOpen();
    public static native int native_ledCtrl(int which, int status);
    //public static native void native_ledClose();
}