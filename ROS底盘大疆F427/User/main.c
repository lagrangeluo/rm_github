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

#define MAX_MOTOR_SPEED   15336				//电机最大转速，宏定义方便修改   范围0 - 10000   15336
#define MAX_BASE_LINEAR_SPEED    217.817f    //底盘最大平移速度，单位cm/s
#define MAX_BASE_ROTATIONAL_SPEED    7.260570f    //底盘最大旋转速度，单位rad/s


extern char receiveBuffer[MAX_LENGTH];
extern char json_Buffer[MAX_LENGTH];
int main()
{
	All_Init();												//机器人所有配置初始化
	max_base_linear_speed = MAX_BASE_LINEAR_SPEED;							//底盘中心最大线速度
	max_base_rotational_speed = MAX_BASE_ROTATIONAL_SPEED;			//地盘中心最大角速度
	max_motor_speed = MAX_MOTOR_SPEED;		//电机最大转速，通过上面宏定义更改

		
	//	set_moto_angle(4000,0,0,0);		//调试   设定指定角度
	
//	set_moto_speed(0,0,0,0);			//调试    设定指定速度
	
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
extern uint8_t flag_command_recieved4;
extern uint8_t flag_command_recieved5;




//定时器3中断服务函数			1ms
void TIM3_IRQHandler(void)
{
	static int time_count=1;
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) 	//溢出中断
	{
		time_count++;
		
		/*****   遥控器控制    ******/
		//Remote_Control();				//遥控器控制代码
		
		/****  ROS上位机控制  *****/
		if(flag_command_recieved == 1)	//每一毫秒检查一次是否收到控制指令
		{
			//如果自动控制才可以给命令的目标速度赋值
			if(1)//(Control_Mode & auto_control) == auto_control
			resolve_json_chassis_command();
		
			flag_command_recieved = 0;	//命令接收标志位清零
		}
   
	 if(flag_command_recieved1 == 1)
		{
		if(1)//(Control_Mode & auto_control) == auto_control
				 
			flag_command_recieved1 = 0;	//命令接收标志位清零
		}
	 
	 	if(flag_command_recieved2 == 1)
		{
		if(1)//(Control_Mode & auto_control) == auto_control
			resolve_json_mode_command();

	
			flag_command_recieved2 = 0;	//命令接收标志位清零
		}
		if(flag_command_recieved3 == 1)
		{
		if(1)//(Control_Mode & auto_control) == auto_control
			resolve_json_gimbal_command();
		  caclulate_pwm_pulse();		
			
			flag_command_recieved3 = 0;	//命令接收标志位清零
		}
			if(flag_command_recieved4 == 1)	//每一毫秒检查一次是否收到控制指令
		{
			//如果自动控制才可以给命令的目标速度赋值
			if(1)//(Control_Mode & auto_control) == auto_control
		 resolve_json_fric_command();
		
			flag_command_recieved4 = 0;	//命令接收标志位清零
		}
		/*	if(flag_command_recieved5 == 1)	//每一毫秒检查一次是否收到控制指令
		{
			//如果自动控制才可以给命令的目标速度赋值
			if(1)//(Control_Mode & auto_control) == auto_control
			resolve_json_chassis_command();
		
			flag_command_recieved5 = 0;	//命令接收标志位清零
		}*/
		/****  机器人运动控制  *****/
		if(time_count%7 ==0)		//7ms
			Robo_Move();
		
		/*****    按键扫描   ******/
		if(time_count%31 == 0)		//31ms  消抖
			Debug_Key();
		
		/*****    工作状态指示灯   ******/
		if(time_count%500 == 0)			//500ms
		{
			LED1=!LED1;							//单片机正常工作状态指示灯
			time_count = 0;
		}
		
		if(time_count%4 == 0)		//4ms  测速
			Get_Base_Velocities();		//计算底盘中心实际速度
		
		
		if(time_count%20 == 0)		//20ms，50Hz 
		
{   

	//send_chassis_info_by_json();
	//send_gimbal_info_by_json();
		  send_cgf_info_by_json();
  
			                      } 		    //发送摩擦轮信息给上位机*****************************新增
		                              
//			send_chassis_info_by_hex();		//发送实时速度十六进制形式给
		
		if(time_count>=1000)			//清除计数标志    1s
			time_count=1;
		
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}

