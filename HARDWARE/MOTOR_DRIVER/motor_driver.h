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


//�˿ڶ���
#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1	

//״̬����
#define FORWARD ENABLE
#define BACKWARD 	DISABLE
#define MotorEnable ENABLE
#define MotorDisable DISABLE
#define BREAK ENABLE
#define NoBREAK	DISABLE

#define Right 1
#define Left 2
#define Advance 0

typedef enum{
	True = 1,
	False = 0
}bool;

typedef	struct{
	FunctionalState enable;
	FunctionalState dir;
	FunctionalState Break;
	u32 Acceleration;
}Motor_StatusDef;//���״̬����

void Motor_Init(void);//��ʼ��	
void PWMInit(u32 arr,u32 psc);
void SetMotorPWM(u8 flag,u32 arry);
void Motor_Status_Setup(Motor_StatusDef* StatusSetup);

extern double Diff_Calculation(double alpha);
extern Motor_StatusDef Motor_StatusInit;
#endif
