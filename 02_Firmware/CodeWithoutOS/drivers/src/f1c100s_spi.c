#include "f1c100s_spi.h"
#include "f1c100s_periph.h"
#include "io.h"
static void Spi_Fifo_Rst(Spi_handle_def *handle);
static void Spi_Write_Txfifo(Spi_handle_def *handle, uint8_t *tx, uint32_t length);
static void Spi_Start_transmit(Spi_handle_def *handle);
void Spi_Init(Spi_handle_def *handle)
{

    // freq=Freq_AHB/(2*(n+1))
    handle->instance->CCR &= 0xEF00;                          // DRS CDR1 clear 使用CDR2分频
    handle->instance->CCR |= (handle->Prescaler | (1 << 12)); // 0-2 1-4 2-6 3-8

    handle->instance->GCR &= (~(1 << 1));
    handle->instance->GCR |= (handle->MODE << 1) | (1 << 7) | (1 << 31);
    while (handle->instance->GCR & (1 << 31))
        ;

    handle->instance->TCR &= 0xEFBC; // bit 0 1 6 12 clear
    handle->instance->TCR |= handle->CPHA | (handle->CPOL << 1) | (handle->FirstBit << 12) | (1 << 2) | (handle->NSS << 6);

    // handle->instance->FCR |= (1 << 31 | 1 << 15); // tx fifo ,rx fifo reset
    Spi_Fifo_Rst(handle);

    _SPI_ENABLE(handle);
}

static void Spi_Start_transmit(Spi_handle_def *handle)
{
    handle->instance->TCR |= 1 << 31; // xch burst, start transmit fifo
    while (handle->instance->TCR & (1 << 31))
        ; // wait send over;
}

uint8_t Spi_Read_Write_Byte(Spi_handle_def *handle, uint8_t txdata)
{
    uint8_t rxdata = 0;
    handle->instance->MBC = 1;
    handle->instance->MTC = 1;
    handle->instance->BBC = 1;
    handle->instance->TXDATA = txdata & 0xff;
    Spi_Start_transmit(handle);
    while ((handle->instance->FSR & 0xff) < 1)
        ; // 等待fifo清空
    rxdata = handle->instance->RXDATA;
    return rxdata;
}

void Spi_Soft_CS(Spi_handle_def *handle, uint8_t state)
{
    handle->instance->TCR &= ~((0x03 << 4) | (0x01 << 7)); // clear bit 4 5 7
    handle->instance->TCR |= (state << 7);
}

static void Spi_Fifo_Rst(Spi_handle_def *handle)
{
    handle->instance->FCR |= (1 << 31 | 1 << 15); // tx fifo ,rx fifo reset
}

static void Spi_Write_Txfifo(Spi_handle_def *handle, uint8_t *tx, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++)
    {
        if (tx == NULL)
            handle->instance->TXDATA = 0;
        else
            handle->instance->TXDATA = tx[i];
    }
}

uint32_t Spi_Tranmit(Spi_handle_def *handle, uint8_t *txfifo, uint8_t *rxfifo, uint32_t length)
{
    int64_t count = length;
    uint32_t n = 0, i = 0;
    uint8_t *tx = txfifo;
    uint8_t *rx = rxfifo;
    uint8_t rxtemp = 0;
    Spi_Fifo_Rst(handle);

    while (count > 0)
    {
        if (count > 64)
            n = 64;
        else
            n = count;
        handle->instance->MBC = n;
        handle->instance->MTC = n;
        handle->instance->BBC = n;
        Spi_Write_Txfifo(handle, tx, n);
        Spi_Start_transmit(handle);
        while ((handle->instance->FSR & 0xff) < n)
            ; // 等待fifo清空

        for (i = 0; i < n; i++)
        {
            rxtemp = handle->instance->RXDATA; // 按字节读取 RXDATA，RXFIFO接收数据 自动减1
            if (rx != NULL)
            {
                *rx++ = rxtemp;
            }
        }

        if (tx != NULL)
        {
            tx += n;
        }
        count -= n;
    }
    return length;
}

void Spi_Tx_Then_Rx(Spi_handle_def *handle, uint8_t *tx, uint32_t txlen, uint8_t *rx, uint32_t rxlen)
{
    Spi_Tranmit(handle, tx, NULL, txlen);
    Spi_Tranmit(handle, NULL, rx, rxlen);
}