#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define BUFFER_SIZE 128
#define PROC_NAME "hello"

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
	.owner = THIS_MODULE,
	.read = proc_read,
};

/*This function is called when the module is loaded*/
int proc_init(void){
/*Creates the /proc/hello entry*/
	proc_create(PROC_NAME, 0666, NULL, &proc_ops);

	return 0;
}

/*This function is called when the module is remove*/
void proc_exit(void){
/*removes the /proc/hello entry*/
	remove_proc_entry(PROC_NAME, NULL);
}

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos){
	int rv =0;
	char buffer[BUFFER_SIZE];
	static int completed = 0;

	if(completed){
	completed =0;
	return 0;
	}
	
	completed =1;

	rv = sprintf(buffer, "Hello world\n");

	copy_to_user(usr_buf, buffer, rv);
	
	return rv;
	}

	module_init(proc_init);
	module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HELLO MODULE");
MODULE_AUTHOR("SGG");
