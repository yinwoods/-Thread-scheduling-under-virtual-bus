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

    int left;   //算式左侧运算数
    int right;  //算式右侧运算数
    int result; //算式运算结果
    
    int status; //进程状态位

    char *mutex;//进程间交互变量
};

struct yinwoods_data s = {
    .name = "yinwoods_one",
    .status = 0,
    .left = 1,
    .right = 1,
    .mutex = "translate this message from device1\n",
};

struct platform_device yinwoods_device[] = {
    [0] = { 
        .name = "yinwoods",
        .id = 1,    
        .dev = {
            .platform_data = &s,
        },
    },

    [1] = {
        .name = "yinwoods",
        .id = 2,
        .dev = {
            .platform_data = &s,
        },
    },
};

static int __init yinwoods_init(void) {

    sprintf(s.a, "this message is from device1 to driver!\n");

    platform_device_register(&yinwoods_device[0]);
    platform_device_register(&yinwoods_device[1]);
    //platform_add_devices(yinwoods_device, ARRAY_SIZE(yinwoods_device));
    
    //platform_device_register(&yinwoods_device);
    return 0;
}

static void __exit yinwoods_exit(void) {

    platform_device_unregister(&yinwoods_device[0]);
    platform_device_unregister(&yinwoods_device[1]);
    //platform_device_unregister(&yinwoods_device);
}

module_init(yinwoods_init);
module_exit(yinwoods_exit);
