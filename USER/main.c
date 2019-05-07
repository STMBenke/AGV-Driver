#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "motor_driver.h"
#include "controler_capture.h"
#include "controler_logic.h"
#include "steering_driver.h"
//#include	"SpdCap.h"
#include "key.h"

//#include "usmart.h"

u8 RxProcess(void);//�������ݴ���
void TxProcess(u8 sendData);//�������ݷ���
//void SetMotorPWM(u32 arry);
//void RemoteControler(void);

int main(void)
{  
	//u8 temp=0;int i=0;

	u8 keyval;//�������

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);//��ʼ�����ڲ�����Ϊ115200
	Motor_Init();
 	PWMInit(500-1,84-1);	//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.  
  SetMotorPWM(0,500);//500��Ӧ0ռ�ձ�
	KEY_Init();					//������ʼ��
	TIM4_Cap_Init(0XFFFF,84-1); //��1Mhz��Ƶ�ʼ��� �����ʱ��65535us,ң���������������2160us
//  TIM3_Cap_Init(0xffff,84-1); //��1Mhz��Ƶ�ʼ�
	
	Driver_Init();			//���������������ʼ��
	TIM8_OPM_RCR_Init(999,168-1); //TIM8Ϊ168MHz������1MHz����Ƶ��  ������+�ظ�����ģʽ  

   while(1) 
	{
 		//delay_ms(10);
		
#if 0    //PC�˴��ڵ��Գ���
		temp= RxProcess();
		if(USART_RX_BUF[0]!=0)printf("�������:%d\r\n",100-temp);
		//�����������,׼����һ�ν���
		for(i=0;i<USART_RX_STA;i++)
		{
			USART_RX_BUF[i]=0;
		}
		USART_RX_STA=0;
#endif
		
		//ң�س���
		RemoteControler();
	
		//�ٶȷ�������
		//delay = SpeedFeedback(delay);

		
		//ת��΢��	
		keyval=KEY_Scan(0);
		if(keyval==KEY3_PRES)
		{
			Locate_Abs(0,7000);//����KEY3�������
		}else if(keyval==KEY1_PRES)
		{
			Locate_Rle(1000,7000,CW);//����KEY1����7000Hz��Ƶ�� ˳ʱ�뷢1000����, 
		}else if(keyval==KEY2_PRES)
		{
			Locate_Rle(1000,7000,CCW);//����KEY2����7000Hz��Ƶ�� ��ʱ�뷢1000����
		}			
		
	

	
	}	
}

//void SetMotorPWM(u32 arry)
//{
//			double	alpha;//��ǰת���
//			u32 duty= arry;
//			
//			if(flag==1){//��ת
//				alpha = abs(current_pos)*0.00063;//ÿ������ת��ԼΪ0.00063��
//				duty=1.0/Diff_Calculation(alpha)*arry; //����ռ��ϵ��*����Ȧ�ٶȱ���
//				TIM_SetCompare1(TIM5,duty);
//				TIM_SetCompare2(TIM5,duty);
//			}
//			else if(flag==2){//��ת
//				alpha = abs(current_pos)*0.00063;//ÿ������ת��ԼΪ0.00063��
//				duty=1.0/Diff_Calculation(alpha)*arry; //����ռ��ϵ��*����Ȧ�ٶȱ���
//				TIM_SetCompare3(TIM5,duty);
//				TIM_SetCompare4(TIM5,duty);
//				//printf("diff: %d \r\n",duty);
//			}
//			else
//			{
//				TIM_SetCompare1(TIM5,duty);
//				TIM_SetCompare2(TIM5,duty);
//				TIM_SetCompare3(TIM5,duty);
//				TIM_SetCompare4(TIM5,duty);
//			}
//}

//void RemoteControler() //ң�س���
//{
//		
//		k1=(float)(500-0)/(float)(216-87);//����Ĳ���ֵ��87~215���궨��0~500
//		k_right=(float)35000/(float)(21-15);//��תϵ��
//		k_left=(float)35000/(float)(15-8);//��תϵ��
//		count1=tim4up1/10;		   //ȥ�����һλ���ȶ�����������֮��Χ�����87~215��
//		count2=tim4up2/10;
//		count3=tim4up3/10;		   //ȥ�����һλ���ȶ�����������֮��Χ�����87~215��
//		count4=tim4up4/10;
//		
//		
//		//printf("HIGH:%lld us\r\n",count1); 
//		//printf("DIR: %d \r\n",count2);
//		//printf("DIR: %lld \n",count3);
//	
//	
//		//���������ת����PWM�������
//		//count1=count1/10;//
//		if(count1<300)
//		{
//			//printf("HIGH:%d us\r\n",tim4up1); 
//			count1=500-(int)(k1*(count1-87));//�궨����500��0������
//			//printf("duty:%d \r\n",count1); 
//			SetMotorPWM(flag,count1);
//		}
//		if(count2<109)
//		{
//			GPIO_SetBits(GPIOH,GPIO_Pin_9);//��ת
//			GPIO_ResetBits(GPIOH,GPIO_Pin_14);
//			GPIO_SetBits(GPIOH,GPIO_Pin_13);//��ɲ��
//			GPIO_SetBits(GPIOH,GPIO_Pin_15);//���ʹ
//	
//		}
//		else if(count2>190&&count2<300)
//		{
//			GPIO_ResetBits(GPIOH,GPIO_Pin_9);//��ת
//			GPIO_SetBits(GPIOH,GPIO_Pin_14);
//			GPIO_SetBits(GPIOH,GPIO_Pin_13);//��ɲ��
//			GPIO_SetBits(GPIOH,GPIO_Pin_15);//���ʹ
//			
//		}
//		else if(count2>140&&count2<160) 
//		{
//			 GPIO_ResetBits(GPIOH,GPIO_Pin_15);//�ر�ʹ��
//			
//		}
//		
//		pst=count3/10;  //��ң��������궨��(8,21)
//		if(pst<300&&pst!=pst_old)//ң�ط����б仯
//		{
//			dir_delay ++;
//			if(dir_delay==5000)//��ʱ����ֹƵ��ת��
//			{
//				if(pst!=pst_old)
//				{
//					dir_delay=0;
//					if(pst>15&&pst<30&&pst!=pst_old)//��ת
//					{
//						pst_old=pst;
//						flag=1;
//						Locate_Abs(((int)k_right*(pst-15)),7000);
//						
//						
//					}
//					else if(pst>0&&pst<15&&pst!=pst_old)//��ת
//					{
//						pst_old=pst;
//						flag=2;
//						Locate_Abs(-35000+(int)k_left*(pst-8),7000);
//						
//						
//					}
//					else if(pst==15&&pst!=pst_old)
//					{	
//						if(current_pos!=0)
//						{
//							flag=0;
//							Locate_Abs(0,7000);
//							
//							
//						}
//						pst_old =15;
//					}
//				}
//			}
//		}
//	

//}	
//u32 SpeedFeedback(u32 delay)
//{
//		 
//		delay ++;
//		if(delay == 5)
//		{
//			delay=0;
//			printf("[ motor 1: %d hz ]  ",f1);
//			printf("[ motor 2: %d hz ] \n",f2);
//			printf("[ motor 3: %d us ]  ",f3);
//			printf("[ motor 4: %d us ]\r\n",f4);
//		}
//	return delay;
//}
//u8 RxProcess(void)//�������ݴ���
//{
//	u8 temp = 0;
//	float k;

//	switch(USART_RX_BUF[0])//ѡ��ģʽ
//	{
//		case 0x10://10�ݶ��������ģʽ
//		{
//			//GPIO_ResetBits(GPIOH,GPIO_Pin_9);
//			GPIO_SetBits(GPIOH,GPIO_Pin_9);//��ת
//			GPIO_ResetBits(GPIOH,GPIO_Pin_14);
//			GPIO_SetBits(GPIOH,GPIO_Pin_13);//��ɲ��
//			temp = USART_RX_BUF[1];
//			k=(float)(500-0)/(float)(100-0);
//			if(temp<=100)
//			{
//				SetMotorPWM(0,(u32)k*temp);
//				GPIO_SetBits(GPIOH,GPIO_Pin_15);//���ʹ�
//			}
//			else	printf("���ݴ���\r\n");
//		}break;
//		case 0x11:
//		{
//			GPIO_ResetBits(GPIOH,GPIO_Pin_9);//��ת
//			GPIO_SetBits(GPIOH,GPIO_Pin_14);
//			GPIO_SetBits(GPIOH,GPIO_Pin_13);//��ɲ��
//			//GPIO_SetBits(GPIOH,GPIO_Pin_9);
//			temp = USART_RX_BUF[1];
//			k=(float)(500-0)/(float)(100-0);
//			if(temp<=100)
//			{
//					SetMotorPWM(0,(u32)k*temp);	
//					GPIO_SetBits(GPIOH,GPIO_Pin_15);//���ʹ�
//			}
//		}break;
//		case 0X12:
//			GPIO_ResetBits(GPIOH,GPIO_Pin_15);break;//�ر�ʹ��
//			
//		case 0X13:
//			GPIO_ResetBits(GPIOH,GPIO_Pin_13);break;//ɲ��
//	}
//	return temp;//����ռ�ձ�
//}
//void TxProcess(u8 sendData)//�������ݷ���
//{
//	switch(USART_RX_BUF[0])
//	{	
//		case 0x10:
//			USART_SendData(USART1,sendData);
//	}
//}
