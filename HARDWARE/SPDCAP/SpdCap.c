#include "SpdCap.h"

u8 TIM3CH1_CAPTURE_STA = 0;	//ͨ��1���벶���־������λ�������־����6λ�������־		
u16 TIM3CH1_CAPTURE_UPVAL;
u16 TIM3CH1_CAPTURE_DOWNVAL;

u8 TIM3CH2_CAPTURE_STA = 0;	//ͨ��2���벶���־������λ�������־����6λ�������־		
u16 TIM3CH2_CAPTURE_UPVAL;
u16 TIM3CH2_CAPTURE_DOWNVAL;

u8 TIM3CH3_CAPTURE_STA = 0;	//ͨ��3���벶���־������λ�������־����6λ�������־		
u16 TIM3CH3_CAPTURE_UPVAL;
u16 TIM3CH3_CAPTURE_DOWNVAL;

u8 TIM3CH4_CAPTURE_STA = 0;	//ͨ��1���벶���־������λ�������־����6λ�������־		
u16 TIM3CH4_CAPTURE_UPVAL;
u16 TIM3CH4_CAPTURE_DOWNVAL;

u32 tempup1 = 0;	//�����ܸߵ�ƽ��ʱ��
u32 tempup2 = 0;	//�����ܸߵ�ƽ��ʱ��
u32 tempup3 = 0;	//�����ܸߵ�ƽ��ʱ��
u32 tempup4 = 0;	//�����ܸߵ�ƽ��ʱ��
u32 f1=0;//����Ƶ��
u32 f2=0;//����Ƶ��
u32 f3=0;//����Ƶ��
u32 f4=0;//����Ƶ��



u32 tim3_T1;
u32 tim3_T2;
u32 tim3_T3;
u32 tim3_T4;

int pwmout1, pwmout2, pwmout3, pwmout4; 				//���ռ�ձ�

//��ʱ��4ͨ��1���벶������

TIM_ICInitTypeDef TIM3_ICInitStructure;

void TIM3_Cap_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ��TIM3ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  //ʹ��GPIOCʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //ʹ��GPIOBʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  //PB6,7,8,9 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PC
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  //PB6,7,8,9 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PB
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3); //PC6����λ��ʱ��3
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3); //PC7����λ��ʱ��3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3); //PB0����λ��ʱ��3
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3); //PB1����λ��ʱ��3


	//��ʼ����ʱ��4 TIM3	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 	//Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	//��ʼ��TIM3���벶����� ͨ��1
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ,����Ƶ 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//��ʼ��TIM3���벶����� ͨ��2
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ,����Ƶ 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//��ʼ��TIM3���벶����� ͨ��3
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ,����Ƶ 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//��ʼ��TIM3���벶����� ͨ��4
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ,����Ƶ 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�1��
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);   //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 

	TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4,ENABLE);   //����������жϣ�����CC1IE,CC2IE,CC3IE,CC4IE�����ж�	

	TIM_Cmd(TIM3, ENABLE); 		//ʹ�ܶ�ʱ��3

}

//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)
{
	if ((TIM3CH1_CAPTURE_STA & 0X80) == 0) 		//��δ�ɹ�����	
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) 		//����1���������¼�
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC1); 		//����жϱ�־λ
			if (TIM3CH1_CAPTURE_STA & 0X40)		//������һ��������
			{
				TIM3CH1_CAPTURE_DOWNVAL = TIM_GetCapture1(TIM3);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM3CH1_CAPTURE_DOWNVAL <= TIM3CH1_CAPTURE_UPVAL)
				{
					tim3_T1 = (65535-TIM3CH1_CAPTURE_UPVAL)+TIM3CH1_CAPTURE_DOWNVAL ;
				}
				else
					tim3_T1 = 0;
				tempup1 = TIM3CH1_CAPTURE_DOWNVAL - TIM3CH1_CAPTURE_UPVAL + tim3_T1;		//�õ����������ص�ʱ���
				TIM3CH1_CAPTURE_STA = 0;		//�����־λ����
				f1=1000000/tempup1;
			}
			else //��һ�β�������
			{
				TIM3CH1_CAPTURE_UPVAL = TIM_GetCapture1(TIM3);		//��ȡ����ֵ
				TIM3CH1_CAPTURE_STA |= 0X40;		//����Ѳ���������
			}
		}
	}

	if ((TIM3CH2_CAPTURE_STA & 0X80) == 0) 		//��δ�ɹ�����	
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET) 		//����1���������¼�
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC2); 		//����жϱ�־λ
			if (TIM3CH2_CAPTURE_STA & 0X40)		//������һ��������
			{
				TIM3CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM3);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM3CH2_CAPTURE_DOWNVAL < TIM3CH2_CAPTURE_UPVAL)
				{
					tim3_T2 = (65535-TIM3CH2_CAPTURE_UPVAL)+TIM3CH2_CAPTURE_DOWNVAL ;
					
				}
				else
					tim3_T2 = 0;
				tempup2 = TIM3CH2_CAPTURE_DOWNVAL - TIM3CH2_CAPTURE_UPVAL + tim3_T2;		//�õ����������ص�ʱ���
				TIM3CH2_CAPTURE_STA = 0;		//�����־λ����
			  f2=1000000/tempup2;

			}
			else //��һ�β�������
			{
				TIM3CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM3);		//��ȡ����ֵ
				TIM3CH2_CAPTURE_STA |= 0X40;		//����Ѳ���������
			}
		}
	}
	
	if ((TIM3CH3_CAPTURE_STA & 0X80) == 0) 		//��δ�ɹ�����	
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET) 		//����3���������¼�
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC3); 		//����жϱ�־λ
			if (TIM3CH3_CAPTURE_STA & 0X40)		//������һ��������
			{
				TIM3CH3_CAPTURE_DOWNVAL = TIM_GetCapture3(TIM3);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM3CH3_CAPTURE_DOWNVAL < TIM3CH3_CAPTURE_UPVAL)
				{
					//tim3_T3 = (65535-TIM3CH3_CAPTURE_UPVAL)+TIM3CH3_CAPTURE_DOWNVAL ;
					tim3_T3 = 65535;
				}
				else
					tim3_T3 = 0;
				tempup3 = TIM3CH3_CAPTURE_DOWNVAL - TIM3CH3_CAPTURE_UPVAL + tim3_T3;		//�õ����������ص�ʱ���
				TIM3CH3_CAPTURE_STA = 0;		//�����־λ����
				f3=1000000/tempup3;
			}
			else //��һ�β�������
			{
				TIM3CH3_CAPTURE_UPVAL = TIM_GetCapture3(TIM3);		//��ȡ����ֵ
				TIM3CH3_CAPTURE_STA |= 0X40;		//����Ѳ���������
			}
		}
	}

		if ((TIM3CH4_CAPTURE_STA & 0X80) == 0) 		//��δ�ɹ�����	
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET) 		//����4���������¼�
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC4); 		//����жϱ�־λ
			if (TIM3CH4_CAPTURE_STA & 0X40)		//������һ��������
			{
				TIM3CH4_CAPTURE_DOWNVAL = TIM_GetCapture4(TIM3);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM3CH4_CAPTURE_DOWNVAL < TIM3CH4_CAPTURE_UPVAL)
				{
					tim3_T4 = (65535-TIM3CH4_CAPTURE_UPVAL)+TIM3CH4_CAPTURE_DOWNVAL ;
				}
				else
					tim3_T4 = 0;
				tempup4 = TIM3CH4_CAPTURE_DOWNVAL - TIM3CH4_CAPTURE_UPVAL + tim3_T4;		//�õ����������ص�ʱ���
				TIM3CH4_CAPTURE_STA = 0;		//�����־λ����
				f4=1000000/tempup4;
			}
			else //��һ�β�������
			{
				TIM3CH4_CAPTURE_UPVAL = TIM_GetCapture4(TIM3);		//��ȡ����ֵ
				TIM3CH4_CAPTURE_STA |= 0X40;		//����Ѳ���������
			}
		}
	}
}
