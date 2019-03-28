#include "pcie_driver.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include "pcie_debug.h"

#define DEBUG_LEVEL      ERROR_OUTPUT


static struct pci_device_id ids[] = {
    { PCI_DEVICE(MEM_PCI_VENDOR_ID, MEM_PCI_DEVICE_ID), },
    { 0, }
};
MODULE_DEVICE_TABLE(pci, ids);

unsigned int irq_data [30] = {1}; // just for test

void pci_disable_irq(void)
{
	iowrite32(0, REG_INTRC_CTRL);
}

void pci_eable_irq(void)
{
	iowrite32(1, REG_INTRC_CTRL);
}

void pci_set_intr_mask_reg(int mask)
{
	iowrite32(mask, REG_INTRC_CTRL);
}

unsigned int pci_get_intr_status_reg(void)
{
	return ioread32(REG_INTRC_STATUS);
}

void pci_set_intr_status_reg(int reg)
{
	return iowrite32(reg, REG_INTRC_STATUS);
}

static irqreturn_t pcie_intr(int irq, void *data){
	unsigned int status;
	pci_disable_irq();
	pci_set_intr_mask_reg(0x1);
	status = pci_get_intr_status_reg();
	pci_set_intr_status_reg(0);
	
	PCIE_DEBUG_PRINTK("exit %s\n",__func__);
	pci_set_intr_mask_reg(0);
	pci_eable_irq();
	return IRQ_HANDLED;
}


static int pcie_request_irq(struct pci_dev *dev)
{
	irq_handler_t handler = pcie_intr;
	int irq_flags = IRQF_SHARED;
	int err;
	PCIE_DEBUG_PRINTK("enter %s irq = 0x%x\n",__func__,dev->irq);

	err = request_irq(dev->irq, handler, irq_flags, "pcie_pci", (void *)irq_data);
	if (err) {
		printk("Unable to allocate interrupt Error: %d\n", err);
	}

	PCIE_DEBUG_PRINTK("exit %s\n",__func__);
	return err;
}

static void pcie_free_irq(struct pci_dev *dev)
{
	PCIE_DEBUG_PRINTK("enter %s\n",__func__);

	free_irq(dev->irq, (void *)irq_data);
	PCIE_DEBUG_PRINTK("exit %s\n",__func__);
}


static unsigned char skel_get_revision(struct pci_dev *dev)
{
    u8 revision;
 
    pci_read_config_byte(dev, PCI_REVISION_ID, &revision);
    return revision;
}
 
 
//return 0 means success
static int probe(struct pci_dev *dev, const struct pci_device_id *id)
{
    	/* Do probing type stuff here.  
    	 * Like calling request_region();
    	 */	
	//unsigned char revision_id;
	int bar ;
    unsigned long flags;

	PCIE_DEBUG_PRINTK("enter probe..............\n"); 
	if (skel_get_revision(dev) != MEM_PCI_REVISION_ID)
		return 1;
 	PCIE_DEBUG_PRINTK("start to enable device.....\n");
	pci_enable_device(dev);
	PCIE_DEBUG_PRINTK("after enable device ...........\n");

	/*irq*/
	pcie_request_irq(dev);
 	// pcie framebuffer
	bar = 5;
	g_pcie_mem_addr_start = pci_resource_start(dev, bar);
	g_pcie_mem_len = pci_resource_len(dev, bar);
	flags = pci_resource_flags(dev, bar);
	PCIE_DEBUG_PRINTK("bar=%d start=0x%llx len=0x%llx flags=0x%lx\n",bar,g_pcie_mem_addr_start,g_pcie_mem_len,flags);
	 
	g_pcie_mem_cpu_addr = pci_iomap(dev,bar,g_pcie_mem_len);

	//pcie registers 
	bar = 0;
	g_pcie_register_start = pci_resource_start(dev, bar);
	g_pcie_register_size = pci_resource_len(dev, bar);
	flags = pci_resource_flags(dev, bar);
	PCIE_DEBUG_PRINTK("bar=%d start=0x%llx len=0x%llx flags=0x%lx\n",bar,g_pcie_register_start,g_pcie_register_size,flags);
	 
	g_pcie_register_cpu_addr = pci_iomap(dev,bar,g_pcie_register_size);
	return 0;
}
 
static void remove(struct pci_dev *dev)
{
	/* clean up any allocated resources and stuff here.
	* like call release_region();
	*/
	pcie_free_irq(dev);
	pci_disable_device(dev);
}
 
static struct pci_driver pcie_driver = {
    .name = "pcie_icube",
    .id_table = ids,
    .probe = probe,
    .remove = remove,
};
 
int pcie_mem_pci_init(void)
{
    return pci_register_driver(&pcie_driver);
}
 
void pcie_mem_pci_exit(void)
{
    pci_unregister_driver(&pcie_driver);
}
 
 module_init(pcie_mem_pci_init);
 module_exit(pcie_mem_pci_exit);
 
 MODULE_LICENSE("Dual BSD/GPL");
 MODULE_AUTHOR("PCIE_ICUBE");
 MODULE_DESCRIPTION("PCIE_ICUBE driver");

