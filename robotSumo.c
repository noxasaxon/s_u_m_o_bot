#pragma config(Sensor, S1,     sonic,          sensorSONAR)
#pragma config(Sensor, S2,     lightRear,      sensorLightActive)
#pragma config(Sensor, S3,     lightLeft,      sensorLightActive)
#pragma config(Sensor, S4,     lightRight,     sensorLightActive)
#pragma config(Motor,  motorA,          swivel,        tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          rightTrack,    tmotorNXT, PIDControl, driveRight, encoder)
#pragma config(Motor,  motorC,          leftTrack,     tmotorNXT, PIDControl, driveLeft, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int state = 0;

void pivot90(bool right){
	//right is true, left is false
		if(right){
			motor[leftTrack] = 100;
			motor[rightTrack] = -100;
			wait1Msec(625);
		}else {
			motor[leftTrack] = -100;
			motor[rightTrack] = 100;
			wait1Msec(625);

		}

}

int lightsClear(){
	if(SensorValue(lightLeft) > threshold && SensorValue(lightRight) > threshold && SensorValue(lightRear) > threshold){
		return 1;
	}else{
		return 0;
	}
}

void lightTripped(){
	// light sensor has been tripped
	
	if(SensorValue(lightLeft) > threshold){
		// reverse robot to the right
		while(SensorValue(lightRight) > threshold) && SensorValue(lightRear) > threshold){
		motor[leftTrack] = -50;
		motor[rightTrack] = -25;
		wait1Msec(700);
		}
	}else if(SensorValue(lightRight) > threshold){
	  //reverse robot to the left
	  while(SensorValue(lightLeft) > threshold) && SensorValue(lightRear) > threshold){
		  
	  	motor[leftTrack] = -25;
		motor[rightTrack] = -50;
		wait1Msec(700);
		break;
	  }
	}else if(SensorValue(lightRear) > threshold){
	// forward robot
		while(SensorValue(lightLeft) > threshold) && SensorValue(lightRight) > threshold){
		motor[leftTrack] = 50;
		motor[rightTrack] =  50;
		wait1Msec(1000);
		break;
	
	}

}
 
void scanningState(int state){
	//assumes back against the wall
	
	while(lightsClear()){
		clearTimer(T1);
		while(SensorValue(sonic) > 70){
			while(time1[T1] < 625){
				motor[leftTrack] = 100;
				motor[rightTrack] = -100;	
						
			}
			clearTimer(T1);
			while(time1[T1] < 1250){
				motor[leftTrack] = -100;
				motor[rightTrack] = 100;	
			}
		}
		//target found
		
		motor[leftTrack] = 100;
		motor[rightTrack] = 100;


}

void turtleState(int state){
	int distance1, distance2;
	
	while(lightsClear()){
		//checking for wall, first point of data
		motor[leftTrack] = 50;
		motor[rightTrack] = 50;
		
		if(SensorValue(sonic) <= 35) {
			//when seeing wall for first time, reverse for a short bit then rotate 90, move a bit, rotate 90 back and check for wall again
			while(lightsClear()){
			
				motor[leftTrack] = -50;
				motor[rightTrack] = -50;
				wait1Msec(500);
				distance1 = SensorValue(sonic); //save distance to supposed wall
				//spin 90 deg
				motor[leftTrack] = 100;
				motor[rightTrack] = -100;
				wait1Msec(625);
				//move forward parallel to supposed wall
				motor[leftTrack] = 50;
				motor[rightTrack] = 50;
				wait1Msec(1500);
			    //rotate 90 back to face wall and check distance
				motor[leftTrack] = -100;
				motor[rightTrack] = 100;
				wait1Msec(625);
				distance2 = SensorValue(sonic); //save second distance var to check against distance1
				//calculate to determine if obj is wall
				if(distance1 - distance2 < 4 && distance1 - distance2 > -4){
					//we are facing a wall, spin 180
					while(lightsClear()){
						displayStringAt(0,31,"Spinning");
						motor[leftTrack] = 100;
						motor[rightTrack] = -100;
						wait1Msec(1250);
						break;
					}
				}
			}//end while loop
			lightTripped();
		}//end if statement
		lightTripped();
	
		state++; //goes to scanning state
		
	}//end first while loop
	
	lightTripped();
	
}//end turtle state

task main()
{
	//RC control mode
	//wait1Msec(30000);

while(1){
	while(lightsClear()){
		
		switch(state){
	
			case 0: turtleState(state); //find wall, put back to it
			break;
			case 1: scanningState(state); //with back to wall, then search for targets and attack
			break;
			case 2: resetState(state); //scan 360 and keep distance
			break;

		}


	}
	// light has been tripped
	lightTripped();
}

	//end of program
}