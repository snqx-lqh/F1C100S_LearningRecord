#pragma once
#include "f1c100s_clock.h"
#include "f1c100s_periph.h"
#ifndef TIMEOUT
#define TIMEOUT -1
#endif
typedef struct
{
    volatile uint32_t SADDR;
    volatile uint32_t XADDR;
    volatile uint32_t DATA;
    volatile uint32_t CNTR;
    volatile uint32_t STAT;
    volatile uint32_t CCR;
    volatile uint32_t SRST;
    volatile uint32_t EFR;
    volatile uint32_t LCR;
} Twi_reg_def;

#define TWI2 ((Twi_reg_def *)(0x01c27800))
#define TWI1 ((Twi_reg_def *)(0x01c27400))
#define TWI0 ((Twi_reg_def *)(0x01c27000))

/* CNTR register*/
#define A_ACK (1 << 2)
#define INT_FLAG (1 << 3)
#define M_STOP (1 << 4)
#define M_START (1 << 5)
#define BUS_EN (1 << 6)
#define CNTR_INT_EN (1 << 7)

typedef struct
{
    Twi_reg_def *instance; // TWI0 TWI1 TWI2
    uint32_t device_addr;  // 从机设备写地址7bit 默认地址不包括读写位，如24C02 设备地址0x50 库函数写设置地址会左移1位 左移后0xA0,
    uint32_t reg_size;     // 从机设备 数据地址数据长度 8bit 16 bit
    uint32_t clock;        // 100000 400000 100k-400k

} Twi_handle_def;

#ifndef _TWI2_CLK_ENABLE
#define _TWI2_CLK_ENABLE()                      \
    do                                          \
    {                                           \
        clk_enable(CCU_BUS_CLK_GATE2, 18);      \
        clk_reset_clear(CCU_BUS_SOFT_RST2, 18); \
    } while (0)
#endif

#ifndef _TWI1_CLK_ENABLE
#define _TWI1_CLK_ENABLE()                      \
    do                                          \
    {                                           \
        clk_enable(CCU_BUS_CLK_GATE2, 17);      \
        clk_reset_clear(CCU_BUS_SOFT_RST2, 17); \
    } while (0)
#endif

#ifndef _TWI0_CLK_ENABLE
#define _TWI0_CLK_ENABLE()                      \
    do                                          \
    {                                           \
        clk_enable(CCU_BUS_CLK_GATE2, 16);      \
        clk_reset_clear(CCU_BUS_SOFT_RST2, 16); \
    } while (0)
#endif

void Twi_Init(Twi_handle_def *handle);

void Twi_Reset(Twi_handle_def *handle);
void Twi_Enable(Twi_handle_def *handle);
int Twi_Start(Twi_handle_def *handle);
int Twi_SendByte(Twi_handle_def *handle, uint8_t byte);
int Twi_Ack(Twi_handle_def *handle, uint8_t ack);

int Twi_Stop(Twi_handle_def *handle);
void Twi_write_reg(Twi_handle_def *handle, uint16_t reg, uint8_t *data, uint32_t len);
void Twi_read_reg(Twi_handle_def *handle, uint16_t reg, uint8_t *rx, uint32_t len);
uint8_t Twi_ReadByte(Twi_handle_def *handle, uint8_t ack);
void Twi_read_reg(Twi_handle_def *handle, uint16_t reg, uint8_t *rx, uint32_t len);

void Twi_24c02_write(Twi_handle_def *handle, uint16_t addr, uint8_t *data, uint32_t len);
void Twi_24c02_read(Twi_handle_def *handle, uint16_t addr, uint8_t *data, uint32_t len);