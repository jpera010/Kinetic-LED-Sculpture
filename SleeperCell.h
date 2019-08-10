
//      ******************************************************************
//      *                                                                *
//      *                    Sleeper Cell Motion Table                   *
//      *                                                                *
//      ******************************************************************

//
// Sleeper Cell mode uses the distance sensing ultrasonic sensor to allow your Kinetic Sculpture to interact with its audience.
//
// You can program what to do when it senses an audience, how long that audience needs to be present before it begins to 
// interact and, how close that audience needs to be for it to interact.
//
// Your Kinetic Sculpture must interact using disc motion and backlight displays.
//
// Sleeper Cell should run a show for at least 2 minutes while it has an audience before it begins to repeat the show. Of course if the audience leaves, it will stop the show.
//

// name displayed on the LCD when the Sleeper Cell mode is selected
//
#define SLEEPER_CELL_LCD_NAME "SLEEPER CELL"

//
// Sleeper Cell constants.  These constants determine what ultrasonic measurements
// are needed to wake up.  It wakes up when a person is viewing for more than the 
// minimum number of milliseconds.  Once it wakes up, it goes back to sleep after
// a person is nolonger viewing for at least the minimum number of milliseconds.
//
const int MINIMUM_DISTANCE_IN_CM_TO_DETECTED_PERSON_IS_VIEWING = 100;
const int MINIMUM_NUMBER_OF_MILLISECONDS_NEEDED_TO_DETERMINE_PERSON_IS_VIEWING = 1000;     // NOTE: THIS NUMBER MUST BE A MULTIPLE OF 100 (ie, 100, 200, 300...)
const int MINIMUM_NUMBER_OF_MILLISECONDS_TO_DETERMINE_PERSON_IS_NOLONGER_VIEWING = 1000;   // NOTE: THIS NUMBER MUST BE A MULTIPLE OF 100 (ie, 100, 200, 300...)


//
// the backlight color while it is sleeping
//
//const COLOR_ENTRY BACKGROUND_COLOR_WHILE_SLEEPING = WHITE_LOW;
const COLOR_ENTRY BACKGROUND_COLOR_WHILE_SLEEPING = {125,0,255};


//
// Sleeper Cell: playbook table of motions & colors
//
//
// MOTION_ENTRY = {Front_Disk_Velocity_In_RPM, Back_Disk_Velocity_In_RPM, Color, Duration}, 
//
// Where: 
// 
//  Front_Disk_Velocity_In_RPM - is any number from 1 to 30 RPM
//  Back_Disk_Velocity_In_RPM - is any number from 1 to 30 RPM
//  Color - is either a defined color or a actual color defined by mixing {red,green,blue}. 
//      Where:
//             red is some number from 0-255. 0 being no red color, 255 being full red color
//             green is some number from 0-255. 0 being no green color, 255 being full green color
//             blue is some number from 0-255. 0 being no blue color, 255 being full blue color
//
//  Duration - is the amount of time in milliseconds to get from the previous velocities and color to the newly specified velocities and color.
//
MOTION_ENTRY sleeperCellTbl[] PROGMEM = {
  {   0,    0,  WHITE,            2000},    // turn on the back light

  {  -30,   30,  RED,       100},    
  {  -30,   30,  BLUE,             100},    
  {  -30,   30, RED,        100},   
  {  -30,   30,  BLUE,             100},
  {  -30,   30, RED,        100}, 
  {  -30,   30,  BLUE,       100},    
  {  -30,   30,  RED,             100},    
  {  -30,   30, BLUE,        100},   
  {  -30,   30,  RED,             100},
  {  -30,   30, BLUE,        100},
  
  {  -30,   30,  RED,       100},    
  {  -30,   30,  BLUE,             100},    
  {  -30,   30, RED,        100},   
  {  -30,   30,  BLUE,             100},
  {  -30,   30, RED,        100}, 
  {  -30,   30,  BLUE,       100},    
  {  -30,   30,  RED,             100},    
  {  -30,   30, BLUE,        100},   
  {  -30,   30,  RED,             100},
  {  -30,   30, BLUE,        100},
  
  {  -30,   30,  RED,       100},    
  {  -30,   30,  BLUE,             100},    
  {  -30,   30, RED,        100},   
  {  -30,   30,  BLUE,             100},
  {  -30,   30, RED,        100}, 
  {  -30,   30,  BLUE,       100},    
  {  -30,   30,  RED,             100},    
  {  -30,   30, BLUE,        100},   
  {  -30,   30,  RED,             100},
  {  -30,   30, BLUE,        100},
  
  {  -30,   30,  RED,       100},    
  {  -30,   30,  BLUE,             100},    
  {  -30,   30, RED,        100},   
  {  -30,   30,  BLUE,             100},
  {  -30,   30, RED,        100}, 
  {  -30,   30,  BLUE,       100},    
  {  -30,   30,  RED,             100},    
  {  -30,   30, BLUE,        100},   
  {  -30,   30,  RED,             100},
  {  -30,   30, BLUE,        100},
  
  {  -30,   30,  RED,       100},    
  {  -30,   30,  BLUE,             100},    
  {  -30,   30, RED,        100},   
  {  -30,   30,  BLUE,             100},
  {  -30,   30, RED,        100}, 
  {  -30,   30,  BLUE,       100},    
  {  -30,   30,  RED,             100},    
  {  -30,   30, BLUE,        100},   
  {  -30,   30,  RED,             100},
  {  -30,   30, BLUE,        100},
  
  {  -30,   30,  RED,       100},    
  {  -30,   30,  BLUE,             100},    
  {  -30,   30, RED,        100},   
  {  -30,   30,  BLUE,             100},
  {  -30,   30, RED,        100}, 
  {  -30,   30,  BLUE,       100},    
  {  -30,   30,  RED,             100},    
  {  -30,   30, BLUE,        100},   
  {  -30,   30,  RED,             100},
  {  -30,   30, BLUE,        100},
  
  {  -30,   30,  RED,       100},    
  {  -30,   30,  BLUE,             100},    
  {  -30,   30, RED,        100},   
  {  -30,   30,  BLUE,             100},
  {  -30,   30, RED,        100}, 
  {  -30,   30,  BLUE,       100},    
  {  -30,   30,  RED,             100},    
  {  -30,   30, BLUE,        100},   
  {  -30,   30,  RED,             100},
  {  -30,   30, BLUE,        100},
  
  {  -30,   30,  RED,       100},    
  {  -30,   30,  BLUE,             100},    
  {  -30,   30, RED,        100},   
  {  -30,   30,  BLUE,             100},
  {  -30,   30, RED,        100}, 
  {  -30,   30,  BLUE,       100},    
  {  -30,   30,  RED,             100},    
  {  -30,   30, BLUE,        100},   
  {  -30,   30,  RED,             100},
  {  -30,   30, BLUE,        100},
  
  {  -30,   30,  RED,       100},    
  {  -30,   30,  BLUE,             100},    
  {  -30,   30, RED,        100},   
  {  -30,   30,  BLUE,             100},
  {  -30,   30, RED,        100}, 
  {  -30,   30,  BLUE,       100},    
  {  -30,   30,  RED,             100},    
  {  -30,   30, BLUE,        100},   
  {  -30,   30,  RED,             100},
  {  -30,   30, BLUE,        100},
  
  {  -30,   30,  RED,       100},    
  {  -30,   30,  BLUE,             100},    
  {  -30,   30, RED,        100},   
  {  -30,   30,  BLUE,             100},
  {  -30,   30, RED,        100}, 
  {  -30,   30,  BLUE,       100},    
  {  -30,   30,  RED,             100},    
  {  -30,   30, BLUE,        100},   
  {  -30,   30,  RED,             100},
  {  -30,   30, BLUE,        100},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {   30,   -30,  GREEN,       250},    
  {  30,   -30,  BLUE,             250},    
  {  30,   -30, GREEN,        250},   
  {  30,   -30,  BLUE,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  VIOLET,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  VIOLET,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  VIOLET,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  VIOLET,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  VIOLET,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  VIOLET,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  VIOLET,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  VIOLET,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  VIOLET,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  VIOLET,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  VIOLET,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  VIOLET,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  VIOLET,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  VIOLET,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  VIOLET,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  VIOLET,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  VIOLET,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  VIOLET,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  VIOLET,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  VIOLET,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  VIOLET,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  VIOLET,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  VIOLET,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  VIOLET,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  VIOLET,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  VIOLET,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  VIOLET,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  VIOLET,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  VIOLET,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  VIOLET,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  VIOLET,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  VIOLET,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  LIGHT_PURPLE,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  LIGHT_PURPLE,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  LIGHT_PURPLE,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  LIGHT_PURPLE,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  LIGHT_PURPLE,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  LIGHT_PURPLE,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  LIGHT_PURPLE,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  LIGHT_PURPLE,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  LIGHT_PURPLE,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  LIGHT_PURPLE,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  LIGHT_PURPLE,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  LIGHT_PURPLE,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  LIGHT_PURPLE,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  LIGHT_PURPLE,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  LIGHT_PURPLE,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  LIGHT_PURPLE,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  LIGHT_PURPLE,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  LIGHT_PURPLE,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  LIGHT_PURPLE,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  LIGHT_PURPLE,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  LIGHT_PURPLE,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  LIGHT_PURPLE,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  LIGHT_PURPLE,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  LIGHT_PURPLE,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  LIGHT_PURPLE,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  LIGHT_PURPLE,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  LIGHT_PURPLE,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  LIGHT_PURPLE,             250},
  
  {  25,   -30,  VIOLET,       250},    
  {  25,   -30,  LIGHT_PURPLE,             250},    
  {  25,   -30, VIOLET,        250},   
  {  25,   -30,  LIGHT_PURPLE,             250},
  
  {   -25,    30,  TURQUOISE,        250},
  {   -25,    30,  CYAN,             250},
  {   -25,    30,  ICE,              250}, 
  {   -25,    30,  LIGHT_BLUE,       250},
  {   -25,    30,  OCEAN_BLUE,       250},
  {   -25,    30,  BLUE,             250},
  
  {   -25,    30,  TURQUOISE,        250},
  {   -25,    30,  CYAN,             250},
  {   -25,    30,  ICE,              250}, 
  {   -25,    30,  LIGHT_BLUE,       250},
  {   -25,    30,  OCEAN_BLUE,       250},
  {   -25,    30,  BLUE,             250},
  
  {   -25,    30,  TURQUOISE,        250},
  {   -25,    30,  CYAN,             250},
  {   -25,    30,  ICE,              250}, 
  {   -25,    30,  LIGHT_BLUE,       250},
  {   -25,    30,  OCEAN_BLUE,       250},
  {   -25,    30,  BLUE,             250},
  
  {   -25,    30,  TURQUOISE,        250},
  {   -25,    30,  CYAN,             250},
  {   -25,    30,  ICE,              250}, 
  {   -25,    30,  LIGHT_BLUE,       250},
  {   -25,    30,  OCEAN_BLUE,       250},
  {   -25,    30,  BLUE,             250},
  
  {   -25,    30,  TURQUOISE,        250},
  {   -25,    30,  CYAN,             250},
  {   -25,    30,  ICE,              250}, 
  {   -25,    30,  LIGHT_BLUE,       250},
  {   -25,    30,  OCEAN_BLUE,       250},
  {   -25,    30,  BLUE,             250},
  
  {   -25,    30,  TURQUOISE,        250},
  {   -25,    30,  CYAN,             250},
  {   -25,    30,  ICE,              250}, 
  {   -25,    30,  LIGHT_BLUE,       250},
  {   -25,    30,  OCEAN_BLUE,       250},
  {   -25,    30,  BLUE,             250},
  
  {   -25,    30,  TURQUOISE,        250},
  {   -25,    30,  CYAN,             250},
  {   -25,    30,  ICE,              250}, 
  {   -25,    30,  LIGHT_BLUE,       250},
  {   -25,    30,  OCEAN_BLUE,       250},
  {   -25,    30,  BLUE,             250},
  
  {   30,    29,  RED,        250},
  {   30,    29,  ORANGE,             250},
  {   30,    29,  YELLOW,              250}, 
  {   30,    29,  RED,       250},
  {   30,    29,  ORANGE,       250},
  {   30,    29,  YELLOW,             250},
  
  {   30,    -30,  RED,        250},
  {   30,    -30,  ORANGE,             250},
  {   30,    -30,  YELLOW,              250}, 
  {   30,    -30,  RED,       250},
  {   30,    -30,  ORANGE,       250},
  {   30,    -30,  YELLOW,             250},
  
  {   -30,    29,  RED,        250},
  {   -30,    29,  ORANGE,             250},
  {   -30,    29,  YELLOW,              250}, 
  {   -30,    29,  RED,       250},
  {   -30,    29,  ORANGE,       250},
  {   -30,    29,  YELLOW,             250},
  
    {   30,    29,  RED,        250},
  {   30,    29,  ORANGE,             250},
  {   30,    29,  YELLOW,              250}, 
  {   30,    29,  RED,       250},
  {   30,    29,  ORANGE,       250},
  {   30,    29,  YELLOW,             250},
  
  {   30,    -30,  RED,        250},
  {   30,    -30,  ORANGE,             250},
  {   30,    -30,  YELLOW,              250}, 
  {   30,    -30,  RED,       250},
  {   30,    -30,  ORANGE,       250},
  {   30,    -30,  YELLOW,             250},
  
  {   -30,    29,  RED,        250},
  {   -30,    29,  ORANGE,             250},
  {   -30,    29,  YELLOW,              250}, 
  {   -30,    29,  RED,       250},
  {   -30,    29,  ORANGE,       250},
  {   -30,    29,  YELLOW,             250},
  
  
    {   30,    29,  RED,        250},
  {   30,    29,  ORANGE,             250},
  {   30,    29,  YELLOW,              250}, 
  {   30,    29,  RED,       250},
  {   30,    29,  ORANGE,       250},
  {   30,    29,  YELLOW,             250},
  
  {   30,    -30,  RED,        250},
  {   30,    -30,  ORANGE,             250},
  {   30,    -30,  YELLOW,              250}, 
  {   30,    -30,  RED,       250},
  {   30,    -30,  ORANGE,       250},
  {   30,    -30,  YELLOW,             250},
  
  {   -30,    29,  RED,        250},
  {   -30,    29,  ORANGE,             250},
  {   -30,    29,  YELLOW,              250}, 
  {   -30,    29,  RED,       250},
  {   -30,    29,  ORANGE,       250},
  {   -30,    29,  YELLOW,             250},
  
    {   30,    29,  RED,        250},
  {   30,    29,  ORANGE,             250},
  {   30,    29,  YELLOW,              250}, 
  {   30,    29,  RED,       250},
  {   30,    29,  ORANGE,       250},
  {   30,    29,  YELLOW,             250},
  
  {   30,    -30,  RED,        250},
  {   30,    -30,  ORANGE,             250},
  {   30,    -30,  YELLOW,              250}, 
  {   30,    -30,  RED,       250},
  {   30,    -30,  ORANGE,       250},
  {   30,    -30,  YELLOW,             250},
  
  {   -30,    29,  BLUE,        250},
  {   -30,    29,  LIGHT_BLUE,             250},
  {   -30,    29,  CYAN,              250}, 
  {   -30,    29,  BLUE,       250},
  {   -30,    29,  LIGHT_BLUE,       250},
  {   -30,    29,  CYAN,             250},
  
  {   -30,    29,  BLUE,        250},
  {   -30,    29,  LIGHT_BLUE,             250},
  {   -30,    29,  CYAN,              250}, 
  {   -30,    29,  BLUE,       250},
  {   -30,    29,  LIGHT_BLUE,       250},
  {   -30,    29,  CYAN,             250},
  
  {   -30,    29,  BLUE,        250},
  {   -30,    29,  LIGHT_BLUE,             250},
  {   -30,    29,  CYAN,              250}, 
  {   -30,    29,  BLUE,       250},
  {   -30,    29,  LIGHT_BLUE,       250},
  {   -30,    29,  CYAN,             250},
  
  {   -30,    29,  BLUE,        250},
  {   -30,    29,  LIGHT_BLUE,             250},
  {   -30,    29,  CYAN,              250}, 
  {   -30,    29,  BLUE,       250},
  {   -30,    29,  LIGHT_BLUE,       250},
  {   -30,    29,  CYAN,             250},
  
  {   -30,    29,  BLUE,        250},
  {   -30,    29,  LIGHT_BLUE,             250},
  {   -30,    29,  CYAN,              250}, 
  {   -30,    29,  BLUE,       250},
  {   -30,    29,  LIGHT_BLUE,       250},
  {   -30,    29,  CYAN,             250},
  
  {   -30,    29,  BLUE,        250},
  {   -30,    29,  LIGHT_BLUE,             250},
  {   -30,    29,  CYAN,              250}, 
  {   -30,    29,  BLUE,       250},
  {   -30,    29,  LIGHT_BLUE,       250},
  {   -30,    29,  CYAN,             250},
  
  {   -30,    29,  BLUE,        250},
  {   -30,    29,  LIGHT_BLUE,             250},
  {   -30,    29,  CYAN,              250}, 
  {   -30,    29,  BLUE,       250},
  {   -30,    29,  LIGHT_BLUE,       250},
  {   -30,    29,  CYAN,             250},
  
  {   -30,    29,  BLUE,        250},
  {   -30,    29,  LIGHT_BLUE,             250},
  {   -30,    29,  CYAN,              250}, 
  {   -30,    29,  BLUE,       250},
  {   -30,    29,  LIGHT_BLUE,       250},
  {   -30,    29,  CYAN,             250},
  
  {   -30,    29,  BLUE,        250},
  {   -30,    29,  LIGHT_BLUE,             250},
  {   -30,    29,  CYAN,              250}, 
  {   -30,    29,  BLUE,       250},
  {   -30,    29,  LIGHT_BLUE,       250},
  {   -30,    29,  CYAN,             250},
  
  {   -30,    29,  BLUE,        250},
  {   -30,    29,  LIGHT_BLUE,             250},
  {   -30,    29,  CYAN,              250}, 
  {   -30,    29,  BLUE,       250},
  {   -30,    29,  LIGHT_BLUE,       250},
  {   -30,    29,  CYAN,             250},
  
  {   30,    29,  VIOLET,        250},
  {   30,    29,  MAGENTA,             250},
  {   30,    29,  RASPBERRY,              250}, 
  {   30,    29,  VIOLET,       250},
  {   30,    29,  MAGENTA,       250},
  {   30,    29,  RASPBERRY,             250},
  
  {   30,    29,  VIOLET,        250},
  {   30,    29,  MAGENTA,             250},
  {   30,    29,  RASPBERRY,              250}, 
  {   30,    29,  VIOLET,       250},
  {   30,    29,  MAGENTA,       250},
  {   30,    29,  RASPBERRY,             250},
  
  {   30,    29,  VIOLET,        250},
  {   30,    29,  MAGENTA,             250},
  {   30,    29,  RASPBERRY,              250}, 
  {   30,    29,  VIOLET,       250},
  {   30,    29,  MAGENTA,       250},
  {   30,    29,  RASPBERRY,             250},
  
  {   30,    29,  VIOLET,        250},
  {   30,    29,  MAGENTA,             250},
  {   30,    29,  RASPBERRY,              250}, 
  {   30,    29,  VIOLET,       250},
  {   30,    29,  MAGENTA,       250},
  {   30,    29,  RASPBERRY,             250},
  
  {   30,    29,  VIOLET,        250},
  {   30,    29,  MAGENTA,             250},
  {   30,    29,  RASPBERRY,              250}, 
  {   30,    29,  VIOLET,       250},
  {   30,    29,  MAGENTA,       250},
  {   30,    29,  RASPBERRY,             250},
  
  {   30,    29,  VIOLET,        250},
  {   30,    29,  MAGENTA,             250},
  {   30,    29,  RASPBERRY,              250}, 
  {   30,    29,  VIOLET,       250},
  {   30,    29,  MAGENTA,       250},
  {   30,    29,  RASPBERRY,             250},
  
  {   30,    29,  VIOLET,        250},
  {   30,    29,  MAGENTA,             250},
  {   30,    29,  RASPBERRY,              250}, 
  {   30,    29,  VIOLET,       250},
  {   30,    29,  MAGENTA,       250},
  {   30,    29,  RASPBERRY,             250},
  
  {   30,    29,  VIOLET,        250},
  {   30,    29,  MAGENTA,             250},
  {   30,    29,  RASPBERRY,              250}, 
  {   30,    29,  VIOLET,       250},
  {   30,    29,  MAGENTA,       250},
  {   30,    29,  RASPBERRY,             250},
  
  {   30,    29,  VIOLET,        250},
  {   30,    29,  MAGENTA,             250},
  {   30,    29,  RASPBERRY,              250}, 
  {   30,    29,  VIOLET,       250},
  {   30,    29,  MAGENTA,       250},
  {   30,    29,  RASPBERRY,             250},
  
  {   30,    29,  VIOLET,        250},
  {   30,    29,  MAGENTA,             250},
  {   30,    29,  RASPBERRY,              250}, 
  {   30,    29,  VIOLET,       250},
  {   30,    29,  MAGENTA,       250},
  {   30,    29,  RASPBERRY,             250},
  
  {   30,    -30,  WHITE_FULL,        250},
  {   30,    -30,  BLACK,             250},
  {   30,    -30,  WHITE_FULL,              250}, 
  {   30,    -30,  BLACK,       250},
  {   30,    -30,  WHITE_FULL,       250},
  {   30,    -30,  BLACK,             250},
  
  {   30,    -30,  WHITE_FULL,        250},
  {   30,    -30,  BLACK,             250},
  {   30,    -30,  WHITE_FULL,              250}, 
  {   30,    -30,  BLACK,       250},
  {   30,    -30,  WHITE_FULL,       250},
  {   30,    -30,  BLACK,             250},
  
  {   30,    -30,  WHITE_FULL,        250},
  {   30,    -30,  BLACK,             250},
  {   30,    -30,  WHITE_FULL,              250}, 
  {   30,    -30,  BLACK,       250},
  {   30,    -30,  WHITE_FULL,       250},
  {   30,    -30,  BLACK,             250},
  
  {   30,    -30,  WHITE_FULL,        250},
  {   30,    -30,  BLACK,             250},
  {   30,    -30,  WHITE_FULL,              250}, 
  {   30,    -30,  BLACK,       250},
  {   30,    -30,  WHITE_FULL,       250},
  {   30,    -30,  BLACK,             250},
  
  {   30,    -30,  WHITE_FULL,        250},
  {   30,    -30,  BLACK,             250},
  {   30,    -30,  WHITE_FULL,              250}, 
  {   30,    -30,  BLACK,       250},
  {   30,    -30,  WHITE_FULL,       250},
  {   30,    -30,  BLACK,             250},
  
  {   30,    -30,  WHITE_FULL,        250},
  {   30,    -30,  BLACK,             250},
  {   30,    -30,  WHITE_FULL,              250}, 
  {   30,    -30,  BLACK,       250},
  {   30,    -30,  WHITE_FULL,       250},
  {   30,    -30,  BLACK,             250},
  
  {   30,    -30,  WHITE_FULL,        250},
  {   30,    -30,  BLACK,             250},
  {   30,    -30,  WHITE_FULL,              250}, 
  {   30,    -30,  BLACK,       250},
  {   30,    -30,  WHITE_FULL,       250},
  {   30,    -30,  BLACK,             250},
  
  {   30,    -30,  WHITE_FULL,        250},
  {   30,    -30,  BLACK,             250},
  {   30,    -30,  WHITE_FULL,              250}, 
  {   30,    -30,  BLACK,       250},
  {   30,    -30,  WHITE_FULL,       250},
  {   30,    -30,  BLACK,             250},
  
  {   30,    -30,  WHITE_FULL,        250},
  {   30,    -30,  BLACK,             250},
  {   30,    -30,  WHITE_FULL,              250}, 
  {   30,    -30,  BLACK,       250},
  {   30,    -30,  WHITE_FULL,       250},
  {   30,    -30,  BLACK,             250},
  
    
  {   0,    0,  BLACK,            4000}     // delay while stopped
};

const int sleeperCellTblLength = sizeof(sleeperCellTbl) / sizeof(MOTION_ENTRY);



