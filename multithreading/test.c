#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kthread.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

static struct task_struct * _tsk;
static struct task_struct * _tsk1;

static int tc = 0;
static wait_queue_head_t log_wait_queue;

static int thread_function_1(void *data) {
    do {
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
    _tsk = kthread_run(thread_function_1, NULL, "mythread1");
    if(IS_ERR(_tsk)) {
        printk(KERN_INFO "first create kthread failed!\n");
    } else {
        printk(KERN_INFO "first create kthread ok!\n");
    }

    _tsk1 = kthread_run(thread_function_2, NULL, "mythread2");
    if(IS_ERR(_tsk1)) {
        printk(KERN_INFO "second create kthread failed!\n");
    } else {
        printk(KERN_INFO "second create kthread ok !\n");
    }
    return 0;
}

static void hello_exit(void) {
    printk(KERN_INFO "Hello, exit!\n");
    if(!IS_ERR(_tsk)) {
        int ret = kthread_stop(_tsk);
        printk(KERN_INFO "First thread function has stopped, return %d\n", ret);
    }
    if(!IS_ERR(_tsk1)) {
        int ret = kthread_stop(_tsk1);
        printk(KERN_INFO "Second thread function has stopped, return %d\n", ret);
    }
}

module_init(hello_init);
module_exit(hello_exit);
