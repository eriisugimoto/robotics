/**
 * real-local.cc
 * 
 * Sample code for a robot that has two front bumpers and a laser scanner,
 * and. Suitable for use with the roomba.
 *
 * This version is intended to run with the AMCL localization proxy,
 * which provides multiple hypotheses.
 *
 * Written by: Simon Parsons
 * Date:       22nd November 2009
 *  
 **/


#include <iostream>
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;  

/**
 * Function headers
 *
 **/

player_pose2d_t readPosition(LocalizeProxy& lp);
void printLaserData(LaserProxy& sp);
void printRobotData(BumperProxy& bp, player_pose2d_t pose);
bool stop = false;

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
  player_laser_data laser; // For handling laser data
  int stuck_count = 0;
  int turn_count = 0;
  bool turning = false;
  double dest = -6;
  bool spinning = false;
  int spin_count = 0;
  double prev_abs_dist_x = 0;
  double prev_abs_dist_y = 0;
  bool stuck = false;
  int init_spin_count = 0;
  double init_x = 0;
  double init_y = 0;

  // Set up proxies. These are the names we will use to connect to 
  // the interface to the robot.
  PlayerClient    robot("localhost");  
  BumperProxy     bp(&robot,0);  
  Position2dProxy pp(&robot,0);
  LocalizeProxy   lp (&robot, 0);
  LaserProxy      sp (&robot, 0);

  // Allow the program to take charge of the motors (take care now)
  pp.SetMotorEnable(true);

  // Main control loop
  while(true) 
    {        
      // Update information from the robot.
      robot.Read();
      // Read new information about position
      pose = readPosition(lp);
      // Print information about the laser. Check the counter first to stop
      // problems on startup
      if(counter > 2){
	       //printLaserData(sp);
      }

      // Print data on the robot to the terminal
      printRobotData(bp, pose);

      if(init_spin_count < 60){
            speed= 0.05;
            turnrate=dtor(80);
            init_spin_count++;

      }else{
        std::cout << "amcl failed: "<< std::endl;
        return 0;
      
      // If either bumper is pressed, stop. Otherwise just go forwards

      if(!turning && !spinning){
        std::cout << "!turning: "<< std::endl; 
        if(bp[0] || bp[1]){
           std::cout << "bump: "<< std::endl;
  	       speed= -1.5;
           turnrate=dtor(30);
  	       
           turning = true;
           turn_count++;          
        } 
        else {
          std::cout << "!bump: "<< std::endl;

          if(abs(dest-pose.px) < 0.8 && abs(dest-pose.py) < 0.8){
    	      speed= 0.5;
          }else
            speed = 1.5;

            turnrate=0;

            spin_count++;
            std::cout << "spin_count: " << spin_count << std::endl;
          if(spin_count > 120)
            spinning = true;

        } 
      }else{
        if(spinning){
          std::cout << "spinning: " << std::endl; 
          if(spin_count > 0){
            speed= 0.1;
            turnrate=dtor(80);
          }else
            spinning = false;

            spin_count-=-2;
        }else{
          std::cout << "turning: " << std::endl; 
          if(turn_count > 10){
            turning = false;
            turn_count = 0;
          }
            turn_count++;
        }
      }   
      } 

      std::cout << "abs(prev_abs_dist_x - abs(dest-pose.px)): " << abs(prev_abs_dist_x - abs(dest-pose.px)) << std::endl;
      std::cout << "abs(prev_abs_dist_y - abs(dest-pose.py)): " << abs(prev_abs_dist_y - abs(dest-pose.py)) << std::endl;
      if(abs(prev_abs_dist_x - abs(dest-pose.px)) < 0.0001 
        && abs(prev_abs_dist_y - abs(dest-pose.py)) < 0.0001) {
        stuck_count++;
        std::cout << "stuck_count: " << stuck_count << std::endl;
      }else{
        stuck_count = 0;
      }

      if(stuck_count > 10){
        spin_count = 0;
        std::cout << "stuck_count > 60: " << std::endl;
        if(rand() % 2 >0)  
          speed = -0.5;
        else
          speed = 0.5;

        if(rand() % 2 > 0)
          {turnrate=dtor(-60);}
              else {turnrate=dtor(60);}

              stuck_count = 0;

      }


     

      // What are we doing?
      std::cout << "Speed: " << speed << std::endl;      
      std::cout << "Turn rate: " << turnrate << std::endl << std::endl;


      // Send the commands to the robot
      pp.SetSpeed(speed, turnrate);  
      // Count how many times we do this
      counter++;

      std::cout << "(dest-pose.px): " << (dest-pose.px) << std::endl;
      std::cout << "(dest-pose.py): " << (dest-pose.py) << std::endl;
      if(stop && abs(dest-pose.px) < 0.5 && abs(dest-pose.py) < 0.5 && counter > 5){
        std::cout << "SUCCESS: counter = " << counter << std::endl;

        return 0;
      }

      prev_abs_dist_x = abs(dest-pose.px);
      prev_abs_dist_y = abs(dest-pose.py);

    }
  
} // end of main()


/**
 * readPosition()
 *
 * Read the position of the robot from the localization proxy. 
 *
 * The localization proxy gives us a set of "hypotheses", each of
 * which is a number of possible locations for the robot, and from
 * each we extract the mean, which is a pose.
 *
 * As the number of hypotheses drops, the robot should be more sure
 * of where it is.
 *
 **/

player_pose2d_t readPosition(LocalizeProxy& lp)
{

  player_localize_hypoth_t hypothesis;
  player_pose2d_t          pose;
  uint32_t                 hCount;
  double                   weight;
  player_pose2d_t          pose_best;
  double                   weight_best = -1;

  // Need some messing around to avoid a crash when the proxy is
  // starting up.

  hCount = lp.GetHypothCount();

  std::cout << "AMCL gives us " << hCount + 1 
            << " possible locations:" << std::endl;

  if(hCount > 0){
    for(int i = 0; i <= hCount; i++){
      hypothesis = lp.GetHypoth(i);
      pose       = hypothesis.mean;
      weight     = hypothesis.alpha;
      std::cout << "X: " << pose.px << "\t";
      std::cout << "Y: " << pose.py << "\t";
      std::cout << "A: " << pose.pa << "\t";
      std::cout << "W: " << weight  << std::endl;

      if(weight > weight_best){
        weight_best = weight;
        pose_best = pose;
      }
    }
    stop = false;
  }

  if(hCount < 2){
    std::cout << "I am at : "  << "\t";
    std::cout << "X: " << pose_best.px << "\t";
    std::cout << "Y: " << pose_best.py << "\t";
    std::cout << "A: " << pose_best.pa << "\t";
    std::cout << "I am " << weight_best*100 << " % sure of my position" << std::endl;
    stop = true;
    return pose_best;
  }

  // This just returns the mean of the last hypothesis, it isn't necessarily
  // the right one.

  return pose;
} // End of readPosition()

void printLaserData(LaserProxy& sp)
{

  double maxRange, minLeft, minRight, range, bearing;
  int points;

  maxRange  = sp.GetMaxRange();
  minLeft   = sp.MinLeft();
  minRight  = sp.MinRight();
  range     = sp.GetRange(5);
  bearing   = sp.GetBearing(5);
  points    = sp.GetCount();

  //Print out useful laser data
  std::cout << "Laser says..." << std::endl;
  std::cout << "Maximum distance I can see: " << maxRange << std::endl;
  std::cout << "Number of readings I return: " << points << std::endl;
  std::cout << "Closest thing on left: " << minLeft << std::endl;
  std::cout << "Closest thing on right: " << minRight << std::endl;
  std::cout << "Range of a single point: " << range << std::endl;
  std::cout << "Bearing of a single point: " << bearing << std::endl;

  return;
} // End of printLaserData()

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
  // Can also print the bumpers with:
  //std::cout << bp << std::endl;

  // Print out where we are
  std::cout << "We are at" << std::endl;
  std::cout << "X: " << pose.px << std::endl;
  std::cout << "Y: " << pose.py << std::endl;
  std::cout << "A: " << pose.pa << std::endl;

  
} // End of printRobotData()
