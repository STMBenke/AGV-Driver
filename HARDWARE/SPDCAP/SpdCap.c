#include "SpdCap.h"
#include "motor_driver.h"
#include "usart.h"
#include "sys.h"

u32 TIM3_CNT=0;
bool RisingFlag_EXIT0 =False;//������flag
bool RisingFlag_EXIT1 =False;
bool RisingFlag_EXIT2 =False;
bool RisingFlag_EXIT3 =False;
u32 EXIT0_CNT;//�ⲿ�жϼ���
u32 EXIT1_CNT;
u32 EXIT2_CNT;
u32 EXIT3_CNT;
u32 EXIT0_TIME;//���ڼ���ʱ��
u32 EXIT1_TIME;
u32 EXIT2_TIME;
u32 EXIT3_TIME;
u32 EXIT0_SPEED;//�ٶȼ���
u32 EXIT1_SPEED;
u32 EXIT2_SPEED;
u32 EXIT3_SPEED;

u8 num1,num2,num3,num0;//���ڼ���ƽ��ֵ

u32 EXIT0_SPEEDS[15]={0};


TIM_ICInitTypeDef TIM3_ICInitStructure;

void TIM3_Cap_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ��TIM3ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);  //ʹ��GPIOGʱ��
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;  //PG0,1,2,3 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//���빦��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //������IOĬ�ϵ͵�ƽ
	GPIO_Init(GPIOG,&GPIO_InitStructure); //��ʼ��PG
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ�� SYSCFG ʱ��
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource0);//��PG0����EXTI0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource1);//��PG1����EXTI1
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource2);//��PG2����EXTI2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource3);//��PG3����EXTI3
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1 | EXTI_Line2 | EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure);//����

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//�����ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//�ⲿ�ж�1
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//�����ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//�ⲿ�ж�2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//�ⲿ�ж�3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����

	
	//��ʼ����ʱ��3 TIM11	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 	//Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ�� 3 �����ж�
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ�� 3 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //��ռ���ȼ� 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //��Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);// ��ʼ�� NVIC

	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ�� 3
	
	


}

//�ٶȼ������

u32 Speed_Cal(u32 time)
{
	u32 f,speed;
	f= 1000000/time;
	speed=f;
	return speed;
	
}


//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)
{
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) 		//��������ж�
		{
				if(RisingFlag_EXIT0==1)
				TIM3_CNT++;
				if(TIM3_CNT>0xFFFF)TIM3_CNT=0;
		}
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	
}

//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{	
		if(RisingFlag_EXIT0 == True)//��2��������
		{
			if(TIM3_CNT>EXIT0_CNT)
			{
				EXIT0_CNT=TIM3_CNT-EXIT0_CNT;//�����ڴ��ڼ�����ֵ
				EXIT0_TIME=100*EXIT0_CNT;
				EXIT0_CNT=0;	
				EXIT0_SPEED+= Speed_Cal(EXIT0_TIME);
				num0++;
				if(num0==20){
					num0=0;
					EXIT0_SPEED=EXIT0_SPEED/20;
					printf("[ motor 0: %d  \r\n]  ", EXIT0_SPEED);
				}
			}			
			RisingFlag_EXIT0=False;			
		}
		else if( RisingFlag_EXIT0==0)//��һ�μ�⵽������
		{
			TIM3->CNT=0;
			EXIT0_CNT=TIM3_CNT;//��¼��ǰ�����ֵ
			RisingFlag_EXIT0 = True;
		}
		
	 EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ 
}	
//�ⲿ�ж�1�������
void EXTI1_IRQHandler(void)
{
		if(RisingFlag_EXIT1 == True)//��2��������
		{
			if(TIM3_CNT>EXIT1_CNT)
			{
				EXIT1_CNT=TIM3_CNT-EXIT1_CNT;//�����ڴ��ڼ�����ֵ
				EXIT1_TIME=100*EXIT1_CNT;
				EXIT1_CNT=0;	
				EXIT1_SPEED+= Speed_Cal(EXIT0_TIME);
				num1++;
				if(num1==20){
					num1=0;
					EXIT1_SPEED=EXIT1_SPEED/20;
					printf("[ motor 1: %d  \r\n]  ", EXIT1_SPEED);
				}
			}			
			RisingFlag_EXIT1=False;			
		}
		else if( RisingFlag_EXIT1==0)//��һ�μ�⵽������
		{
			TIM3->CNT=0;
			EXIT1_CNT=TIM3_CNT;//��¼��ǰ�����ֵ
			RisingFlag_EXIT1 = True;
		}
	 EXTI_ClearITPendingBit(EXTI_Line1);//���LINE2�ϵ��жϱ�־λ 
}
//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
		if(RisingFlag_EXIT2 == True)//��2��������
		{
			if(TIM3_CNT>EXIT2_CNT)
			{
				EXIT2_CNT=TIM3_CNT-EXIT2_CNT;//�����ڴ��ڼ�����ֵ
				EXIT2_TIME=100*EXIT2_CNT;
				EXIT2_CNT=0;	
				EXIT2_SPEED+= Speed_Cal(EXIT2_TIME);
				num2++;
				if(num2==20){
					num2=0;
					EXIT2_SPEED=EXIT2_SPEED/20;
					printf("[ motor 2: %d  \r\n]  ", EXIT2_SPEED);
				}
			}			
			RisingFlag_EXIT2=False;			
		}
		else if( RisingFlag_EXIT2==0)//��һ�μ�⵽������
		{
			TIM3->CNT=0;
			EXIT2_CNT=TIM3_CNT;//��¼��ǰ�����ֵ
			RisingFlag_EXIT2 = True;
		}
	 EXTI_ClearITPendingBit(EXTI_Line2);  //���LINE3�ϵ��жϱ�־λ  
}
//�ⲿ�ж�3�������
void EXTI3_IRQHandler(void)
{
		if(RisingFlag_EXIT3 == True)//��2��������
		{
			if(TIM3_CNT>EXIT3_CNT)
			{
				EXIT3_CNT=TIM3_CNT-EXIT3_CNT;//�����ڴ��ڼ�����ֵ
				EXIT3_TIME=100*EXIT3_CNT;
				EXIT3_CNT=0;	
				EXIT3_SPEED+= Speed_Cal(EXIT3_TIME);
				num3++;
				if(num3==20){
					num3=0;
					EXIT3_SPEED=EXIT3_SPEED/20;
					printf("[ motor 3: %d  \r\n]  ", EXIT3_SPEED);
				}
			}			
			RisingFlag_EXIT3=False;			
		}
		else if( RisingFlag_EXIT3==0)//��һ�μ�⵽������
		{
			TIM3->CNT=0;
			EXIT3_CNT=TIM3_CNT;//��¼��ǰ�����ֵ
			RisingFlag_EXIT3 = True;
		}
	 EXTI_ClearITPendingBit(EXTI_Line3);//���LINE4�ϵ��жϱ�־λ  
}
