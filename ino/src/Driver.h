#ifndef Driver_h
#define Driver_h

#include <math.h>

class Driver
{
private:
	int enable;
	int pwm1;
	int pwm2;

public:
	// Initialisers

	Driver(){};
	~Driver(){};

	void init(){
		enable = 7;
		pwm1 = 8;
		pwm2 = 9;

		pinMode(enable, OUTPUT);
  		pinMode(pwm1, OUTPUT);
  		pinMode(pwm2, OUTPUT);

  		digitalWrite(enable, LOW);
  		analogWrite(pwm1, 0);
		analogWrite(pwm2, 0);
		delay(500);
	}

	// Core functions

	void forward_Max(){
		analogWrite(pwm1, 255);
		analogWrite(pwm2, 0);
	}

	void forward(double vel){
		int n = static_cast<int>(vel);
		if(n >= 30){ n = 30;} //30 is arbitrary
		analogWrite(pwm1, n);
		analogWrite(pwm2, 0);
	}

	void backward(double vel){
		int n = static_cast<int>(vel);
		if(n >= 30){ n = 30;} //30 is arbitrary
		analogWrite(pwm1, 0);
		analogWrite(pwm2, n);
	}

	void parse(double vel){
		if(vel >= 0.0){
			forward(vel);
		}
		else{
			backward(abs(vel));
		}
	}
};

#endif
