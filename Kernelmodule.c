#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("skanda");
MODULE_DESCRIPTION("OS project");
MODULE_ALIAS("kernel");
static int task_count = 0;
static int recursion_depth_limit = 5; // Limit depth for debugging

static int task_limit = 100; // Limit task count for debugging
static void display_task(struct task_struct *task, int level) {
struct task_struct *child_task;
struct list_head *list;
if (task_count >= task_limit) return;
if (level > recursion_depth_limit) return;
task_count++;
printk(KERN_INFO "%*sProcess: %s (PID: %d, Parent PID: %d)\n",
level * 4, "", task->comm, task->pid, task->parent ? task->parent-
>pid : -1);
// Iterate over threads of the current task
for_each_thread(task, child_task) {
printk(KERN_INFO "%*s-Thread: %s (PID: %d)\n", (level + 1) * 4,
"",
child_task->comm, child_task->pid);
}
// Recursively display the children of this task
list_for_each(list, &task->children) {
child_task = list_entry(list, struct task_struct, sibling);
display_task(child_task, level + 1);
}
}
static int __init kernel_init(void) {
printk(KERN_INFO "Loading our module...\n");
char *user_program_name = "/Desktop/os_project/user_program";
char *argv[] = { user_program_name, NULL };
static char *envp[] = { "HOME=/", "TERM=linux",
"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
int ret = call_usermodehelper(user_program_name, argv, envp,
UMH_WAIT_PROC);
if (ret) {

printk(KERN_ERR "Error launching user program: %d\n", ret);
}
display_task(&init_task, 0);
printk(KERN_INFO "Our Module loaded.\n");
return 0;
}
static void __exit kernel_exit(void) {
printk(KERN_INFO "Module removed.\n");
}
module_init(kernel_init);
module_exit(kernel_exit);
