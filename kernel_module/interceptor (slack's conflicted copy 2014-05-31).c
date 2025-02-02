#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <asm/uaccess.h>
#include <asm/cacheflush.h>
#include <linux/syscalls.h>
#include <linux/delay.h>    /* loops_per_jiffy */

#define CR0_WP 0x00010000   /* The Write Protect bit is the 16th bit of the cr0 register (CR0:16) and we can manipulate it by some simple bitmasking. */

/* Just so we do not taint the kernel */
MODULE_LICENSE("GPL");

void **syscall_table;
unsigned long **find_sys_call_table(void);

long (*orig_sys_open)(const char __user *filename, int flags, int mode);

unsigned long **find_sys_call_table() {
    
    unsigned long ptr;
    unsigned long *p;

    for (ptr = (unsigned long)sys_close;
         ptr < (unsigned long)&loops_per_jiffy;
         ptr += sizeof(void *)) {
             
        p = (unsigned long *)ptr;

        if (p[__NR_close] == (unsigned long)sys_close) {
            printk(KERN_ALERT "Found the sys_call_table at %lx\n", p[__NR_close]);
            return (unsigned long **)p;
        }
    }
    
    return NULL;
}

long my_sys_open(const char __user *filename, int flags, int mode) {
    long ret;

    ret = orig_sys_open(filename, flags, mode);
    printk(KERN_ALERT "file %s has been opened with mode %d\n", filename, mode);
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

static int __init syscall_init(void)
{
    int ret;
    unsigned long addr;
    unsigned long cr0;
 
   /* pointer to the sys_call_table */ 
    syscall_table = (void **)find_sys_call_table();

    if (!syscall_table) {
        printk(KERN_ALERT "Cannot find the system call address\n"); 
        return -1;
    }

    /* backup control register */
    cr0 = read_cr0();
    /* The WP bit is the 16th bit of the cr0 register and we can manipulate it by some simple bitmasking. 
     * When WP flag is set to 1 any memory page that is set read-only cannot be changed to be writable, 
     * so we need to change this flag to 0. */
    write_cr0(cr0 & ~CR0_WP);

    /* address of the first element of the syscall table */
    addr = (unsigned long)syscall_table;
    ret = set_memory_rw(PAGE_ALIGN(addr) - PAGE_SIZE, 3);

    if(ret) {
        printk(KERN_ALERT "Cannot set the memory to rw (%d) at addr %16lX\n", ret, PAGE_ALIGN(addr) - PAGE_SIZE);
    } else {
	printk(KERN_ALERT "ret is %d\n", ret);
        printk(KERN_ALERT "3 pages set to rw");
    }
    
    /* back up open() syscall */
    orig_sys_open = syscall_table[__NR_open];
    /* replace open() syscall with my own */
    syscall_table[__NR_open] = my_sys_open;
    /* write to control register */
    write_cr0(cr0);
  
    return 0;
}

static void __exit syscall_release(void)
{
    unsigned long cr0;
    
    /* set control register's write protection flag back along with the original syscall */
    cr0 = read_cr0();
    write_cr0(cr0 & ~CR0_WP);
   
    syscall_table[__NR_open] = orig_sys_open;
    
    write_cr0(cr0);
}

module_init(syscall_init);
module_exit(syscall_release);
