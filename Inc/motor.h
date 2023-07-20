//
// Created by LMTX on 2023/7/17.
//

#ifndef INC_4W_MOTOR_H
#define INC_4W_MOTOR_H

enum {
    forward,
    back,
    left,
    right,
    stop
};

#define default_dpeed  3000

void Motor(char F_B ,int speed_L,int speed_R);
void MOTOR_L_F(int speedl);
void MOTOR_R_B(int speedr);
void MOTOR_R_F(int speedr);
void MOTOR_L_B(int speedl);







#endif //INC_4W_MOTOR_H
