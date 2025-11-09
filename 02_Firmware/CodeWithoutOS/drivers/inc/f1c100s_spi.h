#pragma once
#include "stdint.h"
#include "f1c100s_clock.h"
#include <stddef.h>
#ifndef SPI0_BASE
#define SPI0_BASE (0x01C05000)
#endif
#ifndef SPI1_BASE
#define SPI1_BASE (0x01C06000)
#endif
#ifndef __IO
#define __IO volatile
#endif

typedef struct
{
    __IO uint32_t RESERVED0;      // 0->0X00
    __IO uint32_t GCR;            // 4->0X04
    __IO uint32_t TCR;            // 8->0X08
    __IO uint32_t RESERVED1;      // 12->0X0C
    __IO uint32_t IER;            // 16->0X10
    __IO uint32_t ISR;            // 20->0X14
    __IO uint32_t FCR;            // 24->0X18
    __IO uint32_t FSR;            // 28->0X1C
    __IO uint32_t WCR;            // 32->0X20
    __IO uint32_t CCR;            // 36->0X24
    __IO uint32_t RESERVED2[2];   // 40->0x28 44->0x2C
    __IO uint32_t MBC;            // 48->0X30
    __IO uint32_t MTC;            // 52->0x34
    __IO uint32_t BBC;            // 56->0x38
    __IO uint32_t RESERVED3[113]; //
    __IO uint8_t TXDATA;          // 0X200 写入时一次写入一个Byte，需要转为 uint8_t * 详情见用户手册294页
    __IO uint8_t TXDATA8;
    __IO uint8_t TXDATA16;
    __IO uint8_t TXDATA24;
    __IO uint32_t RESERVED4[63]; //
    __IO uint8_t RXDATA;         // 0x300 读取时按Byte读取，需要转为 uint8_t * 详情见用户手册294页
    __IO uint8_t RXDATA8;
    __IO uint8_t RXDATA16;
    __IO uint8_t RXDATA24;

} Spi_reg_def;
#ifdef SPI0
#error "err:SPI0 defined!"
#endif
#define SPI0 ((Spi_reg_def *)0x01C05000)
#define SPI1 ((Spi_reg_def *)0x01C06000)

#define SPI_NSS_SOFT 1
#define SPI_NSS_HARD 0

#define SPI_MODE_MASTER 1
#define SPI_MODE_SLAVE 0

#define SPI_FIRST_BIT_MSB 0
#define SPI_FIRST_BIT_LSB 1

#define SPI_CPOL_LOW 1
#define SPI_CPOL_HIGH 0

#define SPI_CPHA_1EDGE 1
#define SPI_CPHA_2EDGE 0

/*SPI HANDLE DEFINE*/
typedef struct
{
    Spi_reg_def *instance; /*SPI0 SPI1*/
    uint8_t CPOL;          /*SCLK (0:low  1:high) when SCLK in idle state*/
    uint8_t CPHA;          /*采样和边沿触发模式，和POL 一共四种模式 详情见用户手册282页*/
    uint8_t MODE;          /*master:1 slave:0*/
    uint8_t NSS;           /* hard_nss:0 soft:1 control*/
    uint32_t Prescaler;    /*分频 0-128(0:2 1:4 2：6 8 ..  F_AHB/2*(n+1)  )*/
    uint32_t FirstBit;     /*MSB:0 LSB:1*/
} Spi_handle_def;

#ifndef _SPI0_CLK_ENABLE
#define _SPI0_CLK_ENABLE()                      \
    do                                          \
    {                                           \
        clk_enable(CCU_BUS_CLK_GATE0, 20);      \
        clk_reset_clear(CCU_BUS_SOFT_RST0, 20); \
    } while (0)
#endif

#ifndef _SPI1_CLK_ENABLE
#define _SPI1_CLK_ENABLE()                      \
    do                                          \
    {                                           \
        clk_enable(CCU_BUS_CLK_GATE0, 21);      \
        clk_reset_clear(CCU_BUS_SOFT_RST0, 21); \
    } while (0)
#endif

#define _SPI_ENABLE(handle)            \
    do                                 \
    {                                  \
        handle->instance->GCR |= 0x01; \
    } while (0)

#define _SPI_DISABLE(handle)                  \
    do                                        \
    {                                         \
        handle->instance->GCR &= (~(1 << 0)); \
    } while (0)

void Spi_Init(Spi_handle_def *handle);
void Spi_Soft_CS(Spi_handle_def *handle, uint8_t state);
uint8_t Spi_Read_Write_Byte(Spi_handle_def *handle, uint8_t txdata);
uint32_t Spi_Tranmit(Spi_handle_def *handle, uint8_t *txfifo, uint8_t *rxfifo, uint32_t length);
void Spi_Tx_Then_Rx(Spi_handle_def *handle, uint8_t *tx, uint32_t txlen, uint8_t *rx, uint32_t rxlen);