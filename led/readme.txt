1. driver下放的是linux下的驱动，要放入linux内核下的drivers/char下，修改该目录下的Makefile文件,添加一行
    obj-M            += led2.o
   重新编译linux内核和安卓sdk，生成烧录固件

2. 命令行编译HAL/led/jni/hardcor.c文件：
   arm-linux-gnueabi-gcc  -fPIC -shared hardcor.c -o libhardcor.so  -I /usr/lib/jvm/java-7-openjdk-amd64/include/ -nostdlib /home/root/cqa64_android_v5.1/android/prebuilts/ndk/9/platforms/android-21/arch-arm/usr/lib/libc.so -I /home/root/cqa64_android_v5.1/android/prebuilts/ndk/9/platforms/android-21/arch-arm/usr/include/ /home/root/cqa64_android_v5.1/android/prebuilts/ndk/9/platforms/android-21/arch-arm/usr/lib/liblog.so
   编译完毕以后，放到AndroidApp/app/libs/armeabi目录下
   
3. Android系统运行起来之后，在文件系统中要给LED驱动的设备节点可执行权限：chmod 777 /dev/first，要不然APP是运行不起来的，直接崩溃掉