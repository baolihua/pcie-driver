#ifndef __PCIE__DEBUG__H__
#define __PCIE__DEBUG__H__

#include <stddef.h>

#define INFO_OUTPUT      3
#define WARNING_OUTPUT   2
#define DEBUG_OUTPUT     1
#define ERROR_OUTPUT     0

/*
#define DEBUG_LEVEL      DEBUG_OUTPUT
需在使用以下打印宏的.c文件中定义“DEBUG_LEVEL”变量
*/

#define PCIE_INFO_PRINTK(info,...)\
do{\
	if(DEBUG_LEVEL>=INFO_OUTPUT){\
		printk("Info %s,%s,%d:"info"",__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);\
	}\
}while(0)

#define PCIE_WARNING_PRINTK(info,...)\
do{\
	if(DEBUG_LEVEL>=WARNING_OUTPUT){\
		printk("Warning %s,%s,%d:"info"",__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);\
	}\
}while(0)

#define PCIE_DEBUG_PRINTK(info,...)\
do{\
	if(DEBUG_LEVEL>=DEBUG_OUTPUT){\
		printk("Debug %s,%s,%d:"info"",__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);\
	}\
}while(0)

#endif
