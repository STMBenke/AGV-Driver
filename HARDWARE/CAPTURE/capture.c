#include "pwm.h"
#include "usart.h"
#include "capture.h"
TIM_ICInitTypeDef  TIM4_ICInitStructure;


u8 TIM4CH1_CAPTURE_STA = 0;	//ͨ��1���벶���־������λ�������־����6λ�������־		
u16 TIM4CH1_CAPTURE_UPVAL;
u16 TIM4CH1_CAPTURE_DOWNVAL;

u8 TIM4CH2_CAPTURE_STA = 0;	//ͨ��2���벶���־������λ�������־����6λ�������־		
u16 TIM4CH2_CAPTURE_UPVAL;
u16 TIM4CH2_CAPTURE_DOWNVAL;

u8 TIM4CH3_CAPTURE_STA = 0;	//ͨ��3���벶���־������λ�������־����6λ�������־		
u16 TIM4CH3_CAPTURE_UPVAL;
u16 TIM4CH3_CAPTURE_DOWNVAL;

u8 TIM4CH4_CAPTURE_STA = 0;	//ͨ��4���벶���־������λ�������־����6λ�������־		
u16 TIM4CH4_CAPTURE_UPVAL;
u16 TIM4CH4_CAPTURE_DOWNVAL; 

u32 tim4up1 = 0;	//�����ܸߵ�ƽ��ʱ��
u32 tim4up2 = 0;	//�����ܸߵ�ƽ��ʱ��
u32 tim4up3 = 0;	//�����ܸߵ�ƽ��ʱ��
u32 tim4up4 = 0;	//�����ܸߵ�ƽ��ʱ��

u32 tim4_T1;//��Ϊ�������µĲ���ֵ
u32 tim4_T2;
u32 tim4_T3;
u32 tim4_T4;

//��ʱ��4ͨ��1���벶������
//arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
//psc��ʱ��Ԥ��Ƶ��
void TIM4_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM4ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	//ʹ��PORTDʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; //GPIOD12, 13, 14, 15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PD

	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); //PD����λ��ʱ��5
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_TIM4);
  
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	

	//��ʼ��TIM4���벶��ch1����
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_1; 
  TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  TIM_ICInit(TIM4, &TIM4_ICInitStructure);	
	
		//��ʼ��TIM3���벶����� ͨ��2
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ,����Ƶ 
	TIM4_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM4, &TIM4_ICInitStructure);

	//��ʼ��TIM3���벶����� ͨ��3
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ,����Ƶ 
	TIM4_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM4, &TIM4_ICInitStructure);

	//��ʼ��TIM3���벶����� ͨ��4
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ,����Ƶ 
	TIM4_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM4, &TIM4_ICInitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//����CC1,2,3,4IE�����ж�//����û�п��ǵ����������������Ҳ�����Ǹ����ж�
	TIM_ITConfig(TIM4,TIM_IT_CC2,ENABLE);
  TIM_Cmd(TIM4,ENABLE ); 	//ʹ�ܶ�ʱ��4

 
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	
}
//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)

//��ʱ��5�жϷ������	 
void TIM4_IRQHandler(void)
{ 		    

 	if((TIM4CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		
		if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC1); 		//����жϱ�־λ
			if (TIM4CH1_CAPTURE_STA & 0X40)		//�����½���
			{
				TIM4CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM4CH1_CAPTURE_DOWNVAL = TIM_GetCapture1(TIM4);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM4CH1_CAPTURE_DOWNVAL <= TIM4CH1_CAPTURE_UPVAL)
				{
					tim4_T1 = (0xFFFF-TIM4CH1_CAPTURE_UPVAL)+TIM4CH1_CAPTURE_DOWNVAL ;
				}
				else
					tim4_T1 = 0;
				tim4up1 = TIM4CH1_CAPTURE_DOWNVAL - TIM4CH1_CAPTURE_UPVAL + tim4_T1;		//�õ�������ʱ���
				TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Rising); //����Ϊ�����ز���
				TIM4CH1_CAPTURE_STA=0;
			}
			else //��һ�β�������
			{
				TIM4CH1_CAPTURE_UPVAL = TIM_GetCapture1(TIM4);		//��ȡ����ֵ
				TIM4CH1_CAPTURE_STA |= 0X40;		//����Ѳ���������
				TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//����Ϊ�½��ز���
			}

		}			     	    					   
 	}
	
	if((TIM4CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		
		if(TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)//����2���������¼�
		{	
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC2); 		//����жϱ�־λ
			if (TIM4CH2_CAPTURE_STA & 0X40)		//�����½���
			{
				TIM4CH2_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM4CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM4);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM4CH2_CAPTURE_DOWNVAL <= TIM4CH2_CAPTURE_UPVAL)
				{
					tim4_T2 = (0xFFFF-TIM4CH2_CAPTURE_UPVAL)+TIM4CH2_CAPTURE_DOWNVAL ;
				}
				else
					tim4_T2 = 0;
				tim4up2 = TIM4CH2_CAPTURE_DOWNVAL - TIM4CH2_CAPTURE_UPVAL + tim4_T2;		//�õ�������ʱ���
				TIM_OC2PolarityConfig(TIM4,TIM_ICPolarity_Rising); //����Ϊ�����ز���
				TIM4CH2_CAPTURE_STA=0;
			}
			else //��һ�β�������
			{
				TIM4CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM4);		//��ȡ����ֵ
				TIM4CH2_CAPTURE_STA |= 0X40;		//����Ѳ���������
				TIM_OC2PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//����Ϊ�½��ز���
			}

		}			     	    					   
 	}
	
	if((TIM4CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		
		if(TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)//����3���������¼�
		{	
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC3); 		//����жϱ�־λ
			if (TIM4CH3_CAPTURE_STA & 0X40)		//�����½���
			{
				TIM4CH3_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM4CH3_CAPTURE_DOWNVAL = TIM_GetCapture3(TIM4);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM4CH3_CAPTURE_DOWNVAL <= TIM4CH3_CAPTURE_UPVAL)
				{
					tim4_T3 = (0xFFFF-TIM4CH3_CAPTURE_UPVAL)+TIM4CH3_CAPTURE_DOWNVAL ;
				}
				else
					tim4_T3 = 0;
				tim4up3 = TIM4CH3_CAPTURE_DOWNVAL - TIM4CH3_CAPTURE_UPVAL + tim4_T3;		//�õ�������ʱ���
				TIM_OC3PolarityConfig(TIM4,TIM_ICPolarity_Rising); //����Ϊ�����ز���
				TIM4CH3_CAPTURE_STA=0;
			}
			else //��һ�β�������
			{
				TIM4CH3_CAPTURE_UPVAL = TIM_GetCapture3(TIM4);		//��ȡ����ֵ
				TIM4CH3_CAPTURE_STA |= 0X40;		//����Ѳ���������
				TIM_OC3PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//����Ϊ�½��ز���
			}

		}			     	    					   
 	}
	if((TIM4CH4_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		
		if(TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)//����4���������¼�
		{	
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC4); 		//����жϱ�־λ
			if (TIM4CH4_CAPTURE_STA & 0X40)		//�����½���
			{
				TIM4CH4_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM4CH4_CAPTURE_DOWNVAL = TIM_GetCapture4(TIM4);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM4CH4_CAPTURE_DOWNVAL <= TIM4CH4_CAPTURE_UPVAL)
				{
					tim4_T4 = (0xFFFF-TIM4CH4_CAPTURE_UPVAL)+TIM4CH4_CAPTURE_DOWNVAL ;
				}
				else
					tim4_T4 = 0;
				tim4up4 = TIM4CH4_CAPTURE_DOWNVAL - TIM4CH4_CAPTURE_UPVAL + tim4_T4;		//�õ�������ʱ���
				TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Rising); //����Ϊ�����ز���
				TIM4CH4_CAPTURE_STA=0;
			}
			else //��һ�β�������
			{
				TIM4CH4_CAPTURE_UPVAL = TIM_GetCapture4(TIM4);		//��ȡ����ֵ
				TIM4CH4_CAPTURE_STA |= 0X40;		//����Ѳ���������
				TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//����Ϊ�½��ز���
			}

		}			     	    					   
 	}
	/*
	//ch2
	if((TIM4CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//���
		{	     
			if(TIM4CH2_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM4CH2_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM4CH2_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
					TIM4CH2_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM4CH2_CAPTURE_STA++;
			}	 
		}
		if(TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)//����2���������¼�
		{	
			if(TIM4CH2_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM4CH2_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
			  TIM4CH2_CAPTURE_VAL=TIM_GetCapture2(TIM4);//��ȡ��ǰ�Ĳ���ֵ.
	 			TIM_OC2PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC2P=0 ����Ϊ�����ز���
			}
			else  								//��δ��ʼ,��һ�β���������
			{
				TIM4CH2_CAPTURE_STA=0;			//���
				TIM4CH2_CAPTURE_VAL=0;
				TIM4CH2_CAPTURE_STA|=0X40;		//��ǲ�����������
				TIM_Cmd(TIM4,DISABLE ); 	//�رն�ʱ��5
	 			//TIM_SetCounter(TIM4,0);
	 			TIM_OC2PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC2P=1 ����Ϊ�½��ز���
				TIM_Cmd(TIM4,ENABLE ); 	//ʹ�ܶ�ʱ��5
			}		    
		}			     	    					   
 	}
		//ch3
	if((TIM4CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//���
		{	     
			if(TIM4CH3_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM4CH3_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM4CH3_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
					TIM4CH3_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM4CH3_CAPTURE_STA++;
			}	 
		}
		if(TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)//����3���������¼�
		{	
			if(TIM4CH3_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM4CH3_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
			  TIM4CH3_CAPTURE_VAL=TIM_GetCapture3(TIM4);//��ȡ��ǰ�Ĳ���ֵ.
	 			TIM_OC3PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC3P=0 ����Ϊ�����ز���
			}
			else  								//��δ��ʼ,��һ�β���������
			{
				TIM4CH3_CAPTURE_STA=0;			//���
				TIM4CH3_CAPTURE_VAL=0;
				TIM4CH3_CAPTURE_STA|=0X40;		//��ǲ�����������
				TIM_Cmd(TIM4,DISABLE ); 	//�رն�ʱ��5
	 			TIM_SetCounter(TIM4,0);
	 			TIM_OC3PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC3P=1 ����Ϊ�½��ز���
				TIM_Cmd(TIM4,ENABLE ); 	//ʹ�ܶ�ʱ��5
			}		    
		}			     	    					   
 	}
	//ch4
	if((TIM4CH4_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//���
		{	     
			if(TIM4CH4_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM4CH4_CAPTURE_STA&0X4F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM4CH4_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
					TIM4CH4_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM4CH4_CAPTURE_STA++;
			}	 
		}
		if(TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)//����4���������¼�
		{	
			if(TIM4CH4_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM4CH4_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
			  TIM4CH4_CAPTURE_VAL=TIM_GetCapture4(TIM4);//��ȡ��ǰ�Ĳ���ֵ.
	 			TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC4P=0 ����Ϊ�����ز���
			}
			else  								//��δ��ʼ,��һ�β���������
			{
				TIM4CH4_CAPTURE_STA=0;			//���
				TIM4CH4_CAPTURE_VAL=0;
				TIM4CH4_CAPTURE_STA|=0X40;		//��ǲ�����������
				TIM_Cmd(TIM4,DISABLE ); 	//�رն�ʱ��5
	 			TIM_SetCounter(TIM4,0);
	 			TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC3P=1 ����Ϊ�½��ز���
				TIM_Cmd(TIM4,ENABLE ); 	//ʹ�ܶ�ʱ��5
			}		    
		}			     	    					   
 	}
	*/
}



/*
u32 period = 0;
u32 duty  = 0;
u8 CollectFlag = 1;

void TIM4_IRQHandler(void)
{

	if(CollectFlag){
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			duty = TIM_GetCapture1(TIM4); 				//�ɼ�ռ�ձ�		
		  period	=	TIM_GetCapture2(TIM4);     //�ɼ�����
		}
		
		CollectFlag = 0;
	}	
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
}*/


