#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "GPIO.h"
#include "pwm.h"
#include "capture.h"
#include	"driver.h"
#include	"SpdCap.h"
#include "key.h"
//#include "usmart.h"

u8 RxProcess(void);//�������ݴ���
void TxProcess(u8 sendData);//�������ݷ���
int main(void)
{ 
	//u16 led0pwmval=0;    
	u8 temp=0;int i=0;
	u32 delay=0;//���ڴ�����ʱ���ͣ�
	u8 keyval;//�������
	long long count1=0;//����ֵ
	long long count2=0;//����ֵ
	long long count3=0;//����ֵ
	long long count4=0;//����ֵ
	int pst=0;
	float	k1,k2,k3;//����ת��ϵ��
	k1=(float)(500-0)/(float)(215-87);//����Ĳ���ֵ��87~215���궨��0~500
	k2=(float)100000/(float)(21-15);
	k3=(float)100000/(float)(15-8);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);//��ʼ�����ڲ�����Ϊ115200
	IO_Init();
 	PWMInit(500-1,84-1);	//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.  
  TIM_SetCompare1(TIM5,500);	//�޸ıȽ�ֵ���޸�ռ�ձ�, 500��Ӧռ�ձ�0
	TIM_SetCompare2(TIM5,500);
	TIM_SetCompare3(TIM5,500);
	TIM_SetCompare4(TIM5,500);
	
	KEY_Init();					//������ʼ��
	//usmart_dev.init(84); 	//��ʼ��USMART			
	TIM4_Cap_Init(0XFFFFF,84-1); //��1Mhz��Ƶ�ʼ��� �����ʱ��65535us,ң���������������2160us
  TIM3_Cap_Init(0xffff,84-1); //��1Mhz��Ƶ�ʼ�
	
	Driver_Init();			//���������������ʼ��
	TIM8_OPM_RCR_Init(999,168-1); //TIM8Ϊ168MHz������1MHz����Ƶ��  ������+�ظ�����ģʽ  
#if 1	
   while(1) 
	{
 		delay_ms(10);
		
#if 1    //PC�˴��ڵ��Գ���
		temp= RxProcess();
		if(USART_RX_BUF[0]!=0)printf("�������:%d\r\n",100-temp);
		//�����������,׼����һ�ν���
		for(i=0;i<USART_RX_STA;i++)
		{
			USART_RX_BUF[i]=0;
		}
		USART_RX_STA=0;
#endif
		
#if 1 //ң�س���
		delay_ms(10);		 

		count1=tim4up1/10;		   //ȥ�����һλ���ȶ�����������֮��Χ�����87~215��
		count2=tim4up2/10;
		count3=tim4up3/10;		   //ȥ�����һλ���ȶ�����������֮��Χ�����87~215��
		count4=tim4up4/10;
		
		
	//printf("HIGH:%lld us\r\n",count1); 
		//printf("DIR: %lld \n",count2);
		//printf("DIR: %lld \n",count3);
	
	
		//���������ת����PWM�������
		//count1=count1/10;//
		if(count1<300)
		{
			count1=500-(int)(k1*(count1-87));//�궨����500��0������
			TIM_SetCompare1(TIM5,count1);
			TIM_SetCompare2(TIM5,count1);
			TIM_SetCompare3(TIM5,count1);
			TIM_SetCompare4(TIM5,count1);
		}
		if(count2<109)
		{
			GPIO_SetBits(GPIOH,GPIO_Pin_9);//��ת
			GPIO_ResetBits(GPIOH,GPIO_Pin_14);
			GPIO_SetBits(GPIOH,GPIO_Pin_13);//��ɲ��
			GPIO_SetBits(GPIOH,GPIO_Pin_15);//���ʹ�
		}
		else if(count2>194&&count2<300)
		{
			GPIO_ResetBits(GPIOH,GPIO_Pin_9);//��ת
			GPIO_SetBits(GPIOH,GPIO_Pin_14);
			GPIO_SetBits(GPIOH,GPIO_Pin_13);//��ɲ��
			GPIO_SetBits(GPIOH,GPIO_Pin_15);//���ʹ�
		}
		else if(count2>145&&count2<160) 
		{
			GPIO_ResetBits(GPIOH,GPIO_Pin_15);//�ر�ʹ��
			
		}
		pst=count3/10;
		//printf("PST:%lld us\r\n",(long long)pst);
		if(count3>160&&count3<300)
		{
			
			Locate_Abs((int)k2*(pst-15),58000);
			printf("PST:%lld us\r\n",(long long)(int)k3*(pst-15));
		}
		else if(count3<145)
		{
			Locate_Abs(-(int)k3*(pst-8),58000);
			printf("PST:%lld us\r\n",(long long)-(int)k3*(pst-8));
		}
		else if(count3>145&&count3<160)Locate_Abs(0,58000);
#endif
		
#if 0 //�ٶȷ�������
		delay ++;
		if(delay == 100)
		{
			delay=0;
			printf("[ motor 1: %d rpm ]  ",f1);
			printf("[ motor 2: %d rpm ] \n",f2);
			printf("[ motor 3: %d hz ]  ",f3);
			printf("[ motor 4: %d hz ]\r\n",f4);
		}
#endif
		keyval=KEY_Scan(0);
		if(keyval==KEY3_PRES)
		{
			Locate_Abs(0,500);//����WKUP�������
		}else if(keyval==KEY0_PRES)
		{
			Locate_Rle(200,500,CW);//����KEY0����500Hz��Ƶ�� ˳ʱ�뷢200����
		}else if(keyval==KEY1_PRES)
		{
			Locate_Rle(400,500,CCW);//����KEY1����500Hz��Ƶ�� ��ʱ�뷢400����
		}			
		delay_ms(10);
	}
#endif
}


u8 RxProcess(void)//�������ݴ���
{
	u8 temp = 0;
	float k;

	switch(USART_RX_BUF[0])//ѡ��ģʽ
	{
		case 0x10://10�ݶ��������ģʽ
		{
			//GPIO_ResetBits(GPIOH,GPIO_Pin_9);
			GPIO_SetBits(GPIOH,GPIO_Pin_9);//��ת
			GPIO_ResetBits(GPIOH,GPIO_Pin_14);
			GPIO_SetBits(GPIOH,GPIO_Pin_13);//��ɲ��
			temp = USART_RX_BUF[1];
			k=(float)(500-0)/(float)(100-0);
			if(temp<=100)
			{
				TIM_SetCompare1(TIM5,(int)k*temp);
				TIM_SetCompare2(TIM5,(int)k*temp);
				TIM_SetCompare3(TIM5,(int)k*temp);
				TIM_SetCompare4(TIM5,(int)k*temp);
				GPIO_SetBits(GPIOH,GPIO_Pin_15);//���ʹ�
			}
			else	printf("���ݴ���\r\n");
		}break;
		case 0x11:
		{
			GPIO_ResetBits(GPIOH,GPIO_Pin_9);//��ת
			GPIO_SetBits(GPIOH,GPIO_Pin_14);
			GPIO_SetBits(GPIOH,GPIO_Pin_13);//��ɲ��
			//GPIO_SetBits(GPIOH,GPIO_Pin_9);
			temp = USART_RX_BUF[1];
			k=(float)(500-0)/(float)(100-0);
			if(temp<=100)
			{
					TIM_SetCompare1(TIM5,(int)k*temp);
					TIM_SetCompare2(TIM5,(int)k*temp);
					TIM_SetCompare3(TIM5,(int)k*temp);
					TIM_SetCompare4(TIM5,(int)k*temp);
					GPIO_SetBits(GPIOH,GPIO_Pin_15);//���ʹ�
			}
		}break;
		case 0X12:
			GPIO_ResetBits(GPIOH,GPIO_Pin_15);break;//�ر�ʹ��
			
		case 0X13:
			GPIO_ResetBits(GPIOH,GPIO_Pin_13);break;//ɲ��
	}
	return temp;//����ռ�ձ�
}
void TxProcess(u8 sendData)//�������ݷ���
{
	switch(USART_RX_BUF[0])
	{	
		case 0x10:
			USART_SendData(USART1,sendData);
	}
}
