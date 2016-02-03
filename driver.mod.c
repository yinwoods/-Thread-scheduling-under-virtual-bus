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
	{ 0xb89a34a1, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xc1379ae, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0x3d1792ec, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x19b39bf0, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0xf431e371, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x57d45739, __VMLINUX_SYMBOL_STR(device_create_file) },
	{ 0x6c227c38, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xa36180a2, __VMLINUX_SYMBOL_STR(device_unregister) },
	{ 0x56a7cbeb, __VMLINUX_SYMBOL_STR(device_remove_file) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "BA26374CE835DCB901F983C");
