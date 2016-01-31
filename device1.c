#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/wait.h>

MODULE_LICENSE("GPL");

struct yinwoods_data {
    char a[4096];
    char *name;
};

static struct task_struct *_tsk1;
static struct task_struct *_tsk2;

static int tc = 0;
static wait_queue_head_t log_wait_queue;

struct yinwoods_data s = {
    .name = "yinwoods_one",
};

struct platform_device yinwoods_device = {
    .name = "yinwoods",
    .id = 1,
    .dev = {
        .platform_data = &s,
    },
};

static int thread_function_1(void *data) {
    do {
        printk(KERN_ALERT "In thread_function_1 thread_function : %d times FROM DEVICE1\n", tc);
        wait_event_interruptible(log_wait_queue, tc == 10);
        tc = 0;
        sprintf(s.a, "has been woke up ! FROM DEVICE1\n");
    } while(!kthread_should_stop());
    return tc;
}

static int thread_function_2(void *data) {
    do {
        printk(KERN_ALERT "In thread_function_2 thread_function : %d times FROM DEVICE1\n", ++tc);
        if(tc == 10 && waitqueue_active(&log_wait_queue)) {
            wake_up_interruptible(&log_wait_queue);
        }
        msleep_interruptible(1000);
    } while(!kthread_should_stop());
    return tc;
}

static int __init yinwoods_init(void) {

    sprintf(s.a, "this message is from device1 to driver! FROM DEVICE1\n");
    
    init_waitqueue_head(&log_wait_queue);
    _tsk1 = kthread_run(thread_function_1, NULL, "mythread1");

    if(IS_ERR(_tsk1)) {
        printk(KERN_ALERT "first create kthread failed! FROM DEVICE1\n");
    } else {
        printk(KERN_ALERT "first create kthread succeed FROM DEVICE1\n");
    }

    _tsk2 = kthread_run(thread_function_2, NULL, "muthread2");

    if(IS_ERR(_tsk2)) {
        printk(KERN_ALERT "second create kthread failed! FROM DEVICE1\n");
    } else {
        printk(KERN_ALERT "second create kthread succeed FROM DEVICE1\n");
    }

    platform_device_register(&yinwoods_device);
    return 0;
}

static void __exit yinwoods_exit(void) {

    printk(KERN_ALERT "yinwoods_exit! FROM DEVICE1\n");

    if(!IS_ERR(_tsk1)) {
        int ret = kthread_stop(_tsk1);
        printk(KERN_ALERT "First thread function_1 has stopped, return %d\n FROM DEVICE1", ret);
    }

    if(!IS_ERR(_tsk2)) {
        int ret = kthread_stop(_tsk2);
        printk(KERN_ALERT "Second thread function_2 has stopped, return %d\n FROM DEVICE1", ret);
    }

    platform_device_unregister(&yinwoods_device);
}

module_init(yinwoods_init);
module_exit(yinwoods_exit);
