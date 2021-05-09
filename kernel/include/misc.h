#ifndef __MISC_H
#define __MISC_H
#define NULL ((void *)0)
#define EOF (-1)
#include <stdint.h>
#include <stddef.h>

/**
 * @brief       Set value by mask
 *
 * @param[in]   bits        The one be set
 * @param[in]   mask        mask value
 * @param[in]   value       The value to set
 */
void set_bit(volatile uint32_t *bits, uint32_t mask, uint32_t value);

/**
 * @brief       Set value by mask
 *
 * @param[in]   bits        The one be set
 * @param[in]   mask        Mask value
 * @param[in]   offset      Mask's offset
 * @param[in]   value       The value to set
 */
void set_bit_offset(volatile uint32_t *bits, uint32_t mask, size_t offset, uint32_t value);

/**
 * @brief       Set bit for gpio, only set one bit
 *
 * @param[in]   bits        The one be set
 * @param[in]   idx         Offset value
 * @param[in]   value       The value to set
 */
void set_gpio_bit(volatile uint32_t *bits, size_t idx, uint32_t value);

/**
 * @brief      Get bits value of mask
 *
 * @param[in]   bits        The source data
 * @param[in]   mask        Mask value
 * @param[in]   offset      Mask's offset
 *
 * @return      The bits value of mask
 */
uint32_t get_bit(volatile uint32_t *bits, uint32_t mask, size_t offset);

/**
 * @brief       Get a bit value by offset
 *
 * @param[in]   bits        The source data
 * @param[in]   offset      Bit's offset
 *
 *
 * @return      The bit value
 */
uint32_t get_gpio_bit(volatile uint32_t *bits, size_t offset);

#define readb(addr) (*(volatile uint8_t *)(addr))
#define readw(addr) (*(volatile uint16_t *)(addr))
#define readl(addr) (*(volatile uint32_t *)(addr))
#define readq(addr) (*(volatile uint64_t *)(addr))

#define writeb(v, addr)                      \
    {                                        \
        (*(volatile uint8_t *)(addr)) = (v); \
    }
#define writew(v, addr)                       \
    {                                         \
        (*(volatile uint16_t *)(addr)) = (v); \
    }
#define writel(v, addr)                       \
    {                                         \
        (*(volatile uint32_t *)(addr)) = (v); \
    }
#define writeq(v, addr)                       \
    {                                         \
        (*(volatile uint64_t *)(addr)) = (v); \
    }
typedef int bool;
#endif