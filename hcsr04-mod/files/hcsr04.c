#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>
#include <asm/div64.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/rtc.h>
#include <linux/string.h>

#define GPIO_OUT 20
#define GPIO_IN 21

static dev_t hcsr04_dev;
struct cdev hcsr04_cdev;
static int hcsr04_lock = 0;
static struct kobject *hcsr04_kobject;

static ktime_t rising, falling;
struct timespec time;
struct rtc_time tm;

static ssize_t hcsr04_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);

static ssize_t hcsr04_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    int pulse = (int)ktime_to_us(ktime_sub(falling, rising));
    return sprintf(buf, "Time: %04d-%02d-%02d %02d:%02d:%02d;\nPulse Distance=%d us\nDistance=%d cm", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, pulse, pulse / 58);
}

int hcsr04_open(struct inode *inode, struct file *file)
{
    int ret = 0;
    if (hcsr04_lock > 0)
        ret = -EBUSY;
    else
        hcsr04_lock++;
    return (ret);
}

int hcsr04_close(struct inode *inode, struct file *file)
{
    hcsr04_lock = 0;
    return (0);
}

ssize_t hcsr04_write(struct file *filp, const char *buffer, size_t length, loff_t *offset)
{
    gpio_set_value(GPIO_OUT, 0);
    gpio_set_value(GPIO_OUT, 1);
    udelay(10);
    gpio_set_value(GPIO_OUT, 0);
    while (gpio_get_value(GPIO_IN) == 0)
        ;
    rising = ktime_get();
    while (gpio_get_value(GPIO_IN) == 1)
        ;
    falling = ktime_get();
    return (1);
}

ssize_t hcsr04_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    int ret;
    int pulse;
    char timeChar[64];
    unsigned long local_time;
    getnstimeofday(&time);
    local_time = (u32)(time.tv_sec - (sys_tz.tz_minuteswest * 60));
    rtc_time_to_tm(local_time, &tm);
    pulse = (int)ktime_to_us(ktime_sub(falling, rising));
    sprintf(timeChar, "Time: %04d-%02d-%02d %02d:%02d:%02d\nPulseDistance=%dus\nDistance=%dcm", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, pulse, pulse / 58);
    ret = copy_to_user(buf, timeChar, 64);
    return 64;
}

static struct kobj_attribute hcsr04_attribute = __ATTR(hcsr04, 0660, hcsr04_show, hcsr04_store);

static ssize_t hcsr04_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    return 1;
}

struct file_operations hcsr04_fops = {
    .owner = THIS_MODULE,
    .read = hcsr04_read,
    .write = hcsr04_write,
    .open = hcsr04_open,
    .release = hcsr04_close,
};

static int __init hcsr04_module_init(void)
{
    char buffer[64];
    alloc_chrdev_region(&hcsr04_dev, 0, 1, "hcsr04_dev");
    printk(KERN_INFO "%s\n", format_dev_t(buffer, hcsr04_dev));
    cdev_init(&hcsr04_cdev, &hcsr04_fops);
    hcsr04_cdev.owner = THIS_MODULE;
    cdev_add(&hcsr04_cdev, hcsr04_dev, 1);
    gpio_request(GPIO_OUT, "hcsr04_dev");
    gpio_request(GPIO_IN, "hcsr04_dev");
    gpio_direction_output(GPIO_OUT, 0);
    gpio_direction_input(GPIO_IN);
    hcsr04_kobject = kobject_create_and_add("hcsr04", kernel_kobj);
    sysfs_create_file(hcsr04_kobject, &hcsr04_attribute.attr);
    return 0;
}

static void __exit hcsr04_module_cleanup(void)
{
    gpio_free(GPIO_OUT);
    gpio_free(GPIO_IN);
    hcsr04_lock = 0;
    cdev_del(&hcsr04_cdev);
    unregister_chrdev_region(hcsr04_dev, 1);
    kobject_put(hcsr04_kobject);
}

module_init(hcsr04_module_init);
module_exit(hcsr04_module_cleanup);
MODULE_AUTHOR("Michael Adu");
MODULE_LICENSE("GPL");
