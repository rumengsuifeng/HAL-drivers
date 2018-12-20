
#define LOG_TAG "LedService"

#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"

#include <utils/misc.h>
#include <utils/Log.h>
#include <hardware_legacy/vibrator.h>

#include <stdio.h>


#include <stdlib.h>		//包含了malloc函数
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


namespace android
{
    static jint fd;

    jint ledOpen(JNIEnv *env , jobject cla)	//在JAVA文件中即使是空的参数也要在C文件这边有这两个参数，再加上定义整形数组类型的参数
    {
        ALOGI("native ledOpen...");		//打印函数，可以在studio的logcat栏看到输出

        fd = open("/dev/first", O_RDWR);
        if (fd < 0) {
            ALOGI("open device first...");//在stdio.h中定义
            exit(1);//在<stdlib.h>中定义
        }

        return 0;
    }

    void ledClose(JNIEnv *env , jobject cla)
    {
        ALOGI("native ledClose...");
        close(fd);//在<unistd.h>中定义
    }

    jint ledCtrl(JNIEnv *env , jobject cla, jint which, jint status)
    {
        ALOGI("native ledCtrl : %d, %d", which, status);
        int ret = ioctl(fd, which, status);

        return 0;
    }

    static const JNINativeMethod method_table[] = {
        {"native_ledOpen", "()I", (void *)ledOpen},			//第二个参数：()表示函数的参数，I代表函数的返回值是int
        //{"native_ledClose", "()V", (void *)ledClose},
        {"native_ledCtrl", "(II)I", (void *)ledCtrl},
    };


int register_android_server_LedService(JNIEnv *env)
{
    return jniRegisterNativeMethods(env, "com/android/server/LedService",
            method_table, NELEM(method_table));
}

};
