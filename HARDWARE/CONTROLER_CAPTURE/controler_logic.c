#include "controler_logic.h"
#include "controler_capture.h"
#include "motor_driver.h"
#include "steering_driver.h"


u32 count1=0;//����ֵ
u32 count2=0;//����ֵ
u32 count3=0;//����ֵ
u32 count4=0;//����ֵ
u32 pst=0,pst_old=0;
u8 dir=0;
float	k1,k_right,k_left;//����ת��ϵ��
u32 delay=0;//���ڴ�����ʱ���ͣ�
u32 dir_delay;//����ת����ʱ

void RemoteControler() //ң�س���
{
		
		k1=(float)(500-0)/(float)(216-87);//����Ĳ���ֵ��87~215���궨��0~500
		k_right=(float)35000/(float)(21-15);//��תϵ��
		k_left=(float)35000/(float)(15-8);//��תϵ��
		count1=tim4up1/10;		   //ȥ�����һλ���ȶ�����������֮��Χ�����87~215��
		count2=tim4up2/10;
		count3=tim4up3/10;		   //ȥ�����һλ���ȶ�����������֮��Χ�����87~215��
		count4=tim4up4/10;
		
		
		//printf("HIGH:%lld us\r\n",count1); 
		//printf("DIR: %d \r\n",count2);
		//printf("DIR: %lld \n",count3);
	
	
		//���������ת����PWM�������
		//count1=count1/10;//
		if(count1<300)
		{
			//printf("HIGH:%d us\r\n",tim4up1); 
			count1=500-(int)(k1*(count1-87));//�궨����500��0������
			//printf("duty:%d \r\n",count1); 
			SetMotorPWM(dir,count1);
		}
		if(count2<109)
		{
			GPIO_SetBits(GPIOH,GPIO_Pin_9);//��ת
			GPIO_ResetBits(GPIOH,GPIO_Pin_14);
			GPIO_SetBits(GPIOH,GPIO_Pin_13);//��ɲ��
			GPIO_SetBits(GPIOH,GPIO_Pin_15);//���ʹ
	
		}
		else if(count2>190&&count2<300)
		{
			GPIO_ResetBits(GPIOH,GPIO_Pin_9);//��ת
			GPIO_SetBits(GPIOH,GPIO_Pin_14);
			GPIO_SetBits(GPIOH,GPIO_Pin_13);//��ɲ��
			GPIO_SetBits(GPIOH,GPIO_Pin_15);//���ʹ
			
		}
		else if(count2>140&&count2<160) 
		{
			 GPIO_ResetBits(GPIOH,GPIO_Pin_15);//�ر�ʹ��
			
		}
		
		pst=count3/10;  //��ң��������궨��(8,21)
		if(pst<300&&pst!=pst_old)//ң�ط����б仯
		{
			dir_delay ++;
			if(dir_delay==5000)//��ʱ����ֹƵ��ת��
			{
				if(pst!=pst_old)
				{
					dir_delay=0;
					if(pst>15&&pst<30&&pst!=pst_old)//��ת
					{
						pst_old=pst;
						dir=Right;
						Locate_Abs(k_right*(pst-15),7000);
						
						
					}
					else if(pst>0&&pst<15&&pst!=pst_old)//��ת
					{
						pst_old=pst;
						dir=Left;
						
						Locate_Abs((-35000+k_left*(pst-8)),7000);
						
						
					}
					else if(pst==15&&pst!=pst_old)
					{	
						if(current_pos!=0)
						{
							dir=Advance;
							Locate_Abs(0,7000);
							
							
						}
						pst_old =15;
					}
				}
			}
		}
	

}	
