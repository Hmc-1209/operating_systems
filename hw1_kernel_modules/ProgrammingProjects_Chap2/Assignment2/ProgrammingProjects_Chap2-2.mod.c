#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

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
	{ 0x9931d80, "module_layout" },
	{ 0x697567ed, "proc_remove" },
	{ 0x92997ed8, "_printk" },
	{ 0x5204ad50, "proc_create" },
	{ 0x8da6585d, "__stack_chk_fail" },
	{ 0x1e49e99f, "cpu_hwcaps" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0xb788fb30, "gic_pmr_sync" },
	{ 0x31909811, "cpu_hwcap_keys" },
	{ 0x14b89635, "arm64_const_caps_ready" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x15ba50a6, "jiffies" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x4b0a3f52, "gic_nonsecure_priorities" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "21FC186C68A199EA79B0A02");
