#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/mm.h>

MODULE_LICENSE("GPL");

struct yinwoods_data {
    char a[4096];
    char *name;
};

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

static int __init yinwoods_init(void) {
    sprintf(s.a, "yinwoods very good!\n");

    platform_device_register(&yinwoods_device);
    return 0;
}

static void __exit yinwoods_exit(void) {
    platform_device_unregister(&yinwoods_device);
}

module_init(yinwoods_init);
module_exit(yinwoods_exit);
