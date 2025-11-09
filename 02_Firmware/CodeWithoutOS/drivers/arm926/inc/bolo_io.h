#ifndef __BOLO_IO_H__
#define __BOLO_IO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "io.h"
#define bolo_reg_read(reg) (*((volatile uint32_t *)(reg)))

#define bolo_reg_write(reg, vla) (*((volatile uint32_t *)(reg)) = vla)

#define bolo_set_bit(reg, bp) (write32((reg), (read32(reg) | ((bp)))))

#define bolo_clr_bit(reg, bp) (write32((reg), (read32(reg) & (~((bp))))))

#define bolo_set_shift(reg, bp) (write32((reg), (read32(reg) | ((1 << bp)))))
#define bolo_clr_shift(reg, bp) (write32((reg), (read32(reg) & (~((1 << bp))))))

#define bolo_clr_and_set_bit(reg, mask, bp) \
	do                                      \
	{                                       \
		unsigned int _r = read32(reg);      \
		_r &= (~(unsigned int)(mask));      \
		_r |= ((bp) & (mask));              \
		write32((reg), _r);                 \
	} while (0)

#ifdef __cplusplus
}
#endif

#endif /* __IO_H__ */
