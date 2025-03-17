#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/pid.h>

#define PROC_NAME "read_task_info"

static pid_t pid_to_read = -1;

// When writing data to proc file
static ssize_t proc_write(struct file *file, const char __user *buf, size_t count, loff_t *pos) {
    char pid_buffer[16];
    
    if (count > sizeof(pid_buffer) - 1) return -EINVAL; // Prevent buffer overflow
    if (copy_from_user(pid_buffer, buf, count)) return -EFAULT; // Get data from user

    pid_buffer[count] = '\0'; // Ensure null-terminated when user writes data in /proc/read_task_info
    if (kstrtoint(pid_buffer, 10, &pid_to_read)) return -EINVAL; // Transform to PID number(int)

    return count;
}

static int task_info_show(struct seq_file *m, void *v) {
    struct task_struct *task;

    // If PID not set
    if (pid_to_read == -1) {
        seq_printf(m, "No PID specified\n");
        return 0;
    }

    // Find the process
    task = pid_task(find_vpid(pid_to_read), PIDTYPE_PID);
    if (!task) seq_printf(m, "No such process\n");
    else seq_printf(m, "PID: %d\nPriority: %d\nPolicy: %d\nComm: %s\nParent: %d\nUser time: %llu\n", task->pid, task->prio, task->policy, task->comm, task->parent->pid, task->utime); // Get the process data and print out using seq_printf

    return 0;
}

static int task_info_open(struct inode *inode, struct file *file) {
    return single_open(file, task_info_show, NULL); // Open the file and get the content of it
}

static const struct proc_ops proc_fops = {
    .proc_open = task_info_open,
    .proc_read = seq_read,
    .proc_write = proc_write,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init read_task_info_init(void) {
    struct proc_dir_entry *entry;

    entry = proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    if (!entry) return -ENOMEM; // Check entry
    printk(KERN_INFO "Module loaded.");
    return 0;
}

static void __exit read_task_info_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
}

module_init(read_task_info_init);
module_exit(read_task_info_exit);

MODULE_LICENSE("GPL");

