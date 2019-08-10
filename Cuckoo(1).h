
//      ******************************************************************
//      *                                                                *
//      *                       Cuckoo Motion Table                      *
//      *                                                                *
//      ******************************************************************
//
//
// Note: the Cuckoo mode runs once every 15 minutes (on the hour, quarter hour, half hour 
// and three-quarter hour).  When it is not running, it swings the backlight back-and-forth 
// every second (ticking like a pendulum).
//
// You need to program a unique disc motion and backlight display for your sculpture to perform every quarter hour. 
//
// Cuckoo should run a show for at least 2 minutes every quarter hour before it goes to sleep again.
//

//
// name displayed on the LCD when the Cuckoo mode is selected
//
#define CUCKOO_LCD_NAME "CUCKOO"


//
// the backlight colors while it is sleeping, it toggles between the two colors every second
//
const COLOR_ENTRY CUCKOO_BACKGROUND_COLOR_WHILE_SLEEPING_1 = WHITE;
const COLOR_ENTRY CUCKOO_BACKGROUND_COLOR_WHILE_SLEEPING_2 = WHITE_MEDIUM;


//
// Cuckoo: playbook table of motions & colors
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

 MOTION_ENTRY cuckooTbl[] PROGMEM = {
  {  10,  -10,  GREEN,            10000},    // run opposite directions at a constant slow speed  
  {  10,  -15,  TURQUOISE,        10000},
  {  10,  -20,  LIGHT_GREEN,      15000},
  {  10,   10,  BLACK,            5000},
  {  -30,  30,  WHITE_FULL,       30000},
  {  -15,  20,  WHITE,            20000},
  {  -10,  15,  WHITE_MEDIUM,     20000},
  {  -5,  20,  WHITE_LOW,        20000},
  
  


  {   0,    0,  BLACK,            2000}
};

const int cuckooTblLength = sizeof(cuckooTbl) / sizeof(MOTION_ENTRY);

