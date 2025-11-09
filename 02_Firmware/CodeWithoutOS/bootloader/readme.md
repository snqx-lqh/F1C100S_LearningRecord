## bootloader 引导程序

默认使用 `NDMA_CH3` 方式读取flash代码到DRAM
 Audio例子使用 `NDMA_CH0`。

main.c中宏定义可以按需要打开DMA方式读取flash

`#define USE_DMA_READ_FLASH 1`