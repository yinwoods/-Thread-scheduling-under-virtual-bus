#include <linux/module.h>
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

    int left;
    int right;
    int result;

    int status;

    char *mutex;

    struct device *dev;
};

static char *tmp;               //作为进程间通信的中间变量
static int result;              //保存主状态位进程的计算结果
struct class *yinwoods_class;
struct device *yinwoods_dev;

static ssize_t yinwoods_store(struct device *dev, struct device_attribute *attr, char *buf, size_t size) {
    if(size > 4096) {
        printk(KERN_ALERT "size too large!");
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

    printk(KERN_ALERT "%s\n", p->mutex);

    //判断是id 为 1的设备, 计算算式的值
    //并将该设备的一个字符串保存下来，在下一步传送给另一个设备
    if(dev->id == 1) {
        //状态位是主
        if(p->status == 0) {
            p->result = p->left + p->right;
            result = p->result;
        }
        printk(KERN_ALERT "device1 result = %d\n", p->result);
        tmp = p->mutex;
    }

    //判断是id 不为 1的设备, 计算算式的值
    if(dev->id != 1) {
        p->result = p->left + p->right;
        printk(KERN_ALERT "device%d result = %d\n", dev->id, p->result);
        if(strlen(tmp) != 0) {
            p->mutex = tmp;
        }
        printk(KERN_ALERT "device%d mutex = %s\n", dev->id, p->mutex);
    }

    return 0;
}

static int yinwoods_remove(struct platform_device *dev) {
    struct yinwoods_data *p = (dev->dev).platform_data;
    device_remove_file(p->dev, &dev_attr_brightness);
    device_unregister(p->dev);

    return 0;
}

static struct platform_driver yinwoods_driver = {
    .probe = yinwoods_probe,
    .remove = yinwoods_remove,
    .driver = {
        .name = "yinwoods",
        .owner = THIS_MODULE,
    },
};

static int __init yinwoods_init(void) {
    yinwoods_class = class_create(THIS_MODULE, "yinwoods");
    return platform_driver_register(&yinwoods_driver);
}

static void __exit yinwoods_exit(void) {
    class_destroy(yinwoods_class);
    platform_driver_unregister(&yinwoods_driver);
}

module_init(yinwoods_init);
module_exit(yinwoods_exit);
