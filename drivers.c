#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("yinwoods");

struct yinwoods_data {
    char a[4096];
    char *name;

    int left;
    int right;
    int result;

    int status;

    struct device *dev;
};

//static char *tmp;               //作为进程间通信的中间变量
static int result;              //保存主状态位进程的计算结果
struct class *yinwoods_class;
struct device *yinwoods_dev;

static ssize_t yinwoods_store(struct device *dev, struct device_attribute *attr, char *buf, size_t size) {
    if(size > 4096) {
        printk(KERN_ALERT "fuck you!");
        return 0;
    }
    return sprintf(((struct yinwoods_data*) dev->platform_data)->a, buf);
}

static ssize_t yinwoods_show(struct device *dev, struct device_attribute *attr, char *buf) {
    return sprintf(buf, ((struct yinwoods_data *) dev->platform_data)->a);
}

struct device_attribute dev_attr_brightness = {
    .attr = {
        .name = "love",
        .mode = 0644
    },
    .show = yinwoods_show,
    .store = yinwoods_store,
};

static int yinwoods_probe(struct platform_device *dev) {

    //-------------------------------
    //对字符设备的操作部分

    struct file *fp; //用来读写字符设备
    mm_segment_t fs;
    loff_t pos;
    static char w_info[100] = "";

    //设备对应的字符设备位置
    static char dev_pos[100] = "";

    //-------------------------------

    struct yinwoods_data *p = (dev->dev).platform_data;
    yinwoods_dev = device_create(yinwoods_class, &(dev->dev), 0, NULL, "%s", p->name);
    p->dev = yinwoods_dev;
    yinwoods_dev->platform_data = p;

    device_create_file(yinwoods_dev, &dev_attr_brightness);

    //将该设备的一个字符串保存下来，在下一步传送给另一个设备
    //状态位是主，计算并保存结果
    if(p->status == 0) {
        p->result = p->left + p->right;
        result = p->result;
    }
    //状态位是热，只计算，不保存结果
    else if(p->status == 1) {
        p->result = p->left + p->right;
    }
    //状态位是冷，不计算


    //-------------------------------------
    //对字符设备的操作部分
    snprintf(dev_pos, sizeof(dev_pos), "/dev/memdev%d", dev->id);

    snprintf(w_info, sizeof(w_info), "设备编号： %d\n状态位：%d\n计算结果：%d\n", dev->id, p->status, p->result);

    fp = filp_open(dev_pos, O_RDWR|O_CREAT, 0644);

    if(IS_ERR(fp)) {
        printk(KERN_ALERT "Create File Error!\n");
        return -1;
    }
    fs = get_fs();
    set_fs(KERNEL_DS);
    pos = 0;
    vfs_write(fp, w_info, sizeof(w_info), &pos);


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
