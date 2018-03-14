#ifndef Regulator_h
#define Regulator_h

const unsigned float d_alpha, Kp = 2000.0;
unsigned long current_time, time, dt, trans_vel;

void compute_v(){
	nointerrupts();
	current_time = millis();
	dt = current_time - time;
	time = current_time;
	trans_vel = d_alpha / dt;
	nointerrupts();
}

long get_Up(long des_vel){
  long e = des_vel - trans_vel;
  return Kp * e;
}

#endif