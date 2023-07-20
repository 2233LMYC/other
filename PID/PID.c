#include "PID.h"

PID_struct pid_motor_R;
PID_struct pid_motor_L;

void PID_Init(PID_struct*pid)
{
	pid->target = 0;
	pid->actual = 0;
	pid->err = 0;
	pid->err_last = 0;
	pid->err_sum = 0;
	pid->Kp = 0;
	pid->Ki = 0;
	pid->Kd = 0;
}

void PID_Set(PID_struct*pid,float target,float kp,float ki,float kd)
{
    pid->target = target;
    pid->Kp     = kp;
    pid->Ki     = ki;
    pid->Kd     = kd;
}


float P_Proc(PID_struct*pid,float actual)
{
	pid->actual = actual;//ʵ��ֵ����
	pid->err = pid->target - pid->actual;//�������
	pid->actual = (pid->Kp * pid->err);//���������
	return pid->actual;
	
}

float PI_Proc(PID_struct*pid,float actual)
{
	pid->actual = actual;//ʵ��ֵ����
	pid->err = pid->target - pid->actual;//�������
	pid->err_sum += pid->err;//����ۻ�
	pid->actual = (pid->Kp * pid->err) + (pid->Ki * pid->err_sum);//���������+������
	return pid->actual;
	
}

float PD_Proc(PID_struct*pid,float actual)
{
	pid->actual = actual;//ʵ��ֵ����
	pid->err = pid->target - pid->actual;//�������
	pid->actual = (pid->Kp * pid->err) + (pid->Kd * (pid->err - pid->err_last));//���������+΢����
	pid->err_last = pid->err;
	return pid->actual;
	
}

float PID_Proc(PID_struct*pid,float actual)
{
	pid->actual = actual;//ʵ��ֵ����
	pid->err = pid->target - pid->actual;//�������
	pid->err_sum += pid->err;//����ۻ�
    if(pid->err_sum>=5000)pid->err_sum = 5000;
    if(pid->err_sum<=-5000)pid->err_sum = -5000;

	pid->actual = (pid->Kp * pid->err) + (pid->Ki * pid->err_sum) + (pid->Kd * (pid->err - pid->err_last));//���������+������+΢����
	pid->err_last = pid->err;
	return pid->actual;
	
}




