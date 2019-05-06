#ifndef __MOTOR_DRIVER_H
#define __MOTOR_DRIVER_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//LED�˿ڶ���
#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1	 
#define FORWARD 1
#define BACK 	0
#define MotorEnable 1
#define MotorDisable 0
#define BREAK 1
#define NoBREAK	0

#define Right 1
#define Left 2
#define Advance 0

void IO_Init(void);//��ʼ��	
void PWMInit(u32 arr,u32 psc);
void SetMotorPWM(u8 flag,u32 arry);
void Motor_Status_Setup(u8 en,u8 dir,u8 bk,u32 speed);

extern double Diff_Calculation(double alpha);

#endif
