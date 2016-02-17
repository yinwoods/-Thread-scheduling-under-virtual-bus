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

int i;
static int device_num = 3;
static int global_status[10] = {0, 1, 2};

module_param(device_num, int, 0000);
module_param_array(global_status, int, &device_num, 0000);
//MODULE_PARM(device_num);

MODULE_LICENSE("GPL");

struct yinwoods_data {
    char a[4096];
    char *name;

    int left;   //算式左侧运算数
    int right;  //算式右侧运算数
    int result; //算式运算结果
    
    int status; //进程状态位

    char *mutex;//进程间交互变量
};

struct yinwoods_data s1 = {
    .name = "yinwoods_one",
    //主
    .status = 0,
    .left = 1,
    .right = 1,
    .mutex = "translate this message from device1 s1",
};

struct yinwoods_data s2 = {
    .name = "yinwoods_two",
    //热
    .status = 0,
    .left = 1,
    .right = 2,
    .mutex = "translate this message from device1 s2",
};

struct yinwoods_data s3 = {
    .name = "yinwoods_three",
    //冷
    .status = 2,
    .left = 2,
    .right = 2,
    .mutex = "translate this message from device1 s3",
};

static struct platform_device yinwoods_device[] = {
    [0] = {
        .name = "yinwoods",
        .id = 0,
        .dev = {
            .platform_data = &s1,
        },
    },

    [1] = {
        .name = "yinwoods",
        .id = 1,
        .dev = {
            .platform_data = &s2,
        },
    },

    [2] = {
        .name = "yinwoods",
        .id = 2,
        .dev = {
            .platform_data = &s3,
        },
    },
};

static int __init yinwoods_init(void) {

    sprintf(s1.a, "this message is from device1 s1 to driver!");
    sprintf(s2.a, "this message is from device1 s2 to driver!");
    sprintf(s3.a, "this message is from device1 s3 to driver!");

    printk(KERN_ALERT "device_num = %d", device_num);

    i = 0;
    for(i=0; i<device_num; ++i) {
        struct yinwoods_data *p = yinwoods_device[i].dev.platform_data;

        p->status = global_status[i];
        platform_device_register(&yinwoods_device[i]);
        //printk(KERN_ALERT " --- register device[%d]\n", i);
    }

    //platform_add_devices(yinwoods_device, ARRAY_SIZE(yinwoods_device));
    return 0;
}

static void __exit yinwoods_exit(void) {

    i = 0;
    for(i=0; i<device_num; ++i) {
        printk(KERN_ALERT " --- unregister device[%d]\n", i);
        //platform_device_unregister(&yinwoods_device[i]);
    }
}

module_init(yinwoods_init);
module_exit(yinwoods_exit);
