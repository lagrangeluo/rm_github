#ifndef __JSON_H
#define	__JSON_H

//���ֵ������̫�󣬷��������
#define MAX_LENGTH 150







void send_chassis_info_by_json(void);
void send_gimbal_info_by_json(void);          //���� ������̨��Ϣ����λ��
void send_fric_info_by_json(void);            //���� ����Ħ������Ϣ����λ��
void send_cgf_info_by_json(void);      


void jansson_pack_test(void);
void resolve_json_chassis_command(void);
void resolve_json_gimbal_command(void);       //����  ������̨����ָ��
void resolve_json_fric_command(void);  //����  ������̨����ָ��
void resolve_json_trigger_command(void);
void resolve_json_mode_command(void);
void resolve_chassis_mode_command(void);
void resolve_gimbal_mode_command(void);
void resolve_fric_mode_command(void);
void caclulate_pwm_pulse(void);
#endif
