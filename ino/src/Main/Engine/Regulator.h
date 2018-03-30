#ifndef Regulator_h
#define Regulator_h

// TODO: think about implementing EngineRegulator as static class, rewrite compute_v callback function

// class EngineRegulator holds important values for encoders disk, koefficents
// and computes output value for engine, and current velocity

class EngineRegulator {
private:
    unsigned float d_alpha;
    float Kp;
    float des_vel;

public:
    // args: angle of one hole of disk, propotional koeff and desired velocity
    EngineRegulator(unsigned float angle, float koeff, float desiredVelocity = 0){
        d_alpha = angle;
        Kp = koeff;
        des_vel = desiredVelocity;
    }

    void setDesVel(float desiredVelocity){
        des_vel = desiredVelosity;
    }


    long get_Up(long des_vel){
        long e = des_vel - trans_vel;
        return Kp * e;
    }
};

unsigned long current_time, time, dt, trans_vel;

void compute_v(){
	noInterrupts();
	current_time = millis();
	dt = current_time - time;
	//TODO SHOULD NEXT 2 LINES BE HERE?
	time = current_time;
	trans_vel = d_alpha / dt;
	interrupts();
}



#endif
