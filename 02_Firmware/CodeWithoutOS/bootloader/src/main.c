#include <stdint.h>
#include <stdio.h>
#include "system.h"
#include <string.h>
#include "io.h"
#include "arm32.h"
#include "sys_dram.h"
#include "spi_flash.h"
#include "f1c100s_timer.h"
#include "f1c100s_gpio.h"

//默认不使用DMA读取flash
#define USE_DMA_READ_FLASH  1

void dump_regs(uint32_t addr, uint32_t len);
void reset_cpu(void);

#define FW_START_ADDR 0x10000
#define FW_MAGIC 0x555AAAFF
uint8_t *dram = (uint8_t *)0x80000000;

typedef struct
{
    uint32_t jump_instr;
    uint32_t magic;
    uint32_t start;
    uint32_t end;
    uint32_t reserved;
    uint32_t crc32;
} fw_header_t;

int main(void)
{
    system_init();
    printf("SPL_bootloader started compiled_time:%s %s\r\n", __DATE__, __TIME__);

    //gpio_init(GPIOF, PIN0 | PIN1 | PIN3 | PIN5, GPIO_MODE_AF3, GPIO_PULL_NONE, GPIO_DRV_3);
    //printf("JTAG enabled, use pin:PF0 PF1 PF3 PF5 .\r\n");

    uint8_t size = sys_dram_init();
    printf("DRAM size: %uMB\r\n", size);

    spi_flash_init();
    uint32_t id = spi_flash_read_id();
    printf("SPI flash ID: %06lX\r\n", id);

    // uint8_t read[1024];
    // spi_flash_read(0, read, 1024);
    // for (int i = 0; i < 1024; i++)
    // {
    //     printf("%02X ", read[i]);
    //     if ((i + 1) % 8 == 0)
    //         printf("\r\n");
    // }

    spi_flash_read(FW_START_ADDR, dram, sizeof(fw_header_t));
    fw_header_t *hdr = (fw_header_t *)dram;
    if ((hdr->magic == FW_MAGIC) && (hdr->end > hdr->start))
    {
        uint32_t len = hdr->end - hdr->start;
        printf("app header OK! app length: %lu\r\n", len);
		#if USE_DMA_READ_FLASH
        sys_dma_init();
        spi_flash_init();
        sys_spi_flash_read_dma(FW_START_ADDR, dram, len);
        sys_dma_exit();
		#else
		spi_flash_read(FW_START_ADDR, dram, len);
		#endif
        printf("copy to dram done, bootloader app.\r\n");
        __asm__ __volatile__("ldr pc, =0x80000000" : : : "memory");
    }
    else
    {
        printf("No valid header found\r\n");
        // __asm__ __volatile__("ldr pc, =0x10000" : : : "rom");
    }

    while (1)
    {
    }

    return 0;
}

void dump_regs(uint32_t addr, uint32_t len)
{
    for (uint32_t i = addr; i < addr + len; i += 4)
    {
        printf("%08lX: %08lX\r\n", i, read32(i));
    }
}

void reset_cpu(void)
{
    arm32_interrupt_disable();
    wdg_init(WDG_MODE_RESET, WDG_INTV_500MS);
    while (1)
        ;
}
