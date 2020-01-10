#ifndef _REMOTE_CODE_H_
#define _REMOTE_CODE_H_
#include "sys.h"

//Control_Mode��1λ   FSΪ0   DJiΪ1
#define FS_Remote_Control			0x0
#define DJi_Remote_Control		0x1

//Control_Mode��2λ  autoΪ1   remoteΪ0
#define auto_control					0x2
#define remote_control        0x1
#define ROS_control						0x4

//Control_Mode��1λң����ѡ�񣬵ڶ�λģʽѡ��
#define control_mode     (auto_control | DJi_Remote_Control)			//�Զ�+��ң����

extern u8 Control_Mode;
extern float pwm_pulse1;
extern float pwm_pulse2;
/*
��˹ң��������Χ				     				 ��ң����������Χ
CH1��1197-2200-3198									 ch0:364-1024-1684
CH2��1197-2200-3198									 ch1:364-1024-1684
CH3��1197-2200-3198									 ch2:364-1024-1684
CH4��1197-2200-3198                  ch3:364-1024-1684
CH5��1197-2200-3198                  s1: 1-3-2
CH6��1197-2200-3198                  s2: 1-3-2
CH7��1197-2200-3198
CH8��1197-2200-3198

��˹CH5-CH8
������    1197-3198
SWA|SWD:  1197-3198
SWB|SWC:  1198-2198-3198
��ť��    1198-2198-3198

*/
#if (control_mode == (auto_control | FS_Remote_Control))					//��˹ң����

	#define x_CH_width            Pulse_Width.CH1         //x����ͨ������   ��ҡ������
	#define y_CH_width            Pulse_Width.CH2         //y����ͨ������   ��ҡ������
	#define r_CH_width            Pulse_Width.CH4         //r����ͨ������   ��ҡ������
	#define stop_CH_width         Pulse_Width.CH5					//��ͣ����				 SWB��������
	#define remote_CH_width       Pulse_Width.CH5					//ң��ģʽ����    SWB�����м�
	       	
	/*���¶����е���ֵ��ͨ��watch�й۲��ͨ��ֵ����*/
	
  //�������ʼֵ
	#define x_initial_value       2200              
	#define y_initial_value       2200
	#define r_initial_value       2200
	
	//�����������Сֵ
	#define x_max_value           3198             
	#define x_min_value           1197
	#define y_max_value           3198
	#define y_min_value           1197
	#define r_max_value           3198
	#define r_min_value           1197
	//��ֵ������
	#define stop_max_value				4000		
	#define stop_min_value				3000
	#define remote_max_value      4000
	#define remote_min_value      2000		//remote��ΧӦ����stop
	
#elif (control_mode == (auto_control | DJi_Remote_Control))				//��ң����

  #define x_CH_width            rc.ch0         //x����ͨ������   ��ҡ������
	#define y_CH_width            rc.ch1         //y����ͨ������   ��ҡ������
	#define r_CH_width            rc.ch2         //r����ͨ������   ��ҡ������
	#define i_CH_width            rc.ch3         //��̨����ͨ������ ��ҡ������
	#define stop_CH_width         rc.s1					 //��ͣ����				s1��������
	#define remote_CH_width       rc.s1  				 //ң��ģʽ����    s1�����м�
	#define trigger_CH_width      rc.s2          //�����ֿ���
	
	/*���¶����е���ֵ��ͨ��watch�й۲��ͨ��ֵ����*/
	
  //�������ʼֵ
	#define x_initial_value       1024            
	#define y_initial_value       1024
	#define r_initial_value       1024
	#define i_initial_value       1024
	//�����������Сֵ
	#define x_max_value           1684             
	#define x_min_value           364
	#define y_max_value           1684
	#define y_min_value           364
	#define r_max_value           1684
	#define r_min_value           364
	#define i_max_value           1684
	#define i_min_value           364
	//��ֵ������
	#define stop_max_value				2.5
	#define stop_min_value				1.5
	#define remote_max_value      4
	#define remote_min_value      1.5			//remote��ΧӦ����stop

#endif

void Remote_Control(void);

#endif
