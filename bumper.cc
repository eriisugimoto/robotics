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
  int stuck1A = 0;
  int stuck1B = 0;
  int stuck2A = 0;
  int stuck2B = 0;
  int stuck3W = 0;
  int stuck3O = 0;
  int stuck4W = 0;
  int stuck4O = 0;
  bool checkPoints[5];
  checkPoints[0] = 0;
  checkPoints[1] = 0;
  checkPoints[2] = 0;
  checkPoints[3] = 0;
  checkPoints[4] = 0;
  double turnrate, speed;

  // Control loop
  while(!(checkPoints[0] == 1 && checkPoints[1] == 1 && checkPoints[2] == 1 && checkPoints[3] == 1 && checkPoints[4] == 1))
  {    
      

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

      std::cout << "cp[0]: " << checkPoints[0] << std::endl;
      std::cout << "cp[1]: " << checkPoints[1] << std::endl;
      std::cout << "cp[2]: " << checkPoints[2] << std::endl;
      std::cout << "cp[3]: " << checkPoints[3] << std::endl;
      std::cout << "cp[4]: " << checkPoints[4] << std::endl;
      


	//if in the zone 1 
	if(pp.GetXPos() < 10 && pp.GetYPos() < 5){
		std::cout << "zone 1: going straight " << std::endl;
		//if in the zone 1 A
		if(pp.GetXPos() < 10 && pp.GetYPos() < 2){
			checkPoints[0] = 1;
			if(checkPoints[0] == 1 && checkPoints[1] == 1 && checkPoints[2] == 1 && checkPoints[3] == 1)
			checkPoints[4] = 1;
			//and facing east, go straight ahead
			std::cout << "zone A1: " << std::endl;
			if(pp.GetYaw() < 0.1 && pp.GetYaw() > -0.1){
				
				std::cout << "zone A1: going straight " << std::endl;
				turnrate = 0;   
		    	speed=0.5;

				if(bp[0] || bp[1]){
			    	std::cout << "wall in contact: ";	
					speed=-0.2;
					if (bp[0] && !bp[1]) {  
						turnrate=dtor(-30); 
						std::cout << "1" << std::endl;
						stuck1A++;
					}
					if (!bp[0] && bp[1]) { 
						turnrate=dtor(30); 
						std::cout << "2" << std::endl;
						stuck1A++;			
					}

					if (bp[0] && bp[1]) { stuck1A++; std::cout << "3" << std::endl;
					if(rand()%2 > 0){turnrate=dtor(-30);}
					else {turnrate=dtor(30);}}
						stuck1A++;
					if(stuck1A > 20){
						std::cout << "stuck count max: ";
						if(rand()%2>0)	
						speed = -0.3;
						else
						speed = 0.3;
						if(rand()%2 > 0){turnrate=dtor(-30);}
						else {turnrate=dtor(30);}
					}
				}

				std::cout << "x: " << pp.GetXPos()  << std::endl;
				std::cout << "y: " << pp.GetYPos()  << std::endl;
				std::cout << "a: " << pp.GetYaw()  << std::endl;
			    std::cout << "Speed: " << speed << std::endl;      
			    std::cout << "Turn rate: " << turnrate << std::endl << std::endl;
				std::cout << "set speed" << std::endl;	
	   			pp.SetSpeed(speed, turnrate); 
			    robot.Read();			    
			}else{
				//but facing other than east, turn to east
				if(!(pp.GetYaw() < 0.1 && pp.GetYaw() > -0.1)){
					speed=0.2;
					if(pp.GetYaw() > 0.1)
						turnrate=dtor(-20);
					else
						turnrate=dtor(20);	

					if(bp[0] || bp[1]){
			    	std::cout << "wall in contact: ";	
					speed=-0.2;
					if (bp[0] && !bp[1]) {  
						turnrate=dtor(-30); 
						std::cout << "1" << std::endl;
						stuck1A++;
					}
					if (!bp[0] && bp[1]) { 
						turnrate=dtor(30); 
						std::cout << "2" << std::endl;
						stuck1A++;			
					}

					if (bp[0] && bp[1]) { stuck1A++; std::cout << "3" << std::endl;
					if(rand()%2 > 0){turnrate=dtor(-30);}
					else {turnrate=dtor(30);}}
						stuck1A++;
					if(stuck1A > 20){
						std::cout << "stuck count max: ";
						if(rand()%2>0)	
						speed = -0.3;
						else
						speed = 0.3;
						if(rand()%2 > 0){turnrate=dtor(-30);}
						else {turnrate=dtor(30);}
					}
				}
					pp.SetSpeed(speed, turnrate); 
					std::cout << "zone 1: turning east " << std::endl;
	      			robot.Read();
				}
			}
		}else{
			//if in zone 1 B, turn south
			if(!(pp.GetYaw() < -1.5 && pp.GetYaw() >-1.6)){
				//if hitting an object
			    if(bp[0] || bp[1]){
			    	stuck1B++;
			    	std::cout << "object in contact: stuck count = " << stuck1B << " ";	
					speed=-0.2;
					if (bp[0] && !bp[1]) {  turnrate=dtor(-30); std::cout << "1" << std::endl;
						stuck1B++;
					}
					if (!bp[0] && bp[1]) { turnrate=dtor(30); std::cout << "2" << std::endl;
						stuck1B++;
					}
					if (bp[0] && bp[1]) { stuck1B++; std::cout << "3" << std::endl;
					if(rand()%2 > 0){turnrate=dtor(-30);}
					else {turnrate=dtor(30);}}
					stuck1B++;
					if(stuck1B > 20){
						std::cout << "stuck count max: ";
						if(rand()%2>0)	
						speed = -0.3;
						else
						speed = 0.3;
						if(rand()%2 > 0){turnrate=dtor(-30);}
						else {turnrate=dtor(30);}
					}
				}else{
					speed=0.2;
					turnrate=dtor(30);		 
					std::cout << "zone 1 B: turning south " << std::endl;				
				}
				
	   			pp.SetSpeed(speed, turnrate); 
			    robot.Read();			    
			}
			//and move forward till you reach zone 1 A
			if(!(pp.GetXPos() < 10 && pp.GetYPos() < 2)){
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
		if(pp.GetXPos() > 10 && pp.GetYPos() < 10){
			std::cout << "zone 2" << std::endl;
			
		//if in the zone 2 A
		if(pp.GetXPos() > 7 && pp.GetYPos() < 10){
			if(checkPoints[0] == 1)
				checkPoints[1] = 1;
			//and facing east, go straight ahead
			std::cout << "zone 2A:" << std::endl;
			if(pp.GetYaw() < 1.7 && pp.GetYaw() > 1.4){
				
				std::cout << "zone 2A: going straight " << std::endl;
				turnrate = 0;   
		    	speed=0.5;

				if(bp[0] || bp[1]){
			    	std::cout << "wall in contact: ";	
					speed=-0.2;
					if (bp[0] && !bp[1]) {  
						turnrate=dtor(-30); 
						std::cout << "1" << std::endl;
						stuck2A++;
					}
					if (!bp[0] && bp[1]) { 
						turnrate=dtor(30); 
						std::cout << "2" << std::endl;
						stuck2A++;			
					}

					if (bp[0] && bp[1]) { stuck1A++; std::cout << "3" << std::endl;
					if(rand()%2 > 0){turnrate=dtor(-30);}
					else {turnrate=dtor(30);}}
						stuck2A++;
					if(stuck2A > 20){
						std::cout << "stuck count max: ";
						if(rand()%2>0)	
						speed = -0.3;
						else
						speed = 0.3;
						if(rand()%2 > 0){turnrate=dtor(-30);}
						else {turnrate=dtor(30);}
					}
				}
	
	   			pp.SetSpeed(speed, turnrate); 
			    robot.Read();			    
			}else{
				//but facing other than north, turn to north
				if(!(pp.GetYaw() < 1.7 && pp.GetYaw() > 1.4)){
					speed=0.2;

					if(pp.GetYaw() > 1.7)
						turnrate=dtor(-20);
					else
						turnrate=dtor(20);	

					if(bp[0] || bp[1]){
			    	std::cout << "wall in contact: ";	
					speed=-0.2;
					if (bp[0] && !bp[1]) {  
						turnrate=dtor(-30); 
						std::cout << "1" << std::endl;
						stuck2A++;
					}
					if (!bp[0] && bp[1]) { 
						turnrate=dtor(30); 
						std::cout << "2" << std::endl;
						stuck2A++;			
					}

					if (bp[0] && bp[1]) { stuck2A++; std::cout << "3" << std::endl;
					if(rand()%2 > 0){turnrate=dtor(-30);}
					else {turnrate=dtor(30);}}
						stuck2A++;
					if(stuck2A > 20){
						std::cout << "stuck count max: = " << stuck2A;
						if(rand()%2>0)	
						speed = -0.3;
						else
						speed = 0.3;
						if(rand()%2 > 0){turnrate=dtor(-30);}
						else {turnrate=dtor(30);}
					}
				}
					pp.SetSpeed(speed, turnrate); 
					std::cout << "zone 2: turning north " << std::endl;
	      			robot.Read();
				}
			}
		}else{
			//if in zone 2B and not facing east, turn east
			std::cout << "zone 2 B" << std::endl;
			if(!(pp.GetYaw() < 0.1  && pp.GetYaw() > -0.1)){
				speed=0.2;
				if(pp.GetYaw() > 0.1)
					turnrate=dtor(20);
				else
					turnrate=dtor(-20);		 
				std::cout << "urning east " << std::endl;
				//if hitting an object
			    if(bp[0] || bp[1]){
			    	stuck2B++;
			    	std::cout << "object in contact: stuck count = " << stuck2B << " ";	
					speed=-0.2;
					if (bp[0] && !bp[1]) {  turnrate=dtor(-30); std::cout << "1" << std::endl;
						stuck2B++;
					}
					if (!bp[0] && bp[1]) { turnrate=dtor(30); std::cout << "2" << std::endl;
						stuck2B++;
					}
					if (bp[0] && bp[1]) { stuck1B++; std::cout << "3" << std::endl;
					if(rand()%2 > 0){turnrate=dtor(-30);}
					else {turnrate=dtor(30);}}
					stuck2B++;
					if(stuck2B > 20){
						std::cout << "stuck count max: ";
						if(rand()%2>0)	
						speed = -0.3;
						else
						speed = 0.3;
						if(rand()%2 > 0){turnrate=dtor(-30);}
						else {turnrate=dtor(30);}
						stuck2B = 0;
					}
				}
				
	   			pp.SetSpeed(speed, turnrate); 
			    robot.Read();			    
			}

			//and move forward till you reach zone 2 A
			if(!(pp.GetXPos() > 7 && pp.GetYPos() < 10)){
				turnrate = 0;   
		    	speed=0.2;		    	
			    std::cout << "zone 2B: moving towards zone 2A " << std::endl;
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
			//if in the zone 3	 
			if(pp.GetXPos() > 2 && pp.GetYPos() > 8){
				std::cout << "zone 3:" << std::endl;
				
				//if in the zone 3 A
				if(pp.GetXPos() > 2 && pp.GetYPos() > 10){
					if(checkPoints[0] == 1 && checkPoints[1] == 1)
					checkPoints[2] = 1;
					std::cout << "zone 3A:" << std::endl;
					//and facing west, go straight ahead to the west
					if(pp.GetYaw() < 3.1 && pp.GetYaw() > 3.0){		
						std::cout << "going straight to the west" << std::endl;
						turnrate = 0;   
				    	speed=0.5;

					if(bp[0] || bp[1]){
				    	std::cout << "wall in contact: ";	
						speed=-0.2;
						if (bp[0] && !bp[1]) {  
							turnrate=dtor(-30); 
							stuck3W++;
						}

						if (!bp[0] && bp[1]) { 
							turnrate=dtor(30); 
							stuck3W++;			
						}

						if (bp[0] && bp[1]) {
						 	stuck3W++; 

							if(rand()%2 > 0){
								turnrate=dtor(-30);
							}else {
								turnrate=dtor(30);
							}
						}
							
						if(stuck3W > 20){
							std::cout << "stuck count max: ";
							if(rand()%2>0)	
								speed = -0.3;
							else
								speed = 0.3;
							if(rand()%2 > 0){
								turnrate=dtor(-30);
							}else{
								turnrate=dtor(30);
							}
							stuck3W = 0;
						}
					}

	   			pp.SetSpeed(speed, turnrate); 
			    robot.Read();			    
			}else{
				//but facing other than west, turn west
				if(!(pp.GetYaw() < 3.2 && pp.GetYaw() > 3.0)){
					std::cout << "turning west" << std::endl;
					speed=0.2;
					if(pp.GetYaw() > 3.2 || pp.GetYaw() < 0)
						turnrate=dtor(-20);
					else
						turnrate=dtor(20);	

						if(bp[0] || bp[1]){
				    	std::cout << "wall in contact: ";
				    	std::cout << "stuck count: " << stuck3W;	
						speed=-0.2;
						stuck3W;
						if (bp[0] && !bp[1]) {  
							turnrate=dtor(-30); 
							stuck3W++;
						}

						if (!bp[0] && bp[1]) { 
							turnrate=dtor(30); 
							stuck3W++;			
						}

						if (bp[0] && bp[1]) {
						 	stuck3W++; 

							if(rand()%2 > 0){
								turnrate=dtor(-30);
							}else {
								turnrate=dtor(30);
							}
						}
							
						if(stuck3W > 10){
							std::cout << "stuck count max: ";
							if(rand()%2>0)	
								speed = -0.3;
							else
								speed = 0.3;

							if(rand()%2 > 0){
								turnrate=dtor(-30);
							}else{
								turnrate=dtor(30);
							}
							stuck3W = 0;
						}
					}

					pp.SetSpeed(speed, turnrate); 
	      			robot.Read();
				}
			}
		}else{
			//if in zone 3B, but nor facing north, turn north
			std::cout << "zone 3B" << std::endl;
			if(!(pp.GetYaw() < 1.7 && pp.GetYaw() >1.4)){
				speed=0.2;
				if(pp.GetYaw() > 1.7)
					turnrate=dtor(-20);
				else
					turnrate=dtor(20);

				//if hitting an object
				if(bp[0] || bp[1]){
				    	std::cout << "object in contact: ";	
						speed=-0.2;
						if (bp[0] && !bp[1]) {  
							turnrate=dtor(-30); 
							stuck3O++;
						}

						if (!bp[0] && bp[1]) { 
							turnrate=dtor(30); 
							stuck3O++;			
						}

						if (bp[0] && bp[1]) {
						 	stuck3O++; 

							if(rand()%2 > 0){
								turnrate=dtor(-30);
							}else {
								turnrate=dtor(30);
							}
						}
							
						if(stuck3O > 10){
							std::cout << "stuck count max: ";
							if(rand()%2>0)	
								speed = -0.3;
							else
								speed = 0.3;
							if(rand()%2 > 0){
								turnrate=dtor(-30);
							}else{
								turnrate=dtor(30);
							}
							stuck3O = 0;
						}
					}
				
	   			pp.SetSpeed(speed, turnrate); 
			    robot.Read();			    
			}

			//in zone 3B and facing north, go straight ahead 
			if(!(pp.GetXPos() > 2 && pp.GetYPos() > 10)){
				std::cout << "going back to zone 3A" << std::endl;
					turnrate = 0;   
		    		speed=0.2;		    	
			    
	      	 //if hitting an object
				if(bp[0] || bp[1]){
				    	std::cout << "object in contact: ";	
						speed=-0.2;
						if (bp[0] && !bp[1]) {  
							turnrate=dtor(-30); 
							stuck3O++;
						}

						if (!bp[0] && bp[1]) { 
							turnrate=dtor(30); 
							stuck3O++;			
						}

						if (bp[0] && bp[1]) {
						 	stuck3O++; 

							if(rand()%2 > 0){
								turnrate=dtor(-30);
							}else {
								turnrate=dtor(30);
							}
						}
							
						if(stuck3O > 10){
							std::cout << "stuck count max: ";
							if(rand()%2>0)	
								speed = -0.3;
							else
								speed = 0.3;
							if(rand()%2 > 0){
								turnrate=dtor(-30);
							}else{
								turnrate=dtor(30);
							}
							stuck3O = 0;
						}
					}
				


	   			pp.SetSpeed(speed, turnrate); 
	      		robot.Read();
			}
		}
			}else{
				//if in the zone 4	 			 
				if(pp.GetXPos() < 3 && pp.GetYPos() > 2){
					std::cout << "zone 4:" << std::endl;
					//if in the zone 4A
				if(pp.GetXPos() < 1 || pp.GetYPos() > 10){
					if(checkPoints[0] == 1 && checkPoints[1] == 1 && checkPoints[2] == 1)
						checkPoints[3] = 1;
					std::cout << "zone 4A:" << std::endl;
					//and facing south, keep moving to south
					if(pp.GetYaw() < -1.5 && pp.GetYaw() > -1.6){		
						std::cout << "keep moving to south" << std::endl;
						turnrate = 0;   
				    	speed=0.5;

					if(bp[0] || bp[1]){
				    	std::cout << "wall in contact: ";	
						speed=-0.2;
						if (bp[0] && !bp[1]) {  
							turnrate=dtor(-30); 
							stuck4W++;
						}

						if (!bp[0] && bp[1]) { 
							turnrate=dtor(30); 
							stuck4W++;			
						}

						if (bp[0] && bp[1]) {
						 	stuck4W++; 

							if(rand()%2 > 0){
								turnrate=dtor(-30);
							}else {
								turnrate=dtor(30);
							}
						}
							
						if(stuck4W > 10){
							std::cout << "stuck count max: ";
							if(rand()%2>0)	
								speed = -0.3;
							else
								speed = 0.3;
							if(rand()%2 > 0){
								turnrate=dtor(-30);
							}else{
								turnrate=dtor(30);
							}
							stuck4W = 0;
						}
					}

	   			pp.SetSpeed(speed, turnrate); 
			    robot.Read();			    
			}else{
				//but facing other than south, turn south
				if(!(pp.GetYaw() < -1.5 && pp.GetYaw() > -1.6)){
					std::cout << "turning south" << std::endl;
					speed=0.2;
					if(pp.GetYaw() > -1.5 && pp.GetYaw() < 1.6)
						turnrate=dtor(-20);
					else
						turnrate=dtor(20);	

						if(bp[0] || bp[1]){
				    	std::cout << "wall in contact: ";
				    	std::cout << "stuck count: " << stuck3W;	
						speed=-0.2;
						stuck4W;
						if (bp[0] && !bp[1]) {  
							turnrate=dtor(-30); 
							stuck4W++;
						}

						if (!bp[0] && bp[1]) { 
							turnrate=dtor(30); 
							stuck4W++;			
						}

						if (bp[0] && bp[1]) {
						 	stuck4W++; 

							if(rand()%2 > 0){
								turnrate=dtor(-30);
							}else {
								turnrate=dtor(30);
							}
						}
							
						if(stuck4W > 10){
							std::cout << "stuck count max: ";
							if(rand()%2>0)	
								speed = -0.3;
							else
								speed = 0.3;

							if(rand()%2 > 0){
								turnrate=dtor(-30);
							}else{
								turnrate=dtor(30);
							}
							stuck4W = 0;
						}
					}

					pp.SetSpeed(speed, turnrate); 
	      			robot.Read();
				}
			}
		}else{
			//if in zone 4B, but not facing west, turn west
			std::cout << "zone 4B" << std::endl;

			if(!(pp.GetYaw() < 3.1 && pp.GetYaw() > 3.0)){
				std::cout << "turning west" << std::endl;
				speed=0.2;
				if(pp.GetYaw() > 3.1|| pp.GetYaw() < 0)
					turnrate=dtor(-20);
				else
					turnrate=dtor(20);

				//if hitting an object
				if(bp[0] || bp[1]){
				    	std::cout << "object in contact: ";	
						speed=-0.2;
						if (bp[0] && !bp[1]) {  
							turnrate=dtor(-30); 
							stuck4O++;
						}

						if (!bp[0] && bp[1]) { 
							turnrate=dtor(30); 
							stuck4O++;			
						}

						if (bp[0] && bp[1]) {
						 	stuck4O++; 

							if(rand()%2 > 0){
								turnrate=dtor(-30);
							}else {
								turnrate=dtor(30);
							}
						}
							
						if(stuck4O > 10){
							std::cout << "stuck count max: ";
							if(rand()%2>0)	
								speed = -0.3;
							else
								speed = 0.3;
							if(rand()%2 > 0){
								turnrate=dtor(-30);
							}else{
								turnrate=dtor(30);
							}
							stuck4O = 0;
						}
					}
				
	   			pp.SetSpeed(speed, turnrate); 
			    robot.Read();			    
			}

			//in zone 4B and facing west, go straight ahead 
			if(!(pp.GetXPos() < 1 || pp.GetYPos() > 10)){
				std::cout << "going back to zone 4A" << std::endl;
					turnrate = 0;   
		    		speed=0.2;		    	
			    
	      	 //if hitting an object
				if(bp[0] || bp[1]){
				    	std::cout << "object in contact: ";	
						speed=-0.2;
						if (bp[0] && !bp[1]) {  
							turnrate=dtor(-30); 
							stuck4O++;
						}

						if (!bp[0] && bp[1]) { 
							turnrate=dtor(30); 
							stuck4O++;			
						}

						if (bp[0] && bp[1]) {
						 	stuck4O++; 

							if(rand()%2 > 0){
								turnrate=dtor(-30);
							}else {
								turnrate=dtor(30);
							}
						}
							
						if(stuck4O > 10){
							std::cout << "stuck count max: ";
							if(rand()%2>0)	
								speed = -0.3;
							else
								speed = 0.3;
							if(rand()%2 > 0){
								turnrate=dtor(-30);
							}else{
								turnrate=dtor(30);
							}
							stuck4O = 0;
						}
					}
				
	   			pp.SetSpeed(speed, turnrate); 
	      		robot.Read();
			}
		}
			}
		}
	}
}
      // Send the motion commands that we decided on to the robot.
      pp.SetSpeed(speed, turnrate);  
    }

    speed = 0;
    pp.SetSpeed(speed, turnrate);
}


