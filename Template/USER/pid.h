#ifndef PID_H
#define PID_H

typedef struct {
    float fed;
    float ref;
    float err;
    float Ts;
    float Kp;
    float Ki;
    float Kd;
    float out;
    float Maxout;
    float Minout;
    float MaxIn;
    float pOut;
    float iOut;
    float dOut;
    float Last_err;
} PID_Structure;

void PID_Init(PID_Structure *PID, float Kp, float Ki, float Kd, float Maxout, float Minout, float MaxIn, float Ts);

float PID_Realize(PID_Structure *PID, float fed);

#endif