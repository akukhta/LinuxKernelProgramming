#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0xa879d70c, "module_layout" },
	{ 0x1a187dca, "cdev_alloc" },
	{ 0x2af57f44, "cdev_del" },
	{ 0xc875eb96, "cdev_init" },
	{ 0x7adc5eab, "device_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xc5850110, "printk" },
	{ 0x213ef32e, "device_create" },
	{ 0x4bf89aa2, "cdev_add" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x111bd0bd, "class_destroy" },
	{ 0x6c496836, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "A4FF71C850DD9E95A770CEB");
