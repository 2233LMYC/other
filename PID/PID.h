#ifndef _PID_H_
#define _PID_H_

typedef struct
{
	float target;
	float actual;
	float err;
	float err_last;
	float err_before;
	float err_sum;
	float Kp,Ki,Kd;
	
}PID_struct;

void PID_Init(PID_struct*pid);
float P_Proc(PID_struct*pid,float actual);
float PI_Proc(PID_struct*pid,float actual);
float PD_Proc(PID_struct*pid,float actual);
float PID_Proc(PID_struct*pid,float actual);
void PID_Set(PID_struct*pid,float target,float kp,float ki,float kd);


#endif

