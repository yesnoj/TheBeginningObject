#include "core/lv_obj_style.h"
#include "lv_api_map_v8.h"
#include "font/lv_font.h"
#include "misc/lv_area.h"
#include "widgets/msgbox/lv_msgbox.h"
#include <cstddef>
#include "core/lv_obj_tree.h"
#include "core/lv_obj.h"
#include "misc/lv_event.h"
#include "misc/lv_types.h"
#include "misc/lv_style.h"
#include <sys/_stdint.h>
/**
 * @file definitions.h
 *
 */


#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
 
#include <lvgl.h>

/*********************
 *      DEFINES
 *********************/

/********************* SCREEN DEFINES *********************/

/*********************
* Button sizes
*********************/
#define BUTTON_PROCESS_HEIGHT 40
#define BUTTON_PROCESS_WIDTH  95

#define BUTTON_START_HEIGHT 90
#define BUTTON_START_WIDTH  140

#define BUTTON_MBOX_HEIGHT 45
#define BUTTON_MBOX_WIDTH  110

#define BUTTON_POPUP_CLOSE_HEIGHT 30
#define BUTTON_POPUP_CLOSE_WIDTH  30

#define BUTTON_TUNE_HEIGHT 35
#define BUTTON_TUNE_WIDTH  80

#define LOGO_HEIGHT    89
#define LOGO_WIDTH     102



/*********************
*LovyanGFX Parameters 
*********************/

/*Set to your screen resolution*/
#define TFT_WIDTH   320
#define TFT_HEIGHT  480

#define LV_TICK_PERIOD_MS 1
#define LV_TITLE_ANIM_TIME 150
#define LV_TITLE_ANIM_Y (TFT_WIDTH / 20)

#define TFT_HOR_RES   TFT_HEIGHT
#define TFT_VER_RES   TFT_WIDTH

// SD CARD - SPI
#define SDSPI_HOST_ID SPI3_HOST
#define SD_MISO       GPIO_NUM_41 
#define SD_MOSI       GPIO_NUM_2
#define SD_SCLK       GPIO_NUM_42
#define SD_CS         GPIO_NUM_1

#define I2C_SDA 38
#define I2C_SCL 39
#define I2C_INT 40
#define I2C_ADR 0x38

#define LCD_BLK 45

#define LCD_WR   35
#define LCD_RD   48
#define LCD_RS   36
#define LCD_CS   37
#define LCD_RST  -1
#define LCD_BSY  -1

#define LCD_D0   47
#define LCD_D1   21
#define LCD_D2   14
#define LCD_D3   13
#define LCD_D4   12
#define LCD_D5   11
#define LCD_D6   10
#define LCD_D7   9
#define LCD_D8   3
#define LCD_D9   8
#define LCD_D10  16
#define LCD_D11  15
#define LCD_D12  7
#define LCD_D13  6
#define LCD_D14  5
#define LCD_D15  4

/********************* APPLICATION DEFINES *********************/

/*********************
* Image logo
*********************/
LV_IMG_DECLARE(splash_img);


// Assign human-readable names to some common 16-bit color values: Use http://www.barth-dev.de/online/rgb565-color-picker/ to pick color RGB888 code

/*********************
* Some colors
*********************/

#define RED              0xff2600
#define RED_DARK         0x91060a
#define BLUE             0x1d53ab
#define BLUE_DARK        0x123773     
#define GREEN            0x77bb41
#define GREEN_DARK       0x0e6b2c
#define CYAN             0x00fdff
#define ORANGE           0xcc871b
#define ORANGE_DARK      0x6b470e
#define ORANGE_LIGHT     0xfcba03
#define YELLOW           0xf5ec00
#define GREY             0xaaaaaa
#define WHITE            0xffffff
#define BLACK            0x000000
#define LIGHT_BLUE       0x1fd3e0
#define LIGHT_BLUE_DARK  0x16939c


/*********************
* Icon chars
*********************/
char oldCamera_icon [14] = {"\xEF\x82\x83"};
char beuta_icon [14] = {"\xEF\x83\x83"};
char plusplus_icon [14] = {"\xEF\x84\x81"};
char film_icon [14] = {"\xEF\x80\x88"};
char image_icon [14] = {"\xEF\x80\xBE"};

char tabProcess_label [13] = {"Process list"};
char tabProcess_icon [13] = {"\xEF\x85\xA0"};
char tabSetting_label [9] = {"Settings"};
char tabSetting_icon [14] = {"\xEF\x93\xBE"};
char tabTools_label [6] = {"Tools"};
char tabTools_icon [14] = {"\xEF\x9F\x99"};

char filter_icon       [14] = {"\xEF\x87\x9E"};
char funnel_icon       [14] = {"\xEF\x82\xB0"};
char newProcess_icon   [14] = {"\xEF\x83\xBE"};
char preferred_icon    [14] = {"\xEF\x80\x84"};
char colorpalette_icon [14] = {"\xEF\x94\xBF"};
char temp_icon         [14] = {"\xEF\x8B\x89"};
char blackwhite_icon   [14] = {"\xEF\x81\x82"};
char questionMark_icon [14] = {"\xEF\x81\x99"};
char closePopup_icon   [14] = {"\xEF\x80\x8D"};
char play_Icon         [14] = {"\xEF\x81\x8B"};

char save_Icon         [14] = {"\xEF\x83\x87"};
char trash_Icon        [14] = {"\xEF\x8B\xAD"};
char moveUp_Icon       [14] = {"\xEF\x81\xB7"};
char moveDown_Icon     [14] = {"\xEF\x81\xB8"};
char chemical_Icon     [14] = {"\xEF\x83\x83"};
char rinse_Icon         [14] = {"\xEF\x81\x83"};
char multiRinse_Icon   [14] = {"\xEF\x86\xB8"};
char edit_Icon         [14] = {"\xEF\x81\x84"};
char copy_Icon         [14] = {"\xEF\x83\x85"};
char checkStep_Icon    [14] = {"\xEF\x80\x8C"};
char arrowStep_Icon    [14] = {"\xEF\x81\xA1"};
char dotStep_Icon      [14] = {"\xEF\x86\x92"};
char clock_Icon        [14] = {"\xEF\x80\x97"};

/*********************
* Process tab strings
*********************/

char buttonFilter_text [8] = {"Filters"};
char Processes_text [10] = {"Processes"};
char keyboard_placeholder_text [19] = {"Write film name..."};
char filterPopupTitle_text [20] = {"Process list filter"};
char filterPopupNamePlaceHolder_text [23] = {"Part of name to filter"};
char filterPopupName_text [5] = {"Name"};
char filterPopupColor_text [6] = {"Color"};
char filterPopupBnW_text [6] = {"B&W"};
char filterPopupBoth_text [6] = {"Both"};
char filterPopupPreferred_text [15] = {"Preferred only"};
char filterPopupApplyButton_text [6] = {"Apply"};
char filterPopupResetButton_text [6] = {"Reset"};


/*********************
* Config tab strings
*********************/
char Settings_text [17] = {"Machine settings"};
char tempUnit_text [12] = {"Temp. unit:"};
char tempSensorTuning_text [20] = {"Calib. temp. sensor"};
char tuneButton_text [5] = {"TUNE"};
char tempAlertMBox_text [205] = {"Verify that the machine's temperature is steady, take a reading of the surrounding air, enter the value in the text field, and press 'Tune' button.To reset the tuning values, long-press the 'Tune' button."};
char soundAlertMBox_text [118] = {"Continue to sound the alert when a process is about to end or when the heating phase reaches the desired temperature."};
char autostartAlertMBox_text [100] = {"When the desired temperature is reached, automatically initiate the temperature-controlled process."};
char filmRotationSpeedAlertMBox_text [64] = {"Stated as rpm, with a range of 20 to 90 revolutions per minute."};
char rotationInverseIntervalAlertMBox_text [139] = {"The duration, which may be adjusted between 5 and 60 seconds, during which the film will spin in one direction before moving to the other."};
char filmRotationRandomAlertMBox_text [307] = {"To guarantee a perfectly equal development, you may add a random variation to the inversion time using this option. For instance, if the randomization value is set to ~10% and the inversion time is set to 10 seconds, each inversion will have a different real inversion time, ranging from 8 to 10 seconds."};
char drainFillTimeAlertMBox_text [354] = {"Both filling and emptying the tank need time and cannot be completed quickly. You can select how much of the filling and draining time has to be taken into account when calculating the processing time of each step (the amount of time you designate for each step of the process) using the overlap % between filling and draining time and processing time. "};
char rotationSpeed_text [20] = {"Film rotation speed"};
char rotationInversionInterval_text [23] = {"Rotation inv. interval"};
char rotationRandom_text [11] = {"Randomness"};
char celsius_text [4] = {"°C"};
char fahrenheit_text [4] = {"°F"};
char waterInlet_text [17] = {"Water inlet link"};
char persistentAlarm_text [18] = {"Persistent alarms"};
char autostart_text [18] = {"Process autostart"};
char drainFillTime_text [24] = {"Drain/fill time overlap"};


/*********************
* Tools tab strings/vars
*********************/
char Maintenance_text [12] = {"Maintenance"};
char Utilities_text [10] = {"Utilities"};
char Statistics_text [11] = {"Statistics"};
char Software_text [9] = {"Software"};
char cleanMachine_text [14] = {"Clean machine"};
char drainMachine_text [14] = {"Drain machine"};
char importConfigAndProcesses_text [7] = {"Import"};
char importConfigAndProcessesMBox_text [56] = {"Import configuration and processes from micro USB drive"};
char exportConfigAndProcesses_text [7] = {"Export"};
char exportConfigAndProcessesMBox_text [56] = {"Export configuration and processes from micro USB drive"};
char statCompleteProcesses_text [20] = {"Completed processes"};
char statTotalProcessTime_text [19] = {"Total process time"};
char statCompleteCleanProcess_text [23] = {"Completed clean cycles"};
char statStoppedProcess_text [18] = {"Stopped processes"};
char softwareVersion_text [17] = {"Software version"};
char softwareVersionValue_text [9] = {"v0.0.0.1"};
char softwareSerialNum_text [14] = {"Serial number"};
char softwareSerialNumValue_text [11] = {"1234567890"};
char softwareCredits_text [8] = {"Credits"};

char softwareCreditsValue_text [34] = {"Credit to Frank P. \nand \nPete B."};
uint8_t completedProcesses = 0;
uint8_t totalMinProcesses = 0;
uint8_t completedClean = 0;
uint8_t stoppedProcesses = 0;
lv_obj_t * tempSensorTuneButton;


/*********************
* Process detail strings/vars
*********************/
char processDetailStep_text [6] = {"Steps"};
char processDetailInfo_text [8] = {"Details"};
char processDetailIsColor_text [20] = {"For color negative"};
char processDetailIsBnW_text [17] = {"For b&w negative"};
char processDetailIsTempControl_text [14] = {"Temp. control"};
char processDetailTemp_text [13] = {"Temperature:"};
char processDetailIsPreferred_text [11] = {"Preferred:"};
char processDetailTotalTime_text [20] = {"Total time:"};
char processDetailTempPlaceHolder_text [4] = {"Tap"};
char processDetailTempTolerance_text [11] = {"Tolerance:"};
uint8_t isPreferred = 0;

lv_obj_t * processTempTextArea;
lv_obj_t * processToleranceTextArea;

/*********************
* Step detail strings/vars
*********************/
char stepDetailTitle_text [9] = {"New step"};
char stepDetailName_text [6] = {"Name:"};
char stepDetailDuration_text [10] = {"Duration:"};
char stepDetailDurationTimeSep_text [2] = {":"};
char stepDetailDurationMinPlaceHolder_text [4] = {"min"};
char stepDetailDurationSecPlaceHolder_text [4] = {"sec"};
char stepDetailType_text [6] = {"Type:"};
char stepDetailSource_text [8] = {"Source:"};
char stepDetailDiscardAfter_text [15] = {"Discard after:"};
char stepDetailCurrentSourceTemp_text [14] = {"Current temp:"};
char stepDetailPlaceHolder_text [24] = {"Enter name for new step"};
char stepDetailSave_text [5] = {"Save"};
char stepDetailCancel_text [7] = {"Cancel"};
char stepDetailCurrentTemp_text [12] = {"Current   :"};
lv_obj_t * stepDetailMinTextArea;
lv_obj_t * stepDetailSecTextArea;

const char * stepTypeList = "Chemistry\n"
                            "Rinse\n"
                            "MultiRinse";

const char * stepSourceList = "C1\n"
                              "C2\n"
                              "C3\n"
                              "WB";

char processSourceList[4][3]  = {"C1","C2","C3","WB"};
char processTempControlList[3][6]  = {"Off","Run","Susp."};
                        
uint8_t stepType = 0;
uint8_t stepSource = 0;
uint8_t discardAfter = 0;
double stepSourceTemp = 20.8;
lv_obj_t * stepDetailNamelTextArea;

/*********************
* Popup elements
*********************/
char * minutesOptions;
char * secondsOptions;  
char * tempCelsiusOptions;       
char * tempCelsiusToleranceOptions;  

char stopProcessPopupTitle_text [16] = {"Stop process"};
char warningPopupTitle_text [16] = {"Warning!"};
char setMinutesPopupTitle_text [16] = {"Set minutes"};
char setSecondsPopupTitle_text [16] = {"Set seconds"};
char tuneTempPopupTitle_text [16] = {"Set temperature"};
char tuneRollerButton_text [4] = {"SET"};
char messagePopupDetailTitle_text [19] = {"Detail information"};
char deleteButton_text [7] = {"Delete"};
char deletePopupTitle_text [15] = {"Delete element"};
char deletePopupBody_text [45] = {"Are you sure to delete the selected element?"};
char warningPopupLowWaterTitle_text [151] = {"The water level is too low!Temperature control has been suspended\nRefill the water bath immediately to correct resume the temperature control process"};
char stopProcessPopupBody_text [124] = {"Stopping a process will ruin the film inside the tank and leave the chemistry inside!\nDo you want to stop the process now?"};

char currentStep[3][11]  = {"Filling","Draining","Processing"};

char rollerElementSelected [10];
uint8_t rolleTempSelected = 0;
lv_obj_t * questionMark;
lv_obj_t * roller;
lv_style_t style_roller;
lv_style_t textAreaStyle;
char tempBuffer [10];


/*********************
* Checkup strings/vars
*********************/
char checkupNexStepsTitle_text [12] = {"Next steps:"};
char checkupProcessReady_text [20] = {"Process starting..."};
char checkupTheMachineWillDo_text [18] = {"The machine will:"};
char checkupFillWater_text [20] = {"Fill the water bath"};
char checkupTankRotation_text [38] = {"Check tank presence and film rotation"};
char checkupReachTemp_text [39] = {"Reach the chemistry target temperature"};
char checkupStop_text [5] = {"Stop"};
char checkupStart_text [6] = {"Start"};
char checkupSkip_text [6] = {"Skip"};
char checkupStopNow_text [10] = {"Stop now!"};
char checkupStopAfter_text [21] = {"Stop after!"};
char checkupProcessingTitle_text [12] = {"Processing:"};
char checkupStepSource_text [13] = {"Step source:"};
char checkupTempControl_text [15] = {"Temp. control:"};
char checkupWaterTemp_text [12] = {"Water temp:"};
char checkupNextStep_text [11] = {"Next step:"};
char checkupSelectBeforeStart_text [72] = {"Select the tank size and chemistry amount and click the 'Start' button"};
char checkupTankSize_text [17] = {"Select tank size"};
char checkupChemistryVolume_text [24] = {"Select chemistry volume"};
char checkupMinimumChemistry_text [35] = {"Minimum required chemistry : 500ml"};
char checkupFillWaterMachine_text [111] = {"The machine is not connected to a water source.\n\nFill the water bath manually up to the top water level sensor"};
char checkupTargetTemp_text [19] = {"Target temperature"};
char checkupWater_text [6] = {"Water"};
char checkupChemistry_text [10] = {"Chemistry"};
char checkupTankPosition_text [21] = {"Tank is in position:"};
char checkupFilmRotation_text [18] = {"Film is rotating:"};
char checkupYes_text [4] = {"Yes"};
char checkupNo_text [3] = {"No"};
char checkupChecking_text [12] = {"Checking..."};

const char * checkupTankSizesList = "Small\n"
                                    "Medium\n"
                                    "Large";


const char * checkupStepStatuses [3][14] = {"\xEF\x86\x92", "\xEF\x81\xA1", "\xEF\x80\x8C"};  //dotStep_Icon, arrowStep_Icon, checkStep_icon


uint16_t tankFullSizes [3] = {500, 700, 1000}; 
uint16_t tankLowSizes  [3] = {250, 350, 550}; 

uint8_t tankFullFillDrainTimes [3] = {15, 19, 25}; 
uint8_t tankLowFillDrainTimes  [3] = {8, 11, 16}; 

static lv_obj_t * checkupTankSizeTextArea;
char checkupTankSizePlaceHolder_text [5] = {"Size"};
char checkupChemistryLowVol_text [4] = {"Low"};
char checkupChemistryHighVol_text [5] = {"High"};

SPIClass SD_SPI;

//Test long string
char testLongString [340] = {"Cantami, o diva, del Pelìde Achille l'ira funesta che infiniti addusse lutti agli achei , molte anzi tempo all'orco generose travolse alme d'eroi, e di cani e d'augelli orrido pasto lor salme abbandonò (così di giove l'alto consiglio s'adempia ) , da quando primamente disgiunse aspra contesa il re de' prodi Atride e il divo Achille."};





/**********************
 *      TYPEDEFS
 **********************/


/**********************
 * GLOBAL PROTOTYPES
 **********************/
uint8_t testVal1,testVal2,testVal3 = 10;
char text1[3],text2[3],text3[3];
unsigned long actualMillis;

uint16_t processCounter = 0;
uint16_t stepCounter = 0;

lv_obj_t * screen_home;
lv_obj_t * screen_mainMenu;

//NEW GUI APPROACH 
lv_obj_t * oldTabSelected = NULL;
lv_obj_t * newTabSelected = NULL;

lv_obj_t * processesSection = NULL;
lv_obj_t * processFilterButton;
lv_obj_t * newProcessButton;
lv_obj_t * processElement;
lv_obj_t * stepElement;

lv_obj_t * settingsSection = NULL;
lv_obj_t * toolsSection = NULL;

lv_point_precise_t sectionTitleLine_points[] = { {0, 0}, {310 , 0}};
lv_style_t style_sectionTitleLine;
lv_style_t style_section2TitleLine;
lv_obj_t * sectionTitleLine;

lv_obj_t * messageBox;
lv_point_precise_t mBoxTitleLine_points[] = { {0, 0}, {200 , 0}};
lv_style_t style_mBoxTitleLine;
lv_obj_t * mBoxTitleLine;
lv_obj_t * mBoxFilterPopupParent = NULL;
lv_obj_t * mBoxNameTextArea;
lv_obj_t * mBoxSelectColorRadioButton;
lv_obj_t * mBoxSelectBnWRadioButton;
lv_obj_t * mBoxOnlyPreferredSwitch;
lv_obj_t * mBoxApplyFilterButton;
lv_obj_t * mBoxResetFilterButton;

lv_style_t style_mBoxPopupTitleLine;
lv_obj_t * mBoxPopupTitleLine;
lv_obj_t * mBoxPopupParent = NULL;

lv_style_t style_mBoxRollerTitleLine;
lv_obj_t * mBoxRollerTitleLine;
lv_obj_t * mBoxRollerParent = NULL;

lv_style_t style_mBoxStepPopupTitleLine;
lv_obj_t * mBoxStepPopupTitleLine;

lv_obj_t * keyBoardParent;
lv_obj_t * keyboardFilter = NULL;
lv_obj_t * keyboardStep = NULL;
lv_obj_t * keyboardProcess = NULL;

lv_obj_t * keyboard;

lv_obj_t * keyboard_textArea;

lv_obj_t * processDetailParent = NULL;
lv_obj_t * stepDetailParent = NULL;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*DEFINITIONS_H*/
