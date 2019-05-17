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


//u8 RxProcess(void);//串口数据处理
//void TxProcess(u8 sendData);//串口数据发送


int main(void)
{  
	u8 keyval;//按键检测
	//u8 CanRxBuf[8];//can总线接受数据
	u8 CanTxBuf[7];//can总线发送数据
	u8 CanT_Flag;
	u8 CanR_Flag;
	
	//各种初始化程序
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);//初始化串口波特率为115200
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_Normal);//CAN初始化环回模式（CAN_Mode_LoopBack）,（普通模式：CAN_Mode_Normal）波特率500Kbps   
	Motor_Init();//行走电机驱动初始化
 	PWMInit(500-1,84-1);	//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.  
  SetMotorPWM(0,0);//速度初始0
	KEY_Init();					//按键初始化
	TIM4_Cap_Init(0XFFFF,84-1); //以1Mhz的频率计数 ，溢出时间65536us,遥控器接收器最长脉宽2160us
  TIM3_Cap_Init(0x63,84-1); //以1Mhz的频率计，溢出时间100us
	Driver_Init();			//步进电机驱动器初始化
	TIM8_OPM_RCR_Init(999,168-1); //TIM8为168MHz所以是1MHz计数频率  单脉冲+重复计数模式  

	//主循环
   while(1) 
	{
 		//delay_ms(10);
		



		//遥控程序
		if(Controler_ENABLE)RemoteControler();
		//printf("motor 1: hz \r\n");
		//速度反馈程序
		//delay = SpeedFeedback(delay);

		
		//转向微调	
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
			//Locate_Abs(0,7000);//按下KEY3，回零点
		}else if(keyval==KEY1_PRES)
		{
			Locate_Rle(1000,7500,CW);//按下KEY1，以7000Hz的频率 顺时针发1000脉冲, 
		}else if(keyval==KEY2_PRES)
		{
			Locate_Rle(1000,7500,CCW);//按下KEY2，以7000Hz的频率 逆时针发1000脉冲
		}			

	

	#if 0    //PC端串口调试程序
		temp= RxProcess();
		if(USART_RX_BUF[0]!=0)printf("电机速率:%d\r\n",100-temp);
		//清除串口数据,准备下一次接收
		for(i=0;i<USART_RX_STA;i++)
		{
			USART_RX_BUF[i]=0;
		}
		USART_RX_STA=0;
#endif
	}	
}

//u8 RxProcess(void)//串口数据处理
//{
//	u8 temp = 0;
//	float k;

//	switch(USART_RX_BUF[0])//选择模式
//	{
//		case 0x10://10暂定电机调速模式
//		{
//			//GPIO_ResetBits(GPIOH,GPIO_Pin_9);
//			GPIO_SetBits(GPIOH,GPIO_Pin_9);//正转
//			GPIO_ResetBits(GPIOH,GPIO_Pin_14);
//			GPIO_SetBits(GPIOH,GPIO_Pin_13);//不刹车
//			temp = USART_RX_BUF[1];
//			k=(float)(500-0)/(float)(100-0);
//			if(temp<=100)
//			{
//				SetMotorPWM(0,(u32)k*temp);
//				GPIO_SetBits(GPIOH,GPIO_Pin_15);//电机使�
//			}
//			else	printf("数据错误\r\n");
//		}break;
//		case 0x11:
//		{
//			GPIO_ResetBits(GPIOH,GPIO_Pin_9);//反转
//			GPIO_SetBits(GPIOH,GPIO_Pin_14);
//			GPIO_SetBits(GPIOH,GPIO_Pin_13);//不刹车
//			//GPIO_SetBits(GPIOH,GPIO_Pin_9);
//			temp = USART_RX_BUF[1];
//			k=(float)(500-0)/(float)(100-0);
//			if(temp<=100)
//			{
//					SetMotorPWM(0,(u32)k*temp);	
//					GPIO_SetBits(GPIOH,GPIO_Pin_15);//电机使�
//			}
//		}break;
//		case 0X12:
//			GPIO_ResetBits(GPIOH,GPIO_Pin_15);break;//关闭使能
//			
//		case 0X13:
//			GPIO_ResetBits(GPIOH,GPIO_Pin_13);break;//刹车
//	}
//	return temp;//返回占空比
//}
//void TxProcess(u8 sendData)//串口数据发送
//{
//	switch(USART_RX_BUF[0])
//	{	
//		case 0x10:
//			USART_SendData(USART1,sendData);
//	}
//}
