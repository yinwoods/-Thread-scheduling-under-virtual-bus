#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/wait.h>

#include <asm/uaccess.h>

#ifndef MEMDEV_MAJOR 
#define MEMDEV_MAJOR 0
#endif

#ifndef MEMDEV_NR_DEVS
#define MEMDEV_NR_DEVS 1
#endif

#ifndef MEMDEV_SIZE
#define MEMDEV_SIZE 4096
#endif

int i;
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

    struct cdev cdev; //字符设备
};

struct yinwoods_data *p;

static void yinwoods_release(struct device *dev) {
    return;
}

struct yinwoods_data info0 = {
    .name = "yinwoods_zero",
    //主
    .status = 0,
    .left = 1,
    .right = 0,
};

struct yinwoods_data info1 = {
    .name = "yinwoods_one",
    //热
    .status = 0,
    .left = 1,
    .right = 1,
};

struct yinwoods_data info2 = {
    .name = "yinwoods_two",
    //冷
    .status = 2,
    .left = 1,
    .right = 2,
};

struct yinwoods_data info3 = {
    .name = "yinwoods_three",
    .status = 0,
    .left = 1,
    .right = 3,
};

struct yinwoods_data info4 = {
    .name = "yinwoods_four",
    .status = 0,
    .left = 1,
    .right = 4,
};

struct yinwoods_data info5 = {
    .name = "yinwoods_five",
    .status = 0,
    .left = 1,
    .right = 5,
};

struct yinwoods_data info6 = {
    .name = "yinwoods_six",
    .status = 0,
    .left = 1,
    .right = 6,
};

struct yinwoods_data info7 = {
    .name = "yinwoods_seven",
    .status = 0,
    .left = 1,
    .right = 7,
};

struct yinwoods_data info8 = {
    .name = "yinwoods_eight",
    .status = 0,
    .left = 1,
    .right = 8,
};

struct yinwoods_data info9 = {
    .name = "yinwoods_nine",
    .status = 0,
    .left = 1,
    .right = 9,
};

static struct platform_device yinwoods_device[] = {
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

//---------------------------------------------------

struct mem_dev {
    char *data;
    unsigned long size;
};

static int mem_major = MEMDEV_MAJOR;

module_param(mem_major, int, S_IRUGO);

struct mem_dev *mem_devp; //设备结构体指针

/*文件打开函数*/
int mem_open(struct inode *inode, struct file *filp) {
    struct mem_dev *dev;

    /*获取次设备号*/
    int num = MINOR(inode->i_rdev);

    if(num >= MEMDEV_NR_DEVS)
        return -ENODEV;
    dev = &mem_devp[num];

    /*将设备描述结构指针赋值给文件私有数据指针*/
    filp->private_data = dev;

    return 0;
}

/*文件释放函数*/
int mem_release(struct inode *inode, struct file *filp) {
    return 0;
}

/*读函数*/
static ssize_t mem_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos) {
    unsigned long p = *ppos; /*记录文件指针偏移位置*/
    unsigned int count = size; /*记录需要读取的字节数*/
    int ret = 0; /*返回值*/
    struct mem_dev *dev = filp->private_data; /*获得设备结构体指针*/

    /*判断读位置是否有效*/
    if(p >= MEMDEV_SIZE) /*要读取的偏移大于设备的内存空间*/
        return 0;
    if(count > MEMDEV_SIZE - p) /*要读取的字节大于设备的内存空间*/
        count = MEMDEV_SIZE - p;

    /*读数据到用户空间：内核空间->用户空间交换数据*/
    if(copy_to_user(buf, (void*)(dev->data + p), count))
        ret = -EFAULT;
    else {
        *ppos += count;
        ret = count;
        printk(KERN_INFO "read %u byte(s) from %lu\n", count, p);
    }
    return ret;
}

/*写函数*/
static ssize_t mem_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos) {
    unsigned long p = *ppos;
    unsigned int count = size;
    int ret = 0;
    struct mem_dev *dev = filp->private_data; /*获得设备结构体指针*/

    /*分析和获取有效的写长度*/
    if(p >= MEMDEV_SIZE)
        return 0;

    if(count > MEMDEV_SIZE - p) /*要写入的字节数大于内存空间*/
        count = MEMDEV_SIZE - p;

    /*从用户空间写入数据*/
    if(copy_from_user(dev->data + p, buf, count))
        ret = -EFAULT;
    else {
        *ppos += count; /*增加偏移位置*/
        ret = count; /*返回实际的写入字节数*/
        printk(KERN_INFO "written %u byte(s) from %lu\n", count, p);
    }
    return ret;
}

/*seek文件定位函数*/
static loff_t mem_llseek(struct file *filp, loff_t offset, int whence) {
    loff_t newpos;

    switch(whence) {
        case 0: /* SEEK_SET */ /*相对文件开始位置偏移*/
            newpos = offset; /*更新文件指针位置*/
            break;
        case 1: /* SEEK_CUR */ 
            newpos = filp->f_pos + offset;
            break;
        case 2: /* SEEK_END */
            newpos = MEMDEV_SIZE - 1 + offset;
            break;
        default: /* CANNOT HAPPEN */
            return -EINVAL;
    }

    if((newpos < 0) || (newpos > MEMDEV_SIZE))
        return -EINVAL;

    filp->f_pos = newpos;
    return newpos;
}

/* 文件操作结构体 */
static const struct file_operations mem_fops = {
    .owner = THIS_MODULE,
    .llseek = mem_llseek,
    .read = mem_read,
    .write = mem_write,
    .open = mem_open,
    .release = mem_release,
};

//---------------------------------------------------

/* 设备驱动模块加载函数 */
static int __init yinwoods_init(void) {

    int result;
    int i;

    dev_t devno = MKDEV(mem_major, 0);

    /* 申请设备号， 当×××_major不为0时， 表示静态指定；当为0时，表示动态申请*/
    /* 静态申请设备号 */
    if(mem_major)
        result = register_chrdev_region(devno, 2, "memdev");
    else {
        /* 动态分配设备号 */
        result = alloc_chrdev_region(&devno, 0, 2, "memdev");
        mem_major = MAJOR(devno); /* 获得申请的主设备号 */
    }

    if(result < 0)
        return result;

    //创建相应的字符设备
    //初始化cdev结构，并传递file_operations指针
    //指定所属模块
    cdev_init(&info0.cdev, &mem_fops);
    info0.cdev.owner = THIS_MODULE;
    info0.cdev.ops = &mem_fops;
    cdev_add(&info0.cdev, MKDEV(mem_major-1, 0), MEMDEV_NR_DEVS);

    cdev_init(&info1.cdev, &mem_fops);
    info1.cdev.owner = THIS_MODULE;
    info1.cdev.ops = &mem_fops;
    cdev_add(&info1.cdev, MKDEV(mem_major-2, 0), MEMDEV_NR_DEVS);

    cdev_init(&info2.cdev, &mem_fops);
    info2.cdev.owner = THIS_MODULE;
    info2.cdev.ops = &mem_fops;
    cdev_add(&info2.cdev, MKDEV(mem_major, 0), MEMDEV_NR_DEVS);

    cdev_init(&info3.cdev, &mem_fops);
    info3.cdev.owner = THIS_MODULE;
    info3.cdev.ops = &mem_fops;
    cdev_add(&info3.cdev, MKDEV(mem_major, 0), MEMDEV_NR_DEVS);

    cdev_init(&info4.cdev, &mem_fops);
    info4.cdev.owner = THIS_MODULE;
    info4.cdev.ops = &mem_fops;
    cdev_add(&info4.cdev, MKDEV(mem_major, 0), MEMDEV_NR_DEVS);

    cdev_init(&info5.cdev, &mem_fops);
    info5.cdev.owner = THIS_MODULE;
    info5.cdev.ops = &mem_fops;
    cdev_add(&info5.cdev, MKDEV(mem_major, 0), MEMDEV_NR_DEVS);

    cdev_init(&info6.cdev, &mem_fops);
    info6.cdev.owner = THIS_MODULE;
    info6.cdev.ops = &mem_fops;
    cdev_add(&info6.cdev, MKDEV(mem_major, 0), MEMDEV_NR_DEVS);

    cdev_init(&info7.cdev, &mem_fops);
    info7.cdev.owner = THIS_MODULE;
    info7.cdev.ops = &mem_fops;
    cdev_add(&info7.cdev, MKDEV(mem_major, 0), MEMDEV_NR_DEVS);

    cdev_init(&info8.cdev, &mem_fops);
    info8.cdev.owner = THIS_MODULE;
    info8.cdev.ops = &mem_fops;
    cdev_add(&info8.cdev, MKDEV(mem_major, 0), MEMDEV_NR_DEVS);

    cdev_init(&info9.cdev, &mem_fops);
    info9.cdev.owner = THIS_MODULE;
    info9.cdev.ops = &mem_fops;
    cdev_add(&info9.cdev, MKDEV(mem_major, 0), MEMDEV_NR_DEVS);

    mem_devp = kmalloc(MEMDEV_NR_DEVS * sizeof(struct mem_dev), GFP_KERNEL);

    if(!mem_devp) {
        result = -ENOMEM;
        goto fail_malloc;
    }

    memset(mem_devp, 0, sizeof(struct mem_dev));

    for(i=0; i<MEMDEV_NR_DEVS; ++i) {
        mem_devp[i].size = MEMDEV_SIZE;
        mem_devp[i].data = kmalloc(MEMDEV_SIZE, GFP_KERNEL);
        memset(mem_devp[i].data, 0, MEMDEV_SIZE);
    } 

    printk(KERN_ALERT "device_num = %d", device_num);

    i = 0;

    //如果在为device[1]赋值状态位之前给device[0]赋值状态位会导致global[1]的值改变，因此尝试先为device[1]赋值状态位，再为device[0]赋值状态位
    //bug得到修复

    p = yinwoods_device[1].dev.platform_data;
    p->status = global_status[1];
    platform_device_register(&yinwoods_device[1]);

    p = yinwoods_device[0].dev.platform_data;
    p->status = global_status[0];
    platform_device_register(&yinwoods_device[0]);

    for(i=2; i<device_num; ++i) {

        p = yinwoods_device[i].dev.platform_data;
        p->status = global_status[i];
        platform_device_register(&yinwoods_device[i]);
        //printk(KERN_ALERT " --- register device[%d]\n", i);
    }

    //platform_add_devices(yinwoods_device, ARRAY_SIZE(yinwoods_device));
    return 0;

    fail_malloc:
        unregister_chrdev_region(devno, 1);
    return result;
}

static void __exit yinwoods_exit(void) {

    cdev_del(&info0.cdev);
    cdev_del(&info1.cdev);
    cdev_del(&info2.cdev);
    cdev_del(&info3.cdev);
    cdev_del(&info4.cdev);
    cdev_del(&info5.cdev);
    cdev_del(&info6.cdev);
    cdev_del(&info7.cdev);
    cdev_del(&info8.cdev);
    cdev_del(&info9.cdev);
    kfree(mem_devp);
    unregister_chrdev_region(MKDEV(mem_major, 0), 2);

    i = 0;
    for(i=0; i<device_num; ++i) {
        printk(KERN_ALERT " --- unregister device[%d]\n", i);
        platform_device_unregister(&yinwoods_device[i]);
    }
}

module_init(yinwoods_init);
module_exit(yinwoods_exit);
