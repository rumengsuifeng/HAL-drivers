1. driver�·ŵ���linux�µ�������Ҫ����linux�ں��µ�drivers/char�£��޸ĸ�Ŀ¼�µ�Makefile�ļ�,���һ��
    obj-M            += led2.o
   ���±���linux�ں˺Ͱ�׿sdk��������¼�̼�

2. �����б���HAL/led/jni/hardcor.c�ļ���
   arm-linux-gnueabi-gcc  -fPIC -shared hardcor.c -o libhardcor.so  -I /usr/lib/jvm/java-7-openjdk-amd64/include/ -nostdlib /home/root/cqa64_android_v5.1/android/prebuilts/ndk/9/platforms/android-21/arch-arm/usr/lib/libc.so -I /home/root/cqa64_android_v5.1/android/prebuilts/ndk/9/platforms/android-21/arch-arm/usr/include/ /home/root/cqa64_android_v5.1/android/prebuilts/ndk/9/platforms/android-21/arch-arm/usr/lib/liblog.so
   ��������Ժ󣬷ŵ�AndroidApp/app/libs/armeabiĿ¼��
   
3. Androidϵͳ��������֮�����ļ�ϵͳ��Ҫ��LED�������豸�ڵ��ִ��Ȩ�ޣ�chmod 777 /dev/first��Ҫ��ȻAPP�����в������ģ�ֱ�ӱ�����