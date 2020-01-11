#include <string.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include <jansson.h>
#include "json.h" 
#include "motor.h"
#include "kinematic.h"
#include "mode.h"
#include "bsp_debug_usart.h"
#include "remote_code.h"

  extern char receiveBuffer[MAX_LENGTH];
  enum chassis_mode_t chassis_modes;
  enum gimbal_mode_t gimbal_modes;
  enum fric_mode_t fric_modes;


void send_cgf_info_by_json(void)
{
	/*Kinematics.actual_velocities.linear_x=1;
	Kinematics.actual_velocities.linear_y=2;
	Kinematics.actual_angular.gimbal_angular.yaw_angular=4;
	Kinematics.actual_velocities.angular_z=3;
	Kinematics.actual_angular.gimbal_angular.pitch_angular=5;
	Kinematics.actual_angular.fric_angular=6;*/
	
	json_t *root;
	char *out;           
	root = json_pack("[{sfsfsfsfsfsfsfsfsfsf}[ffffffffff]]",\
						"linear_x", (Kinematics.actual_velocities.linear_x),\
						"linear_y", (Kinematics.actual_velocities.linear_y),\
					  "yaw_angular", (Kinematics.actual_angular.gimbal_angular.yaw_angular),\
	          "angular_z", (Kinematics.actual_velocities.angular_z),\
						"pitch_angular", (Kinematics.actual_angular.gimbal_angular.pitch_angular),\
	          "fric_angular", (Kinematics.actual_angular.fric_angular),\
	          "ceshi1", (Kinematics.actual_angular.fric_angular),\
	          "ceshi2", (Kinematics.actual_angular.fric_angular),\
	          "ceshi3", (Kinematics.actual_angular.fric_angular),\
	          "ceshi4", (Kinematics.actual_angular.fric_angular),\
					  (Kinematics.actual_velocities.linear_x),\
						(Kinematics.actual_velocities.linear_y),\
	          (Kinematics.actual_velocities.angular_z),\
						(Kinematics.actual_angular.gimbal_angular.yaw_angular),\
            (Kinematics.actual_angular.gimbal_angular.pitch_angular),\
	          (Kinematics.actual_angular.fric_angular),\
          	(Kinematics.actual_angular.fric_angular),\
          	(Kinematics.actual_angular.fric_angular),\
          	(Kinematics.actual_angular.fric_angular),\
          	(Kinematics.actual_angular.fric_angular));
	
	out = json_dumps(root, JSON_ENCODE_ANY);//  ��Ϊ�ַ���
	printf("%s\r\n", out);
	json_decref(root);
	//free(root);
	free(out);
}

//���͵���ʵ���ٶ���Ϣ

void send_chassis_info_by_json(void)
{
	Kinematics.actual_velocities.linear_x=1;
	Kinematics.actual_velocities.linear_y=2;
	Kinematics.actual_velocities.angular_z=3;
	json_t *root;
	char *out;           //
	root = json_pack("[{sfsfsf}[fff]]",\
						"linear_x", (Kinematics.actual_velocities.linear_x),\
						"linear_y", (Kinematics.actual_velocities.linear_y),\
						"angular_z", (Kinematics.actual_velocities.angular_z),\
						(Kinematics.actual_velocities.linear_x),\
						(Kinematics.actual_velocities.linear_y),\
						(Kinematics.actual_velocities.angular_z));   //(int),
						
	out = json_dumps(root, JSON_ENCODE_ANY);//  ��Ϊ�ַ���
	printf("%s\r\n", out);
	json_decref(root);//��С���ü��� ������Դ����
	//free(root);
	free(out);
}

void send_gimbal_info_by_json(void)   ///����������̨��Ϣ ������*********************************
{
//����
	//Kinematics.target_angular.gimbal_angular.yaw_angular=5;
	//Kinematics.target_angular.gimbal_angular.yaw_angular=10;
  json_t *root;
	char *out;           //
	root = json_pack("[{sfsf}[ff]]",\
						
					  "yaw_angular", (Kinematics.actual_angular.gimbal_angular.yaw_angular),\
						"pitch_angular", (Kinematics.actual_angular.gimbal_angular.pitch_angular),\
						(Kinematics.actual_angular.gimbal_angular.yaw_angular),\
						(Kinematics.actual_angular.gimbal_angular.pitch_angular));   //(int),
						
	out = json_dumps(root, JSON_ENCODE_ANY);
	printf("%s\r\n", out);
	json_decref(root);
	//free(root);
	free(out);

}
void send_fric_info_by_json()     //����Ħ������Ϣ
{
//����   
Kinematics.actual_angular.fric_angular=7;
json_t *root;
	char *out;           //
	root = json_pack("[{sf}[f]]",\
						
					  "fric_angular", (Kinematics.actual_angular.fric_angular),\
						
						(Kinematics.actual_angular.fric_angular));   //(int),
						
	out = json_dumps(root, JSON_ENCODE_ANY);
	printf("%s\r\n", out);
	json_decref(root);
	//free(root);
	free(out);
}



	float tmp_getx;
	float tmp_gety;

  float pitch_angle;
	float yaw_angle;
//json���ݻ�����
char json_Buffer[MAX_LENGTH];
extern char receiveBuffer[MAX_LENGTH];
//��־λ����־�Ѿ��յ�һ������ָ��ڶ�ʱ���ж��е������н�������
uint8_t flag_command_recieved = 0;
uint8_t flag_command_recieved1 = 0;
uint8_t flag_command_recieved2 = 0;
uint8_t flag_command_recieved3 = 0;
uint8_t flag_command_recieved4 = 0;
uint8_t flag_command_recieved5 = 0;

//�������յ��ĵ����ٶȿ���ָ��
void resolve_json_chassis_command(void)
{
	json_t *root;
	json_t *chassis_obj;
	json_t *item_obj;
	json_error_t error;
	root = json_loads(json_Buffer,0,&error); //����Json�ַ��� ����������������or  object
	chassis_obj = json_object_get( root, "chassis" );  //Get a value corresponding to key from object
	item_obj = json_array_get( chassis_obj, 0 );//Returns the element in array at position index
	Kinematics.target_velocities.linear_x =1.0f*json_integer_value(item_obj);	//real
	item_obj = json_array_get( chassis_obj, 1 );
	Kinematics.target_velocities.linear_y = 1.0f*json_integer_value(item_obj);
	item_obj = json_array_get( chassis_obj, 2 );
	Kinematics.target_velocities.angular_z = 5;//1.0f*json_integer_value(item_obj)/100;///100;
	json_decref(item_obj); //Decrement the reference count of json. As soon as a call to json_decref() drops the reference count to zero, the value is destroyed and it can no longer be used.
	json_decref(chassis_obj);
	json_decref(root);
}
//�����յ�����̨����ָ��
void resolve_json_gimbal_command()
{ 
	json_t *root;
	json_t *gimbal_obj;
	json_t *item_obj;
	json_error_t error;
	root = json_loads(json_Buffer,0,&error);
	gimbal_obj = json_object_get( root, "gimbal" );
	item_obj = json_array_get( gimbal_obj, 0 );
	Kinematics.target_angular.gimbal_angular.yaw_angular=1.0f*json_integer_value(item_obj);
	item_obj = json_array_get( gimbal_obj, 1 );
	Kinematics.target_angular.gimbal_angular.pitch_angular=1.0f*json_integer_value(item_obj);
	json_decref(item_obj);
	json_decref(gimbal_obj);
	json_decref(root);

}
//�����յ���Ħ���ֿ���ָ��
void resolve_json_fric_command()
	{ 
	json_t *root;
	json_t *fric_obj;
	json_t *item_obj;
	json_error_t error;
	root = json_loads(json_Buffer,0,&error);
	fric_obj = json_object_get( root, "fric_angular" );
	item_obj = json_array_get( fric_obj, 0 );
	Kinematics.target_angular.fric_angular=1.0f*json_integer_value(item_obj);
	json_decref(item_obj);
	json_decref(fric_obj);
	json_decref(root);

}
void resolve_json_trigger_command()
	{ 
	json_t *root;
	json_t *trigger_obj;
	json_t *item_obj;
	json_error_t error;
	root = json_loads(json_Buffer,0,&error);
	trigger_obj = json_object_get( root, "trigger_angular" );
	item_obj = json_array_get( trigger_obj, 0 );
	Kinematics.target_angular.trigger_angular=150;//1.0f*json_integer_value(item_obj);
	json_decref(item_obj);
	json_decref(trigger_obj);
	json_decref(root);

}
	
	//����ģʽ����ָ��
void resolve_json_mode_command()
{
  resolve_chassis_mode_command();
	resolve_gimbal_mode_command();
	resolve_fric_mode_command();

}
void resolve_chassis_mode_command()
{ 
  json_t *root;
	json_t *chassis_mode_obj;
	json_t *item_obj;
	json_error_t error;
	root = json_loads(json_Buffer,0,&error);
	chassis_mode_obj = json_object_get( root, "translation" );
	item_obj = json_array_get( chassis_mode_obj, 0 );
	chassis_modes = (chassis_mode_t)(json_integer_value(item_obj));
	json_decref(item_obj);
	json_decref(chassis_mode_obj);
	json_decref(root);

}
void resolve_gimbal_mode_command()
{
  json_t *root;
	json_t *gimbal_mode_obj;
	json_t *item_obj;
	json_error_t error;
	root = json_loads(json_Buffer,0,&error);
	gimbal_mode_obj = json_object_get( root, "gimbal_mode" );
	item_obj = json_array_get( gimbal_mode_obj, 0 );
	gimbal_modes = (gimbal_mode_t)(json_integer_value(item_obj));
	json_decref(item_obj);
	json_decref(gimbal_mode_obj);
	json_decref(root);

}
void resolve_fric_mode_command()
{
  json_t *root;
	json_t *fric_mode_obj;
	json_t *item_obj;
	json_error_t error;
	root = json_loads(json_Buffer,0,&error);
	fric_mode_obj = json_object_get( root, "fric_mode" );
	item_obj = json_array_get( fric_mode_obj, 0 );
	fric_modes = (fric_mode_t)(json_integer_value(item_obj));
	json_decref(item_obj);
	json_decref(fric_mode_obj);
	json_decref(root);

}

void resolve_json(void)
{
	//����json�ն���
	json_t *Chassis_obj;
	json_t *item1_obj;
	json_error_t error;
	//��������������ת��Ϊjson����
	Chassis_obj = json_loads(json_Buffer,0,&error);
	
	if(!Chassis_obj)
	{
		//printf("load json failed\t\n");
	}
	else
	{
		
//		json_t *x, *y, *z;
		//json_object_get:��ȡ������/�����еĶ���/����/��ֵ
		//�����������������ȡ����
//		x = json_object_get( Chassis_obj, "linear_x" );
//		y = json_object_get( Chassis_obj, "linear_y" );
//		z = json_object_get( Chassis_obj, "angular_z" );
//		
//		tmp_getx = json_real_value(x);
//		tmp_gety = json_real_value(y);
		
		
		item1_obj = json_object_get(Chassis_obj, "x");
		//Kinematics.target_velocities.linear_x = json_real_value(item1_obj);
		item1_obj = json_object_get(Chassis_obj, "y");
		Kinematics.target_velocities.linear_y = json_real_value(item1_obj);
		item1_obj = json_object_get(Chassis_obj, "z");
		//Kinematics.target_velocities.angular_z = json_real_value(item1_obj);
		
		
	}
	//json_array_get:��ȡ�������е�Ԫ��
	//json_array_get(Arry_obj,0);
	//
	//sprintf(json_Buffer, "item1_obj value is %f",json_real_value(item1_obj));
	
	json_delete(Chassis_obj);
	json_delete(item1_obj);
}





//****************************Jansson Test*******************************//
void jansson_pack_test(void)
{
	json_t *root;
	char *out;
	
	/* Build an empty JSON object */
	root = json_pack("{}");
	
	out = json_dumps(root, JSON_ENCODE_ANY);
	//printf("out:%s\r\n", out);
	json_delete(root);
	//free(root);
	free(out);
	
	/* Build the JSON object {"foo": 42, "bar": 7} */
	root = json_pack("{sisi}", "foo", 42, "bar", 7);
	
	out = json_dumps(root, JSON_ENCODE_ANY);
	printf("out:%s\r\n", out);
	json_delete(root);
	free(out);
	
	/* Like above, ':', ',' and whitespace are ignored */
	root = json_pack("{s:i, s:i}", "foo", 42, "bar", 7);
	
	out = json_dumps(root, JSON_ENCODE_ANY);
	printf("out:%s\r\n", out);
	json_delete(root);
	free(out);
	
	/* Build the JSON array [[1, 2], {"cool": true}] */
	root = json_pack("[[i,i],{s:b}]", 1, 2, "cool", 1);
	
	out = json_dumps(root, JSON_ENCODE_ANY);
	printf("out:%s\r\n", out);
	json_delete(root);
	free(out);
	
	/* Build a string from a non-null terminated buffer */
	char buffer[4] = {'t', 'e', 's', 't'};
	root = json_pack("[s#]", buffer, 4);

	out = json_dumps(root, JSON_ENCODE_ANY);
	printf("out:%s\r\n", out);
	json_delete(root);
	free(out);
	
	/* Concatenate strings together to build the JSON string "foobarbaz" */
	root = json_pack("[s++]", "foo", "bar", "baz");
	
	out = json_dumps(root, JSON_ENCODE_ANY);
	printf("out:%s\r\n", out);
	json_delete(root);
	free(out);
	
	
	
	
}





void caclulate_pwm_pulse()
{
	float unit_pwm_pulse= (840.0f/360.0f);
	
	pwm_pulse1 = (1080+unit_pwm_pulse * Kinematics.target_angular.gimbal_angular.pitch_angular)*1.0f;
	
	pwm_pulse2 = (1080+unit_pwm_pulse * Kinematics.target_angular.gimbal_angular.yaw_angular)*1.0f;
	
}


//void convert_ascii2json_test(char * receive_buffer)
//{
// json_t *root;
// printf("receive_buffer:%s\r\n", receive_buffer);
// if(receive_buffer[0]=='{')
// {  
//    root=json_loads(receive_buffer,0,&j_error);//���ַ���ת����json������ASCII
//  json_unpack(root, "{s:i, s:i}","cx", &data1,"cy", &data2);
//    //char * output_buffer = json_dumps(root,JSON_ENCODE_ANY);
//    //printf("output_buffer:%s\r\n", output_buffer);
//  printf("data:%d��%d\r\n",(int)data1,(int)data2);    
//  json_delete(root);//��Ҫ��free����free�����ᵼ��Heap���
//  //free(root);
// }
// else
// {
//  //Usart_SendString( USART2, "error");
// }
//}
