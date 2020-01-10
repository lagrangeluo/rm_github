#include "angle_pid.h"
#include "motor.h"
#include <math.h>
#include "speed_pid.h"

#define IntegralUpperLimit    2000		//��������
#define IntegralSeparation    200			//���ַ�����ֵ 
#define aPID_OUT_MAX          1000		//������ٶ�

int ap_pid_flag = ang_pid;	//�ǶȺ�λ��pid��־

// ����: APID_Init()
// ����: �����е�Ƕ�pid������ʼ��
// �����������е�Ƕ�pid�����ṹ��
// �������
// �ڲ��������û��������
void APID_Init(APID_t *apid)
{
	apid->actual_angle=0;
	apid->target_angle=0;
	apid->err=0;
	apid->last_err=0;
	apid->err_integration=0;
	apid->P_OUT=0;
	apid->I_OUT=0;
	apid->D_OUT=0;
	apid->PID_OUT=0;
}

// ����: APID_Init_All()
// ����: ���е����е�Ƕ�pid������ʼ��
// ��������
// �������
void APID_Init_All()
{
	APID_Init(&motor1.apid);
	APID_Init(&motor2.apid);
	APID_Init(&motor3.apid);
	APID_Init(&motor4.apid);
	
 APID_Init(&motor5.apid);
	
}
//if(ptr->angle - ptr->last_angle > 4096)
//	ptr->round_cnt --;
//else if (ptr->angle - ptr->last_angle < -4096)
//	ptr->round_cnt ++;
//	ptr->total_angle = ptr->round_cnt * 8192 + ptr->angle - ptr->offset_angle;
// ����: apid_realize()
// ����: �����е�Ƕ�pid����
// �����������е�Ƕ�pid�����ṹ�壬�����е�Ƕ�pidϵ��
// �������
// �ڲ��������û��������
void apid_realize(APID_t *apid,float kp,float ki,float kd)
{
	//ƫ����
	apid->err = apid->target_angle - apid->actual_angle;
	//��е�Ƕ������СֵԾǨ����
//	if(apid->err > 4096)
//		apid->err = apid->err - 8192;
//	if(apid->err < -4096)
//		apid->err = apid->err + 8192;
	
	//������
	if(abs(apid->err) <= IntegralSeparation)		//���ַ���
		apid->err_integration += apid->err;
	if(apid->err_integration > IntegralUpperLimit)		//�����ֱ���
		apid->err_integration = IntegralUpperLimit;
	else if(apid->err_integration < -IntegralUpperLimit)
		apid->err_integration = -IntegralUpperLimit;
	
	//�������
	apid->P_OUT = kp * apid->err;
	apid->I_OUT = ki * apid->err_integration;
	apid->D_OUT = kd * (apid->err-apid->last_err);
	apid->last_err = apid->err;
	
	//����޷�
	if((apid->P_OUT + apid->I_OUT + apid->D_OUT) > aPID_OUT_MAX) 			
		apid->PID_OUT = aPID_OUT_MAX;
	else if((apid->P_OUT + apid->I_OUT + apid->D_OUT) < -aPID_OUT_MAX) 
		apid->PID_OUT = -aPID_OUT_MAX;
	else
		apid->PID_OUT = apid->P_OUT + apid->I_OUT;
	
}

// ����: apid_PID_realize()
// ����: �����е�Ƕ�pidʵ��
// �����������е�Ƕ�pidϵ��
// �������
void apid_PID_realize(float kp,float ki,float kd)
{
	//��ȡ��ǰ�Ƕ�ֵ
	motor1.apid.actual_angle = motor1.total_angle;
	motor2.apid.actual_angle = motor2.total_angle;
	motor3.apid.actual_angle = motor3.total_angle;
	motor4.apid.actual_angle = motor4.total_angle;
	
	motor5.apid.actual_angle = motor5.total_angle;
	
	//��������е�Ƕ�pid
	apid_realize(&motor1.apid,kp,ki,kd);
	apid_realize(&motor2.apid,kp,ki,kd);
	apid_realize(&motor3.apid,kp,ki,kd);
	apid_realize(&motor4.apid,kp,ki,kd);
	apid_realize(&motor5.apid,kp,ki,kd);
	
	
	//���õ��Ŀ���ٶ�
	set_chassis_motor_speed(motor1.apid.PID_OUT,motor2.apid.PID_OUT,motor3.apid.PID_OUT,motor4.apid.PID_OUT);
}


//���õ��Ŀ���е�Ƕ�
void set_chassis_motor_angle(int motor1_angle,int motor2_angle,int motor3_angle,int motor4_angle)
{
	
	motor1.apid.target_angle = motor1_angle;
	motor2.apid.target_angle = motor2_angle;
	motor3.apid.target_angle = motor3_angle;
	motor4.apid.target_angle = motor4_angle;
}
void set_trigger_motor_angle(int motor5_angle)
{
	
	motor5.apid.target_angle = motor5_angle;

}
