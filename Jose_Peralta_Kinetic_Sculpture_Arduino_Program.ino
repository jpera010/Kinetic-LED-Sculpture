//      ******************************************************************
//      *                                                                *
//      *         DPEA Kinetic Machine - Student Software Version        *
//      *                                                                *
//      *            Stan Reifel                     3/18/2014           *
//      *                                                                *
//      ******************************************************************

//
// UNLESS YOU REALLY KNOW WHAT YOU ARE DOING - DO NOT EDIT THE CODE IN THIS FILE!!!!
// IT IS PRESENTED HERE FOR YOU TO LOOK AT - BUT IS WAY BEYOND THE SCOPE OF THIS CLASS!!!!
// 
// IT IS MY HOPE THAT AT SOME POINT (12th grade year) WE WILL BE ABLE TO RETURN TO THIS CODE AND EXAMINE 
// AND LEARN SECTIONS OF IT.
//
// UNTIL THEN - IF YOU CHOOSE TO EDIT THIS CODE, MAKE SURE YOU HAVE MADE A BACKUP TO FALL
// BACK ON IF YOU MESS IT UP.
//                          MR. HARLOW

#include <Wire.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include "ConstantAndDataTypes.h"
#include "Kinetoscope.h"
#include "LightShow.h"
#include "MellowMan.h"
#include "SleeperCell.h"
#include "Cuckoo.h"

//
// the mode of the machine
//
const byte DISPLAY_MODE_NO_MODE = 100;
const byte DISPLAY_MODE_STOPPED = 0;
const byte DISPLAY_MODE_KINETOSCOPE = 1;
const byte DISPLAY_MODE_LIGHT_SHOW = 2;
const byte DISPLAY_MODE_MELLOW_MAN = 3;
const byte DISPLAY_MODE_SLEEPER_CELL = 4;
const byte DISPLAY_MODE_CUCKOO = 5;
const byte DISPLAY_MODE_METER_STICK = 6;
const byte DISPLAY_MODE_SET_CONTRAST = 7;
const byte DISPLAY_MODE_SET_TIME = 8;
const byte DISPLAY_MODE_LAST = 8;

//
// buttons and remote pin assignments
//
const int BUTTON_UP_PIN = 61;
const int BUTTON_MODE_PIN = 60;
const int BUTTON_DOWN_PIN = 59;

const int REMOTE_BUTTON_A_PIN = 63;
const int REMOTE_BUTTON_B_PIN = 65;
const int REMOTE_BUTTON_C_PIN = 62;
const int REMOTE_BUTTON_D_PIN = 64;


//
// motor pin assignments
//
const int MOTOR_DIRECTION_1_PIN = 15;
const int MOTOR_PWM_1_PIN = 11;
const int MOTOR_TACHOMETER_1_PIN = 19;

const int MOTOR_DIRECTION_2_PIN = 14;
const int MOTOR_PWM_2_PIN = 12;
const int MOTOR_TACHOMETER_2_PIN = 18;


//
// backlight LEDs pin assignments
//
const int BACKLIGHT_RED_PIN = 6;                // port H, bit 3
const int BACKLIGHT_BLUE_PIN = 7;               // port H, bit 4
const int BACKLIGHT_GREEN_PIN = 8;              // port H, bit 5


//
// LCD display pin assignments, NOTE: THESE VALUES CAN NOT BE CHANGED AS PORTS ARE HARD CODED IN THE SOFTWARE
//
const int LCD_CLOCK_PIN = 54;                   // port F, bit 0
const int LCD_DATA_IN_PIN = 55;                 // port F, bit 1
const int LCD_DATA_CONTROL_PIN = 56;            // port F, bit 2
const int LCD_CHIP_ENABLE_PIN = 57;             // port F, bit 3
const int LCD_RESET_PIN = 58;                   // port F, bit 4


//
// ultrasonics pin assignments
//
const int ULTRASONIC_TRIGGER_PIN = 4;
const int ULTRASONIC_ECHO_PIN = 3;


//
// miscellaneous pin assignments
//
const int LCD_BACKLIGHT_PIN = 10;
const int TEST_D9_PIN = 9;
const int LED_PIN = 13;


//
// types of button events
//
const int BUTTON_NO_EVENT = 0;
const int BUTTON_PUSHED = 1;
const int BUTTON_RELEASED = 2;
const int BUTTON_REPEAT = 3;


//
// values for the button numbers (also indexes into ButtonsTable)
//
const byte PUSH_BUTTON_UP = 0;
const byte PUSH_BUTTON_MODE = 1;
const byte PUSH_BUTTON_DOWN = 2;

const byte REMOTE_BUTTON_A = 3;
const byte REMOTE_BUTTON_B = 4;
const byte REMOTE_BUTTON_C = 5;
const byte REMOTE_BUTTON_D = 6;

const byte BUTTON_TABLE_LAST_ENTRY = 6;


//
// EERROM storage locations
//
const int EEPROM_CONTRAST_BYTE_ADDRESS = 0;


//
// global vars
//
byte timeDisplayCount;
int sculptureMode;


// ---------------------------------------------------------------------------------
//                              Hardware and software setup
// ---------------------------------------------------------------------------------

//
// top level setup function
//
void setup()
{ 
  byte contrastValue;

  pinMode(TEST_D9_PIN, OUTPUT);            // configure the Test and LED output bits
  pinMode(LED_PIN, OUTPUT); 
  
  //
  // initialize software modules
  //
  LCDInitialise();                          // initialize the LCD hardware and functions
  motorInitialise();                        // initialize the motor hardware and functions
  diskVelocitiesInitialize();               // initialize functions used to transition between disk velocities
  backlightInitialize();                    // initialize the backlight LEDs
  strobeInitialize();                       // initialize the strobe LED functions
  buttonsInitialize();                      // initialize the buttons hardware and functions
  RTCInitialise();                          // initialize I2C communication with the real time clock
  ultrasonicInitialize();                   // initialize the ultrasonic hardware and functions
  BackgroundProcessingInitialize();         // initialize background processing to run every 10ms

  LCDClearDisplay();                        // start with the LCD display blank
    
  
  //
  // start with the sculpture in the Stopped mode
  //
  sculptureMode = DISPLAY_MODE_NO_MODE;
  setSculptureMode(DISPLAY_MODE_STOPPED);


  //
  // set the LCD contrast using the value saved in EEPROM, read the value from the EEPROM, 
  //  if it has not ever been set choose a default value
  //
  contrastValue = getContrastByteFromEEPROM();
  LCDSetContrast(contrastValue);


  //
  // update the display, including the sculputer's name
  //
  timeDisplayCount = 0;
  LCDPrintCenteredString(SCULPTURE_LCD_NAME, 0);


  //
  // enable interrupts to start background processing
  //
  sei();


  //
  // flash the LEDs showing it is ready
  //
#if BLINK_AFTER_DOWNLOADING
  backlightSetColor(70, 70, 70);
  delay(50);
  backlightSetColor(0, 0, 0);
#endif
}


// ---------------------------------------------------------------------------------
//                      The Main Loop and Top Level Functions
// ---------------------------------------------------------------------------------

//
// main loop, select the display show based on the User Mode Setting
//
void loop()
{
  //
  // execute the mode that the sculpture is set to
  //
  switch(sculptureMode)
  {
    case DISPLAY_MODE_STOPPED:
      showStoppedMode();
      break;
      
    case DISPLAY_MODE_KINETOSCOPE:
      showKinetoscopeMode();
      stopBetweenModes();
      break;
      
    case DISPLAY_MODE_LIGHT_SHOW:
      showLightShowMode();
      stopBetweenModes();
      break;
      
    case DISPLAY_MODE_MELLOW_MAN:
      showMellowManMode();
      stopBetweenModes();
      break; 
 
    case DISPLAY_MODE_SLEEPER_CELL:
      showSleeperCellMode();
      stopBetweenModes();
     break;
      
    case DISPLAY_MODE_CUCKOO:
      showCuckooMode();
      stopBetweenModes();
      break;
 
   case DISPLAY_MODE_METER_STICK:
      showMeterStickMode();
      break;
 
   case DISPLAY_MODE_SET_CONTRAST:
      showSetContrastMode();
      break;
 
   case DISPLAY_MODE_SET_TIME:
      showSetTimeMode();
      break;
  }
}


// ---------------------------------------------------------------------------------
//                                  The Stopped Mode
// ---------------------------------------------------------------------------------

//
// run the Stopped mode, return when no longer in this mode
//
void showStoppedMode()
{
  //
  // loop to run this mode until the mode is changed with a button press
  //
  while(true)
  { 
    //
    // check for button presses and update the clock
    // 
    executeTasks();

    //
    // return if no longer in this mode
    //
    if (sculptureMode != DISPLAY_MODE_STOPPED)
      return;
  }
}


// ---------------------------------------------------------------------------------
//                                The Kinetoscope Mode
// ---------------------------------------------------------------------------------

//
// run the Kinetoscope mode, return when no longer in this mode
//
void showKinetoscopeMode()
{
  int idx = 0;
  int lastTableIdx;

  //
  // run through the table, executing the motions and background color changes
  //
  lastTableIdx = kinetoscopeTblLength - 1;
  
  while(true)
  {  for (idx = 0; idx <= lastTableIdx; idx++)
    {
      //
      // get the disk velocities from the table and transition from the current velocities 
      //   to new velocities over the given duration period
      //
      diskVelocitiesStartTransition(
        pgm_read_float(&kinetoscopeTbl[idx].outerDiskVelocityInRPM), 
        pgm_read_float(&kinetoscopeTbl[idx].innerDiskVelocityInRPM), 
        pgm_read_dword(&kinetoscopeTbl[idx].durationMS));
 
      //
      // get the backlight color from the table and transition from the current RGB values 
      //   to new values over the given duration period
      //
      backlightStartTransition(
        pgm_read_word(&kinetoscopeTbl[idx].rgb.red), 
        pgm_read_word(&kinetoscopeTbl[idx].rgb.green), 
        pgm_read_word(&kinetoscopeTbl[idx].rgb.blue), 
        pgm_read_dword(&kinetoscopeTbl[idx].durationMS));
      
      //
      // update the LCD display with the table entry number currently being executed
      //
      LCDSetCursorXY(26, 3);
      LCDPrintUnsignedIntWithPadding(idx, 3, ' ');

  
      //
      // execute other task while waiting for transition to complete
      //
      while(diskVelocitiesTransitionIsFinished() == false)
      {
        //
        // check for button presses...
        // 
        executeTasks();

        //
        // return if no longer in this mode
        //
        if (sculptureMode != DISPLAY_MODE_KINETOSCOPE)
          return;
      }
    }
  }
}


// ---------------------------------------------------------------------------------
//                               The Light Show Mode
// ---------------------------------------------------------------------------------

//
// run the Light Show mode, return when no longer in this mode
//
void showLightShowMode()
{
  int idx = 0;
  int lastTableIdx;

  //
  // begin the Light Show by fading up to the first color in the table
  //
  diskVelocitiesStartTransition(0, 0, LIGHT_SHOW_FADEUP_DURATION);
  backlightStartTransition(
    pgm_read_word(&lightShowTbl[0].rgb.red), 
    pgm_read_word(&lightShowTbl[0].rgb.green), 
    pgm_read_word(&lightShowTbl[0].rgb.blue), 
    LIGHT_SHOW_FADEUP_DURATION);

  //
  // execute other task while waiting for initialization transition to complete
  //
  while(diskVelocitiesTransitionIsFinished() == false)
  {
    //
    // check for button presses...
    // 
    executeTasks();

    //
    // return if no longer in this mode
    //
    if (sculptureMode != DISPLAY_MODE_LIGHT_SHOW)
      return;
  }


  //
  // run through the table, executing the background color changes
  //
  lastTableIdx = lightShowTblLength - 1;
  
  while(true)
  {  for (idx = 0; idx <= lastTableIdx; idx++)
    {
      //
      // set the disk velocities to 0
      //
      diskVelocitiesStartTransition(0, 0,
        pgm_read_dword(&lightShowTbl[idx].durationMS));
 
      //
      // get the backlight color from the table and transition from the current RGB values 
      //   to new values over the given duration period
      //
      backlightStartTransition(
        pgm_read_word(&lightShowTbl[idx].rgb.red), 
        pgm_read_word(&lightShowTbl[idx].rgb.green), 
        pgm_read_word(&lightShowTbl[idx].rgb.blue), 
        pgm_read_dword(&lightShowTbl[idx].durationMS));
          
      //
      // update the LCD display with the table entry number currently being executed
      //
      LCDSetCursorXY(26, 3);
      LCDPrintUnsignedIntWithPadding(idx, 3, ' ');

  
      //
      // execute other task while waiting for transition to complete
      //
      while(diskVelocitiesTransitionIsFinished() == false)
      {
        //
        // check for button presses...
        // 
        executeTasks();

        //
        // return if no longer in this mode
        //
        if (sculptureMode != DISPLAY_MODE_LIGHT_SHOW)
          return;
      }
    }
  }
}


// ---------------------------------------------------------------------------------
//                                 The Mellow Man Mode
// ---------------------------------------------------------------------------------

//
// run the Mellow Man mode, return when no longer in this mode
//
void showMellowManMode()
{
  int idx = 0;
  int lastTableIdx;

  //
  // run through the table, executing the motions
  //
  lastTableIdx = MellowManTblLength - 1;
  
  while(true)
  {  for (idx = 0; idx <= lastTableIdx; idx++)
    {
      //
      // get the disk velocities from the table and transition from the current velocities 
      //   to new velocities over the given duration period
      //
      diskVelocitiesStartTransition(
        pgm_read_float(&mellowManTbl[idx].outerDiskVelocityInRPM), 
        pgm_read_float(&mellowManTbl[idx].innerDiskVelocityInRPM), 
        pgm_read_dword(&mellowManTbl[idx].durationMS));
 
      //
      // get the backlight color from the table and transition from the current RGB values 
      //   to new values over the given duration period
      //
      backlightStartTransition(
        pgm_read_word(&mellowManTbl[idx].rgb.red), 
        pgm_read_word(&mellowManTbl[idx].rgb.green), 
        pgm_read_word(&mellowManTbl[idx].rgb.blue), 
        pgm_read_dword(&mellowManTbl[idx].durationMS));
        
      //
      // update the LCD display with the table entry number currently being executed
      //
      LCDSetCursorXY(26, 3);
      LCDPrintUnsignedIntWithPadding(idx, 3, ' ');
 
  
      //
      // execute other task while waiting for transition to complete
      //
      while(diskVelocitiesTransitionIsFinished() == false)
      {
        //
        // check for button presses...
        // 
        executeTasks();

        //
        // return if no longer in this mode
        //
        if (sculptureMode != DISPLAY_MODE_MELLOW_MAN)
          return;
      }
    }
  }
}


// ---------------------------------------------------------------------------------
//                                The Sleeper Cell Mode
// ---------------------------------------------------------------------------------

//
// constants for showState
//
const int SLEEPER_CELL_STATE_STARTING_WAITING_FOR_PERSON = 0;
const int SLEEPER_CELL_STATE_WAITING_FOR_PERSON = 1;
const int SLEEPER_CELL_STATE_START_NEW_SHOW = 2;
const int SLEEPER_CELL_STATE_NEXT_ENTRY = 3;
const int SLEEPER_CELL_STATE_RUNNING_ENTRY = 4;
const int SLEEPER_CELL_STATE_STOP_SHOW = 5;
const int SLEEPER_CELL_STATE_STOPPING_SHOW = 6;

//
// misc sleeper cell constants
//
const int PERIOD_OF_ULTRASONIC_MEASUREMENTS_MS = 100;


// ---------------------------------------------------------------------------------

//
// run the Sleeper Cell mode, return when no longer in this mode
//
void showSleeperCellMode()
{
  int nextEntryInShowIdx;
  int lastEntryInShowIdx;
  int showState;
  int distance;
  unsigned long nextEventTime;
  bool waitingForEcho;
  int personDetectedCount;
  int personNotDetectedCount;
  int numberMeasurementsBelowThresholdToStart;
  int numberMeasurementsAboveThresholdToStop;

  //
  // get setup
  //
  showState = SLEEPER_CELL_STATE_STARTING_WAITING_FOR_PERSON;
  lastEntryInShowIdx = sleeperCellTblLength - 1;
  personDetectedCount = 0;
  personNotDetectedCount = 0;
  nextEventTime = millis() + PERIOD_OF_ULTRASONIC_MEASUREMENTS_MS;
  numberMeasurementsBelowThresholdToStart = MINIMUM_NUMBER_OF_MILLISECONDS_NEEDED_TO_DETERMINE_PERSON_IS_VIEWING / PERIOD_OF_ULTRASONIC_MEASUREMENTS_MS;
  numberMeasurementsAboveThresholdToStop = MINIMUM_NUMBER_OF_MILLISECONDS_TO_DETERMINE_PERSON_IS_NOLONGER_VIEWING / PERIOD_OF_ULTRASONIC_MEASUREMENTS_MS;
 

  //
  // wait for a person, run the show if found, also check buttons and updating the clock
  //
  while(true)
  {
    //
    // check for button presses...
    // 
    executeTasks();

    //
    // return if no longer in this mode
    //
    if (sculptureMode != DISPLAY_MODE_SLEEPER_CELL)
    {
      return;
    }

    //
    // check if time to start an ultrasonic measurement
    //
    if (millis() >= nextEventTime)
    {  
      ultrasonicStartMeasurement();
      nextEventTime =  millis() + PERIOD_OF_ULTRASONIC_MEASUREMENTS_MS;
      waitingForEcho = true;
    }

    //
    // check if a ultrasonic measurement has just completed
    //
    if (waitingForEcho)
    {
      if (ultrasonicIsFinished())
      {
        //
        // measurement complete, get the distance to the nearest object in CM
        //
        distance = ultrasonicGetDistanceInCM();
        waitingForEcho = false;
        
        //
        // test recent measurements to decide if person is in front of the sculpture
        //
        if (distance <= MINIMUM_DISTANCE_IN_CM_TO_DETECTED_PERSON_IS_VIEWING)
        {
         personNotDetectedCount = 0;
         if (showState == SLEEPER_CELL_STATE_WAITING_FOR_PERSON)
          {  
            //
            // looks like person is in front
            //
             if (personDetectedCount < numberMeasurementsBelowThresholdToStart)
            {
              personDetectedCount++;
              if (personDetectedCount == numberMeasurementsBelowThresholdToStart)
                showState = SLEEPER_CELL_STATE_START_NEW_SHOW;
            }
          }
        }
        else
        {
          //
          // looks like person is not in front
          //
          personDetectedCount = 0;
          if (personNotDetectedCount < numberMeasurementsAboveThresholdToStop)
          {
            personNotDetectedCount++;
            if (personNotDetectedCount == numberMeasurementsAboveThresholdToStop)
            {
              if (showState != SLEEPER_CELL_STATE_WAITING_FOR_PERSON)
                showState = SLEEPER_CELL_STATE_STOP_SHOW;
            }
          }
        }
      }
    }
 
 
    //
    // determine the state of the show and run if person is in front
    //
    switch(showState)
    {
      //
      // check if just starting to wait for someone to stand in front of the sculpture
      //
      case SLEEPER_CELL_STATE_STARTING_WAITING_FOR_PERSON:
      {
        //
        // turn the backlight on low while waiting for sleeper sell mode
        //
        backlightStartTransitionRGBColor(BACKGROUND_COLOR_WHILE_SLEEPING, 500);
        showState = SLEEPER_CELL_STATE_WAITING_FOR_PERSON;
      }
      
      
      //
      // check if waiting for someone to stand in front of the sculpture
      //
      case SLEEPER_CELL_STATE_WAITING_FOR_PERSON:
      {
       break;
      }
      
      //
      // check if should begin running the showw
      //
      case SLEEPER_CELL_STATE_START_NEW_SHOW:
      {
        nextEntryInShowIdx = 0;
        showState = SLEEPER_CELL_STATE_NEXT_ENTRY;
        break;
      }
        
      //
      // check if running the show and it is time execute the next table entry
      //
      case SLEEPER_CELL_STATE_NEXT_ENTRY:
      {
        //
        // get the disk velocities from the table and transition from the current velocities 
        //   to new velocities over the given duration period
        //
        diskVelocitiesStartTransition(
          pgm_read_float(&sleeperCellTbl[nextEntryInShowIdx].outerDiskVelocityInRPM), 
          pgm_read_float(&sleeperCellTbl[nextEntryInShowIdx].innerDiskVelocityInRPM), 
          pgm_read_dword(&sleeperCellTbl[nextEntryInShowIdx].durationMS));
   
        //
        // get the backlight color from the table and transition from the current RGB values 
        //   to new values over the given duration period
        //
        backlightStartTransition(
          pgm_read_word(&sleeperCellTbl[nextEntryInShowIdx].rgb.red), 
          pgm_read_word(&sleeperCellTbl[nextEntryInShowIdx].rgb.green), 
          pgm_read_word(&sleeperCellTbl[nextEntryInShowIdx].rgb.blue), 
          pgm_read_dword(&sleeperCellTbl[nextEntryInShowIdx].durationMS));
        
        //
        // update the LCD display with the table entry number currently being executed
        //
        LCDSetCursorXY(26, 3);
        LCDPrintUnsignedIntWithPadding(nextEntryInShowIdx, 3, ' ');
          
        //
        // determine which entry comes after this one
        //
        nextEntryInShowIdx = nextEntryInShowIdx + 1;
        if (nextEntryInShowIdx > lastEntryInShowIdx)
          nextEntryInShowIdx = 0;
         
        //
        // set state to wait for this new move to finish
        //
        showState = SLEEPER_CELL_STATE_RUNNING_ENTRY;
        break;
      }
      
      //
      // check if running the show and waiting for the current entry to finish
      //
      case SLEEPER_CELL_STATE_RUNNING_ENTRY:
      {
        //
        // check if the entry has finished
        //
        if (diskVelocitiesTransitionIsFinished())
          showState = SLEEPER_CELL_STATE_NEXT_ENTRY;
        break;
      }
 
      //
      // check if requested to stop all movement because the person is gone
      //
      case SLEEPER_CELL_STATE_STOP_SHOW:
      {
        //
        // start a new motion to transition the disks to stop and background to off
        //
        diskVelocitiesStartTransition(0, 0, 500);  
        backlightStartTransition(0, 0, 0, 500);
         
        //
        // set state to wait for the disks to shop
        //
        showState = SLEEPER_CELL_STATE_STOPPING_SHOW;
        break;
      }
      
      //
      // check if waiting for the stop to finish
      //
      case SLEEPER_CELL_STATE_STOPPING_SHOW:
      {
        //
        // check if the stop motion has finished
        //
        if (diskVelocitiesTransitionIsFinished())
          showState = SLEEPER_CELL_STATE_STARTING_WAITING_FOR_PERSON;
        break;
      }
    }
  }
}


// ---------------------------------------------------------------------------------
//                                   The Cuckoo Mode
// ---------------------------------------------------------------------------------

//
// constants for showState
//
const int CUCKOO_STATE_STARTING_TO_WAIT_FOR_QUARTER_HOUR = 0;
const int CUCKOO_STATE_WAITING_FOR_QUARTER_HOUR = 1;
const int CUCKOO_STATE_START_NEW_SHOW = 2;
const int COCKOO_STATE_NEXT_ENTRY = 3;
const int COCKOO_STATE_RUNNING_ENTRY = 4;
const int COCKOO_STATE_STOP_SHOW = 5;
const int COCKOO_STATE_STOPPING_SHOW = 6;

//
// other Cuckoo constants
//
const int PERIOD_TO_CHECK_CLOCK_MS = 1000;


// ---------------------------------------------------------------------------------

//
// run the Cockoo mode, return when no longer in this mode
//
void showCuckooMode()
{
  int nextEntryInShowIdx;
  int lastEntryInShowIdx;
  int showState;
  unsigned long nextEventTime;
  byte second;
  byte minute;
  byte hour;
  byte minutesAfterHourForNextEvent;
  int minutesToWait;
  bool backLightOnFlg;

  //
  // get setup
  //
  showState = CUCKOO_STATE_STARTING_TO_WAIT_FOR_QUARTER_HOUR;
  lastEntryInShowIdx = cuckooTblLength - 1;
  nextEventTime = millis() + PERIOD_TO_CHECK_CLOCK_MS;

  //
  // determine the time for the next cuckoo event
  //
  minutesAfterHourForNextEvent = nextCuckooTime();  

  //
  // run a little show every 15 minutes on the quarter hour
  //
  while(true)
  {
    //
    // check for button presses...
    // 
    executeTasks();

    //
    // return if no longer in this mode
    //
    if (sculptureMode != DISPLAY_MODE_CUCKOO)
    {
      return;
    }

    //
    // check if time to read the RTC, check if quarter hour has passed
    //
    if ((millis() >= nextEventTime) && (showState == CUCKOO_STATE_WAITING_FOR_QUARTER_HOUR))
    {  
      //
      // determine time for next event
      //
      nextEventTime =  millis() + PERIOD_TO_CHECK_CLOCK_MS;
 
      //
      // read the real time clock to get the current time
      //
      RTCGetTime(&hour, &minute, &second);
      
      if (minute == minutesAfterHourForNextEvent)
      {  
        showState = CUCKOO_STATE_START_NEW_SHOW;
        minutesAfterHourForNextEvent = nextCuckooTime();
      }
      
      //
      // display the number of minutes until the next show
      //
      if (minutesAfterHourForNextEvent == 0)
        minutesToWait = 60 - minute;
      else
        minutesToWait = minutesAfterHourForNextEvent - minute;
        
      LCDSetCursorXY(0, 3);
      LCDPrintString("Next in ");
      LCDPrintInt(minutesToWait);
      LCDPrintString(" min ");

      //
      // toggle the backlight every second
      //
      if (backLightOnFlg)
      {
        backlightStartTransitionRGBColor(CUCKOO_BACKGROUND_COLOR_WHILE_SLEEPING_1, 150);
        backLightOnFlg = false;
      }
      else
      {
        backlightStartTransitionRGBColor(CUCKOO_BACKGROUND_COLOR_WHILE_SLEEPING_2, 150);
        backLightOnFlg = true;
      }
    }
 
    //
    // determine the state of the show and run if person in front
    //
    switch(showState)
    {
      //
      // check if starting to wait for someone to stand in front of the sculpture
      //
      case CUCKOO_STATE_STARTING_TO_WAIT_FOR_QUARTER_HOUR:
      {
        //
        // turn the backlight on low while waiting for the quarter hour
        //
        showState = CUCKOO_STATE_WAITING_FOR_QUARTER_HOUR;
        break;
      } 
 
      //
      // check if waiting for someone to stand in front of the sculpture
      //
      case CUCKOO_STATE_WAITING_FOR_QUARTER_HOUR:
      {
       break;
      }
      
      //
      // check if time to start a new show
      //
      case CUCKOO_STATE_START_NEW_SHOW:
      {
        nextEntryInShowIdx = 0;
        showState = COCKOO_STATE_NEXT_ENTRY;
        break;
      }
        
      //
      // check if time to run the next entry
      //
      case COCKOO_STATE_NEXT_ENTRY:
      {
        //
        // check if running the table is complete
        //
        if (nextEntryInShowIdx > lastEntryInShowIdx)
        {
          showState = COCKOO_STATE_STOP_SHOW;
          break;
        }

        //
        // get the disk velocities from the table and transition from the current velocities 
        //   to new velocities over the given duration period
        //
        diskVelocitiesStartTransition(
          pgm_read_float(&cuckooTbl[nextEntryInShowIdx].outerDiskVelocityInRPM), 
          pgm_read_float(&cuckooTbl[nextEntryInShowIdx].innerDiskVelocityInRPM), 
          pgm_read_dword(&cuckooTbl[nextEntryInShowIdx].durationMS));
   
        //
        // get the backlight color from the table and transition from the current RGB values 
        //   to new values over the given duration period
        //
        backlightStartTransition(
          pgm_read_word(&cuckooTbl[nextEntryInShowIdx].rgb.red), 
          pgm_read_word(&cuckooTbl[nextEntryInShowIdx].rgb.green), 
          pgm_read_word(&cuckooTbl[nextEntryInShowIdx].rgb.blue), 
          pgm_read_dword(&cuckooTbl[nextEntryInShowIdx].durationMS));
          
        //
        // determine which entry comes after this one
        //
        nextEntryInShowIdx++;
        showState = COCKOO_STATE_RUNNING_ENTRY;
      }
      
      //
      // check if waiting for the current entry to finish
      //
      case COCKOO_STATE_RUNNING_ENTRY:
      {
        //
        // check if the entry has finished
        //
        if (diskVelocitiesTransitionIsFinished())
          showState = COCKOO_STATE_NEXT_ENTRY;
        break;
      }
 
      //
      // check if requested to stop movement because person is gone
      //
      case COCKOO_STATE_STOP_SHOW:
      {
        //
        // start the next entry in the show's table
        //
        diskVelocitiesStartTransition(0, 0, 500);  
        backlightStartTransition(0, 0, 0, 500);
         
        //
        // wait for this move to finish
        //
        showState = COCKOO_STATE_STOPPING_SHOW;
        break;
      }
      
      //
      // check if waiting for the stop to finish
      //
      case COCKOO_STATE_STOPPING_SHOW:
      {
        //
        // check if the stop has finished
        //
        if (diskVelocitiesTransitionIsFinished())
          showState = CUCKOO_STATE_STARTING_TO_WAIT_FOR_QUARTER_HOUR;
        break;
      }
    }
  }
}



//
// return the time of the next cuckoo event
//
int nextCuckooTime()
{
  byte second;
  byte minute;
  byte hour;
  int nextShowTime;

  RTCGetTime(&hour, &minute, &second);
  
  if ((minute >= 0) && (minute <= 14)) 
    nextShowTime = 15;
 
  else if ((minute >= 15) && (minute <= 29))
    nextShowTime = 30;
 
  else if ((minute >= 30) && (minute <= 44))
    nextShowTime = 45;
 
  else
    nextShowTime = 0;
  
  return(nextShowTime);
}


// ---------------------------------------------------------------------------------
//                                The Meter Stick Mode
// ---------------------------------------------------------------------------------

//
// run the Meter Stick mode, return when no longer in this mode
//
void showMeterStickMode()
{
  int distance;
  unsigned long nextEventTime;
  bool waitingForEcho;
  int barGraphLength;
  const int BAR_GRAPH_WIDTH = 84;
  const int MAX_BAR_GRAPH_CM = 300;

  //
  // initialize timer
  //
  nextEventTime =  millis() + 120;
  waitingForEcho = false;

  //
  // loop to run this mode until the mode is changed with a button press
  //
  while(true)
  { 
    //
    // check if time to start an ultrasonic measurement
    //
    if (millis() >= nextEventTime)
    {  
      ultrasonicStartMeasurement();
      nextEventTime =  millis() + 100;
      waitingForEcho = true;
    }

    //
    // check if a new measurement is complete
    //
    if (waitingForEcho)
    {
      if (ultrasonicIsFinished())
      {
        //
        // measurement complete, display results
        //
        distance = ultrasonicGetDistanceInCM();
        LCDSetCursorXY(26, 4);
        LCDPrintUnsignedIntWithPadding(distance, 3, ' ');
        LCDPrintString("CM");
        
        //
        // draw a bar graph showing the distance
        //
        barGraphLength = (distance * BAR_GRAPH_WIDTH) / MAX_BAR_GRAPH_CM;

        if (barGraphLength > BAR_GRAPH_WIDTH) 
          barGraphLength = 84;

        if (barGraphLength < 1) 
          barGraphLength = 1;
        
        LCDDrawRowOfPixels(0, barGraphLength-1, 3, 0x3c);
        
        if (barGraphLength < BAR_GRAPH_WIDTH)
        LCDDrawRowOfPixels(barGraphLength, BAR_GRAPH_WIDTH-1, 3, 0x0);

        waitingForEcho = false;
      }
    }
    
    //
    // check for button presses and update the clock
    // 
    executeTasks();

    //
    // return if no longer in this mode
    //
    if (sculptureMode != DISPLAY_MODE_METER_STICK)
      return;
  }
}


// ---------------------------------------------------------------------------------
//                             Set the LCD Contrast Mode
// ---------------------------------------------------------------------------------

//
// run the Set LCD Contrast mode, return when no longer in this mode
//
void showSetContrastMode()
{
  byte event;
  int contrastValue;
  unsigned long startTime;

  //
  // initially draw the contrast display
  //
  updateContrastModeDisplay();

  //
  // turn on the LED so we know we're in the contrast setting mode
  //   in the case where the LCD can not been seen
  //
  digitalWrite(LED_PIN, HIGH);
  startTime =  millis();

  //
  // loop to run this mode until the mode is changed with a button press
  //
  while(true)
  { 
    //
    // check for the UP button and adjust the contract value with each press
    //
    event = CheckButton(PUSH_BUTTON_UP);
    if ((event == BUTTON_PUSHED) || (event == BUTTON_REPEAT))
    {
      //
      // up button pressed, read the current value from EEPROM and add 1 to it
      //
      contrastValue = getContrastByteFromEEPROM();
      contrastValue++;
      
      if (contrastValue >= 127)
        contrastValue = 127;

      //
      // save the value in EEPROM, update the LCD contrast value and update the bar graph
      //
      EEPROM.write(EEPROM_CONTRAST_BYTE_ADDRESS, contrastValue);
      LCDSetContrast(contrastValue);
      updateContrastModeDisplay();
    }
    
    
    //
    // check for the DOWN button and adjust the contract value with each press
    //
    event = CheckButton(PUSH_BUTTON_DOWN);
    if ((event == BUTTON_PUSHED) || (event == BUTTON_REPEAT))
    {
      //
      // down button pressed, read the current value from EEPROM and subtract 1 from it
      //
      contrastValue = getContrastByteFromEEPROM();
      contrastValue--;
      
      if (contrastValue < 0)
        contrastValue = 0;

      //
      // save the value in EEPROM, update the LCD contrast value and update the bar graph
      //
      EEPROM.write(EEPROM_CONTRAST_BYTE_ADDRESS, contrastValue);
      LCDSetContrast(contrastValue);
      updateContrastModeDisplay();
    }   
    
    
    //
    // blink the LED showing that in the "set contrast" mode
    //
    if ((millis() - startTime) < 1500)
      digitalWrite(LED_PIN, HIGH);
    else
      digitalWrite(LED_PIN, LOW);
      
    if ((millis() - startTime) > 3000)
      startTime = millis();
    
    
    //
    // check for button presses and update the clock
    // 
    executeTasks();

    //
    // return if no longer in this mode
    //
    if (sculptureMode != DISPLAY_MODE_SET_CONTRAST)
    {
      digitalWrite(LED_PIN, LOW);        // turn off LED and return
      return;
    }
  }
}



//
// update the display showing the contrast value and bar graph
//
void updateContrastModeDisplay()
{
  int barGraphLength;
  int contrastValue;
  const int BAR_GRAPH_WIDTH = 84;
  const int MAX_BAR_GRAPH_CM = 300;
 
  //
  // get the constrast value stored in EEPROM
  //
  contrastValue = getContrastByteFromEEPROM();

  //
  // draw a bar graph showing the value
  //
  barGraphLength = (contrastValue * BAR_GRAPH_WIDTH) / 128;
  
  if (barGraphLength > BAR_GRAPH_WIDTH) 
    barGraphLength = 84;
  
  if (barGraphLength < 1) 
    barGraphLength = 1;
  
  LCDDrawRowOfPixels(0, barGraphLength-1, 3, 0x3c);
  
  if (barGraphLength < BAR_GRAPH_WIDTH)
  LCDDrawRowOfPixels(barGraphLength, BAR_GRAPH_WIDTH-1, 3, 0x0);

  //
  // display the contract as a number
  //
  LCDSetCursorXY(33, 4);
  LCDPrintUnsignedIntWithPadding(contrastValue, 3, ' ');
}



//
// get the contrast byte stored in EEPROM
//  Exit:  contrast byte returned
//
byte getContrastByteFromEEPROM()
{
  byte contrastValue;
  
  //
  // get the value saved in EEPROM
  //
  contrastValue = EEPROM.read(EEPROM_CONTRAST_BYTE_ADDRESS);
  
  //
  // if the value has never been set, use the default value
  //
  if (contrastValue == 0xff)
    contrastValue = 59;
    
  return(contrastValue);
}



// ---------------------------------------------------------------------------------
//                                Set The Time Mode
// ---------------------------------------------------------------------------------

//
// run the Set Time mode, return when no longer in this mode
//
void showSetTimeMode()
{
  byte event;
  int timeIncrement;
  int repeatCount;
  
  //
  // display help info on the LCD
  //
  LCDPrintCenteredString("Use Up & Down", 3);
  LCDPrintCenteredString("buttons.", 4);


  //
  // loop to run this mode until the mode is changed with a button press
  //
  while(true)
  { 
    //
    // check for a UP button Pressed event
    //
    event = CheckButton(PUSH_BUTTON_UP);
    if (event == BUTTON_PUSHED)
    {
      timeIncrement = 1;
      repeatCount = 0;
      advanceRTCTime(timeIncrement);
    }

    //
    // check for a UP button Repeat event
    //
    if (event == BUTTON_REPEAT)
    {
      repeatCount++;
      if (repeatCount == 15)
        timeIncrement = 5;
      if (repeatCount == 30)
        timeIncrement = 10;
      advanceRTCTime(timeIncrement);     
    }
        
    //
    // check for a DOWN button Pressed event
    //
    event = CheckButton(PUSH_BUTTON_DOWN);
    if (event == BUTTON_PUSHED)
    {
      timeIncrement = 1;
      repeatCount = 0;
      retardRTCTime(timeIncrement);
    }  
    
    //
    // check for a DOWN button Repeat event
    //
    if (event == BUTTON_REPEAT)
    {
      repeatCount++;
      if (repeatCount == 15)
        timeIncrement = 5;
      if (repeatCount == 30)
        timeIncrement = 10;
      retardRTCTime(timeIncrement);
    }   
    
    
    //
    // check for other buttons presses and update the clock display
    // 
    executeTasks();

    //
    // return if no longer in this mode
    //
    if (sculptureMode != DISPLAY_MODE_SET_TIME)
      return;
  }
}


//
// advance the RTC time by n minutes
//
void advanceRTCTime(int minutesForward)
{
  byte second;
  byte minute;
  byte hour;

  //
  // read the current time
  //
  RTCGetTime(&hour, &minute, &second);
  
  //
  // advance the time
  //
  minute += minutesForward;
  minute = minute - (minute % minutesForward);
  
  if (minute >= 60)
  {
    minute = 0;
    hour++;
    
    if (hour >= 24)
      hour = 0;
  }

  //
  // set the new time
  //
  RTCSetTimeAndDate(2013, 1, 1, 1, hour, minute, 0);

  //
  // update the display
  //
  DisplayTimeOnLCD();
}



//
// retard the RTC time by n minutes
//
void retardRTCTime(int minutesBackward)
{
  byte second;
  byte minute;
  byte hour;
  int hours;
  int minutes;

  //
  // read the current time
  //
  RTCGetTime(&hour, &minute, &second);
  hours = hour;
  minutes = minute;
  
  //
  // retard the time
  //
  minutes = minutes - (minutes % minutesBackward);
  if (minutes == minute)
    minutes -= minutesBackward;
  
  if (minutes < 0)
  {
    minutes += 60;
    hours--;
    
    if (hours <= 0)
      hours = 23;
  }
  hour = hours;
  minute = minutes;

  //
  // set the new time
  //
  RTCSetTimeAndDate(2013, 1, 1, 1, hour, minute, 0);

  //
  // update the display
  //
  DisplayTimeOnLCD();
}


// ---------------------------------------------------------------------------------
//                               Mode Support Functions
// ---------------------------------------------------------------------------------

//
// transition the disks to a stopped velocity and the backlight to off
//
void stopBetweenModes()
{
  diskVelocitiesStartTransition(0, 0, 500);
  backlightStartTransition(0, 0, 0, 500);
  
  while(diskVelocitiesTransitionIsFinished() == false)
    executeTasks();
    
  motorZeroIntergralTerms();
}



//
// execute other tasks while waiting for the current motion to complete
//
void executeTasks()
{
  byte event;
  int newSculptureMode;
  
  //
  // check if the Mode button has been press indicating the mode has changed
  //
  event = CheckButton(PUSH_BUTTON_MODE);
  if (event == BUTTON_PUSHED)
  { 
    newSculptureMode = sculptureMode + 1;
    if (newSculptureMode > DISPLAY_MODE_LAST)
      newSculptureMode = DISPLAY_MODE_STOPPED;
      
    setSculptureMode(newSculptureMode);
  }
  
  
  //
  // check if the remote A button has been pressed: Kinetoscope mode
  //
  event = CheckButton(REMOTE_BUTTON_A);
  if (event == BUTTON_PUSHED)
     setSculptureMode(DISPLAY_MODE_KINETOSCOPE);
  
  
  //
  // check if the remote B button has been pressed: Light Show mode
  //
  event = CheckButton(REMOTE_BUTTON_B);
  if (event == BUTTON_PUSHED)
     setSculptureMode(DISPLAY_MODE_LIGHT_SHOW);
  
  
  //
  // check if the remote C button has been pressed: Sleeper Cell mode
  //
  event = CheckButton(REMOTE_BUTTON_C);
  if (event == BUTTON_PUSHED)
    setSculptureMode(DISPLAY_MODE_SLEEPER_CELL);

  
  //
  // check if the remote D button has been pressed: Stopped mode
  //
  event = CheckButton(REMOTE_BUTTON_D);
  if (event == BUTTON_PUSHED)
     setSculptureMode(DISPLAY_MODE_STOPPED);


  //
  // periodically update the time shown on the LCD
  //
  if(timeDisplayCount == 0)
  {
    DisplayTimeOnLCD(); 
    timeDisplayCount = 20;
  } 
  timeDisplayCount--;  
  
  
  //
  // delay a bit before checking for next event
  //
  delay(20);
}



//
// set the sculpture's mode
//  Enter: mode = DISPLAY_MODE_STOPPED, DISPLAY_MODE_KINETOSCOPE, DISPLAY_MODE_STROBOCOPIC...
//
void setSculptureMode(int mode)
{
  char *s;

  //
  // check if already in the desired mode
  //
  if (sculptureMode == mode)
    return;
    
  //
  // set the new mode
  //
  sculptureMode = mode;
  
  //
  // display the mode name on the LCD
  //
  switch(mode)
  {
    case DISPLAY_MODE_STOPPED:
      s = "STOPPED";
      break;
      
    case DISPLAY_MODE_KINETOSCOPE:
      s = KINETOSCOPE_LCD_NAME;
//    s = "KINETOSCOPE";
      break;
      
    case DISPLAY_MODE_LIGHT_SHOW:
      s = LIGHT_SHOW_LCD_NAME;
//    s = "LIGHT SHOW";
      break;
      
    case DISPLAY_MODE_MELLOW_MAN:
      s = MELLOW_MAN_LCD_NAME;
//    s = "MELLOW MAN";
      break;
      
    case DISPLAY_MODE_SLEEPER_CELL:
      s = SLEEPER_CELL_LCD_NAME;
//    s = "SLEEPER CELL";
      break;
      
    case DISPLAY_MODE_CUCKOO:
      s = CUCKOO_LCD_NAME;
//    s = "CUCKOO";
      break;
      
    case DISPLAY_MODE_METER_STICK:
      s = "METER STICK";
      break;
     
    case DISPLAY_MODE_SET_CONTRAST:
      s = "SET CONTRAST";
      break;
      
    case DISPLAY_MODE_SET_TIME:
      s = "SET TIME";
      break;
            
    default:
      s = "??????";
      break;
  }
  
  //
  //display the name of the owner on the screen
  //
  LCDPrintCenteredString(SCULPTURE_LCD_NAME,0);

  //
  // display the mode name on the screen
  //
  LCDPrintCenteredString(s, 2);

  //
  // blank the LCD lines used by the modes
  //
  LCDPrintCenteredString(" ", 3);
  LCDPrintCenteredString(" ", 4);
}



//
// display the time on the LCD
//
void DisplayTimeOnLCD()
{
  byte second;
  byte minute;
  byte hour;
  char *AMPMString;
  
  //
  // read the real time clock to get the current time
  //
  RTCGetTime(&hour, &minute, &second);
  
  //
  // convert 24 hour time to 12 hour
  //
  if (hour >= 12)
    AMPMString = " PM";
  else
    AMPMString = " AM";

  if (hour == 0)
    hour = 12;

  if (hour > 12)
    hour -= 12;
 
  //
  // display time on the LCD
  //
  LCDSetCursorXY(8, 5);
  LCDPrintUnsignedIntWithPadding(hour, 2, ' ');
  LCDPrintCharacter(':');
  
  LCDPrintUnsignedIntWithPadding(minute, 2, '0');
  LCDPrintCharacter(':');
  
  LCDPrintUnsignedIntWithPadding(second, 2, '0');
  
  LCDPrintString(AMPMString);
}



// ---------------------------------------------------------------------------------
//              Background Processing - Timer 3 Interrupt Service Routine 
// ---------------------------------------------------------------------------------

//
// initialize background processing
//
void BackgroundProcessingInitialize()
{
  //
  // configure Timer3 to interrupt every 10 ms
  //
  cli();                               // disable interrupts during configuration
  TCCR3A = 0;                          // clear all bits in Timer/Counter Control Reg A
  TCCR3B = 0;                          // clear all bits in Timer/Counter Control Reg B
  TCCR3B |= (1 << WGM32);              // enable CTC mode (Clear Timer on Compare Match)
  TCCR3B |= ((1 << CS30) | (1 << CS31));  // set the prescaler to divide by 64 giving a 250Khz count rate
  TCNT3  = 0;                          // initialize the counter to 0
  OCR3A = 2499;                        // set compare register to count to 2500-1, giving a 100Hz interrupt
  TIMSK3 |= (1 << OCIE3A);             // enable the interrupt when compare register matches
}



//
// the background process called every 10ms (interrupt service routine for Timer3)
//
ISR(TIMER3_COMPA_vect)
{
  //
  // enable global interrupts so faster interrupts can happen while processing background tasks
  //
  sei(); 

//digitalWrite(TEST_D9_PIN, HIGH);


  //
  // update the backlight LEDs as they transition from one color to the next
  //
  backlightTransition();
   
  //
  // update the desired disk velocities as they transition from one speed to the next
  //
  diskVelocitiesTransition();
  
  //
  // undate the motor servos 
  //
  motorProportionalIntegralControl1();
  motorProportionalIntegralControl2();
 
  
//digitalWrite(TEST_D9_PIN, LOW);
}



// ---------------------------------------------------------------------------------
//                         Outer & Inner Rotating Disk Functions
// ---------------------------------------------------------------------------------

//
// global variables by the motors for transition from one speed to another
//
float diskVelocitiesCurrentSpeedOuter;
float diskVelocitiesTransitionInitialSpeedOuter;
float diskVelocitiesTransitionDeltaSpeedOuter;
float diskVelocitiesTransitionFinalSpeedOuter;

float diskVelocitiesCurrentSpeedInner;
float diskVelocitiesTransitionInitialSpeedInner;
float diskVelocitiesTransitionDeltaSpeedInner;
float diskVelocitiesTransitionFinalSpeedInner;

long diskVelocitiesTransitionDurationMS;
unsigned long diskVelocitiesTransitionStartTimeMS;
unsigned long diskVelocitiesTransitionFinishTimeMS;
bool diskVelocitiesTransitionCompleteFlg;


//
// motor constants
//
const int DIRECTION_CW = LOW;
const int DIRECTION_CCW = HIGH;


// ---------------------------------------------------------------------------------

//
// initialize the disk velocty functions
//
void diskVelocitiesInitialize(void)
{
  diskVelocitiesSet(0, 0);
  diskVelocitiesTransitionCompleteFlg = true;
}



//
// start a transition from the current disk velocities to a new ones over a period of time
//   Enter: outerDiskVelocityInRPM = velocity to transition to for the outer disk in RPM, negative value goes counter counter-clockwise
//          innerDiskVelocityInRPM = velocity to transition to for the inner disk in RPM, negative value goes counter counter-clockwise
//          TransitionDurationMS = number of milliseconds for the transition (1 - 60000)
//
void diskVelocitiesStartTransition(float outerDiskVelocityInRPM, float innerDiskVelocityInRPM, long TransitionDurationMS)
{
  unsigned long startTimeMS;
  unsigned long finishTimeMS;

  diskVelocitiesTransitionCompleteFlg = true;

  startTimeMS = millis();
  
  finishTimeMS = startTimeMS + (unsigned long) TransitionDurationMS;
  if (finishTimeMS < startTimeMS)
    finishTimeMS = startTimeMS;
  
  
  //
  // make sure the new velocities meet the minimums
  //
  if ((outerDiskVelocityInRPM >= -0.2) && (outerDiskVelocityInRPM <= 0.2))
    outerDiskVelocityInRPM = 0.0;
  else
  {
    if ((outerDiskVelocityInRPM > 0.1) && (outerDiskVelocityInRPM < MINIMUM_VELOCITY_IN_RPM))
      outerDiskVelocityInRPM = MINIMUM_VELOCITY_IN_RPM;

    if ((outerDiskVelocityInRPM < -0.1) && (-outerDiskVelocityInRPM < MINIMUM_VELOCITY_IN_RPM))
      outerDiskVelocityInRPM = -MINIMUM_VELOCITY_IN_RPM;
  }
    
  if ((innerDiskVelocityInRPM >= -0.2) && (innerDiskVelocityInRPM <= 0.2))
    innerDiskVelocityInRPM = 0.0;
  else
  {
    if ((innerDiskVelocityInRPM > 0.1) && (innerDiskVelocityInRPM < MINIMUM_VELOCITY_IN_RPM))
      innerDiskVelocityInRPM = MINIMUM_VELOCITY_IN_RPM;

    if ((innerDiskVelocityInRPM < -0.1) && (-innerDiskVelocityInRPM < MINIMUM_VELOCITY_IN_RPM))
      innerDiskVelocityInRPM = -MINIMUM_VELOCITY_IN_RPM;
  }

  //
  // remember what the final velocities will be
  //
  diskVelocitiesTransitionFinalSpeedOuter = outerDiskVelocityInRPM;
  diskVelocitiesTransitionFinalSpeedInner = innerDiskVelocityInRPM;
  

  //
  // start the transition using the disk's current velocity
  //
  diskVelocitiesTransitionInitialSpeedOuter = diskVelocitiesCurrentSpeedOuter;  
  diskVelocitiesTransitionInitialSpeedInner = diskVelocitiesCurrentSpeedInner;


  //
  // if the disk is stopped now, set the initial velocity to a minimum for a 
  // smoother startup
  //
  if (diskVelocitiesTransitionInitialSpeedOuter == 0)
  {
    if (outerDiskVelocityInRPM > 0.1)
      diskVelocitiesTransitionInitialSpeedOuter = MINIMUM_VELOCITY_IN_RPM;

    if (outerDiskVelocityInRPM < -0.1)
      diskVelocitiesTransitionInitialSpeedOuter = -MINIMUM_VELOCITY_IN_RPM;
  }

  if (diskVelocitiesTransitionInitialSpeedInner == 0)
  {
    if (innerDiskVelocityInRPM > 0.1)
      diskVelocitiesTransitionInitialSpeedInner = MINIMUM_VELOCITY_IN_RPM;

    if (innerDiskVelocityInRPM < -0.1)
      diskVelocitiesTransitionInitialSpeedInner = -MINIMUM_VELOCITY_IN_RPM;
  }


  //
  // if the final velocity of the disk is stopped, but its moving now, set the final 
  // velocity to a minimum for a smoother slow down
  //
  if (outerDiskVelocityInRPM == 0)
  {
    if (diskVelocitiesTransitionInitialSpeedOuter > MINIMUM_VELOCITY_IN_RPM)
      outerDiskVelocityInRPM = MINIMUM_VELOCITY_IN_RPM;
      
    if (diskVelocitiesTransitionInitialSpeedOuter < -MINIMUM_VELOCITY_IN_RPM)
      outerDiskVelocityInRPM = -MINIMUM_VELOCITY_IN_RPM;
  }
  
  if (innerDiskVelocityInRPM == 0)
  {
    if (diskVelocitiesTransitionInitialSpeedInner > MINIMUM_VELOCITY_IN_RPM)
      innerDiskVelocityInRPM = MINIMUM_VELOCITY_IN_RPM;
      
    if (diskVelocitiesTransitionInitialSpeedInner < -MINIMUM_VELOCITY_IN_RPM)
      innerDiskVelocityInRPM = -MINIMUM_VELOCITY_IN_RPM;
  }
  

  //
  // determine how much the velocity will change over time
  //
  diskVelocitiesTransitionDeltaSpeedOuter = outerDiskVelocityInRPM - diskVelocitiesTransitionInitialSpeedOuter;
  
  diskVelocitiesTransitionDeltaSpeedInner = innerDiskVelocityInRPM - diskVelocitiesTransitionInitialSpeedInner;
 
  cli();
  diskVelocitiesTransitionDurationMS = TransitionDurationMS;
  diskVelocitiesTransitionStartTimeMS = startTimeMS;
  diskVelocitiesTransitionFinishTimeMS = finishTimeMS;
  sei();

  diskVelocitiesTransitionCompleteFlg = false;
}



//
// check if the disk velocities transistion has finished
//  Exit:  true returned if finished, false returned if not finished
//
bool diskVelocitiesTransitionIsFinished()
{
  return(diskVelocitiesTransitionCompleteFlg);
}



//
// transition the disk velocities, this must be called every 20ms or so
//
void diskVelocitiesTransition()
{
  unsigned long currentTime;
  long elapsedTime;
  float outerDiskVelocityInRPM;
  float innerDiskVelocityInRPM;

  //
  // check if doing a transition, if not return
  //
  if (diskVelocitiesTransitionCompleteFlg)
    return;
  
  //
  // doing a transition, now check if the period has ended
  //
  currentTime = millis();
  elapsedTime = (long) (currentTime - diskVelocitiesTransitionStartTimeMS);
  if (currentTime >= diskVelocitiesTransitionFinishTimeMS)
  {
    //
    // transition has finished, set the final velocities
    //
    diskVelocitiesSet(diskVelocitiesTransitionFinalSpeedOuter, diskVelocitiesTransitionFinalSpeedInner);
    diskVelocitiesTransitionCompleteFlg = true;
    return;
  }


 //
 // determine the velocities for this point in time during the transition
 //
 outerDiskVelocityInRPM = diskVelocitiesTransitionInitialSpeedOuter + 
   (diskVelocitiesTransitionDeltaSpeedOuter * (float) elapsedTime) / (float) diskVelocitiesTransitionDurationMS;

 innerDiskVelocityInRPM = diskVelocitiesTransitionInitialSpeedInner + 
   (diskVelocitiesTransitionDeltaSpeedInner * (float) elapsedTime) / (float) diskVelocitiesTransitionDurationMS;
      
 diskVelocitiesSet(outerDiskVelocityInRPM, innerDiskVelocityInRPM);
}



//
// set the inner and outer disk rotation velocities
//   Enter: outerDiskVelocityInRPM = new velocity of outer disk in RPM, negative value goes counter counter-clockwise
//          innerDiskVelocityInRPM = new velocity of inner disk in RPM, negative value goes counter counter-clockwise
//
void diskVelocitiesSet(float outerDiskVelocityInRPM, float innerDiskVelocityInRPM)
{ 
 int motorVelocity;
  
  //
  // scale the outer disk RPM speed to the motor RPM speed
  //
  diskVelocitiesCurrentSpeedOuter = outerDiskVelocityInRPM;
  motorVelocity = outerDiskVelocityInRPM * GEAR_REUCTION_TO_FINAL_STAGE;

  //
  // determine the direction and speed for the motor
  //
  if (motorVelocity >= 0)
    motorSetSpeedAndDirection1(motorVelocity, DIRECTION_CCW);
  else
    motorSetSpeedAndDirection1(-motorVelocity, DIRECTION_CW);
  
  //
  // scale the inner disk RPM speed to the motor RPM speed
  //
  diskVelocitiesCurrentSpeedInner = innerDiskVelocityInRPM;
  motorVelocity = innerDiskVelocityInRPM * GEAR_REUCTION_TO_FINAL_STAGE;

  //
  // determine the direction and speed for the motor
  //
  if (motorVelocity >= 0)
    motorSetSpeedAndDirection2(motorVelocity, DIRECTION_CCW);
  else
    motorSetSpeedAndDirection2(-motorVelocity, DIRECTION_CW);
}


// ---------------------------------------------------------------------------------
//                                  Motor Functions
// ---------------------------------------------------------------------------------

//
// global variables by the motors for servoing
//
int motorDesiredSpeedInRPM1;
int motorDesiredDirection1;
int motorMeasuredRPM1;
int motorIntegratedSpeedError1;

int motorDesiredSpeedInRPM2;
int motorDesiredDirection2;
int motorMeasuredRPM2;
int motorIntegratedSpeedError2;


//
// global variables used by the tachometer ISR
//
unsigned long motorTachMicrosecondsBetweenLines1;
unsigned long motorTachTimeOfLastMeasurement1;
unsigned long motorTachMicrosecondsBetweenLines2;
unsigned long motorTachTimeOfLastMeasurement2;

// ---------------------------------------------------------------------------------

//
// initialize the motors
//
void motorInitialise(void)
{
  //
  // setup the IO pins
  //
  pinMode(MOTOR_DIRECTION_1_PIN, OUTPUT);
  pinMode(MOTOR_PWM_1_PIN, OUTPUT);
  pinMode(MOTOR_TACHOMETER_1_PIN, INPUT);

  pinMode(MOTOR_DIRECTION_2_PIN, OUTPUT); 
  pinMode(MOTOR_PWM_2_PIN, OUTPUT);
  pinMode(MOTOR_TACHOMETER_2_PIN, INPUT);


  //
  // initially turn the motors off
  //
  motorSetPWMPower1(0, DIRECTION_CW);
  motorSetPWMPower2(0, DIRECTION_CW);


  //
  // initialize global vars used by the motors
  //
  motorDesiredSpeedInRPM1 = 0;
  motorDesiredDirection1 = DIRECTION_CW;
  motorIntegratedSpeedError1 = 0L;
  
  motorDesiredSpeedInRPM2 = 0;
  motorDesiredDirection2 = DIRECTION_CW;
  motorIntegratedSpeedError2 = 0L;


  //
  // change the prescaler for timer 1 (used by the motor PWM) to increase the PWM frequency
  // to 31250Hz so that it can not be heard  (1=31250Hz, 2=3906Hz, 3=488Hz, 4=122Hz, 5=30.5Hz)
  //
  TCCR1B = (TCCR1B & B11111000) | 1;                // set Timer1 prescaler so PWM frequency is 31.2 kHz


  //
  // setup the tachometers to interrupt on the rising edge
  //
  attachInterrupt(4, motorTachometer1ISR, RISING);    // interrupt 4 = D19 = tachometer1
  attachInterrupt(5, motorTachometer2ISR, RISING);    // interrupt 5 = D18 = tachometer2
}


  
//
// set desired speed and direction for motor 1 in RPM, the motor will servo to this speed
//   Enter: desiredMotorSpeedInRPM = desired motor speed in RPM to servo to
//          desiredDirection = motor direction: DIRECTION_CW or DIRECTION_CCW
//
void motorSetSpeedAndDirection1(int desiredMotorSpeedInRPM, int desiredDirection)
{
  cli();                             // disable interrupts 
  motorDesiredSpeedInRPM1 = desiredMotorSpeedInRPM;
  motorDesiredDirection1 = desiredDirection;
  sei();                             // enable interrupts
}


//
// motor 1 servo using proportional-integral control
//   Enter: motorDesiredSpeedInRPM1 = desired motor speed in RPM to servo to
//          desiredDirection1 = desired motor direction: DIRECTION_CW or DIRECTION_CCW
//
void motorProportionalIntegralControl1()
{
  int measuredMotorRPM;
  int speedError;
  int motorPower;

  //
  // read the motor's speed and determine the error in speed
  //
  measuredMotorRPM = motorReadRPM1();
  speedError = motorDesiredSpeedInRPM1 - measuredMotorRPM;

  //
  // integrate the speed error
  //
  motorIntegratedSpeedError1 += speedError;
  if (motorIntegratedSpeedError1 > MOTOR_KINT_SPEED_ERROR_MAX)
    motorIntegratedSpeedError1 = MOTOR_KINT_SPEED_ERROR_MAX;
  if (motorIntegratedSpeedError1 < -MOTOR_KINT_SPEED_ERROR_MAX)
    motorIntegratedSpeedError1 = -MOTOR_KINT_SPEED_ERROR_MAX;
  
  //
  // compute power to motor
  //
  motorPower = (speedError * MOTOR_KP_PROP_CONTROL) / 10;
  motorPower += (motorIntegratedSpeedError1 / MOTOR_KI_PROP_INT_CONTROL);

    
  //
  // output power and direction to motor
  //
  motorSetPWMPower1(motorPower, motorDesiredDirection1);
}


void motorZeroIntergralTerms()
{
  motorIntegratedSpeedError1 = 0;
  motorIntegratedSpeedError2 = 0;
}
  


//
// measure the RPM of motor 1 using the tachometer
//
int motorReadRPM1()
{ 
  unsigned long currentTime;
  unsigned long timeSinceLastTachUpdate;
  unsigned long tachTimeBetweenLines;
  
  currentTime = micros();
  
  cli();
  tachTimeBetweenLines = motorTachMicrosecondsBetweenLines1;
  timeSinceLastTachUpdate = currentTime - motorTachTimeOfLastMeasurement1;
  sei();

  //
  // check if the tach is not moving
  //
  if (timeSinceLastTachUpdate > 50000L)
    motorMeasuredRPM1 = 0;
    
  //
  // check if the tack is moving too fast
  //
  else if (tachTimeBetweenLines < 10)
    motorMeasuredRPM1 = 0;
  
  //
  // return the speed in RPM
  //
  else
    motorMeasuredRPM1 = (int)((1000000 * 60L) / (tachTimeBetweenLines * LINES_ON_TACHOMETER_DISK));

  return(motorMeasuredRPM1);
}



//
// set power to motor 1
//  Enter:  motorPWM = power to motor (0 = 0%, 255 = 100%)
//          motorDirection = motor direction: DIRECTION_CW or DIRECTION_CCW
//
void motorSetPWMPower1(int motorPWM, int motorDirection)
{
  //
  // clip PWM value to minimum and maximum values
  //
  if (motorPWM < 0)
    motorPWM = 0;
  
  if (motorPWM > MOTOR_MAX_PWM)
    motorPWM = MOTOR_MAX_PWM;
    
  //
  // output power and direction to motor
  //
  digitalWrite(MOTOR_DIRECTION_1_PIN, motorDirection);
  analogWrite(MOTOR_PWM_1_PIN, 255 - motorPWM);          // PWM signal inverted so subtract from 255
}
  
  

//
// set desired speed and direction for motor 2 in RPM, the motor will servo to this speed
//   Enter: desiredMotorSpeedInRPM = desired motor speed in RPM to servo to
//          desiredDirection = motor direction: DIRECTION_CW or DIRECTION_CCW
//
void motorSetSpeedAndDirection2(int desiredMotorSpeedInRPM, int desiredDirection)
{
  cli();                             // disable interrupts 
  motorDesiredSpeedInRPM2 = desiredMotorSpeedInRPM;
  motorDesiredDirection2 = desiredDirection;
  sei();                             // enable interrupts
}


//
// motor 2 servo using proportional-integral control
//   Enter: motorDesiredSpeedInRPM2 = desired motor speed in RPM to servo to
//          desiredDirection2 = desired motor direction: DIRECTION_CW or DIRECTION_CCW
//
void motorProportionalIntegralControl2()
{
  int measuredMotorRPM;
  int speedError;
  int motorPower;

  //
  // read the motor's speed and determine the error in speed
  //
  measuredMotorRPM = motorReadRPM2();
  speedError = motorDesiredSpeedInRPM2 - measuredMotorRPM;

  //
  // integrate the speed error
  //
  motorIntegratedSpeedError2 += speedError;
  if (motorIntegratedSpeedError2 > MOTOR_KINT_SPEED_ERROR_MAX)
    motorIntegratedSpeedError2 = MOTOR_KINT_SPEED_ERROR_MAX;
  if (motorIntegratedSpeedError2 < -MOTOR_KINT_SPEED_ERROR_MAX)
    motorIntegratedSpeedError2 = -MOTOR_KINT_SPEED_ERROR_MAX;
  
  //
  // compute power to motor
  //
  motorPower = (speedError * MOTOR_KP_PROP_CONTROL) / 10;
  motorPower += (motorIntegratedSpeedError2 / MOTOR_KI_PROP_INT_CONTROL);

    
  //
  // output power and direction to motor
  //
  motorSetPWMPower2(motorPower, motorDesiredDirection2);
}



//
// measure the RPM of motor 2 using the tachometer
//
int motorReadRPM2()
{ 
  unsigned long currentTime;
  unsigned long timeSinceLastTachUpdate;
  unsigned long tachTimeBetweenLines;
  
  currentTime = micros();
  
  cli();
  tachTimeBetweenLines = motorTachMicrosecondsBetweenLines2;
  timeSinceLastTachUpdate = currentTime - motorTachTimeOfLastMeasurement2;
  sei();
  
  //
  // check if the tach is not moving
  //
  if (timeSinceLastTachUpdate > 25000L)
    motorMeasuredRPM2 = 0;
    
  //
  // check if the tack is moving too fast
  //
  else if (tachTimeBetweenLines < 10)
    motorMeasuredRPM2 = 0;
  
  //
  // return the speed in RPM
  //
  else
    motorMeasuredRPM2 = (int)((1000000 * 60L) / (tachTimeBetweenLines * LINES_ON_TACHOMETER_DISK));

  return(motorMeasuredRPM2);
}



//
// set power to motor 2
//  Enter:  motorPWM = power to motor (0 = 0%, 255 = 100%)
//          motorDirection = motor direction: DIRECTION_CW or DIRECTION_CCW
//
void motorSetPWMPower2(int motorPWM, int motorDirection)
{
  //
  // clip PWM value to minimum and maximum values
  //
  if (motorPWM < 0)
    motorPWM = 0;
  
  if (motorPWM > MOTOR_MAX_PWM)
    motorPWM = MOTOR_MAX_PWM;
    
  //
  // output power and direction to motor
  //
  digitalWrite(MOTOR_DIRECTION_2_PIN, motorDirection);
  analogWrite(MOTOR_PWM_2_PIN, 255 - motorPWM);          // PWM signal inverted so subtract from 255
}



//
// interrupt service routine for Tachometer 1
//
void motorTachometer1ISR()
{
  unsigned long newTime;

  newTime = micros();
  motorTachMicrosecondsBetweenLines1 = newTime - motorTachTimeOfLastMeasurement1;
  motorTachTimeOfLastMeasurement1 = newTime;
}



//
// interrupt service routine for Tachometer 2
//
void motorTachometer2ISR()
{
  unsigned long newTime;

  newTime = micros();  
  motorTachMicrosecondsBetweenLines2 = newTime - motorTachTimeOfLastMeasurement2;
  motorTachTimeOfLastMeasurement2 = newTime;
}


// ---------------------------------------------------------------------------------
//                           Background Lighting Functions
// ---------------------------------------------------------------------------------


//
// global variables by the backlight
//
int backlightCurrentRed;
int backlightTransistionInitialRed;
int backlightTransistionDeltaRed;

int backlightCurrentGreen;
int backlightTransistionInitialGreen;
int backlightTransistionDeltaGreen;

int backlightCurrentBlue;
int backlightTransistionInitialBlue;
int backlightTransistionDeltaBlue;

long backlightTransistionDurationMS;
unsigned long backlightTransistionStartTimeMS;
unsigned long backlightTransistionFinishTimeMS;
bool backlightTransitionCompleteFlg;


//
// table normalize for eye nonlinearity with the function: x ^ 1.8
//
const int LEDTable[] PROGMEM = {
0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 
2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 
6, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 10, 11, 11, 12, 12, 
13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 21, 
21, 22, 22, 23, 24, 24, 25, 26, 26, 27, 28, 28, 29, 30, 30, 31, 
32, 32, 33, 34, 35, 35, 36, 37, 38, 38, 39, 40, 41, 41, 42, 43, 
44, 45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 53, 54, 55, 56, 57, 
58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 
74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 86, 87, 88, 89, 90, 
91, 92, 93, 95, 96, 97, 98, 99, 100, 102, 103, 104, 105, 107, 108, 109, 
110, 111, 113, 114, 115, 116, 118, 119, 120, 122, 123, 124, 126, 127, 128, 129, 
131, 132, 134, 135, 136, 138, 139, 140, 142, 143, 145, 146, 147, 149, 150, 152, 
153, 154, 156, 157, 159, 160, 162, 163, 165, 166, 168, 169, 171, 172, 174, 175, 
177, 178, 180, 181, 183, 184, 186, 188, 189, 191, 192, 194, 195, 197, 199, 200, 
202, 204, 205, 207, 208, 210, 212, 213, 215, 217, 218, 220, 222, 224, 225, 227, 
229, 230, 232, 234, 236, 237, 239, 241, 243, 244, 246, 248, 250, 251, 253, 255
};

const int LEDTableLength = sizeof(LEDTable) / sizeof(int);


// ---------------------------------------------------------------------------------

//
// initialize the backlight LEDs
//
void backlightInitialize(void)
{
  //
  // setup the IO pins
  //
  pinMode(BACKLIGHT_RED_PIN, OUTPUT);
  pinMode(BACKLIGHT_GREEN_PIN, OUTPUT);
  pinMode(BACKLIGHT_BLUE_PIN, OUTPUT);
  
  //
  // start with all LEDs off
  //
  backlightSetColor(0, 0, 0);
  
  backlightTransitionCompleteFlg = true;
}



//
// start a transition of the backlight from current color to a new one over a period of time
//  Enter:  color = rgb color 
//          TransistionDurationMS = number of milliseconds for the transition (1 - 60000)
//
void backlightStartTransitionRGBColor(COLOR_ENTRY color, long TransistionDurationMS)
{
  backlightStartTransition(color.red, color.green, color.blue, TransistionDurationMS);
}



//
// start a transition of the backlight from current color to a new one over a period of time
//  Enter:  red = new value for red LED brightness (0 - 255)
//          green = new value for green LED brightness (0 - 255)
//          blue = new value for blue LED brightness (0 - 255)
//          TransistionDurationMS = number of milliseconds for the transition (1 - 60000)
//
void backlightStartTransition(int red, int green, int blue, long TransistionDurationMS)
{
  unsigned long startTimeMS;
  unsigned long finishTimeMS;

  backlightTransitionCompleteFlg = true;

  startTimeMS = millis();
  
  finishTimeMS = startTimeMS + (unsigned long) TransistionDurationMS;
  if (finishTimeMS < startTimeMS)
    finishTimeMS = startTimeMS;

  backlightTransistionInitialRed = backlightCurrentRed;
  backlightTransistionDeltaRed = red - backlightCurrentRed;

  backlightTransistionInitialGreen = backlightCurrentGreen;
  backlightTransistionDeltaGreen = green - backlightCurrentGreen;

  backlightTransistionInitialBlue = backlightCurrentBlue;
  backlightTransistionDeltaBlue = blue - backlightCurrentBlue;

  cli();
  backlightTransistionDurationMS = TransistionDurationMS;
  backlightTransistionStartTimeMS = startTimeMS;
  backlightTransistionFinishTimeMS = finishTimeMS;
  sei();

  backlightTransitionCompleteFlg = false;
}



//
// check if the backlight transistion has finished
//  Exit:  true returned if finished, false returned if not finished
//
bool backlightTransitionIsFinished()
{
  return(backlightTransitionCompleteFlg);
}



//
// transition the backlight, this must be called every 20ms or so
//
void backlightTransition()
{
  unsigned long currentTime;
  long elapsedTime;
  int red;
  int green;
  int blue;

  //
  // check if doing a transition, if not return
  //
  if (backlightTransitionCompleteFlg)
    return;
  
  //
  // doing a transition, now check if the period has ended
  //
  currentTime = millis();
  elapsedTime = (long) (currentTime - backlightTransistionStartTimeMS);
  if (currentTime >= backlightTransistionFinishTimeMS)
  {
    //
    // transition has finished, set the final colors
    //
    backlightSetColor(
      backlightTransistionInitialRed + backlightTransistionDeltaRed, 
      backlightTransistionInitialGreen + backlightTransistionDeltaGreen,
      backlightTransistionInitialBlue + backlightTransistionDeltaBlue);
      backlightTransitionCompleteFlg = true;

    return;
  }


 //
 // determine the colors for this point in time during the transition
 //
 red = backlightTransistionInitialRed + 
   (int) ((((long) backlightTransistionDeltaRed) * elapsedTime) / backlightTransistionDurationMS);

 green = backlightTransistionInitialGreen + 
   (int) ((((long) backlightTransistionDeltaGreen) * elapsedTime) / backlightTransistionDurationMS);
   
 blue = backlightTransistionInitialBlue + 
   (int) ((((long) backlightTransistionDeltaBlue) * elapsedTime) / backlightTransistionDurationMS);
   
  backlightSetColor(red, green, blue);
}



////
//// set backlight color and brightness
////  Enter:  red = red LED brightness (0 - 255)
////          green = green LED brightness (0 - 255)
////          blue = blue LED brightness (0 - 255)
////
//void backlightSetColor(int red, int green, int blue)
//{
//  if ((unsigned int) red > 255) red = 255;
//  analogWrite(BACKLIGHT_RED_PIN, red);
//  backlightCurrentRed = red;
//  
//  if ((unsigned int) green > 255) green = 255;
//  analogWrite(BACKLIGHT_GREEN_PIN, green);
//  backlightCurrentGreen = green;
//  
//  if ((unsigned int) blue > 255) blue = 255;
//  analogWrite(BACKLIGHT_BLUE_PIN, blue);
//  backlightCurrentBlue = blue;
//}



//
// set backlight color and brightness, normalizing to make fading more linear to the human eye
//  Enter:  red = red LED brightness (0 - 255)
//          green = green LED brightness (0 - 255)
//          blue = blue LED brightness (0 - 255)
//
void backlightSetColor(int red, int green, int blue)
{
  if ((unsigned int) red > 255) red = 255;
  analogWrite(BACKLIGHT_RED_PIN, pgm_read_word(&LEDTable[red]));
  backlightCurrentRed = red;
  
  if ((unsigned int) green > 255) green = 255;
  analogWrite(BACKLIGHT_GREEN_PIN, pgm_read_word(&LEDTable[green]));
  backlightCurrentGreen = green;
  
  if ((unsigned int) blue > 255) blue = 255;
  analogWrite(BACKLIGHT_BLUE_PIN, pgm_read_word(&LEDTable[blue]));
  backlightCurrentBlue = blue;
}


// ---------------------------------------------------------------------------------
//                           Background Strobe Functions
// ---------------------------------------------------------------------------------

//
// NOTE: THE STROBE MODE IS NO LONGER USED.  I'M KEEPING THE CODE BECAUSE IT MIGHT BE 
// USEFUL IF I WANT TO DEVELOP A CONTINUOUSLY CHANGEING "RAINBOW" MODE FOR THE BACKLIGHT
//

//
// global variables by the strobe
//
unsigned int strobeOnTimeMS;
unsigned int strobeOffTimeMS;
unsigned int nextStobeEventTime;
int strobeState;


// ---------------------------------------------------------------------------------

//
// initialize the strobe LEDs
//
void strobeInitialize(void)
{  
  //
  // configure Timer2 to interrupt every 1 ms
  //    Note: The largest prescaler for Timer2 is 64: counting at 250KHz
  //    Counting to 250 results in a 1000Hz interrupt rate
  //
  cli();                             // disable interrupts during configuration
  TCCR2A = 0;                        // clear all bits in Timer/Counter Control Reg A
  TCCR2B = 0;                        // clear all bits in Timer/Counter Control Reg B
  TCNT2  = 0;                        // initialize the counter to 0
  TCCR2B |= (1 << CS22);             // set the prescaler to divide by 64 giving a 250Khz count rate
  OCR2A = 249;                       // set compare register to count to 250-1, giving a 1KHz interrupt
  TCCR2A |= (1 << WGM21);            // enable CTC mode (Clear Timer on Compare Match)
  TIMSK2 &= ~(1 << OCIE2A);          // start with this interrupt disabled 
}



//
// start the strobe flashing
//  Enter:  strobeOnTime = number of milliseconds the strobe is on
//          strobeOffTime = number of milliseconds the strobe is off
//          
//
void strobeStart(unsigned int strobeOnTime, unsigned int strobeOffTime)
{
  TIMSK2 &= ~(1 << OCIE2A);               // disable the interrupt

  strobeOnTimeMS = strobeOnTime;
  strobeOffTimeMS = strobeOffTime;

  strobeLEDsOnOrOff(true);                // turn on the strobe
  strobeState = true;
 
  nextStobeEventTime =  strobeOnTimeMS;   // set time for next event
  
  analogWrite(BACKLIGHT_RED_PIN, 0);     // make sure the LED PWMs are off
  analogWrite(BACKLIGHT_GREEN_PIN, 0);
  analogWrite(BACKLIGHT_BLUE_PIN, 0);
 
  TIMSK2 |= (1 << OCIE2A);               // enable the interrupt
 }



//
// stop the strobe flashing
//          
//
void strobeStop()
{
  TIMSK2 &= ~(1 << OCIE2A);           // disable the interrupt
  strobeLEDsOnOrOff(false);
  strobeLEDsOnOrOff(false);
}



//
// interrupt service routine for Timer2, called every 1ms, strobe the backlight
//
ISR(TIMER2_COMPA_vect)
{
  //
  // check if it is time for next event
  //
  nextStobeEventTime--;
  if (nextStobeEventTime != 0)
    return;

  
  //
  // time for event, change the state of the strobe
  //
  if (strobeState)
  {
    strobeLEDsOnOrOff(false);
    nextStobeEventTime = strobeOffTimeMS;
    strobeState = false;
  }
  else
  {
    strobeLEDsOnOrOff(true);
    nextStobeEventTime = strobeOnTimeMS;
    strobeState = true;
  }
}


//
// turn the strobe lights on or off
//  Enter:  strobeOn = true to turn on, false to turn off
//
void strobeLEDsOnOrOff(bool strobeOn)
{
  //
  // turn on or off the LEDs, optimised for fast speed
  //
  if (strobeOn)
  {
    bitSet(PORTH, 3);
    bitSet(PORTH, 4);
    bitSet(PORTH, 5);
  }
  else
  {
    bitClear(PORTH, 3);
    bitClear(PORTH, 4);
    bitClear(PORTH, 5);
  }

//  //
//  // turn on or off the LEDs, note: this method is too slow for an ISR
//  //
//  digitalWrite(BACKLIGHT_RED_PIN, strobeOn);
//  digitalWrite(BACKLIGHT_GREEN_PIN, strobeOn);
//  digitalWrite(BACKLIGHT_BLUE_PIN, strobeOn);
}


// ---------------------------------------------------------------------------------
//                                   LCD Functions
// ---------------------------------------------------------------------------------


//
// constants for the LCD display
//
const int LCD_COMMAND_BYTE = LOW;
const int LCD_DATA_BYTE = HIGH;
const int LCD_NUMBER_PIXELS_ACROSS = 84;
const int LCD_NUMBER_PIXELS_DOWN = 48;


//
// ASCII font, 5 x 8 pixels, this font is stored in program memory rather than RAM
//

const int Font[][5] PROGMEM = 
  {
   {0x00, 0x00, 0x00, 0x00, 0x00},     // 20  
   {0x00, 0x00, 0x5f, 0x00, 0x00},     // 21 !
   {0x00, 0x07, 0x00, 0x07, 0x00},     // 22 "
   {0x14, 0x7f, 0x14, 0x7f, 0x14},     // 23 #
   {0x24, 0x2a, 0x7f, 0x2a, 0x12},     // 24 $
   {0x23, 0x13, 0x08, 0x64, 0x62},     // 25 %
   {0x36, 0x49, 0x55, 0x22, 0x50},     // 26 &
   {0x00, 0x05, 0x03, 0x00, 0x00},     // 27 '
   {0x00, 0x1c, 0x22, 0x41, 0x00},     // 28 (
   {0x00, 0x41, 0x22, 0x1c, 0x00},     // 29 )
   {0x14, 0x08, 0x3e, 0x08, 0x14},     // 2a *
   {0x08, 0x08, 0x3e, 0x08, 0x08},     // 2b +
   {0x00, 0x50, 0x30, 0x00, 0x00},     // 2c ,
   {0x08, 0x08, 0x08, 0x08, 0x08},     // 2d -
   {0x00, 0x60, 0x60, 0x00, 0x00},     // 2e .
   {0x20, 0x10, 0x08, 0x04, 0x02},     // 2f /
   {0x3e, 0x51, 0x49, 0x45, 0x3e},     // 30 0
   {0x00, 0x42, 0x7f, 0x40, 0x00},     // 31 1
   {0x42, 0x61, 0x51, 0x49, 0x46},     // 32 2
   {0x21, 0x41, 0x45, 0x4b, 0x31},     // 33 3
   {0x18, 0x14, 0x12, 0x7f, 0x10},     // 34 4
   {0x27, 0x45, 0x45, 0x45, 0x39},     // 35 5
   {0x3c, 0x4a, 0x49, 0x49, 0x30},     // 36 6
   {0x01, 0x71, 0x09, 0x05, 0x03},     // 37 7
   {0x36, 0x49, 0x49, 0x49, 0x36},     // 38 8
   {0x06, 0x49, 0x49, 0x29, 0x1e},     // 39 9
   {0x00, 0x36, 0x36, 0x00, 0x00},     // 3a :
   {0x00, 0x56, 0x36, 0x00, 0x00},     // 3b ;
   {0x08, 0x14, 0x22, 0x41, 0x00},     // 3c <
   {0x14, 0x14, 0x14, 0x14, 0x14},     // 3d =
   {0x00, 0x41, 0x22, 0x14, 0x08},     // 3e >
   {0x02, 0x01, 0x51, 0x09, 0x06},     // 3f ?
   {0x32, 0x49, 0x79, 0x41, 0x3e},     // 40 @
   {0x7e, 0x11, 0x11, 0x11, 0x7e},     // 41 A
   {0x7f, 0x49, 0x49, 0x49, 0x36},     // 42 B
   {0x3e, 0x41, 0x41, 0x41, 0x22},     // 43 C
   {0x7f, 0x41, 0x41, 0x22, 0x1c},     // 44 D
   {0x7f, 0x49, 0x49, 0x49, 0x41},     // 45 E
   {0x7f, 0x09, 0x09, 0x09, 0x01},     // 46 F
   {0x3e, 0x41, 0x49, 0x49, 0x7a},     // 47 G
   {0x7f, 0x08, 0x08, 0x08, 0x7f},     // 48 H
   {0x00, 0x41, 0x7f, 0x41, 0x00},     // 49 I
   {0x20, 0x40, 0x41, 0x3f, 0x01},     // 4a J
   {0x7f, 0x08, 0x14, 0x22, 0x41},     // 4b K
   {0x7f, 0x40, 0x40, 0x40, 0x40},     // 4c L
   {0x7f, 0x02, 0x0c, 0x02, 0x7f},     // 4d M
   {0x7f, 0x04, 0x08, 0x10, 0x7f},     // 4e N
   {0x3e, 0x41, 0x41, 0x41, 0x3e},     // 4f O
   {0x7f, 0x09, 0x09, 0x09, 0x06},     // 50 P
   {0x3e, 0x41, 0x51, 0x21, 0x5e},     // 51 Q
   {0x7f, 0x09, 0x19, 0x29, 0x46},     // 52 R
   {0x46, 0x49, 0x49, 0x49, 0x31},     // 53 S
   {0x01, 0x01, 0x7f, 0x01, 0x01},     // 54 T
   {0x3f, 0x40, 0x40, 0x40, 0x3f},     // 55 U
   {0x1f, 0x20, 0x40, 0x20, 0x1f},     // 56 V
   {0x3f, 0x40, 0x38, 0x40, 0x3f},     // 57 W
   {0x63, 0x14, 0x08, 0x14, 0x63},     // 58 X
   {0x07, 0x08, 0x70, 0x08, 0x07},     // 59 Y
   {0x61, 0x51, 0x49, 0x45, 0x43},     // 5a Z
   {0x00, 0x7f, 0x41, 0x41, 0x00},     // 5b [
   {0x02, 0x04, 0x08, 0x10, 0x20},     // 5c 
   {0x00, 0x41, 0x41, 0x7f, 0x00},     // 5d ]
   {0x04, 0x02, 0x01, 0x02, 0x04},     // 5e ^
   {0x40, 0x40, 0x40, 0x40, 0x40},     // 5f _
   {0x00, 0x01, 0x02, 0x04, 0x00},     // 60 `
   {0x20, 0x54, 0x54, 0x54, 0x78},     // 61 a
   {0x7f, 0x48, 0x44, 0x44, 0x38},     // 62 b
   {0x38, 0x44, 0x44, 0x44, 0x20},     // 63 c
   {0x38, 0x44, 0x44, 0x48, 0x7f},     // 64 d
   {0x38, 0x54, 0x54, 0x54, 0x18},     // 65 e
   {0x08, 0x7e, 0x09, 0x01, 0x02},     // 66 f
   {0x0c, 0x52, 0x52, 0x52, 0x3e},     // 67 g
   {0x7f, 0x08, 0x04, 0x04, 0x78},     // 68 h
   {0x00, 0x44, 0x7d, 0x40, 0x00},     // 69 i
   {0x20, 0x40, 0x44, 0x3d, 0x00},     // 6a j 
   {0x7f, 0x10, 0x28, 0x44, 0x00},     // 6b k
   {0x00, 0x41, 0x7f, 0x40, 0x00},     // 6c l
   {0x7c, 0x04, 0x18, 0x04, 0x78},     // 6d m
   {0x7c, 0x08, 0x04, 0x04, 0x78},     // 6e n
   {0x38, 0x44, 0x44, 0x44, 0x38},     // 6f o
   {0x7c, 0x14, 0x14, 0x14, 0x08},     // 70 p
   {0x08, 0x14, 0x14, 0x18, 0x7c},     // 71 q
   {0x7c, 0x08, 0x04, 0x04, 0x08},     // 72 r
   {0x48, 0x54, 0x54, 0x54, 0x20},     // 73 s
   {0x04, 0x3f, 0x44, 0x40, 0x20},     // 74 t
   {0x3c, 0x40, 0x40, 0x20, 0x7c},     // 75 u
   {0x1c, 0x20, 0x40, 0x20, 0x1c},     // 76 v
   {0x3c, 0x40, 0x30, 0x40, 0x3c},     // 77 w
   {0x44, 0x28, 0x10, 0x28, 0x44},     // 78 x
   {0x0c, 0x50, 0x50, 0x50, 0x3c},     // 79 y
   {0x44, 0x64, 0x54, 0x4c, 0x44},     // 7a z
   {0x00, 0x08, 0x36, 0x41, 0x00},     // 7b {
   {0x00, 0x00, 0x7f, 0x00, 0x00},     // 7c |
   {0x00, 0x41, 0x36, 0x08, 0x00},     // 7d }
   {0x10, 0x08, 0x08, 0x10, 0x08},     // 7e ~
   {0x78, 0x46, 0x41, 0x46, 0x78}      // 7f 
  };

// ---------------------------------------------------------------------------------

//
// initialize the LCD display
//
void LCDInitialise(void)
{
  //
  // setup the IO pins
  //
  pinMode(LCD_CHIP_ENABLE_PIN, OUTPUT);
  pinMode(LCD_RESET_PIN, OUTPUT);
  pinMode(LCD_DATA_CONTROL_PIN, OUTPUT);
  pinMode(LCD_DATA_IN_PIN, OUTPUT);
  pinMode(LCD_CLOCK_PIN, OUTPUT);
  pinMode(LCD_BACKLIGHT_PIN, OUTPUT);

  //
  // reset the display
  //
  digitalWrite(LCD_RESET_PIN, LOW);
  digitalWrite(LCD_RESET_PIN, HIGH);
  
  //
  // configure the display
  //
  LCDWriteCommand(0x21);      // set H bit to program config registers

  LCDWriteCommand(0xC0);      // set LCD Vop (Contrast) (0x80 - 0xff)

  LCDWriteCommand(0x06);      // set LCD temp coefficent (0x04 - 0x07)

  LCDWriteCommand(0x14);      // set LCD bias mode to 1:48 (0x10 - 0x17)
  LCDWriteCommand(0x20);      // clear H bit to access X & Y registers
  LCDWriteCommand(0x0C);      // set "normal mode"
  
  //
  // turn on the LCD backlight
  //
  digitalWrite(LCD_BACKLIGHT_PIN, LOW);
}



//
// print an signed int to the LCD display
//  Enter:  n = number to print 
//
void LCDPrintInt(int n)
{
  if (n < 0)
  {
    LCDPrintCharacter('-');
    n = -n;
  }
  
  LCDPrintUnsignedIntWithPadding(n, 0, ' ');
}



//
// print an unsigned int to the LCD display
//  Enter:  n = number to print 
//
void LCDPrintUnsignedInt(unsigned int n)
{
  LCDPrintUnsignedIntWithPadding(n, 0, ' ');
}



//
// print an unsigned int to the LCD display, pad with blank spaces left of the number if desired
//  Enter:  n = number to print 
//          widthToPrint = total width of characters to print (0 to 5), a value of 0 does not pad
//          padCharacter = character to used for padding (typically a '0' or ' ')
//
void LCDPrintUnsignedIntWithPadding(unsigned int n, byte widthToPrint, char padCharacter)
{
  char buf[6];
  char *str;
  unsigned int m;
  char c;
  int characterCount;

  str = &buf[sizeof(buf) - 1];
  *str = '\0';
  characterCount = 0;

  //
  // convert number to a string of digits
  //
  do 
  {
    m = n;
    n /= 10;
    c = m - (10 * n);
    *--str = c + '0';
    characterCount++;
  } while(n);

  //
  // add padding before the number if desired
  //
  characterCount = widthToPrint - characterCount;
  while (characterCount > 0)
  {
    *--str = padCharacter;
    characterCount--;
  }
  
  //
  // display the string
  //
  LCDPrintString(str);
}



//
// print a string to the LCD display
//  Enter:  s -> a null terminated string 
//          lineNumber = character line (0 - 5, 0 = top row)
//
void LCDPrintCenteredString(char *s, int lineNumber)
{
  int charCount;
  int padding;
  int paddingCount;
  
  //
  // move cursor to the beginning of the line
  //
  LCDSetCursorXY(0, lineNumber);

  //
  // count number of characters in the string
  //
  charCount = 0;
  while(s[charCount] != 0)
    charCount++;

  //
  // blank pixels on left of string
  //
  padding = (LCD_NUMBER_PIXELS_ACROSS - (charCount * 6)) / 2;
  paddingCount = padding;
  while(paddingCount > 0)
  { 
    LCDWriteData(0x00);
    paddingCount--;
  }
  
  //
  // print the string
  //
  LCDPrintString(s);

  //
  // blank pixels on right of string
  //
  padding = LCD_NUMBER_PIXELS_ACROSS - (padding + (charCount * 6));
  paddingCount = padding;
  while(paddingCount > 0)
  { 
    LCDWriteData(0x00);
    paddingCount--;
  }
}



//
// print a string to the LCD display
//  Enter:  s -> a null terminated string 
//
void LCDPrintString(char *s)
{
  //
  // loop, writing one character at a time until the end of the string is reached
  //
  while (*s)
  {
    LCDPrintCharacter(*s++);
  }
}



//
// print one ASCII charater to the display
//  Enter:  c = character to display
//
void LCDPrintCharacter(byte character)
{
  int pixelColumn;
  
  //
  // make sure character is in range of the font table
  //
  if ((character < 0x20) or (character > 0x7f))
    character = 0x20;
  
  //
  // from the character, get the index into the font table
  //
  character -= 0x20;
  
  //
  // write all 5 columns of the character
  //
  for (pixelColumn = 0; pixelColumn < 5; pixelColumn++)
  {
    LCDWriteData(pgm_read_word(&Font[character][pixelColumn]));   
  }
  
  //
  // write a column of blank pixels after the character
  //
  LCDWriteData(0x00);
}



//
// clear the LCD display by writing blank pixels
//
void LCDClearDisplay(void)
{
  int characterColumn;
  int lastCharacterColumn;
  
  //
  // determine how many bytes must be written to clear the display
  //
  lastCharacterColumn = LCD_NUMBER_PIXELS_ACROSS * LCD_NUMBER_PIXELS_DOWN / 8;
  
  //
  // write enough blank pixels to clear the display
  //
  for (int characterColumn = 0; characterColumn < lastCharacterColumn; characterColumn++)
  {
    LCDWriteData(0x00);
  }
}



//
// draw a row of pixels, repeating the same column of 8 pixels across
//  Enter:  X1 = starting pixel column (0 - 83, 0 = left most column)
//          X2 = ending pixel column (0 - 83)
//          lineNumber = character line (0 - 5, 0 = top row)
//          byteOfPixels = byte of pixels to repeat
//
void LCDDrawRowOfPixels(int X1, int X2, int lineNumber, byte byteOfPixels)
{
  int pixelColumn;

  //
  // move to the first pixel
  //
  LCDSetCursorXY(X1, lineNumber);


  //
  // write the pixels
  //
  for (pixelColumn = 0; pixelColumn <= X2-X1; pixelColumn++)
  {
    LCDWriteData(byteOfPixels);   
  }
}



//
// set the coords where the next character will be written to the LCD display
//  Enter:  column = pixel column (0 - 83, 0 = left most column)
//          lineNumber = character line (0 - 5, 0 = top row)
//
void LCDSetCursorXY(int column, int lineNumber)
{
  LCDWriteCommand(0x80 | column);
  LCDWriteCommand(0x40 | lineNumber);
}



//
// set the LCD screen contrast value 
//  Enter:  contrastValue = value to set for the screen's contrast (0 - 127)
//
void LCDSetContrast(int contrastValue)
{
  LCDWriteCommand(0x21);                      // set H bit to program config registers
  LCDWriteCommand(contrastValue + 0x80);      // set LCD Vop (Contrast) (0x80 - 0xff)
  LCDWriteCommand(0x20);      // clear H bit to access X & Y registers
}



//
// write a single command byte to the LCD display
//
void LCDWriteCommand(byte command)
{
  digitalWrite(LCD_DATA_CONTROL_PIN, LCD_COMMAND_BYTE);
  digitalWrite(LCD_CHIP_ENABLE_PIN, LOW);
  shiftOut(LCD_DATA_IN_PIN, LCD_CLOCK_PIN, MSBFIRST, command);
  digitalWrite(LCD_CHIP_ENABLE_PIN, HIGH);
}



//
// write a single data byte to the LCD display
//
//void LCDWriteData(byte data)
//{
//  digitalWrite(LCD_DATA_CONTROL_PIN, LCD_DATA_BYTE);
//  digitalWrite(LCD_CHIP_ENABLE_PIN, LOW);
//  shiftOut(LCD_DATA_IN_PIN, LCD_CLOCK_PIN, MSBFIRST, data);
//  digitalWrite(LCD_CHIP_ENABLE_PIN, HIGH);
//}


//
// write a single data byte to the LCD display
// NOTE: This version of the function is optimised for speed
//
void LCDWriteData(byte data)
{
  byte i;

  //
  // set the "LCD_DATA_CONTROL_PIN" to "Data"
  //
  bitSet(PORTF, 2); 
  
  //
  // enable the chip enable for the display
  //
  bitClear(PORTF, 3);

  //
  // clock the data byte out, starting with the MSB, one bit at a time
  // write to the IO ports directly for a faster speed
  //
  for (i = 0; i < 8; i++)  
  {
    if (data & 0x80)
      bitSet(PORTF, 1); 
    else 
      bitClear(PORTF, 1);
    
    data = data << 1;
    
    bitSet(PORTF, 0);	
    bitClear(PORTF, 0);
  }

  //
  // disable the chip enable for the display
  //
  bitSet(PORTF, 3);
}


// ---------------------------------------------------------------------------------
//                             Real Time Clock Functions
// ---------------------------------------------------------------------------------

//
// constants for the RTC
//
#define DS1307_I2C_ADDRESS 0x68


// ---------------------------------------------------------------------------------

//
// initialize the motors
//
void RTCInitialise(void)
{
  //
  // Initialize communication with the Real Time Clock
  //
  Wire.begin();
}


//
// set the real time clock's time and date
//
void RTCSetTimeAndDate(byte year, byte month, byte dayOfMonth, byte dayOfWeek, byte hour, byte minute, byte second)
{ 
  Wire.beginTransmission(DS1307_I2C_ADDRESS);  // open I2C line in write mode
 
  Wire.write((byte)0x00);                      // set the register pointer to (0x00)
  Wire.write(RTC_DecToBCD(second));            // write seven bytes
  Wire.write(RTC_DecToBCD(minute));
  Wire.write(RTC_DecToBCD(hour));      
  Wire.write(RTC_DecToBCD(dayOfWeek));
  Wire.write(RTC_DecToBCD(dayOfMonth));
  Wire.write(RTC_DecToBCD(month));
  Wire.write(RTC_DecToBCD(year));
  
  Wire.endTransmission();                      // end write mode
}



//
// get the real time clock's time (without the date)
//
void RTCGetTime(byte *hour, byte *minute, byte *second)
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);   // open I2C line in write mode
  Wire.write((byte)0x00);                       // set the register pointer to (0x00)
  Wire.endTransmission();                       // end write rransmission 
 
  Wire.requestFrom(DS1307_I2C_ADDRESS, 3);      // open the I2C line in send mode
 
  *second     = RTC_BCDToDec(Wire.read() & 0x7f); // read 3 bytes of data
  *minute     = RTC_BCDToDec(Wire.read());
  *hour       = RTC_BCDToDec(Wire.read() & 0x3f);  
}



//
// get the real time clock's time and date
//
void RTCGetTimeAndDate(byte *year, byte *month, byte *dayOfMonth, byte *dayOfWeek, byte *hour, byte *minute, byte *second)
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);   // open I2C line in write mode
  Wire.write((byte)0x00);                       // set the register pointer to (0x00)
  Wire.endTransmission();                       // end write rransmission 
 
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);      // open the I2C line in send mode
 
  *second     = RTC_BCDToDec(Wire.read() & 0x7f); // read seven bytes of data
  *minute     = RTC_BCDToDec(Wire.read());
  *hour       = RTC_BCDToDec(Wire.read() & 0x3f);  
  *dayOfWeek  = RTC_BCDToDec(Wire.read());
  *dayOfMonth = RTC_BCDToDec(Wire.read());
  *month      = RTC_BCDToDec(Wire.read());
  *year       = RTC_BCDToDec(Wire.read());
}


//
// Convert normal decimal numbers to binary coded decimal
//
byte RTC_DecToBCD(byte val)
{
  return ((val/10*16) + (val%10));
}
 
 
// 
// Convert binary coded decimal to normal decimal numbers
//
byte RTC_BCDToDec(byte val)
{
  return ((val/16*10) + (val%16));
}


// ---------------------------------------------------------------------------------
//                                Push Button Functions
// ---------------------------------------------------------------------------------

//
// values for State in ButtonsTable
//
const int WAITING_FOR_BUTTON_DOWN = 0;
const int DEBOUNCE_AFTER_BUTTON_DOWN = 1;
const int WAITING_FOR_BUTTON_UP = 2;
const int WAITING_FOR_BUTTON_UP_AFTER_AUTO_REPEAT = 3;
const int DEBOUNCE_AFTER_BUTTON_UP = 4;


//
// delay periods for dealing with buttons in milliseconds
//
const long BUTTON_DEBOUNCE_PERIOD = 30;
const long BUTTON_AUTO_REPEAT_DELAY = 800;
const long BUTTON_AUTO_REPEAT_RATE = 130;


//
// structure for storing the state of a push button
//
typedef struct _BUTTON_TABLE_ENTRY
{
  byte ButtonPinNumber;
  byte State;
  byte ButtonActiveLowFlg;
  unsigned long EventStartTime;
} BUTTON_TABLE_ENTRY;

static BUTTON_TABLE_ENTRY ButtonsTable[BUTTON_TABLE_LAST_ENTRY + 1];


/* ------------------------------------------------------------------------ */

//
// initialize the buttons
//
void buttonsInitialize(void)
{
  BUTTON_TABLE_ENTRY buttonTableEntry;
  
  
  //
  // setup the IO pins
  //
  pinMode(BUTTON_MODE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);

  pinMode(REMOTE_BUTTON_A_PIN, INPUT);
  pinMode(REMOTE_BUTTON_B_PIN, INPUT);
  pinMode(REMOTE_BUTTON_C_PIN, INPUT);
  pinMode(REMOTE_BUTTON_D_PIN, INPUT);


  //
  // initialize the button table for each push button
  //
  buttonTableEntry.State = WAITING_FOR_BUTTON_DOWN;
  buttonTableEntry.ButtonActiveLowFlg = true;

  buttonTableEntry.ButtonPinNumber = BUTTON_UP_PIN;
  ButtonsTable[PUSH_BUTTON_UP] = buttonTableEntry;

  buttonTableEntry.ButtonPinNumber = BUTTON_MODE_PIN;
  ButtonsTable[PUSH_BUTTON_MODE] = buttonTableEntry;

  buttonTableEntry.ButtonPinNumber = BUTTON_DOWN_PIN;
  ButtonsTable[PUSH_BUTTON_DOWN] = buttonTableEntry;

  //
  // initialize the button table for each remote button
  //
  buttonTableEntry.ButtonActiveLowFlg = false;

  buttonTableEntry.ButtonPinNumber = REMOTE_BUTTON_A_PIN;
  ButtonsTable[REMOTE_BUTTON_A] = buttonTableEntry;

  buttonTableEntry.ButtonPinNumber = REMOTE_BUTTON_B_PIN;
  ButtonsTable[REMOTE_BUTTON_B] = buttonTableEntry;

  buttonTableEntry.ButtonPinNumber = REMOTE_BUTTON_C_PIN;
  ButtonsTable[REMOTE_BUTTON_C] = buttonTableEntry;

  buttonTableEntry.ButtonPinNumber = REMOTE_BUTTON_D_PIN;
  ButtonsTable[REMOTE_BUTTON_D] = buttonTableEntry;
}



//
// check for an event from a push button
//		Enter:	buttonIdx = index of which button to test
//		Exit:	event value returned, BUTTON_NO_EVENT returned if no event
//
byte CheckButton(byte buttonIdx)
{
  byte buttonValue;
  unsigned long currentTime;
  
  //
  // read the button, invert polarity if set
  //
  buttonValue = digitalRead(ButtonsTable[buttonIdx].ButtonPinNumber);
  if (ButtonsTable[buttonIdx].ButtonActiveLowFlg)
    buttonValue = !buttonValue;

  //
  // check if nothing is going on (waiting for button to go down and it's not pressed)
  //
  if ((ButtonsTable[buttonIdx].State == WAITING_FOR_BUTTON_DOWN) && (buttonValue == false))
    return(BUTTON_NO_EVENT);                      // return no new event
  
  //
  // something is going on, read the time
  //
  currentTime = millis();		
  
  //
  // check the state that the button was in last
  //
  switch(ButtonsTable[buttonIdx].State)
  {
    //
    // check if waiting for button to go down
    //
    case WAITING_FOR_BUTTON_DOWN:
    {
      if (buttonValue == true)                   // check if button is now down
      {                                          // button is down, start timer
        ButtonsTable[buttonIdx].EventStartTime = currentTime;
        ButtonsTable[buttonIdx].State = DEBOUNCE_AFTER_BUTTON_DOWN;
        return(BUTTON_PUSHED);                  // return button "pressed" event
      }
      break;
    }	
    
    //
    // check if waiting for timer after button has gone down
    //		
    case DEBOUNCE_AFTER_BUTTON_DOWN:
    {                                           // check if it has been up long enough
      if (currentTime >= (ButtonsTable[buttonIdx].EventStartTime + BUTTON_DEBOUNCE_PERIOD))
      {                                         // debouncing period over, start auto repeat timer
        ButtonsTable[buttonIdx].EventStartTime = currentTime;
        ButtonsTable[buttonIdx].State = WAITING_FOR_BUTTON_UP;
      }
      break;
    }

    //
    // check if waiting for button to go back up
    //			
    case WAITING_FOR_BUTTON_UP:
    {
      if (buttonValue == false)                 // check if button is now up
      {                                         // button up, start debounce timer
        ButtonsTable[buttonIdx].EventStartTime = currentTime;
        ButtonsTable[buttonIdx].State = DEBOUNCE_AFTER_BUTTON_UP;
        return(BUTTON_RELEASED);                // return button "released" event
      }
                                                // button still down, check if time to auto repeat
      if (currentTime >= (ButtonsTable[buttonIdx].EventStartTime + BUTTON_AUTO_REPEAT_DELAY))
      {                                         // reset auto repeat timer
        ButtonsTable[buttonIdx].EventStartTime = currentTime;
        ButtonsTable[buttonIdx].State = WAITING_FOR_BUTTON_UP_AFTER_AUTO_REPEAT;
        return(BUTTON_REPEAT);                  // return button "auto repeat" event
      }
      break;
    }

    //
    // check if waiting for button to repeat
    //
    case WAITING_FOR_BUTTON_UP_AFTER_AUTO_REPEAT:
    {
      if (buttonValue == false)                 // check if button is now up
      {                                         // button up, start debounce timer
        ButtonsTable[buttonIdx].EventStartTime = currentTime;
        ButtonsTable[buttonIdx].State = DEBOUNCE_AFTER_BUTTON_UP;
        return(BUTTON_RELEASED);                // return button "released" event
      }
                                                // button still down, check if time to auto repeat
      if (currentTime >= (ButtonsTable[buttonIdx].EventStartTime + BUTTON_AUTO_REPEAT_RATE))
      {                                         // reset auto repeat timer
        ButtonsTable[buttonIdx].EventStartTime = currentTime;
        return(BUTTON_REPEAT);                  // return button "auto repeat" event
      }
      break;
    }			
    
    //
    // check if waiting for debouncing after button has gone up
    //
    case DEBOUNCE_AFTER_BUTTON_UP:
    {                                            // after button goes up, wait before check for down
      if (currentTime >= (ButtonsTable[buttonIdx].EventStartTime + BUTTON_DEBOUNCE_PERIOD))
        ButtonsTable[buttonIdx].State = WAITING_FOR_BUTTON_DOWN;
      break;
    }
  }
  
  return(BUTTON_NO_EVENT);                      // return no new event
}


// ---------------------------------------------------------------------------------
//                               Ultrasonic Functions
// ---------------------------------------------------------------------------------

//
// NOTES: Sound travels at 1130 feet per second, or 340.29 meters per second.
// that results in about 29.4 microseconds per cm.  Round trip is about 58.8 
// microseconds per cm to the object and back.
//
// The transducer needs a high level trigger signal of at least 10us to start
// a measurement.
//
// The Echo pin falls low when an echo is detected.
//
// A minimum of 60ms is recommend from the start of one measurment until the
// start of the next one.
//

//
// measurement constants
//
const long ULTRASONIC_TIMEOUT_PERIOD = 60000;

//
// global variables by the ultrasonics
//
unsigned long ultrasonicStartTime;
unsigned long ultrasonicEchoTime;
bool ultrasonicMeasurementCompleteFlg;

// ---------------------------------------------------------------------------------

//
// initialize the ultrasonics
//
void ultrasonicInitialize(void)
{  
  //
  // setup the IO pins
  //
  pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  
  //
  // start with the trigger off
  //
  digitalWrite(ULTRASONIC_TRIGGER_PIN, 0);
  
  //
  // attach the echo pin to an interrupt service routine
  //
  attachInterrupt(1, ultrasonicEchoISR, FALLING);    // interrupt 1 = D3 = echo signal from ultrasonic
}


//
// start an ultrasonic measurement
//
void ultrasonicStartMeasurement()
{
  ultrasonicMeasurementCompleteFlg = false;         // indicate that the measurement is not complete
  digitalWrite(ULTRASONIC_TRIGGER_PIN, 1);          // trigger ultrasonics to start a measurement
  ultrasonicStartTime = micros();                   // record the time when the measurement was started
  delayMicroseconds(50);                            // delay while trigger signal is on
  digitalWrite(ULTRASONIC_TRIGGER_PIN, 0);          // turn trigger off
}



//
// check if the ultrasonic measurement has finished
//  Exit:  true returned if finished, false returned if not finished
//
bool ultrasonicIsFinished()
{
  //
  // check if the echo has been received
  //
  if (ultrasonicMeasurementCompleteFlg)
    return(true);
    
  //
  // no echo, check for timeout
  //
  if ((micros() - ultrasonicStartTime) > ULTRASONIC_TIMEOUT_PERIOD)
  {
    ultrasonicMeasurementCompleteFlg = true;
    ultrasonicEchoTime = 0L;
    return(true);
  }
  
  //
  // return false, echo not received yet
  //
  return(false);
}


//
// get the results of the last ultrasonic measurement
//  Exit:  distance in centimeters returned, 0 returned if no echo
//
int ultrasonicGetDistanceInCM()
{
  int distanceInCM;

  ultrasonicMeasurementCompleteFlg = false;
  
  //
  // check if the measurement timed out, if so return 0
  //
  if (ultrasonicEchoTime == 0)
    return(0);
  
  //
  // compute the distance to the object in CM
  //
//distanceInCM = ((ultrasonicEchoTime - 950L) / 58L);
  distanceInCM = ((ultrasonicEchoTime - 500L) / 58L);
  if (distanceInCM < 3) distanceInCM = 3;
  
  //
  // return the distance
  //
  return(distanceInCM);
}



//
// interrupt service routine for the ultrasonic echo
//
void ultrasonicEchoISR()
{
  unsigned long echoTime;

  //
  // determine how long in microseconds it took to get the echo
  //
  ultrasonicEchoTime = micros() - ultrasonicStartTime;
  ultrasonicMeasurementCompleteFlg = true;         // indicate that the measurement is complete
}


// -------------------------------------- End --------------------------------------


