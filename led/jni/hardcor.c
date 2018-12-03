#include <stdio.h>
#include <jni.h>		//该头文件的路径在PC机(ubuntu)的/usr/lib/jvm/java-1.7.0-openjdk-amd64/include/路径
#include <stdlib.h>		//包含了malloc函数
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <android/log.h>	//liblog库



#if 0
typedef struct {
    char *name;          /* Java里调用的函数名 */
    char *signature;    /* JNI字段描述符, 用来表示Java里调用的函数的参数和返回值类型 */
    void *fnPtr;          /* C语言实现的本地函数 */
} JNINativeMethod;
#endif

static jint fd;

jint ledOpen(JNIEnv *env , jobject class)	//在JAVA文件中即使是空的参数也要在C文件这边有这两个参数，再加上定义整形数组类型的参数
{
	__android_log_print(ANDROID_LOG_DEBUG, "LEDDemo", "native ledOpen...");		//打印函数，可以在studio的logcat栏看到输出

	fd = open("/dev/first", O_RDWR);
	if (fd < 0) {
		__android_log_print(ANDROID_LOG_DEBUG, "LEDDemo", "open device first...");//在stdio.h中定义
		exit(1);//在<stdlib.h>中定义
	}

	return 0;
}

void ledClose(JNIEnv *env , jobject class)
{
	__android_log_print(ANDROID_LOG_DEBUG, "LEDDemo", "native ledClose...");
	close(fd);//在<unistd.h>中定义
}

jint ledCtrl(JNIEnv *env , jobject class, jint which, jint status)
{
	int ret = ioctl(fd, which, status);
	__android_log_print(ANDROID_LOG_DEBUG, "LEDDemo", "native ledCtrl : %d, %d, %d", which, status, ret);
	
	return 0;
}

static const JNINativeMethod methods[] = {
	{"ledOpen", "()I", (void *)ledOpen},			//第二个参数：()表示函数的参数，I代表函数的返回值是int
	{"ledClose", "()V", (void *)ledClose},	
	{"ledCtrl", "(II)I", (void *)ledCtrl},	
};

/* System.loadLibrary */
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *jvm, void *reserved)		//由jni.pdf文件中拷贝而来，P117
{
	JNIEnv *env;
	jclass cls;

	if ((*jvm)->GetEnv(jvm, (void **)&env, JNI_VERSION_1_4)) {	//获取运行环境
		return JNI_ERR; /* JNI version not supported */
	}
	cls = (*env)->FindClass(env, "com/example/administrator/hardware/HardCor");	//从运行环境中查找JNIDemo这个类
	if (cls == NULL) {
		return JNI_ERR;
	}

	/* 2. map java hello <-->c c_hello */
	if ((*env)->RegisterNatives(env, cls, methods, 3) < 0)	//建立映射，函数对照表在methods数组中，参数3表示数组中的项，目前只有3项，所以是3
		return JNI_ERR;

	return JNI_VERSION_1_4;		//返回环境版本号
}