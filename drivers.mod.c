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
	{ 0xd5fc10ca, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x2a7434cf, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0x1c4ec16f, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x128ceb6f, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0x1f0601c7, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x38bb0fb, __VMLINUX_SYMBOL_STR(filp_close) },
	{ 0xf7c56327, __VMLINUX_SYMBOL_STR(vfs_read) },
	{ 0x77a800e3, __VMLINUX_SYMBOL_STR(vfs_write) },
	{ 0x4c4fef19, __VMLINUX_SYMBOL_STR(kernel_stack) },
	{ 0xbe3e288c, __VMLINUX_SYMBOL_STR(filp_open) },
	{ 0x12960434, __VMLINUX_SYMBOL_STR(device_create_file) },
	{ 0xf44c2e07, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x3685dc4b, __VMLINUX_SYMBOL_STR(device_unregister) },
	{ 0x93640f88, __VMLINUX_SYMBOL_STR(device_remove_file) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "FF977814DA83C821713C1D6");
