#ifndef __CAPTURE_H
#define __CAPTURE_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.2
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//�޸�˵��
//V1.1 20140504
//����TIM14_PWM_Init����,����PWM���
//V1.2 20140504
//����TIM5_CH1_Cap_Init����,�������벶��
////////////////////////////////////////////////////////////////////////////////// 	

extern u8  TIM4CH1_CAPTURE_STA;		//���벶��״
extern u8  TIM4CH2_CAPTURE_STA;		//���벶��״̬		    				
extern u8  TIM4CH3_CAPTURE_STA;		//���벶��״̬		    				
extern u8  TIM4CH4_CAPTURE_STA;		//���벶��״̬		    				

extern u32 tim4up1;	//�����ܸߵ�ƽʱ��
extern u32 tim4up2;	//�����ܸߵ�ƽʱ��
extern u32 tim4up3;	//�����ܸߵ�ƽʱ��
extern u32 tim4up4;	//�����ܸߵ�ƽʱ��


//void TIM14_PWM_Init(u32 arr,u32 psc);
void TIM4_Cap_Init(u32 arr,u16 psc);

#endif























