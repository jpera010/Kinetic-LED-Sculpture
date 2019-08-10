
//      ******************************************************************
//      *                                                                *
//      *                       DPEA Kinetic Machine                     *
//      *                     Data types and constants                   *
//      *                                                                *
//      *            Stan Reifel                     3/17/2014           *
//      *                                                                *
//      ******************************************************************


//
// THERE ARE REALLY ONLY TWO THINGS THAT SHOULD BE CHANGED IN THIS FILE.
//
//  1. TRUNING ON AND OFF THE BACKLIGHT BLINK - DESCRIBED BELOW.
//
//  2. ENTER YOUR FIRST AND LAST NAME IN THE SPACE BELOW - SO THAT YOUR SCULPTURE DISPLAYS YOUR NAME ON THE TOP OF THE LCD
//
//  3. MODIFYING AND OR ADDING TO THE COLOR CONSTANTS DEFINED BELOW.
//
//  IT IS ADVISED YOU LEAVE EVERYTHING ELSE AS IT IS. 
//
//                          MR. HARLOW


//
// Setting this constant to "true" will cause the backlight to blink once after a program
// is downloaded.  This is very useful when developing.  Once developement is complete,
// the constant should be changed to "false".  (It must be set to true or false using 
// lower case without quotes)
//
#define BLINK_AFTER_DOWNLOADING true


// Use this space to put your First Last Name on the TOP of the LCD of your Sculpture.
// You MUST enter the letters in the format given.
//
// IF your Last Name is really long - Put your First Inital and Last Name.
//
// You have a MAXIMUM of 14 Letters
//
#define SCULPTURE_LCD_NAME "JosePeralta"


//
// color constants
//
// THESE COLOR CONSTANTS CAN BE MODIFIED TO SUIT YOUR PREFERENCES
// AND YOU CAN ADD ADDITIOIINAL COLOR CONSTANTS TO ARRIVE AT YOUR
// OWN CUSTOM COLOR COMBINATIONS. 
//
//
// IF YOU DO SO - JUST BE SURE YOU MAINTAIN THE ESTABLISHED FORMAT DESCRIBED BELOW:
//
// #DEFINE "NAME_ALL_CAPS" {red,green,blue}
//
// WHERE "red" IS ANY NUMBER BETWEEN 0-255
// WHERE "green" IS ANY NUMBER BETWEEN 0-255
// WHERE "blue" IS ANY NUMBER BETWEEN 0-255
//
// REMEMBER YOU ARE MIXING COLORS IN LIGHT SPACE NOT PIGMENT SPACE! 
//
// HENCE - BLACK IS NO COLORS/LIGHT  AND WHITE IS ALL COLORS ON  

#define RED {255, 0,  0}
#define ORANGE {255,125,0}
#define YELLOW {255, 255, 0}
#define LIGHT_GREEN {125,255,0}
#define GREEN {  0, 255, 0}
#define TURQUOISE{0,255,125}
#define CYAN {0, 255, 255}
#define ICE {197,255,255}
#define LIGHT_BLUE {69,136,255}
#define OCEAN_BLUE {0,125,255}
#define BLUE {0, 0, 255}
#define LIGHT_PURPLE {164,99,228}
#define VIOLET {125,0,255}
#define MAGENTA {255, 0, 255}
#define RASPBERRY {255,0,125}

#define BLACK {0, 0, 0}
#define WHITE_FULL {255, 255, 255}
#define WHITE {255, 255, 135}
#define WHITE_MEDIUM {128, 128, 67}
#define WHITE_LOW {64, 64, 34}
#define WHITE_VERY_LOW {48, 48, 25}
#define WHITE_VERY_VERY_LOW {32, 32, 17}



//
//  ALL YOUR CUSTOM COLOR CONSTANTS SHOULD BE ENTERED ABOVE.
//
//  IT IS ADVISED YOU LEAVE ALL THE CODE BELOW THIS LINE AS IT IS. 
//
//                          MR. HARLOW
//


//
// structure for storing background colors
//
typedef struct _COLOR_ENTRY
{
  int red;
  int green;
  int blue;
} COLOR_ENTRY;


//
// structure for storing the playbook of disk velocities and background colors
//
typedef struct _MOTION_ENTRY
{
  float outerDiskVelocityInRPM;
  float innerDiskVelocityInRPM;
  COLOR_ENTRY rgb;
  long durationMS;
} MOTION_ENTRY;


//
// motion constants constants
//
const float GEAR_REUCTION_TO_FINAL_STAGE = 127;
const long LINES_ON_TACHOMETER_DISK = 25;
const float MINIMUM_VELOCITY_IN_RPM = 0.9;


//
// servo control constants
//
const int MOTOR_KP_PROP_CONTROL = 5;
const int MOTOR_KINT_SPEED_ERROR_MAX = 2000;
const int MOTOR_MAX_PWM_FROM_INTEGRAL_TERM = 125;
const long MOTOR_KI_PROP_INT_CONTROL = MOTOR_KINT_SPEED_ERROR_MAX / MOTOR_MAX_PWM_FROM_INTEGRAL_TERM;
const int MOTOR_MAX_PWM = 220;

