/**
 * local-roomba.cc
 * 
 * Sample code for a roomba-like robot that has two front bumpers and
 * magically knows where it is. 
 *
 * Written by: Simon Parsons
 * Date:       24th October 2011
 *  
 **/


#include <iostream>
#include <cmath>
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;  

/**
 * Function headers
 *
 **/

player_pose2d_t readPosition(LocalizeProxy& lp);
void printRobotData(BumperProxy& bp, player_pose2d_t pose);

/**
 * main()
 *
 **/

int main(int argc, char *argv[])
{  


  // Variables
  int counter = 0;
  double speed;            // How fast do we want the robot to go forwards?
  double turnrate;         // How fast do we want the robot to turn?
  player_pose2d_t  pose;   // For handling localization data
  double x_des = 5;
  double y_des = -3.5;
  bool arrived = false;

  // Set up proxies. These are the names we will use to connect to 
  // the interface to the robot.
  PlayerClient    robot("localhost");  
  BumperProxy     bp(&robot,0);  
  Position2dProxy pp(&robot,0);
  LocalizeProxy   lp (&robot, 0);

  // Allow the program to take charge of the motors (take care now)
  pp.SetMotorEnable(true); 

  // Main control loop
  while(!arrived) 
    {    
      // Update information from the robot.
      robot.Read();
      // Read new information about position
      pose = readPosition(lp);

      double y = y_des-pose.py;
      double x = x_des-pose.px;

      double theta = atan2(y, x);

      std::cout << "theta: " << theta << std::endl; 
      std::cout << "x: " << x << std::endl;
      std::cout << "y: " << y << std::endl;
      std::cout << "pose.px: " << pose.px << std::endl;
      std::cout << "pose.py: " << pose.py << std::endl;

      if(x < 0.1 && y < 0.1){
      	arrived = true;
      	std::cout << "x: " << x << std::endl;
      	std::cout << "y: " << y << std::endl;
      	std::cout << "arrived!" << std::endl; 
      }
      else{
      		if(abs(x) > 1 || abs(y) > 1)
      			speed = 0.8;
      		else
      			speed = 0.3;
      }

      if(pose.pa < theta){
      	 turnrate = 0.3;
      }
      else{
      	 turnrate = -0.3;
      }





      // This part of the code should be very familiar by now.
      //
      // If either bumper is pressed, stop. Otherwise just go forwards
      if(bp[0] || bp[1]){
		speed= 0;
      } 
       
       // Print data on the robot to the terminal
      printRobotData(bp, pose);

      // What are we doing?
      std::cout << "Speed: " << speed << std::endl;      
      std::cout << "Turn rate: " << turnrate << std::endl << std::endl;

      // Send the commands to the robot
      pp.SetSpeed(speed, turnrate);  
      // Count how many times we do this
      counter++;
    }
  
} // end of main()


/**
 * readPosition()
 *
 * Read the position of the robot from the localization proxy. 
 *
 * The localization proxy gives us a hypothesis, and from that we extract
 * the mean, which is a pose. 
 *
 **/

player_pose2d_t readPosition(LocalizeProxy& lp)
{

  player_localize_hypoth_t hypothesis;
  player_pose2d_t          pose;
  uint32_t                 hCount;

  // Need some messing around to avoid a crash when the proxy is
  // starting up.

  hCount = lp.GetHypothCount();

  if(hCount > 0){
    hypothesis = lp.GetHypoth(0);
    pose       = hypothesis.mean;
  }

  return pose;
} // End of readPosition()


/**
 *  printRobotData
 *
 * Print out data on the state of the bumpers and the current location
 * of the robot.
 *
 **/

void printRobotData(BumperProxy& bp, player_pose2d_t pose)
{

  // Print out what the bumpers tell us:
  std::cout << "Left  bumper: " << bp[0] << std::endl;
  std::cout << "Right bumper: " << bp[1] << std::endl;

  // Print out where we are
  std::cout << "We are at" << std::endl;
  std::cout << "X: " << pose.px << std::endl;
  std::cout << "Y: " << pose.py << std::endl;
  std::cout << "A: " << pose.pa << std::endl;

  
} // End of printRobotData()
