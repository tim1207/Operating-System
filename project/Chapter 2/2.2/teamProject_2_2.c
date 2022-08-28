#include <linux/module.h>   
#include <linux/kernel.h>   
#include <linux/init.h>     
#include <linux/list.h>     
#include <linux/types.h>    
#include <linux/slab.h>     

typedef struct _birthday {
    int day;
    int month;
    int year;
    char *name;
    struct list_head list;
}birthday;

LIST_HEAD(birthday_list);

int birthdayList_init(void) {
    printk(KERN_INFO "Loading Module\n");
    char* name[] = {"Karma","Allen", "Roger", "Tim", "Unknown"};
    birthday *person;
    int i = 0;
    for(i = 0; i < 5; i++) {
        person = kmalloc(sizeof(*person), GFP_KERNEL);
        person->day = 14+i;
        person->month = 02+i;
        person->year = 2001+i;
        person->name = name[i];
        INIT_LIST_HEAD(&person->list);
        list_add_tail(&person->list, &birthday_list);
    }

    birthday *ptr;
    list_for_each_entry(ptr, &birthday_list, list) {
        printk(KERN_INFO "Name : <%s> ", ptr->name);
        printk(KERN_INFO "Birthday: %d/%d/%d\n", ptr->year, ptr->month, ptr->day);
    }
    return 0;
}

void birthdayList_remove(void) {
    printk(KERN_INFO "Removing Module\n");
    birthday *ptr, *next;
    list_for_each_entry_safe(ptr, next, &birthday_list, list) {
        printk(KERN_INFO "Removing %s's Birthday: %d/%d/%d\n", ptr->name, ptr->year, ptr->month, ptr->day);
        list_del(&ptr->list);
        kfree(ptr);
    }
    printk(KERN_INFO "Memory free done\n");

}

module_init(birthdayList_init);
module_exit(birthdayList_remove);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Module");
MODULE_AUTHOR("Karma_Team");
