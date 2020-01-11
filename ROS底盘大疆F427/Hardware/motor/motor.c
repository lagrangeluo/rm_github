#include "motor.h"
#include "can.h"
#include "delay.h"
#include "speed_pid.h"
#include "angle_pid.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx.h"

MOTOR_t motor1,motor2,motor3,motor4,motor5,motor6,motor7,motor8;
LOOPBACK loopback;

int max_motor_speed=0;		//���������ٶ�


void record_motor_callback(MOTOR_t *motor, uint16_t angle, int16_t speed, int16_t current)
{
	motor->last_angle = motor->actual_angle;
	motor->actual_angle = angle;
	motor->actual_speed = speed;
	motor->actual_current = current;
	//motor1.temp = temp;
	if(motor->start_angle_flag==0)
	{
		motor->start_angle = angle;
		motor->start_angle_flag++;	//ֻ������ʱ��¼һ�γ�ʼ�Ƕ�
	}
	
	if(motor->actual_angle - motor->last_angle > 4096)
		motor->round_cnt --;
	else if (motor->actual_angle - motor->last_angle < -4096)
		motor->round_cnt ++;
	motor->total_angle = motor->round_cnt * 8192 + motor->actual_angle;// - motor->start_angle;
}


// ����: motorr_init()
// ����: ���������ʼ��
// ��������
// �������
void motor_init()
{
	//1�ŵ��.
	motor1.start_angle = 0;
	motor1.actual_angle = 0;
	motor1.actual_speed = 0;
	motor1.start_angle_flag = 0;
	motor1.actual_current = 0;
	motor1.target_current = 0;
	//motor1.temp = 0;

	//2�ŵ��
	motor2.start_angle = 0;
	motor2.actual_angle = 0;
	motor2.start_angle_flag = 0;
	motor2.actual_speed = 0;
	motor2.actual_current = 0;
	motor2.target_current = 0;
	//motor2.temp = 0;

	//3�ŵ��
	motor3.start_angle = 0;
	motor3.actual_angle = 0;
	motor3.start_angle_flag = 0;
	motor3.actual_speed = 0;
	motor3.actual_current = 0;
	motor3.target_current = 0;
	//motor3.temp = 0;

	//4�ŵ��
  motor4.start_angle = 0;
	motor4.actual_angle = 0;
	motor4.start_angle_flag = 0;
	motor4.actual_speed = 0;
	motor4.actual_current = 0;
	motor4.target_current = 0;
	//motor4.temp = 0;
}


// ����: set_current()
// ����: �����������ֵ
// ��������
// �������
void set_chassis_current()
{
	u8 current_msg[8];
	
	//���Ŀ�����Ϊ�ٶ�pid���
	motor1.target_current = motor1.vpid.PID_OUT;
	motor2.target_current = motor2.vpid.PID_OUT;
	motor3.target_current = motor3.vpid.PID_OUT;
	motor4.target_current = motor4.vpid.PID_OUT;

	
	//can����ͨ��Э�飬���յ��˵����
	current_msg[0] = motor1.target_current >> 8;			//1�ŵ��������8λ
	current_msg[1] = motor1.target_current & 0xff;		//1�ŵ��������8λ
	current_msg[2] = motor2.target_current >> 8;			//2�ŵ��������8λ
	current_msg[3] = motor2.target_current & 0xff;		//2�ŵ��������8λ
	current_msg[4] = motor3.target_current >> 8;			//3�ŵ��������8λ
	current_msg[5] = motor3.target_current & 0xff;		//3�ŵ��������8λ
	current_msg[6] = motor4.target_current >> 8;			//4�ŵ��������8λ
	current_msg[7] = motor4.target_current & 0xff;		//4�ŵ��������8λ
	
	//can��������֡
	CAN1_Send_CHASSIS_Msg(current_msg);
}

void set_trigger_current()
{
	u8 current_msg[8];
	
	//���Ŀ�����Ϊ�ٶ�pid���
	motor5.target_current = motor5.target_speed;//����
	motor6.target_current = motor6.vpid.PID_OUT;
	motor7.target_current = motor7.vpid.PID_OUT;
	motor8.target_current = motor8.vpid.PID_OUT;

	
	//can����ͨ��Э�飬���յ��˵����
	current_msg[0] =motor5.target_current >> 8;			//5�ŵ��������8λ
	current_msg[1] = motor5.target_current & 0xff;		//5�ŵ��������8λ
	current_msg[2] = motor6.target_current >> 8;			//6�ŵ��������8λ
	current_msg[3] = motor6.target_current & 0xff;		//6�ŵ��������8λ
	current_msg[4] = motor7.target_current >> 8;			//7�ŵ��������8λ
	current_msg[5] = motor7.target_current & 0xff;		//7�ŵ��������8λ*/
	current_msg[6] = motor8.target_current >> 8;			//8�ŵ��������8λ
	current_msg[7] = motor8.target_current & 0xff;		//8�ŵ��������8λ
	
	//can��������֡
	CAN1_Send_Trigger_Msg(current_msg);
}

// ����: stop_allmotor()
// ����: �����е��ͣ�ڵ�ǰ�Ƕ�
// ��������
// �������
void stop_chassis_motor()
{
	//��ȡ��ǰ�Ƕ�ֵ
	motor1.stop_angle = motor1.total_angle;
	motor2.stop_angle = motor2.total_angle;
	motor3.stop_angle = motor3.total_angle;
	motor4.stop_angle = motor4.total_angle;
	
	
	
	//�ı�Ƕ�pidĿ��Ƕ�ֵ
	set_chassis_motor_angle(motor1.stop_angle,motor2.stop_angle,motor3.stop_angle,motor4.stop_angle);
	
	
}
void stop_trigger_motor()
{
motor5.stop_angle = motor5.total_angle;
	
set_trigger_motor_angle(motor5.stop_angle);
trigger_to_motor(0);
set_trigger_motor_speed(motor5.target_speed);
	

}

