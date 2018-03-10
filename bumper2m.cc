/*
*  bumper.cc
* 
*  Sample code for a robot that has two front bumpers and uses them to
*  avoid obstacles. Suitable for use with the Roomba and Create. 
* 
*  Works IRL. Doesn't work so well in Stage, probably because of the
*  crude modeling of the bumpers, and can get stuck in convex shapes
*  when it backs into an object while trying to turn away from it.
*
*  Based on an example provided by Monica Anderson and Jeff Forbes,
*  via Carlos Jaramillo, and changed to (hopefully) make it easier to
*  understand.
*
*  Modified:    Simon Parsons
*  Date:        15th June 2009
*  Last change: 19th September 2011
*  
*/

#include <iostream>
#include <cstdlib>
#include <libplayerc++/playerc++.h>

int main(int argc, char *argv[])
{  
	using namespace PlayerCc;  

	// Set up proxies. Proxies are the datastructures that Player uses to
	// talk to the simulator and the real robot.

	PlayerClient    robot("localhost");  
	BumperProxy     bp(&robot,0);       // The bumper proxy reads from the
	// bumpers.
	Position2dProxy pp(&robot,0);       // The 2D proxy reads odometry from 
	// the robot and sends motion commands.

	// Allow the program to take charge of the motors (take care now)
	pp.SetMotorEnable(true);

	int x; int y;
	bool keepgoing=true;
	//robot starts facing right
	int direction=0;
	//0 going right
	//1 going up
	//2 going left
	int turndirection=1;
	//0 means turn clockwise
	//1 means turn counterclockwise
	int prevturn = 0;
	//0 means previous turn was clockwise
	//1 means previous turn was counterclockwise
	int isturning = 0;
	//used to time the turn from 1-10
	// Control loop
	while(keepgoing)
	{    
		double turnrate, speed;

		// Read from the proxies.
		robot.Read();

		// What does odometry tell us? In other words, how far do we
		// think we have gone?
		std::cout << "x: " << pp.GetXPos()  << std::endl;
		std::cout << "y: " << pp.GetYPos()  << std::endl;
		std::cout << "a: " << pp.GetYaw()  << std::endl;
		
		if(x == pp.GetXPos() && y == pp.GetYPos()){
			speed = 1;
			turnrate = 0;
		}

		x = pp.GetXPos();
		y = pp.GetXPos();


		// Print out what the bumpers tell us:
		std::cout << "Left  bumper: " << bp[0] << std::endl;
		std::cout << "Right bumper: " << bp[1] << std::endl;
		
		// If either bumper is pressed, go backwards. Depending on the
		// combination of bumpers that are pressed, turn some also,
		// trying to turn away from the point of contact. 
		//
		// Otherwise just go forwards
		if(isturning == 0){
			if(bp[0] || bp[1]){
				if(direction == 0){
					speed = -.3;
					turnrate = dtor(90);
					pp.SetSpeed(speed, turnrate);
					prevturn = 1;
					direction = 1;
					isturning++;
				}
				else if(direction == 1){
					speed = -.3;
					if(prevturn == 0){
						turnrate = dtor(-90);
						direction = 0;
					}
					else{
						turnrate = dtor(90);
						direction = 2;
					}
					pp.SetSpeed(speed, turnrate);
					isturning++;
				}
				else{
					speed = -.3;
					turnrate = dtor(-90);
					pp.SetSpeed(speed, turnrate);
					prevturn = 0;
					direction = 1;
					isturning++;
				}
			}
			else {
				turnrate = 0;   
				speed=1.0;
			}     
		}
		// What did we decide to do?
		std::cout << "Speed: " << speed << std::endl;
		std::cout << "Direction: " << direction <<std::endl;
		std::cout << "PrevTurn: " << prevturn <<std::endl;      
		std::cout << "isturning: " << isturning <<std::endl;  
		std::cout << "Turn rate: " << turnrate << std::endl << std::endl;
		if (isturning > 0) isturning++;
		if (isturning > 10) isturning = 0;

		// Send the motion commands that we decided on to the robot.
		pp.SetSpeed(speed, turnrate);  
	}
}


