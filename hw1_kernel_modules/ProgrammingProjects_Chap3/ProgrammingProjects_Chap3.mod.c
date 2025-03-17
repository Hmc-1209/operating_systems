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
	{ 0x6663f577, "single_release" },
	{ 0x20236789, "seq_lseek" },
	{ 0xfec2686f, "seq_read" },
	{ 0xc5b2e28c, "remove_proc_entry" },
	{ 0x92997ed8, "_printk" },
	{ 0x5204ad50, "proc_create" },
	{ 0x8da6585d, "__stack_chk_fail" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0x1e49e99f, "cpu_hwcaps" },
	{ 0x8c8569cb, "kstrtoint" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0xb788fb30, "gic_pmr_sync" },
	{ 0x31909811, "cpu_hwcap_keys" },
	{ 0x14b89635, "arm64_const_caps_ready" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x9f9fb316, "seq_printf" },
	{ 0xe8188723, "pid_task" },
	{ 0x31a2c638, "find_vpid" },
	{ 0x6a238606, "single_open" },
	{ 0x4b0a3f52, "gic_nonsecure_priorities" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "F0D4570EFD09B612D8EC217");
