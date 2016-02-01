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

    int left;
    int right;
    int result;

    char *mutex;
};

struct yinwoods_data s = {
    .name = "yinwoods_one",
    .left = 1,
    .right = 1,
    .mutex = "translate this message from device1\n",
};

struct platform_device yinwoods_device = {
    .name = "yinwoods",
    .id = 1,
    .dev = {
        .platform_data = &s,
    },
};

static int __init yinwoods_init(void) {

    sprintf(s.a, "this message is from device1 to driver!\n");
    
    platform_device_register(&yinwoods_device);
    return 0;
}

static void __exit yinwoods_exit(void) {

    platform_device_unregister(&yinwoods_device);
}

module_init(yinwoods_init);
module_exit(yinwoods_exit);
