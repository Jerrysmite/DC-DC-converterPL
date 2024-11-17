#include "pid.h"

void PID_Init(PID_Structure *PID, float ref, float Kp, float Ki, float Kd, float Maxout, float Minout, float MaxIn, float Ts)
{
    PID->ref      = ref;
    PID->err      = 0;
    PID->fed      = 0;
    PID->Kp       = Kp;
    PID->Ki       = Ki;
    PID->Kd       = Kd;
    PID->Ts       = Ts;
    PID->Maxout   = Maxout;
    PID->Minout   = Minout;
    PID->MaxIn    = MaxIn;
    PID->iOut     = 0;
    PID->pOut     = 0;
    PID->dOut     = 0;
    PID->out      = 0;
    PID->Last_err = 0;
}

float PID_Realize(PID_Structure *PID, float fed)
{
    PID->fed = fed;
    PID->err = PID->ref - PID->fed;

    PID->pOut = PID->Kp * PID->err;
    PID->iOut += PID->Ki * PID->err * PID->Ts;
    PID->dOut = PID->Kd * (PID->err - PID->Last_err) / PID->Ts;
    if (PID->iOut >= PID->MaxIn) {
        PID->iOut = PID->MaxIn;
    }
    if (PID->iOut <= -PID->MaxIn) {
        PID->iOut = -PID->MaxIn;
    }
    PID->out = PID->pOut + PID->iOut + PID->dOut;

    if (PID->out >= PID->Maxout) {
        PID->out = PID->Maxout;
    }
    if (PID->out <= PID->Minout) {
        PID->out = PID->Minout;
    }
    PID->Last_err = PID->err;
    return PID->out;
}