#include "can.h"
#include "motor_driver.h"
#include  "controler_logic.h"
#include "delay.h"
#include "usart.h"
#include "steering_driver.h"
int i=0;
 FunctionalState Controler_ENABLE =DISABLE;//Ĭ��ң����������
 FunctionalState CAN_COMMAND = ENABLE;//Ĭ��CAN����
u8 Can_Motor_Buf[8];
u8 Can_AGV_Buf[8];
u8 Can_Steer_Buf[8];
Motor_StatusDef CAN_Motor_Setup;

//CAN���߳�ʼ��
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

  	GPIO_InitTypeDef GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN1_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif
    //ʹ�����ʱ��
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��PORTBʱ��	                   											 
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);//ʹ��PORTIʱ��	                   											 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	
	
    //��ʼ��GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��PB9
		GPIO_Init(GPIOI, &GPIO_InitStructure);//��ʼ��PI9
	
	  //���Ÿ���ӳ������
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_CAN1); //GPIOB9����ΪCAN1
	  GPIO_PinAFConfig(GPIOI,GPIO_PinSource9,GPIO_AF_CAN1); //GPIOI9����ΪCAN1
	  
  	//CAN��Ԫ����
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//��ʱ�䴥��ͨ��ģʽ   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//����Զ����߹���	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
  	CAN_InitStructure.CAN_NART=ENABLE;	//��ֹ�����Զ����� 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//���Ĳ�����,�µĸ��Ǿɵ�  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//���ȼ��ɱ��ı�ʶ������ 
  	CAN_InitStructure.CAN_Mode= mode;	 //ģʽ���� 
  	CAN_InitStructure.CAN_SJW=tsjw;	//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;  //��Ƶϵ��(Fdiv)Ϊbrp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);   // ��ʼ��CAN1 
    
		//���ù�����
 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  //������0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //����ģʽ
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000; 
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
		
#if CAN1_RX0_INT_ENABLE
	
	  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}   
 
#if CAN1_RX0_INT_ENABLE	//ʹ��RX0�ж�
//CAN�����жϷ�����			    
void CAN1_RX0_IRQHandler(void)
{
  	CanRxMsg RxMessage;
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0) != RESET)
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
		CAN_Receive(CAN1, 0, &RxMessage);

		if(RxMessage.IDE == CAN_ID_STD && RxMessage.RTR==CAN_RTR_DATA && RxMessage.DLC==8)
		{
			switch(RxMessage.StdId)
			{
				case 0x100:
					Can_AGV_Control(RxMessage.Data)	;break;//С������						
				case 0x102:
					if(CAN_COMMAND)Can_Motor_Control(RxMessage.Data);break;//�������
				case 0x104:
					if(CAN_COMMAND)Can_Steering_Control(RxMessage.Data);break;	//ת�����
			}
		}	
	}
}
#endif

//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 CAN1_Send_Msg(u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=0x12;	 // ��׼��ʶ��Ϊ0
  //TxMessage.ExtId=0x12;	 // ������չ��ʾ����29λ��
  TxMessage.IDE= CAN_ID_STD;		  // ʹ�ñ�׼֡
  TxMessage.RTR=CAN_RTR_Data;		  // ��Ϣ����Ϊ����֡��һ֡8λ
  TxMessage.DLC=len;							 // ����len֡�����8֡
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];				 // ��i֡��Ϣ          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
  if(i>=0XFFF)return 1;
  return 0;		

}



//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 CAN1_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
    for(i=0;i<RxMessage.DLC;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}


//С����������
void Can_AGV_Control(u8 *CanReceive)
{
	for(i=0;i<7;i++)Can_AGV_Buf[i]=CanReceive[7-i];//��ȡ����
	
	if(Can_AGV_Buf[0]==0x01){  //ID0x100�ĵ�0λ����С���ٿط�ʽ
		Controler_ENABLE=ENABLE;//ң��������
		CAN_COMMAND = DISABLE;
	}
	else{
		 CAN_COMMAND = ENABLE;//CAN�������
		Controler_ENABLE=DISABLE;
	}
	
}


//С�����CAN�������
void Can_Motor_Control(u8 *CanReceive)
{
	for(i=0;i<7;i++)Can_Motor_Buf[i]=CanReceive[7-i];//��ȡ����
	
	if(Can_Motor_Buf[0]== ENABLE)CAN_Motor_Setup.enable=MotorEnable;
	else CAN_Motor_Setup.enable=MotorDisable;
	if(Can_Motor_Buf[1]== ENABLE)CAN_Motor_Setup.dir=FORWARD;
	else CAN_Motor_Setup.dir = BACKWARD;
	if(Can_Motor_Buf[2]== ENABLE)CAN_Motor_Setup.Break=BREAK;
	else CAN_Motor_Setup.Break = NoBREAK;
	Motor_Status_Setup(&CAN_Motor_Setup);//���״̬����
	if(Can_Motor_Buf[3]<=100)
	{
		SetMotorPWM(dir_flag,Can_Motor_Buf[3]);
	}
	else printf("����ʹ�� \r\n");
	
}

//CANת�����  //ÿת1���Լ1600�����壬ת45�����pluse_num=1600*45����������
void Can_Steering_Control(u8 *CanReceive)
{
	for(i=0;i<7;i++)Can_Steer_Buf[i]=CanReceive[7-i];//��ȡ����
	long pulse_num=0;//��������
	dir_flag=Can_Steer_Buf[3];
	if(Can_Steer_Buf[4]<20)pulse_num=1600*Can_Steer_Buf[4];
	if(dir_flag==1)Locate_Abs(pulse_num,7000);
	else if(dir_flag==2)Locate_Abs(-pulse_num,7000);
	else if(dir_flag==0)Locate_Abs(0,7000);
	
}












