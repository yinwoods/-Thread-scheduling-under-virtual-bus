#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/wait.h>

MODULE_LICENSE("GPL");

struct yinwoods_data {
    char a[4096];
    char *name;
    struct device *dev;
};

struct class *yinwoods_class; 
struct class *yinwoods_class;

struct device *yinwoods_dev;
struct device *yinwoods_dev;

static ssize_t yinwoods_store(struct device *dev, struct device_attribute *attr, char *buf, size_t size) {
    if(size > 4096) {
        printk(KERN_ALERT "fuck you !");
        return 0;
    }
    return sprintf(((struct yinwoods_data *) dev->platform_data)->a, buf);
}

static ssize_t yinwoods_show(struct device *dev, struct device_attribute *attr, char *buf) {
    return sprintf(buf, ((struct yinwoods_data *)dev->platform_data)->a);
}

struct device_attribute dev_attr_brightness = {
    .attr = { .name = "love", .mode = 0644 },
    .show = yinwoods_show,
    .store = yinwoods_store
};

static int yinwoods_probe(struct platform_device *dev) {
    struct yinwoods_data *p = (dev->dev).platform_data;
    yinwoods_dev = device_create(yinwoods_class, &(dev->dev), 0, NULL, "%s", p->name);
    p->dev = yinwoods_dev;
    yinwoods_dev->platform_data = p;
    device_create_file(yinwoods_dev, &dev_attr_brightness);

    printk(KERN_ALERT "%s", p->a);
    return 0;
}

static int yinwoods_remove(struct platform_device *dev) {
    struct yinwoods_data *p = (dev->dev).platform_data;
    device_remove_file(p->dev, &dev_attr_brightness);
    device_unregister(p->dev);
    return 0;
}

static struct platform_driver yinwoods_driver = {
    .probe = yinwoods_data,
    .remove = yinwoods_remove,
    .driver = {
        .name = "yinwoods",
        .owner = THIS_MODULE,
    },
};

/*
static struct task_struct * _tsk1;
static struct task_struct * _tsk2;

static int tc = 0;
static wait_queue_head_t log_wait_queue;
*/

static int thread_function_1(void *data) {
    do {
        yinwoods_class = class_create(THIS_MODULE, "yinwoods");
        printk(KERN_INFO "In thread_function_1 thread_function : %d times \n", tc);
        wait_event_interruptible(log_wait_queue, tc == 10);
        tc = 0;
        printk(KERN_INFO "has been woke up !\n");
    } while(!kthread_should_stop());
    return tc;
}

static int thread_function_2(void *data) {
    do {
        printk(KERN_INFO "In thread_function_2 thread_function : %d times \n", ++tc);
        if(tc == 10 && waitqueue_active(&log_wait_queue)) {
            wake_up_interruptible(&log_wait_queue);
        }
        msleep_interruptible(1000);
    } while(!kthread_should_stop());
    return tc;
}

static int hello_init(void) {
    printk(KERN_INFO "Hello, world!\n");
    init_waitqueue_head(&log_wait_queue);
    _tsk1 = kthread_run(thread_function_1, NULL, "mythread1");
    if(IS_ERR(_tsk1)) {
        printk(KERN_INFO "first create kthread failed!\n");
    } else {
        printk(KERN_INFO "first create kthread ok!\n");
    }

    _tsk2 = kthread_run(thread_function_2, NULL, "mythread2");
    if(IS_ERR(_tsk2)) {
        printk(KERN_INFO "second create kthread failed!\n");
    } else {
        printk(KERN_INFO "second create kthread ok !\n");
    }
    return 0;
}

static void hello_exit(void) {
    printk(KERN_INFO "Hello, exit!\n");
    if(!IS_ERR(_tsk1)) {
        int ret = kthread_stop(_tsk1);
        printk(KERN_INFO "First thread function has stopped, return %d\n", ret);
    }
    if(!IS_ERR(_tsk2)) {
        int ret = kthread_stop(_tsk2);
        printk(KERN_INFO "Second thread function has stopped, return %d\n", ret);
    }
}

module_init(hello_init);
module_exit(hello_exit);
