#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>

#define PROC_NAME "jiffies"

static struct proc_dir_entry *proc_entry;

// Read action
static ssize_t proc_read(struct file *file, char __user *buffer, size_t length, loff_t *offset) {
	char buf[128];
	int bytes;
	unsigned long current_jiffies = jiffies;
	bytes = snprintf(buf, sizeof(buf), "%lu\n", current_jiffies);
	
	if (*offset >= bytes) return 0; // Check if the file read gets the end(EOF)
	if(copy_to_user(buffer, buf + *offset, bytes - *offset)) return -EFAULT; // Try to copy the value to user buffer
	*offset += bytes; // Get to next read section
	return bytes;
}

// Module settings
static const struct proc_ops proc_fops = {
	.proc_read = proc_read, // Set the read action
};

// Init settings when module loaded
static int __init read_jiffies_module_init(void) {
	proc_entry = proc_create(PROC_NAME, 0666, NULL, &proc_fops);
	if(!proc_entry) return -ENOMEM; // Show error
	return 0;
}

// Exit module actions
static void __exit read_jiffies_module_exit(void) {
	proc_remove(proc_entry);
}

module_init(read_jiffies_module_init);
module_exit(read_jiffies_module_exit);

MODULE_LICENSE("GPL");
