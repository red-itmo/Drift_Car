#ifndef Regulator_h
#define Regulator_h


const double d_alpha, Kp = 2000.0;
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

long get_Up(long des_vel){
  long e = des_vel - trans_vel;
  return Kp * e;
}

#endif
