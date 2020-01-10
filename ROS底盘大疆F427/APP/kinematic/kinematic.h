#ifndef _KINEMATIC_H
#define _KINEMATIC_H

#define wheel_diameter  10.000000f			//����ֱ��
#define half_width  13.000000f		//���̰��
#define half_length  13.000000f		//���̰볤

#define PI 			3.141593f
#define RPM2RAD 0.104720f										//ת��ת���ٶ�		1 rpm = 2pi/60 rad/s 
#define RPM2VEL 0.523599f										//ת��ת���ٶ�		vel = rpn*pi*D/60  cm/s
#define VEL2RPM 1.909859f										//���ٶ�תת��
#define M2006_REDUCTION_RATIO 36.000000f		//��������ٱ�
typedef struct
{
	float linear_vel;			//���ٶ�
	float rpm;						//ת��Ȧÿ����
}Speed_t;

typedef struct
{
	Speed_t target_speed;			
	Speed_t actual_speed;						
}Wheel_t;

//���̼������ĵ���/���ٶ�
typedef struct
{
	float linear_x;	//m/s
	float linear_y;
	float angular_z; //���ٶ�rpm
}Velocities_t;
//����
typedef struct
{
	float yaw_angular; 
	float pitch_angular;
	
}Gimbal_t;


typedef struct
{
	Gimbal_t gimbal_angular; 
	float fric_angular;
	float trigger_angular;
	
}Angular_t;


typedef struct
{
	Wheel_t wheel1;
	Wheel_t wheel2;
	Wheel_t wheel3;
	Wheel_t wheel4;
	
	Velocities_t target_velocities;		//Ŀ���ٶ�
	Velocities_t actual_velocities;	//ʵ���ٶ�
  Angular_t  target_angular;
	Angular_t  actual_angular;
	
}Kinematics_t;










extern Kinematics_t Kinematics;
extern float max_base_linear_speed;
extern float max_base_rotational_speed;

extern int stop_flag_1;
extern int stop_flag_2;


void BaseVel_To_WheelVel(float linear_x, float linear_y, float angular_z);
void trigger_to_motor(float trigger_angular);
void Get_Base_Velocities(void);
void speed_control(float speed_x,float speed_y,float speed_r);		//�����������ٶ�ת��Ϊ���ת��
void trigger_control(float trigger_angular);

#endif
