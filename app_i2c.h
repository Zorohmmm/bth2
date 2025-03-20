
#ifndef _APP_I2C_H
#define _APP_I2C_H


#include "stm32f10x.h"
#include "bh1750.h"


void I2C1_config(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 200000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = 0x0B;
	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Cmd(I2C1, ENABLE);
};

void I2C1_write(unsigned char HW_address, unsigned char sub, unsigned char data){

	int ticks = I2C_TIMEOUT;  // Thi?t l?p timeout d? tránh treo h? th?ng
    
    while (ticks > 0) {
        ticks--;
        
        if (ticks == 0) return;
				ticks = I2C_TIMEOUT;
    }
		
		
		// T?o di?u ki?n START
I2C_GenerateSTART(I2C1, ENABLE);

// Ch? cho d?n khi tr?ng thái MASTER du?c ch?n
while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) && ticks--) {}
// Thoát n?u quá th?i gian ch?
if (ticks == 0) return;
ticks = I2C_TIMEOUT;

// G?i d?a ch? thi?t b? ? ch? d? truy?n (TRANSMITTER)
I2C_Send7bitAddress(I2C1, HW_address, I2C_Direction_Transmitter);

// Ch? cho d?n khi ch? d? MASTER-TRANSMITTER du?c ch?n
while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && ticks--) {}
if (ticks == 0) return;
ticks = I2C_TIMEOUT;

// N?u sub != 0xFF thì g?i d?a ch? thanh ghi con
if (sub != 0xFF) {
    // G?i d?a ch? thanh ghi con
    I2C_SendData(I2C1, sub);
    
    // Ch? cho d?n khi byte du?c truy?n di
    while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && ticks--) {}
    if (ticks == 0) return;
    ticks = I2C_TIMEOUT;
}

// G?i d? li?u c?n ghi
I2C_SendData(I2C1, data);

// Ch? cho d?n khi byte du?c truy?n di
while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && ticks--) {}
if (ticks == 0) return;
ticks = I2C_TIMEOUT;

// T?o di?u ki?n STOP d? k?t thúc giao ti?p
I2C_GenerateSTOP(I2C1, ENABLE);

// Ch? cho d?n khi bus I2C không còn b?n
while ((I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) && ticks--) {}
if (ticks == 0) return;

};

char I2C1_read(unsigned char HW_address, unsigned char sub){
    char data;
    
    // T?o di?u ki?n START
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    // G?i d?a ch? thi?t b? ? ch? d? truy?n d? ch?n thanh ghi
    I2C_Send7bitAddress(I2C1, HW_address, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    // N?u sub != 0xFF thì g?i d?a ch? thanh ghi con
    if (sub != 0xFF) {
        I2C_SendData(I2C1, sub);
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
    
    // T?o di?u ki?n START l?p l?i (repeated START) d? chuy?n sang ch? d? nh?n
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    // G?i d?a ch? thi?t b? ? ch? d? nh?n (RECEIVER)
    I2C_Send7bitAddress(I2C1, HW_address, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    // Ð?i cho d?n khi nh?n du?c byte d? li?u
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    
    // Ð?c d? li?u t? thanh ghi d? li?u
    data = I2C1->DR;
    
    // C?u hình v? trí NACK (không xác nh?n)
    I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);
    
    // T?o di?u ki?n STOP d? k?t thúc giao ti?p
    I2C_GenerateSTOP(I2C1, ENABLE);
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    
    // Tr? v? d? li?u dã d?c
    return data;
};


void I2C1_read_buf(unsigned char HW_address, unsigned char sub, unsigned char *p_buf, unsigned char buf_size){
    // T?o di?u ki?n START
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    // G?i d?a ch? thi?t b? ? ch? d? truy?n d? ch?n thanh ghi
    I2C_Send7bitAddress(I2C1, HW_address, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    // N?u sub != 0xFF thì g?i d?a ch? thanh ghi con
    if (sub != 0xFF) {
        I2C_SendData(I2C1, sub);
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
    
    // T?o di?u ki?n START l?p l?i (repeated START) d? chuy?n sang ch? d? nh?n
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    // B?t ch? d? ACK d? xác nh?n d? li?u nh?n du?c
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    
    // G?i d?a ch? thi?t b? ? ch? d? nh?n (RECEIVER)
    I2C_Send7bitAddress(I2C1, HW_address, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    // Ð?c d? li?u vào buffer
    for (uint8_t i = 0; i < buf_size; i++) {
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
        p_buf[i] = I2C1->DR;
    }
    
    // T?t ch? d? ACK sau khi d?c xong t?t c? các byte
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    
    // C?u hình v? trí NACK
    I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);
    
    // T?o di?u ki?n STOP d? k?t thúc giao ti?p
    I2C_GenerateSTOP(I2C1, ENABLE);
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
};


#endif


