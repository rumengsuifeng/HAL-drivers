1. 参考frameworks/base/core/java/android/os/IVibratorService.aidl文件，创建ILedService.aidl，修改frameworks/base/Android.mk文件，添加对ILedService.aidl文件的编译
		+	core/java/android/os/ILedService.aidl \

2. 执行命令mmm frameworks/base，生成out/target/common/obj/JAVA_LIBRARIES/framework_intermediates/src/core/java/android/os/ILedService.java文件

		问：为什么在这个目录下编译，是因为frameworks/base/core/java/android/os/目录下没有Android.mk文件，所以就逐级向上查找Anddoid.mk文件，在frameworks/base/Android.mk中有看到对aidl文件的编译

3. 参考frameworks/base/services/core/java/com/android/server/VibratorService.java文件，创建LedService.java文件，通过它调用到jni文件里的本地函数

4. 修改frameworks/base/services/java/com/android/server/SystemServer.java文件，添加以下内容，完成对LedService服务的注册

        +    Slog.i(TAG, "Led Service");
        +    ServiceManager.addService("led", new LedService());     //注册进系统，实际就是注册进service_manager.c文件
	
5. 参考frameworks/base/services/core/jni/com_android_server_VibratorService.cpp文件， 创建com_android_server_LedService.cpp文件，这部分属于JNI文件，在本文件中编写本地函数

6. 修改frameworks/base/services/core/jni/文件下的Android.mk文件，完成对com_android_server_VibratorService.cpp文件的编译
        
		+    $(LOCAL_REL_DIR)/com_android_server_LedService.cpp \
		
7. 修改frameworks/base/services/core/jni/onload.cpp文件，添加从JAVA到C的映射表(注册本地函数)

		+    int register_android_server_LedService(JNIEnv *env);
	    +    register_android_server_LedService(env);
		
8.  执行命令mmm frameworks/base/services/，完成对所有修改文件的编译。
   
      问：之所以在该目录下执行就能完成所有修改文件的编译，是因为frameworks/base/services/Android.mk文件里的内容：
	  
			LOCAL_SRC_FILES := $(call all-java-files-under,java)			//完成对当前目录下所有java文件的编译(递归调用同级目录下文件夹内的JAVA文件)
	   
			include $(wildcard $(LOCAL_PATH)/*/jni/Android.mk)
			
			include $(patsubst %,$(LOCAL_PATH)/%/Android.mk,$(services))
			
		  由于以上文件以及目录的修改涉及到三个目录：frameworks/base/services/java/com/android/server        和
													frameworks/base/services/core/java/com/android/server   和
													frameworks/base/services/core/jni 
													
		  那么，根据逐级向上找Android.mk文件的原则，frameworks/base/services/Android.mk文件是可以完成对所有修改文件的编译的
	   

调试该代码中遇到的问题：

	1. 安卓系统更改完毕以后，烧写到系统上，在开机动画播放完毕以后怎么都进入不了开机界面，而且稍等一会整个系统就会崩溃，经几天的摸索，发现是linux中led2.c在文件系统下生成的设备节点没有可读写执行权限导致，修改驱动节点
	
	默认权限的文件是system/core/rootdir/ueventd.rc，在这里面照葫芦画瓢写上
	
		/dev/first                0777   root       root
		
	即可。重新编译，烧写，没问题了。
	
	2. 在编写测试app的时候遇到的问题第一个是编译时候添加jar文件，当java文件里引用android.os.ILedService和android.os.ServiceManager的时候，虽然能编译过，但是程序源码界面上都是红色，看着很不美丽，解决这个问题的办法是
	
	把jar包放入app\libs目录下，再以jar dependency的形式添加进工程即可，颜色即可正常。第二个问题是发现HelloWorldActivity.java文件中的iLedService.ledCtrl并没有调用到Linux驱动led2.c中的ioctl函数，经过查资料得知驱动中需
	
	要添加.unlocked_ioctl函数定义，在这里，当app中的iLedService.ledCtrl执行以后，就会调用到linux系统中led2.c里的.unlocked_ioctl函数。
	
	
	
	

		