#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
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
__used __section("__versions") = {
	{ 0x19fdd0c3, "module_layout" },
	{ 0x65022fd0, "cdev_del" },
	{ 0x5f3af44c, "kmalloc_caches" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x804ff8b3, "cdev_init" },
	{ 0x69e052b6, "param_ops_int" },
	{ 0x1501b61a, "device_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xc5850110, "printk" },
	{ 0xf2204d2a, "device_create" },
	{ 0xaf610163, "cdev_add" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xd1ff3d1f, "kmem_cache_alloc_trace" },
	{ 0x37a0cba, "kfree" },
	{ 0xed3264a3, "class_destroy" },
	{ 0x1446a138, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "56450FFF89684F4A22EB51C");
