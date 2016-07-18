#pragma config(Motor,  motor1,          leftMotor,     tmotorVexIQ, openLoop, encoder)
#pragma config(Motor,  motor6,          rightMotor,    tmotorVexIQ, openLoop, reversed, encoder)
#pragma config(Sensor, port9,           colorSensor,   sensorVexIQ_ColorGrayscale)

const int LINE_THRESHOLD = 180;
const int DRIVE_SPEED = 20;

/// called when program is run
task main(){
	// track line
	while(true){
		if (getColorGrayscale(colorSensor) > LINE_THRESHOLD){
			setMotorSpeed(leftMotor, DRIVE_SPEED);
			setMotorSpeed(rightMotor, 0);
		}else{
			setMotorSpeed(leftMotor, 0);
			setMotorSpeed(rightMotor, DRIVE_SPEED);
		}
	}
}
