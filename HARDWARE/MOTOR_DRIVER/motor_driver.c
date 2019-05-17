/************************************************************************
*�˲��ֳ���ΪAGV����ֱ�������������������������
*����TIM5 PWM���
*����������������
*				extern double Diff_Calculation(double alpha);//���Ҳ��ټ���
*				extern Motor_StatusDef Motor_StatusInit;//���״̬�ṹ��
*************************************************************************/



#include "motor_driver.h" 
#include "steering_driver.h"
#include "math.h"
#include "usart.h"
#define PI 3.1416

//PH9,PH14Ϊ���ת�����,�͵�ƽΪ��ת
//PH15 Ϊ���ʹ���ź�,�͵�ƽʹ��
//PH13 Ϊɲ���ź� �͵�ƽɲ��

Motor_StatusDef Motor_StatusInit;//���״̬�趨�ṹ��

void Motor_Init(void)
{   
	//IO��ʼ��
  GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);//ʹ��GPIOʱ��
  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOH, &GPIO_InitStructure);
	
	//���״̬��ʼ��
	Motor_StatusInit.enable= MotorDisable;
	Motor_StatusInit.dir = FORWARD;
	Motor_StatusInit.Break = NoBREAK;
	Motor_StatusInit.Acceleration=0;
	
	Motor_Status_Setup(&Motor_StatusInit);
	
//	GPIO_ResetBits(GPIOH,GPIO_Pin_15);//ʹ���ź�ENĬ�ϵ͵�ƽ/ʹ��
//  GPIO_SetBits(GPIOH,GPIO_Pin_9);				//������ת
//	GPIO_ResetBits(GPIOH,GPIO_Pin_14);			//������ת
//	GPIO_SetBits(GPIOH,GPIO_Pin_13);		//ɲ���źţ���ɲ��


	
}

//TIM5 PWM���ֳ�ʼ�� 
//PH10~12,PA3��ӦTIM5CH1~4ͨ�����
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void PWMInit(u32 arr,u32 psc)
{		 					 
	//�˲������ֶ��޸�IO������
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE); 	//ʹ��PORTHʱ��	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource10,GPIO_AF_TIM5); //GPIO����Ϊ��ʱ��5
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource11,GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource12,GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM5);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOH,&GPIO_InitStructure);              //��ʼ��PE5

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����           //��ʼ��PE5
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//��ʼ����ʱ��9
	
	//��ʼ��TIM9 Channel PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM5OC1~OC4
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);
	TIM_OC3Init(TIM5, &TIM_OCInitStructure);
	TIM_OC4Init(TIM5, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIM5��CCR1~4�ϵ�Ԥװ�ؼĴ���
  TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);
	
  TIM_ARRPreloadConfig(TIM5,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIM5
 
										  
}  

//���ռ�ձ��趨
//falg������percent���ٶȰٷֱ�0~100
void SetMotorPWM(u8 flag,u32 percent)
{
			
			double	alpha;//��ǰת���
	if(percent<=100)
	{
			u32 duty= 500-5*percent;//�ĳɣ�500~0������
			
			if(flag== Right){//��ת
				alpha = abs(current_pos)*0.00063;//ÿ������ת��ԼΪ0.00063��
				duty=Diff_Calculation(alpha)*percent; //����ռ��ϵ��*����Ȧ�ٶȱ���
				TIM_SetCompare3(TIM5,duty);//��߼���
				TIM_SetCompare4(TIM5,duty);
			}
			else if(flag== Left){//��ת
				alpha = abs(current_pos)*0.00063;//ÿ������ת��ԼΪ0.00063��
				duty=Diff_Calculation(alpha)*percent; //����ռ��ϵ��*����Ȧ�ٶȱ���
				TIM_SetCompare1(TIM5,duty);//�ұ߼���
				TIM_SetCompare2(TIM5,duty);
				//printf("diff: %d \r\n",duty);
			}
			else//����ǰ�����������
			{
				TIM_SetCompare1(TIM5,duty);
				TIM_SetCompare2(TIM5,duty);
				TIM_SetCompare3(TIM5,duty);
				TIM_SetCompare4(TIM5,duty);
			}
		}
}

//���״̬����
void Motor_Status_Setup(Motor_StatusDef* StatusSetup)
{
	if(StatusSetup->enable==MotorDisable)GPIO_ResetBits(GPIOH,GPIO_Pin_15);//ʹ���ź�ENĬ�ϵ͵�ƽ/��ʹ��
	else if (StatusSetup->enable==MotorEnable)GPIO_SetBits(GPIOH,GPIO_Pin_15);//ʹ��
	
	if(StatusSetup->Break==NoBREAK)GPIO_SetBits(GPIOH,GPIO_Pin_13);	//��ɲ��
	else GPIO_ResetBits(GPIOH,GPIO_Pin_13);//ɲ��
	
	if(StatusSetup->dir==FORWARD){
		GPIO_SetBits(GPIOH,GPIO_Pin_9);				//������ת
		GPIO_ResetBits(GPIOH,GPIO_Pin_14);					//������ת
	}
	else{
		GPIO_ResetBits(GPIOH,GPIO_Pin_9);				//����ת
		GPIO_SetBits(GPIOH,GPIO_Pin_14);					//����ת
	}
	
	//���ٶ��趨��д

	
}


//���ٱ������� 
double Diff_Calculation(double alpha)
{
	double la,lb; 
	double diff;
	double si =sin(alpha*3.1416/180);
	la = 7.0/18.0 * 3.1416* alpha / si;//��Ȧ,·��С
	//diff = 20.0;//�̶�����
	diff = 0.0018*pow(alpha,3) - 0.0395*pow(alpha,2)+0.3532*alpha-0.0238;
	alpha+=diff;
	si =sin(alpha*3.1416/180);
	lb = 7.0/18.0 * PI*alpha / si;//��Ȧ��·�̴�
	return lb/la;
	
	
	
}




