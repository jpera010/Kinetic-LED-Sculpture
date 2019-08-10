
//      ******************************************************************
//      *                                                                *
//      *                     Light Show Motion Table                    *
//      *                                                                *
//      ******************************************************************

//
// Light Show mode is intended to be a 	non-interactive backlight only kinetic sculpture display.
// It is up to you if the display will be calm and soothing like a night light.
// OR upbeat and stimulating like a night club. 
//
// Light Show should be at least 1 minute long before it begins repeating 
//

//
// Notes: The light show will begin by initially fading up.  It will start at black,
// then fade up to the first color found in the Light Show table. The duration of the
// fade up is the number of milliseconds set by the LIGHT_SHOW_FADEUP_DURATION 
// constant.
//

//
// name displayed on the LCD when the Light Show mode is selected
//
#define LIGHT_SHOW_LCD_NAME "LIGHT SHOW"


//
// number of milliseconds for the fadeup at the beginning of the light show
//
const long LIGHT_SHOW_FADEUP_DURATION = 1200;


//
// Light Show: playbook table of motions & colors
//
//
// MOTION_ENTRY = {0, 0, Color, Duration}, 
//
// Where: 
// 
//  Color - is either a defined color or a actual color defined by mixing {red,green,blue}. 
//      Where:
//             red is some number from 0-255. 0 being no red color, 255 being full red color
//             green is some number from 0-255. 0 being no green color, 255 being full green color
//             blue is some number from 0-255. 0 being no blue color, 255 being full blue color
//
//  Duration - is the amount of time in milliseconds to get from the previous velocities and color to the newly specified velocities and color.
//

MOTION_ENTRY lightShowTbl[] PROGMEM = {
{   0,    0,  WHITE,            5000},
{   0,    0,  RED,              5000},
{   0,    0,  ORANGE,           5000},
{   0,    0,  YELLOW,           5000}, 
{   0,    0,  LIGHT_GREEN,      5000},
{   0,    0,  GREEN,            5000},
{   0,    0,  TURQUOISE,        5000},
{   0,    0,  CYAN,             5000},
{   0,    0,  ICE,              5000},
{   0,    0,  LIGHT_BLUE,       5000},
{   0,    0,  OCEAN_BLUE,       5000},
{   0,    0,  BLUE,             5000},
{   0,    0,  LIGHT_PURPLE,     5000},
{   0,    0,  VIOLET,           5000},
{   0,    0,  MAGENTA,          5000}, 
{   0,    0,  RASPBERRY,        5000}, 
{   0,    0,  WHITE_FULL,       5000},
{   0,    0,  BLACK,            2000},
{   0,    0,  RASPBERRY,        5000},
{   0,    0,  MAGENTA,          5000}, 
{   0,    0,  VIOLET,           5000},
{   0,    0,  LIGHT_PURPLE,     5000},
{   0,    0,  BLUE,             5000},
{   0,    0,  OCEAN_BLUE,       5000},
{   0,    0,  LIGHT_BLUE,       5000},
{   0,    0,  ICE,              5000},
{   0,    0,  CYAN,             5000},
{   0,    0,  TURQUOISE,        5000},
{   0,    0,  GREEN,            5000},
{   0,    0,  LIGHT_GREEN,      5000},
{   0,    0,  YELLOW,           5000}, 
{   0,    0,  ORANGE,           5000},
{   0,    0,  RED,              5000},
{   0,    0,  WHITE_FULL,       5000},
};

const int lightShowTblLength = sizeof(lightShowTbl) / sizeof(MOTION_ENTRY);


