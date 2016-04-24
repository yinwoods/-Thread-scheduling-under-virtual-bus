#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x4ee00782, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xaeffc409, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0x1c4ec16f, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x9d27f39d, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0x1f0601c7, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x50018599, __VMLINUX_SYMBOL_STR(filp_close) },
	{ 0xb176beb1, __VMLINUX_SYMBOL_STR(vfs_write) },
	{ 0x4c4fef19, __VMLINUX_SYMBOL_STR(kernel_stack) },
	{ 0x3f707835, __VMLINUX_SYMBOL_STR(filp_open) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0x98d8e56f, __VMLINUX_SYMBOL_STR(device_create_file) },
	{ 0xf95a72e6, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x2fd123cc, __VMLINUX_SYMBOL_STR(device_unregister) },
	{ 0xf758adc3, __VMLINUX_SYMBOL_STR(device_remove_file) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "0C833FA6E6D69DD83600001");
