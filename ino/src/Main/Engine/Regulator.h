
#ifndef Regulator_h
#define Regulator_h

// TODO: think about implementing EngineRegulator as static class, rewrite compute_v callback function

// class EngineRegulator holds important values for encoders disk, coefficients
// and computes output value for engine, and current velocity

// Member compute_velocity which computes velocity should be called from encoder_callback interrupt function

class EngineRegulator {
private:
    // angular size of a hole
    float d_alpha;
    // proportional coefficient
    float Kp;
    // desired velocity we want
    float des_vel;
    // variables for computing elapsed time of passing one hole
    unsigned long current_time = 0, time = 0, dt = 0, trans_vel = 0;

public:
    EngineRegulator(){
        d_alpha = 0;
        Kp = 0;
    }
    // args: angle of one hole of disk, proportional coefficient
    void setParam(float angle, float coeff){
        d_alpha = angle;
        Kp = coeff;
    }

    // set desired velocity to a passed value
    void setDesVel(float desiredVelocity){
        des_vel = desiredVelocity;
    }

    // Member calculates power value
    long get_Up(long des_vel){
        long e = des_vel - trans_vel;
        return Kp * e;
    }

    // This member get called from encoder's interrupt function and computes current linear velocity of a disk
    void compute_velocity(){
        current_time = millis();
        dt = current_time - time;
        //TODO SHOULD NEXT 2 LINES BE HERE?
        time = current_time;
        trans_vel = d_alpha / dt;
    }
};



#endif
