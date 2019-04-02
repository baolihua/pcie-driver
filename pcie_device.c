
#include <linux/module.h>

#include <linux/device.h>

#include <linux/platform_device.h>

#include <linux/ioport.h>

static void mvp_release(struct device *dev) 
{
	printk("%s, %d\n", __FUNCTION__, __LINE__);
	return ;
}

static struct resource mvp_resource[] =

{
#if 0
	[0] ={

		.start = 0x114000a0,

		.end =  0x114000a0 + 0x4,

		.flags = IORESOURCE_MEM,

	},

 

	[1] ={

		.start = 0x139D0000,

		.end =  0x139D0000 + 0x14,

		.flags = IORESOURCE_IRS,

	}
#endif
};


static struct platform_device icube_device=

{

    .name = "pcie_icube",

    .id = -1,

    .dev.release = mvp_release,

    .num_resources = ARRAY_SIZE(mvp_resource),

    .resource = mvp_resource,

};

 

static int mvp_device_init(void)

{

	printk("mvp_device_init");

	return platform_device_register(&icube_device);

}

 

static void mvp_device_exit(void)

{

	printk("mvp_device_exit");

	platform_device_unregister(&icube_device);

	return;

}

 

MODULE_LICENSE("GPL");

module_init(mvp_device_init);

module_exit(mvp_device_exit);


