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
    struct device *dev;
};

struct class *yinwoods_class;
struct device *yinwoods_dev;

static ssize_t yinwoods_store(struct device *dev, struct device_attribute *attr, char *buf, size_t size) {
    if(size > 4096) {
        printk(KERN_ALERT "fuck you!");
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
