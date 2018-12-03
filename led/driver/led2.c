#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/gpio.h>

#define FIRST_DEV MKDEV(182, 0)
/*
为定义在<linux/kdev.h>中的宏
MKDEV(int major,int minor)
将主设备号和次设备号转换成dev_t类型

*/
static struct cdev firstdrv_cdev;
//static struct class_device *firstdrv_class_dev;

static int firstdrv_open(struct inode *inode, struct file *file)
{
    printk("led drivers firstdrv_open!\n");
	
	if (0 != gpio_direction_output(362, 1)) {
		printk("LED-D5-PL10! gpio_direction_output fail !");
		return -1;
	}	
	if (0 != gpio_direction_output(364, 1)) {
		printk("LED-D6-PL12! gpio_direction_output fail !");
		return -1;
	}	
	if (0 != gpio_direction_output(120, 0)) {
		printk("BEEP-Q1-PD24! gpio_direction_output fail !");
		return -1;
	}	
	
    return 0;
}

static long  firstdrv_ioctl(struct file *filep, unsigned int which,
                unsigned long status)
{
	printk("led drivers led_ioctl!\n");
	__gpio_set_value(which, status);
    return 0;
}

/*
static int  firstdrv_close(struct inode *inode, struct file *file)
{
	printk("led drivers led_close!");
	gpio_free(362);
	gpio_free(364);
	gpio_free(120);
	
	return 0;
}
*/

static struct class firstdrv_class = {
    .name        = "firstdrv_class",
};

static struct file_operations firstdrv_ops = {
    .owner 			= THIS_MODULE,
    .open 			= firstdrv_open,
//	.release		= firstdrv_close, 
    .compat_ioctl	= firstdrv_ioctl,		//重点需要看的地方
};
static int firstdrv_init(void)
{
    int ret;

    ret = register_chrdev_region(FIRST_DEV, 1, "firstdrv");
/*
<linux/fs.h>
int register_chrdev_region(dev_t first, unsigned int count, char *name);
first是你要分配的起始设备编号
count
是你请求的连续设备编号的总数. 注意, 如果 count 太大, 你要求的范围可能溢出到下一个主设备号;
但是只要你要求的编号范围可用, 一切都仍然会正确工作
name
是应当连接到这个编号范围
的设备的名字; 它会出现在 /proc/devices 和 sysfs 中.
*/
    if (ret) {
        printk(KERN_ERR "Unable to register firstdrv\n");
        goto err_reg;
    }
    cdev_init(&firstdrv_cdev, &firstdrv_ops);
    ret = cdev_add(&firstdrv_cdev, FIRST_DEV, 1);
    if (ret) {
        printk(KERN_ERR "Unable to add cdev\n");
        goto err_add_cdev;
    }
    class_register(&firstdrv_class);
    /*
注册一个类，使mdev可以在"/dev/"目录下 面建立设备节点
通过class.subsys.kset.kobj将class注册到层次结构中去*/
    /* /dev/first */
    device_create(&firstdrv_class, NULL, FIRST_DEV, NULL, "first");
/*
device_create(mydriver_class, NULL, MKDEV(MYDRIVER_Major, 0), NULL, DEVICE_NAME);
节点名为DEVICE_NAME
对于字符设备驱动，当file_operations注册好了之后，这只是实现了该驱动的内部功能，
我们还要向用户提供操作接口，也就是在/dev/目录下创建字符设备节点，
其中device_create函数就用于实现该功能
*/
	if(0 != gpio_request(362, "LED-D5-PL10!")) {			//LED的驱动标号计算公式是(x * 32) + 10, x是A的时候就是0，x是B的时候就是1，依次类推
		printk("LED-D5-PL10! gpio_request fail !");			//本例子用的是L，所以是11 * 32; 10就是后边跟的标号，加上10所以就是362
		return -1;
	}
	if(0 != gpio_request(364, "LED-D5-PL12!")) {
		printk("LED-D6-PL12! gpio_request fail !");	
		return -1;
	}
	if(0 != gpio_request(120, "BEEP-D5-PD24!")) {
		printk("BEEP-Q1-PD24! gpio_request fail !");	
		return -1;
	}

    return 0;
    
err_add_cdev:
    cdev_del(&firstdrv_cdev);
err_reg:
    unregister_chrdev_region(FIRST_DEV, 1);
    return 0;
}

static void firstdrv_exit(void)
{
    device_destroy(&firstdrv_class, FIRST_DEV);
    class_destroy(&firstdrv_class);
    cdev_del(&firstdrv_cdev);
    unregister_chrdev_region(FIRST_DEV, 1);
	gpio_free(362);
	gpio_free(364);
	gpio_free(120);
}
module_init(firstdrv_init);
module_exit(firstdrv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Angrad Young");
