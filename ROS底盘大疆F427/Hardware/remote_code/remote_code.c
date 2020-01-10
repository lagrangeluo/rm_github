#include "remote_code.h"
#include "DJi_remote.h"
#include "FS_remote.h"
#include "speed_pid.h"
#include "motor.h"
#include "kinematic.h"
#include "fric.h"
#include "stm32f4xx_tim.h"
u8 Control_Mode = control_mode;

//�ڲ�ȫ�ֱ������������
float x_speed=0,y_speed=0,r_speed=0,trigger_speed=0;
float pwm_pulse1;
float pwm_pulse2=1500;
//�ڲ���������
float caculate_linear_speed(int width,int mid,int min,int max);
float caculate_rotational_speed(int width,int mid,int min,int max);
float caculate_gembal_pitch_angle(int width,int mid,int min,int max);
float caculate_gembal_yaw_angle(int width,int mid,int min,int max);
// ����: Remote_Control()
// ����: ң�ش��룬��ң���������Ӧ�������˾��嶯���ϣ����ڶ�ʱ���ﲻ�ϵ�ˢ
// ��������
// �������
void Remote_Control()    //���������Ͳ��ϵ��ж�ÿ��ͨ����ֵ�������������������Ӧ����
{	
	if(remote_CH_width>remote_min_value && remote_CH_width<remote_max_value)		//�������ң������
	{
		//��־λ��Ϊң��ģʽ
		Control_Mode &= remote_control;											//�޸�Control_Mode�ڶ�λΪ0
	}
	else
	{
		//��־λ��Ϊ�Զ�ģʽ
		Control_Mode |= auto_control;												//�޸�Control_Mode�ڶ�λΪ1
		
	}
	
	if((Control_Mode & auto_control) != auto_control)			//�������ģʽ�������Զ����ƣ���ң�ؿ���
	{
		if(stop_CH_width>stop_max_value || stop_CH_width<stop_min_value)		//�����ǰ����ֹͣ����
		{
			//��ң�������ӳ�䵽�����������ٶ���
				x_speed=caculate_linear_speed(x_CH_width,x_initial_value,x_min_value,x_max_value);
			  y_speed=caculate_linear_speed(y_CH_width,y_initial_value,y_min_value,y_max_value);
			  pwm_pulse1=caculate_gembal_pitch_angle(i_CH_width,i_initial_value,i_min_value,i_max_value);
			if((i_CH_width-1024)>60 || (i_CH_width-1024)<-60)   //�����ٿ���̨����
			  r_speed=0;
			else
			  r_speed=caculate_rotational_speed(r_CH_width,r_initial_value,r_min_value,r_max_value);
			if(trigger_CH_width == 1 )
			{
			  trigger_speed = 0;
				fric1_on(1500);
				fric2_on(1500);
				
			}
		 if(trigger_CH_width ==2)//1.5<trigger_CH_width & trigger_CH_width <2.5 )
			{
			  trigger_speed = 1000;
				fric1_on(1500);
				fric2_on(1500);
			}
			if(trigger_CH_width==3)//trigger_CH_width >1.5 &trigger_CH_width< 4)
			{
			trigger_speed = 0;
				fric1_on(1000);
				fric2_on(1000);
			}
			
		}
		else			//���ֹͣ����
		{
			x_speed = 0;
			y_speed = 0;
			r_speed = 0;
		}
		if((Control_Mode&DJi_Remote_Control) == DJi_Remote_Control)
		{
			y_speed = y_speed;
			r_speed = -r_speed; //ȡ����ʹ��ʱ����תΪ����
		}
		else if((Control_Mode&FS_Remote_Control) == FS_Remote_Control)		//��ΪFS_Remote_Control = 0������ж�ʱ�������else if��
		{
			y_speed = -y_speed;
		}
		speed_control(x_speed,y_speed,r_speed);
		trigger_control(trigger_speed);
		TIM_SetCompare1(TIM1,pwm_pulse1);
		TIM_SetCompare2(TIM1,pwm_pulse2);
	
	}
}

// ����: caculate_speed()
// ����: ��ң����ҡ�����ӳ�䵽�����������ٶ���
// ������width��ͨ��ֵ 
//			 mid��ͨ���м�ֵ 
//			 min��ͨ�������Сֵ
//       max��ͨ��������ֵ
// �������Ӧ���ٶ�ֵ
//�ڲ��������û��������
static float caculate_linear_speed(int width,int mid,int min,int max)
{
  float speed=0;
  if(width>=(mid+2))		//�м���������
    speed=(1.0*(width-(mid+2))/(max-(mid+2))*max_base_linear_speed);
  else if(width<=(mid-2))
    speed=(1.0*(width-(mid-2))/((mid-2)-min)*max_base_linear_speed);
  else
    speed=0;
  return speed;                
}

static float caculate_rotational_speed(int width,int mid,int min,int max)
{
  float speed=0;
  if(width>=(mid+2))		//�м���������
    speed=(1.0*(width-(mid+2))/(max-(mid+2))*max_base_rotational_speed);
  else if(width<=(mid-2))
    speed=(1.0*(width-(mid-2))/((mid-2)-min)*max_base_rotational_speed);
  else
    speed=0;
  return speed;                
}

static float caculate_gembal_pitch_angle(int width,int mid,int min,int max)
{
	float pwm_pulse=1500;
		if(width>=(mid+2))
		pwm_pulse=(1500 - 1.0*(width-(mid+2))/(max-(mid+2))*210);
	else if(width<=(mid-2))
	  pwm_pulse=(1500 + 1.0*((mid-2)-width)/((mid-2)-min)*105);
	else
		pwm_pulse=1500;
	return pwm_pulse;
	
}

/*static float caculate_gembal_yaw_angle(int width,int mid,int min,int max)
{
	float pwm_pulse=1500;
		if(width>=(mid+2))
		pwm_pulse=(1500 - 1.0*(width-(mid+2))/(max-(mid+2))*420);
	else if(width<=(mid-2))
	  pwm_pulse=(1500 + 1.0*((mid-2)-width)/((mid-2)-min)*420);
	else
		pwm_pulse=1500;
	return pwm_pulse;
	
}*/



