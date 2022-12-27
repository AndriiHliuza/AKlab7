#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Hliuza Andrii");
MODULE_DESCRIPTION("Lab 6");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int number = 1;

module_param(number, uint, S_IRUGO);

MODULE_PARM_DESC(number, "Number of Hello, World!");

struct event_list {
    struct list_head list;
    ktime_t event_time;
};

static struct list_head event_list_head;

static LIST_HEAD(event_list_head);

void new_event(int);

static int __init hello_init(void)
{
    if (number == 0 || (number >= 5 && number <= 10)) {
        printk(KERN_WARNING "5 <= parameter <= 10");  
    }

    BUG_ON(number > 10);
    //if (number > 10) {
    //    printk(KERN_ERR "Parameter is very big");
    //    return -EINVAL;
    //}
  
    int i;
    for (i = 0; i < number; i++) {
        printk(KERN_EMERG "Hello, World!\n");
        new_event(i);
    }
  return 0;
}

static void __exit hello_exit(void) {
  struct event_list *md, *tmp;
  list_for_each_entry_safe(md, tmp, &event_list_head, list) {
    printk(KERN_EMERG "Time: %lld\n", md->event_time);
    list_del(&md->list);
    kfree(md);
  }
}

void new_event(int i) {
    struct event_list *element = NULL;
    element = kmalloc(sizeof(struct event_list), GFP_KERNEL);
    
    if(i==5){
	BUG();
    }
    
    element->event_time = ktime_get();
    list_add_tail(&element->list, &event_list_head);
}

module_init(hello_init);
module_exit(hello_exit);

