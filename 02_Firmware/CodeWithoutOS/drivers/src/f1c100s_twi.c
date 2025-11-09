#include "f1c100s_twi.h"
#include "f1c100s_clock.h"
#include "stdio.h"
static int Twi_wait_interupt_flag(Twi_handle_def *handle);
// static int Twi_RX(Twi_handle_def *handle, uint8_t *rx, uint32_t length);
void Twi_Init(Twi_handle_def *handle)
{
    uint64_t pclk = clk_apb_get_freq();
    int64_t freq, delta, best = 0x7fffffffffffffffLL;
    int tm = 5, tn = 0;
    int m, n;

    for (n = 0; n <= 7; n++)
    {
        for (m = 0; m <= 15; m++)
        {
            freq = pclk / (10 * (m + 1) * (1 << n));
            delta = handle->clock - freq;
            if (delta >= 0 && delta < best)
            {
                tm = m;
                tn = n;
                best = delta;
            }
            if (best == 0)
                break;
        }
    }
    handle->instance->SADDR = handle->device_addr << 1;
    handle->instance->XADDR = 0;
    handle->instance->CCR = ((tm & 0xf) << 3) | ((tn & 0x7) << 0);
    freq = pclk / (10 * (tm + 1) * (1 << tn));
    printf("tw clk: N:%d M:%d freq:%lld", tn, tm, freq);
}

inline void Twi_Enable(Twi_handle_def *handle)
{
    handle->instance->CNTR = BUS_EN | M_STOP;
}
inline void Twi_Reset(Twi_handle_def *handle)
{
    handle->instance->SRST |= 1;
}

static int Twi_wait_interupt_flag(Twi_handle_def *handle)
{
    uint16_t timeout = 0;
    while (!(handle->instance->CNTR & INT_FLAG))
    {
        timeout++;
        if (timeout > 5000)
        {
            return TIMEOUT;
        }
    }
    return (int)(handle->instance->STAT & 0XFF);
}

int Twi_Start(Twi_handle_def *handle)
{
    handle->instance->CNTR = A_ACK | M_START | BUS_EN | CNTR_INT_EN;
    uint16_t timeout = 0;
    while (!(handle->instance->CNTR & M_START))
    {
        timeout++;
        if (timeout > 5000)
        {
            return TIMEOUT;
        }
    }
    return Twi_wait_interupt_flag(handle);
}

int Twi_Stop(Twi_handle_def *handle)
{
    uint32_t reg = handle->instance->CNTR;
    reg |= M_STOP;
    reg &= (~CNTR_INT_EN);
    reg &= (~INT_FLAG);
    handle->instance->CNTR = reg;
    uint16_t timeout = 0;
    while (!(handle->instance->CNTR & M_STOP))
    {
        timeout++;
        if (timeout > 5000)
        {
            return TIMEOUT;
        }
    }
    return 0;
}

int Twi_Ack(Twi_handle_def *handle, uint8_t ack)
{
    uint32_t reg = handle->instance->CNTR;
    if (ack)
        reg |= A_ACK;
    else
        reg &= ~A_ACK;
    handle->instance->CNTR |= reg;

    return 0;
}

int Twi_SendByte(Twi_handle_def *handle, uint8_t byte)
{
    handle->instance->DATA = byte & 0xff;
    handle->instance->CNTR &= (~INT_FLAG);
    return Twi_wait_interupt_flag(handle);
}

void Twi_write_reg(Twi_handle_def *handle, uint16_t reg, uint8_t *data, uint32_t len)
{
    Twi_Start(handle);
    Twi_SendByte(handle, handle->device_addr << 1);
    if (handle->reg_size == 8)
    {
        Twi_SendByte(handle, (uint8_t)(reg & 0xff));
    }
    else
    {
        Twi_SendByte(handle, (uint8_t)((reg >> 8) & 0xff));
        Twi_SendByte(handle, (uint8_t)(reg & 0xff));
    }
    for (int i = 0; i < len; i++)
    {
        Twi_SendByte(handle, data[i]);
    }
    Twi_Stop(handle);
}

// 需要先写 器件地址 的读模式 如24C02 写0XA1 ，再读取数据.
uint8_t Twi_ReadByte(Twi_handle_def *handle, uint8_t ack)
{
    uint8_t data = 0;
    uint32_t reg = 0; // = handle->instance->CNTR;
    reg &= (~INT_FLAG);
    if (!ack)
        reg &= ~A_ACK;
    else
        reg |= A_ACK;
    handle->instance->DATA = 0xff; // 可以省略
    handle->instance->CNTR = reg;  // 写0 CNTR INT_FLAG 可以启动一次传输
    int flag = Twi_wait_interupt_flag(handle);
    if (flag)
    {
    }
    data = handle->instance->DATA;
    return data;
}

void Twi_read_reg(Twi_handle_def *handle, uint16_t reg, uint8_t *rx, uint32_t len)
{
    Twi_Start(handle);
    Twi_SendByte(handle, handle->device_addr << 1);
    if (handle->reg_size == 8)
    {
        Twi_SendByte(handle, (uint8_t)(reg & 0xff));
    }
    else
    {
        Twi_SendByte(handle, (uint8_t)((reg >> 8) & 0xff));
        Twi_SendByte(handle, (uint8_t)(reg & 0xff));
    }
    Twi_Stop(handle);

    Twi_Start(handle);
    Twi_SendByte(handle, (handle->device_addr << 1) | 1);
    for (uint32_t i = 0; i < len - 1; i++)
    {
        rx[i] = Twi_ReadByte(handle, 1);
    }
    rx[len - 1] = Twi_ReadByte(handle, 0);
    Twi_Stop(handle);
}

void Twi_24c02_write(Twi_handle_def *handle, uint16_t addr, uint8_t *data, uint32_t len)
{
    for (int i = 0; i < len; i++)
    {
        Twi_write_reg(handle, addr + i, &data[i], 1);
        for (int j = 0; j < 5000; j++)
        {
            asm("nop");
        }
    }
}

void Twi_24c02_read(Twi_handle_def *handle, uint16_t addr, uint8_t *data, uint32_t len)
{
    Twi_read_reg(handle, addr, data, len);
}
