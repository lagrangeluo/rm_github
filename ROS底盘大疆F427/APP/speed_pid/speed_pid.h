#ifndef _SPEED_PID_H
#define _SPEED_PID_H




void VPID_Init_All(void);			//���ת��PID������ʼ��
void vpid_PI_realize(float kp,float ki);				//���ת��PIDʵ��
void set_chassis_motor_speed(int moto1_speed,int moto2_speed,int moto3_speed,int moto4_speed);			//����Ŀ���ٶ�
void set_trigger_motor_speed(int moto5_speed);	
int abs(int input);				//�����ֵ����
#endif
