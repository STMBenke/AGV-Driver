#include "GPIO.h" 
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

//PB0,PB1Ϊ���ת�����,�ߵ�ƽΪ��ת
//PA0Ϊ���ʹ���ź�,�ߵ�ƽʹ��
//LED IO��ʼ��
void IO_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;
	
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);//ʹ��GPIOʱ��
  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOH, &GPIO_InitStructure);
	
	//GPIO_ResetBits(GPIOC,GPIO_Pin_7);//�����ź�,�õ�,����
	GPIO_ResetBits(GPIOH,GPIO_Pin_15);//ʹ���ź�ENĬ�ϸߵ�ƽ/��ʹ��
  GPIO_SetBits(GPIOH,GPIO_Pin_9);				//������ת
	GPIO_ResetBits(GPIOH,GPIO_Pin_14);					//������ת
	GPIO_SetBits(GPIOH,GPIO_Pin_13);		//ɲ���ź�
	//GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);//Ĭ�����Ҷ�����ת

	
}






