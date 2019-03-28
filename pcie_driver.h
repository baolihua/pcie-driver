#ifndef __PCIE__DRIVER__H__
#define __PCIE__DRIVER__H__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>


#define MEM_PCI_VENDOR_ID       0xeeee
#define MEM_PCI_DEVICE_ID       0xbbbb          
#define MEM_PCI_REVISION_ID     0xfe

typedef unsigned char  byte;
typedef unsigned short int uint16;
typedef unsigned int    uint32;

resource_size_t g_pcie_mem_addr_start;
resource_size_t g_pcie_mem_len;
void __iomem * g_pcie_mem_cpu_addr = NULL;

resource_size_t g_pcie_register_start;
resource_size_t g_pcie_register_size;
void __iomem * g_pcie_register_cpu_addr = NULL;

#define PCIE_MEM_BASE g_pcie_mem_cpu_addr
#define PCIE_MEM_SIZE g_pcie_mem_len

#define PCIE_REG_BASE g_pcie_register_cpu_addr
#define PCIE_REG_SIZE g_pcie_register_size

#define REG_GLOBAL_CONTROL  PCIE_REG_BASE
#define REG_RB_BASE_ADDR	PCIE_REG_BASE + 0x4
#define REG_RB_WRITE_ADDR	PCIE_REG_BASE + 0x8
#define REG_RB_READ_ADDR	PCIE_REG_BASE + 0xC

#define REG_INTRC_CTRL      PCIE_REG_BASE + 0x10
#define REG_INTRC_MASK      PCIE_REG_BASE + 0x14
#define REG_INTRC_STATUS	PCIE_REG_BASE + 0x18

#define REG_DMA_CTRL		PCIE_REG_BASE + 0x1C
#define REG_DMA_SRC			PCIE_REG_BASE + 0x20
#define REG_DMA_DST			PCIE_REG_BASE + 0x24



int pcie_mem_pci_init(void);
void pcie_mem_pci_exit(void);

#endif

