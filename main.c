
#ifndef _DEFINES_H
#define _DEFINES_H

#include <stdio.h>
#include <stdlib.h>

#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"

#include "app_i2c.h"
#include "bh1750.h"
#include "tim2.h"
#include "uart.h"

#endif

void BH1750_Init(void) {
    // Kh?i t?o và c?u hình giao ti?p I2C
    I2C1_config();
    
    // G?i l?nh b?t ngu?n d?n c?m bi?n BH1750 theo d?a ch? BH1750_ADDR
    I2C1_write(BH1750_ADDR, 0xFF, BH1750_POWER_ON);
	    uint8_t tmp8[2];  // Khai báo m?ng 2 byte d? ch?a d? li?u t? c?m bi?n
    
    // G?i hàm d?c d? li?u t? BH1750
    I2C1_read_buf(BH1750_ADDR, BH1750_ONE_H_MODE, tmp8, 2);
    
    // X? lý d? li?u nh?n du?c (chuy?n d?i thành giá tr? lux)
    uint16_t lux = (tmp8[0] << 8) | tmp8[1];
    lux = lux / 1.2;

    while (1) {
        // Vòng l?p chính
    }
};
