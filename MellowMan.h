
//      ******************************************************************
//      *                                                                *
//      *                     Mellow Man Motion Table                    *
//      *                                                                *
//      ******************************************************************

//
// Mellow Man mode is intended to be a non-interactive – background type of kinetic sculpture display.
// It neither demands attention from its audience nor does it bore its audience with mundane overly repetitive displays. 
//
// You need to program a unique disc motion and light display for your sculpture to perform while in this mode. 
//
// Mellow Man should be at least 5 minutes long before it begins repeating.
//

//
// name displayed on the LCD when the Mellow Man mode is selected
//
#define MELLOW_MAN_LCD_NAME "MELLOW MAN"


//
// Mellow Man: playbook table of motions & colors
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

MOTION_ENTRY mellowManTbl[] PROGMEM = {
  {   -8,   -5,  BLACK,            5000},  
  
  {   8,   5,  BLUE,               10000}, 
  {   -8,   5,  ORANGE,            10000}, 
  {   5,   -10,  RED,               10000},
  {   5,   -5,  BLUE,              10000},
  {   -10,   10,  GREEN,             10000},
  {   8,   10,  YELLOW,            10000},
  
  {   -8,   -5,  BLACK,            2000}, 
  
  {   10,   8,  GREEN,               10000}, 
  {   10,   -10,  MAGENTA,            10000}, 
  {   -5,   5,  ORANGE,               10000}, 
  {   -10,   5,  ORANGE,              10000},
  {   5,   -8,  MAGENTA,             10000},
  {   5,   8,   GREEN,            10000},
  
  {   -8,   -5,  BLACK,            2000}, 
  
  {   8,   5,  VIOLET,               10000}, 
  {   -8,   5,  RED,                 10000}, 
  {   5,   -10,  LIGHT_GREEN,        10000},
  {   5,   -5,  LIGHT_GREEN,        10000},
  {   -10,   10,  RED,             10000},
  {   8,   10,  VIOLET,            10000},
  
   {   -8,   -5,  BLACK,            2000}, 
  
  {   10,   8,  BLUE,               10000}, 
  {   10,   -10,  MAGENTA,            10000}, 
  {   -5,   5,  YELLOW,               10000}, 
  {   -10,   5,  OCEAN_BLUE,              10000},
  {   5,   -8,  VIOLET,             10000},
  {   5,   8,   ORANGE,            10000},
  
  {   -8,   -5,  BLACK,            2000}, 
  
  {   10,   8,  YELLOW,               10000}, 
  {   -10,   -8,  VIOLET,                 10000}, 
  {   9,   -10,  WHITE_FULL,        10000},
  {   7,   -5,  WHITE,        10000},
  {   -10,   10,  WHITE_MEDIUM,             10000},
  {   8,   10,  WHITE_LOW,            10000},
  
  

  
  
    
   
  
  
  
  
  
  
 
};

const int MellowManTblLength = sizeof(mellowManTbl) / sizeof(MOTION_ENTRY);

