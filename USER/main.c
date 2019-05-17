#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "motor_driver.h"
#include "controler_capture.h"
#include "controler_logic.h"
#include "steering_driver.h"
#include	"SpdCap.h"
#include "key.h"
#include "can.h"


//u8 RxProcess(void);//�������ݴ���
//void TxProcess(u8 sendData);//�������ݷ���


int main(void)
{  
	u8 keyval;//�������
	//u8 CanRxBuf[8];//can���߽�������
	u8 CanTxBuf[7];//can���߷�������
	u8 CanT_Flag;
	u8 CanR_Flag;
	
	//���ֳ�ʼ������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);//��ʼ�����ڲ�����Ϊ115200
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_Normal);//CAN��ʼ������ģʽ��CAN_Mode_LoopBack��,����ͨģʽ��CAN_Mode_Normal��������500Kbps   
	Motor_Init();//���ߵ��������ʼ��
 	PWMInit(500-1,84-1);	//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.  
  SetMotorPWM(0,0);//�ٶȳ�ʼ0
	KEY_Init();					//������ʼ��
	TIM4_Cap_Init(0XFFFF,84-1); //��1Mhz��Ƶ�ʼ��� �����ʱ��65536us,ң���������������2160us
  TIM3_Cap_Init(0x63,84-1); //��1Mhz��Ƶ�ʼƣ����ʱ��100us
	Driver_Init();			//���������������ʼ��
	TIM8_OPM_RCR_Init(999,168-1); //TIM8Ϊ168MHz������1MHz����Ƶ��  ������+�ظ�����ģʽ  

	//��ѭ��
   while(1) 
	{
 		//delay_ms(10);
		



		//ң�س���
		if(Controler_ENABLE)RemoteControler();
		//printf("motor 1: hz \r\n");
		//�ٶȷ�������
		//delay = SpeedFeedback(delay);

		
		//ת��΢��	
		keyval=KEY_Scan(0);
		if(keyval==KEY3_PRES)
		{
					//printf("key4 pressed\r\n");
			for(int i=0;i<8;i++)
			{
				CanTxBuf[i]=i;
			}
			CanT_Flag=CAN1_Send_Msg(CanTxBuf,8);
			if(!CanT_Flag)printf("Transction Success\r\n" );
			else printf("Transction Failed\r\n");
			//Locate_Abs(0,7000);//����KEY3�������
		}else if(keyval==KEY1_PRES)
		{
			Locate_Rle(1000,7500,CW);//����KEY1����7000Hz��Ƶ�� ˳ʱ�뷢1000����, 
		}else if(keyval==KEY2_PRES)
		{
			Locate_Rle(1000,7500,CCW);//����KEY2����7000Hz��Ƶ�� ��ʱ�뷢1000����
		}			

	

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
	}	
}

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
