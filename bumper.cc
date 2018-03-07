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

  // Control loop
  while(true) 
  {    
      double turnrate, speed;

      // Read from the proxies.
      robot.Read();

      // What does odometry tell us? In other words, how far do we
      // think we have gone?
      std::cout << "x: " << pp.GetXPos()  << std::endl;
      std::cout << "y: " << pp.GetYPos()  << std::endl;
      std::cout << "a: " << pp.GetYaw()  << std::endl;

      // Print out what the bumpers tell us:
      std::cout << "Left  bumper: " << bp[0] << std::endl;
      std::cout << "Right bumper: " << bp[1] << std::endl;
      
      // If either bumper is pressed, go backwards. Depending on the
      // combination of bumpers that are pressed, turn some also,
      // trying to turn away from the point of contact. 
      //
      // Otherwise just go forwards
int stuck = 0;
	//if in the zone 1 
	if(pp.GetXPos() < 10 && pp.GetYPos() < 5){

		//if in the zone 1 A
		if(pp.GetXPos() < 10 && pp.GetYPos() < 2){
			//and facing east, go straight ahead
			if(pp.GetYaw() < 0.1 && pp.GetYaw() > -0.1){
				std::cout << "zone 1: going straight " << std::endl;
				turnrate = 0;   
		    	speed=0.5;
			}else{

				if(bp[0] || bp[1]){
			    	std::cout << "wall in contact: ";	
					speed=-0.2;
					if (bp[0] && !bp[1]) {  turnrate=dtor(-30); std::cout << "1" << std::endl;
						stuck++;
					}
					if (!bp[0] && bp[1]) { turnrate=dtor(30); std::cout << "2" << std::endl;
					stuck++;
					
					}
					if (bp[0] && bp[1]) { stuck++; std::cout << "3" << std::endl;
					if(rand()%2 > 0){turnrate=dtor(-30);}
					else {turnrate=dtor(30);}}
					stuck++;
					if(stuck > 20){
						std::cout << "stuck count max: ";
						if(rand()%2>0)	
						speed = -0.3;
						else
						speed = 0.3;
						if(rand()%2 > 0){turnrate=dtor(-30);}
						else {turnrate=dtor(30);}
					}
				}else{
					stuck = 0;
					speed=0.2;
					turnrate=dtor(30);		 
					std::cout << "wall : turning south " << std::endl;				
				}
				std::cout << "x: " << pp.GetXPos()  << std::endl;
				std::cout << "y: " << pp.GetYPos()  << std::endl;
				std::cout << "a: " << pp.GetYaw()  << std::endl;
			    std::cout << "Speed: " << speed << std::endl;      
			    std::cout << "Turn rate: " << turnrate << std::endl << std::endl;
				std::cout << "set speed" << std::endl;	
	   			pp.SetSpeed(speed, turnrate); 
			    robot.Read();			    
			
				//but facing other than east, turn to east
				if(!(pp.GetYaw() < 0.2 && pp.GetYaw() > -0.2)){
					speed=0.2;
					turnrate=dtor(30);
					pp.SetSpeed(speed, turnrate); 
					std::cout << "zone 1: turning east " << std::endl;
					      std::cout << "x: " << pp.GetXPos()  << std::endl;
					      std::cout << "y: " << pp.GetYPos()  << std::endl;
					      std::cout << "a: " << pp.GetYaw()  << std::endl;
					      std::cout << "Speed: " << speed << std::endl;      
	      				  std::cout << "Turn rate: " << turnrate << std::endl << std::endl;
	      				  robot.Read();
				}
			}
		}else{
			//if in zone 1 B, turn south
			int stuck = 0;
			while(!(pp.GetYaw() < -1.5 && pp.GetYaw() >-1.6)){
				//if hitting an object
			    if(bp[0] || bp[1]){
			    	std::cout << "object in contact: ";	
					speed=-0.2;
					if (bp[0] && !bp[1]) {  turnrate=dtor(-30); std::cout << "1" << std::endl;
						stuck++;
					}
					if (!bp[0] && bp[1]) { turnrate=dtor(30); std::cout << "2" << std::endl;
					stuck++;
					
					}
					if (bp[0] && bp[1]) { stuck++; std::cout << "3" << std::endl;
					if(rand()%2 > 0){turnrate=dtor(-30);}
					else {turnrate=dtor(30);}}
					stuck++;
					if(stuck > 20){
						std::cout << "stuck count max: ";
						if(rand()%2>0)	
						speed = -0.3;
						else
						speed = 0.3;
						if(rand()%2 > 0){turnrate=dtor(-30);}
						else {turnrate=dtor(30);}
					}
				}else{
					stuck = 0;
					speed=0.2;
					turnrate=dtor(30);		 
					std::cout << "zone 1 B: turning south " << std::endl;				
				}
				std::cout << "x: " << pp.GetXPos()  << std::endl;
				std::cout << "y: " << pp.GetYPos()  << std::endl;
				std::cout << "a: " << pp.GetYaw()  << std::endl;
			    std::cout << "Speed: " << speed << std::endl;      
			    std::cout << "Turn rate: " << turnrate << std::endl << std::endl;
				std::cout << "set speed" << std::endl;	
	   			pp.SetSpeed(speed, turnrate); 
			    robot.Read();			    
			}
			//and move forward till you reach zone 1 A
			while(!(pp.GetXPos() < 10 && pp.GetYPos() < 2)){
				std::cout << "zone 1 B: going straight " << std::endl;
				turnrate = 0;   
		    	speed=0.2;		    	
			    std::cout << "x: " << pp.GetXPos()  << std::endl;
			    std::cout << "y: " << pp.GetYPos()  << std::endl;
			    std::cout << "a: " << pp.GetYaw()  << std::endl;
			    std::cout << "Speed: " << speed << std::endl;      
	      		std::cout << "Turn rate: " << turnrate << std::endl << std::endl;
	      	    if(bp[0] || bp[1]){
			    std::cout << "object in contact" << std::endl;	
				speed=-0.2;
				if (bp[0] && !bp[1]) {  turnrate=dtor(-30); }
				if (!bp[0] && bp[1]) { turnrate=dtor(30);}
				if (bp[0] && bp[1]) {
				if(rand()%2 > 0){turnrate=dtor(-30);}
				else {turnrate=dtor(30);}}} 
	   			pp.SetSpeed(speed, turnrate); 
	      		robot.Read();
			}
		}

	}else{
			//if in the zone 2 
		if(pp.GetXPos() >= 10 && pp.GetYPos() <= 10){
			//and facing north, go straight ahead
			if(pp.GetYaw() < 1.6 && pp.GetYaw() > 1.5){
					std::cout << "zone 2: going straight " << std::endl;
					turnrate = 0;   
	    			speed=0.5;
			}else{
				//but facing other than south, turn to north
				while(!(pp.GetYaw() < 1.7 && pp.GetYaw() > 1.4)){
					speed=0.2;
					turnrate=dtor(30);
					pp.SetSpeed(speed, turnrate); 
					std::cout << "zone 2: turning north " << std::endl;
				        std::cout << "x: " << pp.GetXPos()  << std::endl;
				        std::cout << "y: " << pp.GetYPos()  << std::endl;
				        std::cout << "a: " << pp.GetYaw()  << std::endl;
      					std::cout << "Speed: " << speed << std::endl;      
      					std::cout << "Turn rate: " << turnrate << std::endl << std::endl;
      					robot.Read();
				}
			}
		}else{
			//if in the zone 3	 
			if(pp.GetXPos() > 2 && pp.GetYPos() > 10){
					//and facing west, go straight ahead
				if(pp.GetYaw() < 3.1 && pp.GetYaw() > 3.0){
						std::cout << "zone 3: going straight " << std::endl;
						turnrate = 0;   
	    				speed=0.5;
				}else{
					//but facing other than west, turn to west
					while(!(pp.GetYaw() < 3.1 && pp.GetYaw() > 3.0)){
							speed=0.2;
							turnrate=dtor(30);
							pp.SetSpeed(speed, turnrate); 
							std::cout << "zone 3: turning west " << std::endl;
							std::cout << "x: " << pp.GetXPos()  << std::endl;
							std::cout << "y: " << pp.GetYPos()  << std::endl;
							std::cout << "a: " << pp.GetYaw()  << std::endl;
		      					std::cout << "Speed: " << speed << std::endl;      
		      					std::cout << "Turn rate: " << turnrate << std::endl << std::endl;
		      					robot.Read();
					}
				}
			}else{
				//if in the zone 4	 
				if(pp.GetXPos() < 1 && pp.GetYPos() > 2){
						//and facing south, go straight ahead
					if(pp.GetYaw() < -1.5 && pp.GetYaw() >-1.6){
							std::cout << "zone 4: going straight " << std::endl;
							turnrate = 0;   
		    				speed=0.5;
					}else{
						//but facing other than south, turn to south
						while(!(pp.GetYaw() < -1.5 && pp.GetYaw() >-1.6)){
								speed=0.2;
								turnrate=dtor(30);
								pp.SetSpeed(speed, turnrate); 
								std::cout << "zone 4: turning south " << std::endl;
								std::cout << "x: " << pp.GetXPos()  << std::endl;
								std::cout << "y: " << pp.GetYPos()  << std::endl;
								std::cout << "a: " << pp.GetYaw()  << std::endl;
			      				std::cout << "Speed: " << speed << std::endl;      
			      				std::cout << "Turn rate: " << turnrate << std::endl << std::endl;
			      				robot.Read();
						}
					}
				}
			}
		}
	}


    
 

      std::cout << "Speed: " << speed << std::endl;      
      std::cout << "Turn rate: " << turnrate << std::endl << std::endl;

      // Send the motion commands that we decided on to the robot.
      pp.SetSpeed(speed, turnrate);  
    }
}


