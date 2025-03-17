#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>

#define PROC_NAME "seconds"

static struct proc_dir_entry *proc_entry;
static unsigned long start_jiffies; // For module start's jiffies
int bytes; // For saving elapsed time datas

static ssize_t proc_read(struct file *file, char __user *buffer, size_t length, loff_t *offset) {
	char buf[128];
	// Only calculate the elapsed time at the first copy section action
	if (*offset == 0) {
		unsigned long current_jiffies = jiffies; // Get the current time
		unsigned long elapsed_secs = (current_jiffies - start_jiffies) / HZ; // as jiffies save as ticks
		bytes = snprintf(buf, sizeof(buf), "%lu\n", elapsed_secs);
	}

	if (*offset >= bytes) return 0;
	if (copy_to_user(buffer, buf + *offset, bytes - *offset)) return -EFAULT;

	*offset += bytes;
	return bytes;
}

// The module settings
static struct proc_ops proc_fops = {
	.proc_read = proc_read,
};

static int __init read_elapsed_seconds_init(void) {
	start_jiffies = jiffies; // Get the jiffies at module load
	proc_entry = proc_create(PROC_NAME, 0666, NULL, &proc_fops);
	if (!proc_entry) return -ENOMEM;
	printk(KERN_INFO "Module loaded. The starting jiffies is %lu\n", start_jiffies);
	return 0;
}

static void __exit read_elapsed_seconds_exit(void) {
	proc_remove(proc_entry);
}

module_init(read_elapsed_seconds_init);
module_exit(read_elapsed_seconds_exit);

MODULE_LICENSE("GPL");
