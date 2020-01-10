#ifndef _ANGLE_PID_H
#define _ANGLE_PID_H

#define pos_pid     0x0001
#define ang_pid     0x0000
extern int ap_pid_flag;

void APID_Init_All(void);			//�����е�Ƕ�PID������ʼ��
void apid_PID_realize(float kp,float ki,float kd);			//�����е�Ƕ�pidʵ��
void set_chassis_motor_angle(int moto1_angle,int moto2_angle,int moto3_angle,int moto4_angle);			//���õ��Ŀ���е�Ƕ�
void set_trigger_motor_angle(int moto8_angle);
#endif
