#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)
{
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
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_ALERT "Goodbye, World!\n");
}
