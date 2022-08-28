#include <linux/init.h>
#include <linux/module.h>

MODULE_DESCRIPTION("Kernel Modules");
MODULE_LICENSE("GPL");

int init_func(void) {
    printk(KERN_INFO "Creating Kernel Modules!\n");
    return 0;
}

void remove_func(void) {
    printk(KERN_INFO "Remove Kernel Modules!\n");
}

module_init(init_func);
module_exit(remove_func);
