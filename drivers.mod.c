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
	{ 0x634251a8, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x7c7dedf, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0x4cb8c80, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x5e4c2631, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0x743b8025, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0xe3f92160, __VMLINUX_SYMBOL_STR(device_create_file) },
	{ 0x986c38d5, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xf9441582, __VMLINUX_SYMBOL_STR(device_unregister) },
	{ 0xb4030034, __VMLINUX_SYMBOL_STR(device_remove_file) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "B57B5DCBCE3EA7DF901B0C9");
