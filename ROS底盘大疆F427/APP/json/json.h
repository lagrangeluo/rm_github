#ifndef __JSON_H
#define	__JSON_H

//这个值不可以太大，否则出问题
#define MAX_LENGTH 150







void send_chassis_info_by_json(void);
void send_gimbal_info_by_json(void);          //新增 发送云台信息给上位机
void send_fric_info_by_json(void);            //新增 发送摩擦轮信息给上位机
void send_cgf_info_by_json(void);      


void jansson_pack_test(void);
void resolve_json_chassis_command(void);
void resolve_json_gimbal_command(void);       //新增  解析云台控制指令
void resolve_json_fric_command(void);  //新增  解析云台控制指令
void resolve_json_trigger_command(void);
void resolve_json_mode_command(void);
void resolve_chassis_mode_command(void);
void resolve_gimbal_mode_command(void);
void resolve_fric_mode_command(void);
void caclulate_pwm_pulse(void);
#endif
