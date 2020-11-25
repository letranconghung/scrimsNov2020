#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	Motor BL (BLPort, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
	Motor BR (BRPort, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
	Motor lRoller(lRollerPort, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
	Motor rRoller(rRollerPort, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
	Motor bouncer(bouncerPort, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
	Motor ejector(ejectorPort, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
	Motor shooter(shooterPort, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
	Motor froller(frollerPort, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
	ADIAnalogIn color(colorPort);
	ADIDigitalIn limit(limitPort);
	Controller master(E_CONTROLLER_MASTER);
	BL.tare_position();
	BR.tare_position();
	// Task mechControlTask (mechControl);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	double BRAKE_POW = 0;
	Motor BL (BLPort);
	Motor BR (BRPort);
	Motor lRoller(lRollerPort);
	Motor rRoller(rRollerPort);
	Motor bouncer(bouncerPort);
	Motor ejector(ejectorPort);
	Motor shooter(shooterPort);
	Motor froller(frollerPort);
	ADIAnalogIn color(colorPort);
	Controller master(E_CONTROLLER_MASTER);
	master.clear();
	bool tankDrive = false;
	while (true) {
		if(master.get_digital_new_press(DIGITAL_Y)) tankDrive = !tankDrive;
		if(tankDrive){
      int l = master.get_analog(ANALOG_LEFT_Y);
      int r = master.get_analog(ANALOG_RIGHT_Y);
      BL.move(l+BRAKE_POW);
      BR.move(r+BRAKE_POW);
    } else{
      int y = master.get_analog(ANALOG_LEFT_Y);
      int x = master.get_analog(ANALOG_RIGHT_X);
      BL.move(y+x+BRAKE_POW);
      BR.move(y-x+BRAKE_POW);
    }
		if(master.get_digital(DIGITAL_R2)){
			// intake with shooting
			allMove(127, 127, 127, 127, 127);
		}else if(master.get_digital(DIGITAL_R1)){
			if(master.get_digital(DIGITAL_L2)){
				// eject from intake
				allMove(127, 127, -127, 0, 127);
			}else{
				// intake without shooting
				allMove(127, 127, 127, 0, 127);
			}
		}else if(master.get_digital(DIGITAL_L2)){
			// eject from storage
			allMove(0, 127, -127, -127, -127);
		}else if(master.get_digital(DIGITAL_L1)){
			// front outtake
			allMove(-127,-127,-127,-127, -127);
		}else{
			// stop bro
			allMove(0,0,0,0,0);
		}
		pros::delay(20);
	}
}
