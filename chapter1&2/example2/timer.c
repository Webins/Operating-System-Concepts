#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <asm/param.h>
/* This function is called when the module is loaded. */
unsigned long firstT = 0;
unsigned long secondT =0;
int simple_init(void){
    if(HZ != 0 && jiffies != 0) firstT = (jiffies / HZ) ;
    printk(KERN_INFO "HZ = %d \nJIFFIES = %lu \n", HZ, jiffies);
    printk(KERN_INFO "First time = %lu \n", firstT);
    return 0;
}
/* This function is called when the module is removed. */
void simple_exit(void){
    
    if(HZ != 0 && jiffies != 0) secondT = (jiffies / HZ) ;
    printk(KERN_INFO "Second time = %lu \n", secondT);
    printk(KERN_INFO "Time enlapsed: %lu\n", (secondT - firstT) );
}
/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Timer Module");
MODULE_AUTHOR("WEBINS");