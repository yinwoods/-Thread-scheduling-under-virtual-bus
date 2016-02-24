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
int j;
static int device_num = 10;
static int global_status[11] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0};

module_param(device_num, int, 0000);
module_param_array(global_status, int, &device_num, 0000);

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

static void yinwoods_release(struct device *dev) {
    return;
}

struct yinwoods_data info0 = {/*{{{*/
    .name = "yinwoods_zero",
    //主
    .status = 0,
    .left = 1,
    .right = 0,
    .mutex = "translate this message from info0",
};

struct yinwoods_data info1 = {
    .name = "yinwoods_one",
    //热
    .status = 0,
    .left = 1,
    .right = 1,
    .mutex = "translate this message from info1",
};

struct yinwoods_data info2 = {
    .name = "yinwoods_two",
    //冷
    .status = 2,
    .left = 1,
    .right = 2,
    .mutex = "translate this message from info2",
};

struct yinwoods_data info3 = {
    .name = "yinwoods_three",
    .status = 0,
    .left = 1,
    .right = 3,
    .mutex = "translate this message from info3",
};

struct yinwoods_data info4 = {
    .name = "yinwoods_four",
    .status = 0,
    .left = 1,
    .right = 4,
    .mutex = "translate this message from info4",
};

struct yinwoods_data info5 = {
    .name = "yinwoods_five",
    .status = 0,
    .left = 1,
    .right = 5,
    .mutex = "translate this message from info5",
};

struct yinwoods_data info6 = {
    .name = "yinwoods_six",
    .status = 0,
    .left = 1,
    .right = 6,
    .mutex = "translate this message from info6",
};

struct yinwoods_data info7 = {
    .name = "yinwoods_seven",
    .status = 0,
    .left = 1,
    .right = 7,
    .mutex = "translate this message from info7",
};

struct yinwoods_data info8 = {
    .name = "yinwoods_eight",
    .status = 0,
    .left = 1,
    .right = 8,
    .mutex = "translate this message from info8",
};

struct yinwoods_data info9 = {
    .name = "yinwoods_nine",
    .status = 0,
    .left = 1,
    .right = 9,
    .mutex = "translate this message from info9",
};/*}}}*/

static struct platform_device yinwoods_device[] = {/*{{{*/
    [0] = {
        .name = "yinwoods",
        .id = 0,
        .dev = {
            .platform_data = &info0,
            .release = yinwoods_release,
        },
    },

    [1] = {
        .name = "yinwoods",
        .id = 1,
        .dev = {
            .platform_data = &info1,
            .release = yinwoods_release,
        },
    },

    [2] = {
        .name = "yinwoods",
        .id = 2,
        .dev = {
            .platform_data = &info2,
            .release = yinwoods_release,
        },
    },

    [3] = {
        .name = "yinwoods",
        .id = 3,
        .dev = {
            .platform_data = &info3,
            .release = yinwoods_release,
        },
    },

    [4] = {
        .name = "yinwoods",
        .id = 4,
        .dev = {
            .platform_data = &info4,
            .release = yinwoods_release,
        },
    },

    [5] = {
        .name = "yinwoods",
        .id = 5,
        .dev = {
            .platform_data = &info5,
            .release = yinwoods_release,
        },
    },

    [6] = {
        .name = "yinwoods",
        .id = 6,
        .dev = {
            .platform_data = &info6,
            .release = yinwoods_release,
        },
    },

    [7] = {
        .name = "yinwoods",
        .id = 7,
        .dev = {
            .platform_data = &info7,
            .release = yinwoods_release,
        },
    },

    [8] = {
        .name = "yinwoods",
        .id = 8,
        .dev = {
            .platform_data = &info8,
            .release = yinwoods_release,
        },
    },

    [9] = {
        .name = "yinwoods",
        .id = 9,
        .dev = {
            .platform_data = &info9,
            .release = yinwoods_release,
        },
    },
};
/*}}}*/
static int __init yinwoods_init(void) {

    sprintf(info0.a, "this message is from info0 to driver!");
    sprintf(info1.a, "this message is from info1 to driver!");
    sprintf(info2.a, "this message is from info2 to driver!");
    sprintf(info3.a, "this message is from info3 to driver!");
    sprintf(info4.a, "this message is from info4 to driver!");
    sprintf(info5.a, "this message is from info5 to driver!");
    sprintf(info6.a, "this message is from info6 to driver!");
    sprintf(info7.a, "this message is from info7 to driver!");
    sprintf(info8.a, "this message is from info8 to driver!");
    sprintf(info9.a, "this message is from info9 to driver!");

    printk(KERN_ALERT "device_num = %d", device_num);

    i = 0;


    for(i=0; i<device_num; ++i) {

        struct yinwoods_data *p = yinwoods_device[i].dev.platform_data;
        j = 0;
        for(j=0; j<device_num; ++j) {
            printk(KERN_ALERT "global_status[%d] = %d\n", j, global_status[j]);
        }

        printk(KERN_ALERT "global_status[%d] = %d\n", i, global_status[i]);
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
        platform_device_unregister(&yinwoods_device[i]);
    }
}

module_init(yinwoods_init);
module_exit(yinwoods_exit);
