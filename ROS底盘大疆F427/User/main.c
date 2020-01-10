#include "My_Init.h"
#include "delay.h"
#include "mode.h"

#include "Tim3_Events.h"
#include "led.h"
#include "stm32f4xx_tim.h"
#include "bsp_debug_usart.h"

#include "remote_code.h"
#include "motor.h"
#include "kinematic.h"
#include "fric.h"

#include "json.h"
#include <jansson.h>
#include <string.h>
#include "gimbal.h"

#define MAX_MOTOR_SPEED   15336				//������ת�٣��궨�巽���޸�   ��Χ0 - 10000   15336
#define MAX_BASE_LINEAR_SPEED    217.817f    //�������ƽ���ٶȣ���λcm/s
#define MAX_BASE_ROTATIONAL_SPEED    7.260570f    //���������ת�ٶȣ���λrad/s


extern char receiveBuffer[MAX_LENGTH];
extern char json_Buffer[MAX_LENGTH];
int main()
{
	All_Init();												//�������������ó�ʼ��
	max_base_linear_speed = MAX_BASE_LINEAR_SPEED;							//��������������ٶ�
	max_base_rotational_speed = MAX_BASE_ROTATIONAL_SPEED;			//�������������ٶ�
	max_motor_speed = MAX_MOTOR_SPEED;		//������ת�٣�ͨ������궨�����

		
	//	set_moto_angle(4000,0,0,0);		//����   �趨ָ���Ƕ�
	
//	set_moto_speed(0,0,0,0);			//����    �趨ָ���ٶ�
	
	while(1)
	{
		LED0=!LED0;
		delay_ms(500);
		//fric_off();
		
//		Usart_SendString( DEBUG_USART, json_Buffer);
//		Usart_SendString( DEBUG_USART, "\r\n");
		
		
	}
}

extern uint8_t flag_command_recieved;
extern uint8_t flag_command_recieved1;
extern uint8_t flag_command_recieved2;
extern uint8_t flag_command_recieved3;



//��ʱ��3�жϷ�����			1ms
void TIM3_IRQHandler(void)
{
	static int time_count=1;
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) 	//����ж�
	{
		time_count++;
		
		/*****   ң��������    ******/
		//Remote_Control();				//ң�������ƴ���
		
		/****  ROS��λ������  *****/
		if(flag_command_recieved == 1)	//ÿһ������һ���Ƿ��յ�����ָ��
		{
			//����Զ����Ʋſ��Ը������Ŀ���ٶȸ�ֵ
			if(1)//(Control_Mode & auto_control) == auto_control
			resolve_json_chassis_command();
			/*resolve_json_gimbal_command();
			resolve_json_fric_command();
			resolve_json_mode_command();*/
			
			
			//��������ָ��
			flag_command_recieved = 0;	//������ձ�־λ����
		}
   
	 if(flag_command_recieved1 == 1)
		{
		if(1)//(Control_Mode & auto_control) == auto_control
			//resolve_json_gimbal_command();
		  resolve_json_trigger_command();
			/*resolve_json_gimbal_command();
			resolve_json_fric_command();
			resolve_json_mode_command();*/
			//��������ָ��
			flag_command_recieved1 = 0;	//������ձ�־λ����
		}
	 
	 	if(flag_command_recieved2 == 1)
		{
		if(1)//(Control_Mode & auto_control) == auto_control
			resolve_json_mode_command();
			/*resolve_json_gimbal_command();
			resolve_json_fric_command();
			resolve_json_mode_command();*/
			
			
			//��������ָ��
			flag_command_recieved2 = 0;	//������ձ�־λ����
		}
		if(flag_command_recieved3 == 1)
		{
		if(1)//(Control_Mode & auto_control) == auto_control
			resolve_json_gimbal_command();
		  caclulate_pwm_pulse();
			
			
			//��������ָ��
			flag_command_recieved3 = 0;	//������ձ�־λ����
		}
		/****  �������˶�����  *****/
		if(time_count%7 ==0)		//7ms
			Robo_Move();
		
		/*****    ����ɨ��   ******/
		if(time_count%31 == 0)		//31ms  ����
			Debug_Key();
		
		/*****    ����״ָ̬ʾ��   ******/
		if(time_count%500 == 0)			//500ms
		{
			LED1=!LED1;							//��Ƭ����������״ָ̬ʾ��
			time_count = 0;
		}
		
		if(time_count%4 == 0)		//4ms  ����
			Get_Base_Velocities();		//�����������ʵ���ٶ�
		
		
		if(time_count%20 == 0)		//20ms��50Hz 
		
{   

	//send_chassis_info_by_json();
	//send_gimbal_info_by_json();
		  send_cgf_info_by_json();
  
			                      } 		    //����Ħ������Ϣ����λ��*****************************����
		                              
//			send_chassis_info_by_hex();		//����ʵʱ�ٶ�ʮ��������ʽ��
		
		if(time_count>=1000)			//���������־    1s
			time_count=1;
		
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}

