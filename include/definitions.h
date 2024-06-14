#include <sys/_stdint.h>
/**
 * @file definitions.h
 *
 */


#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "FreeRTOS.h"
#include "semphr.h"


//#include <OneWire.h>

//Set to 1 to enable all errors (SD/I2C)
#define ENABLE_BOOT_ERRORS 0

// PINS/RELAY DEFINITIONS 

//Digital input for relays
#define RELAY_NUMBER    8

#define WATER_RLY       0
#define DEV_RLY         1
#define STOP_RLY        2
#define FIX_RLY         3
#define WASTE_RLY       4
#define PUMP_IN_RLY     5
#define PUMP_OUT_RLY    6
#define HEATER_RLY      7


//MOTORS PIN
#define MOTOR_PIN_NUMBER 3
#define MAX_MOTOR_SPD 200 //need to be tested, but is max 255

#define MOTOR_ENA_PIN  8
#define MOTOR_IN1_PIN  9
#define MOTOR_IN2_PIN  10


//TEMPERATURE SENSOR PIN
#define TEMPERATURE_CHEMICAL_PIN 11
#define TEMPERATURE_BATH_PIN     12

#define TEST_PIN 15
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



//Definition of LGFX, it's here because can't be managed easly a .cpp library inside a .c file
#ifdef __cplusplus

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <Adafruit_MCP23X17.h>
#include "Adafruit_SHT31.h"
#include <Preferences.h> 

extern Preferences preferences;
Adafruit_MCP23X17 mcp;
Adafruit_SHT31 sensorTempBath     = Adafruit_SHT31();
Adafruit_SHT31 sensorTempChemical = Adafruit_SHT31();


class LGFX : public lgfx::LGFX_Device
{
    static constexpr int I2C_PORT_NUM = I2C_NUM_0;
    static constexpr int I2C_PIN_SDA  = I2C_SDA;
    static constexpr int I2C_PIN_SCL  = I2C_SCL;
    static constexpr int I2C_PIN_INT  = I2C_INT;

    lgfx::Panel_ILI9488     _panel_instance;
    lgfx::Bus_Parallel16    _bus_instance; 
    lgfx::Light_PWM         _light_instance;
    lgfx::Touch_FT5x06      _touch_instance;
public:

  LGFX(void)
    {
        {                                      
            auto cfg = _bus_instance.config(); 
            
            cfg.port = 0;              
            cfg.freq_write = 40000000;//20000000; 
            cfg.pin_wr = LCD_WR;           
            cfg.pin_rd = LCD_RD;           
            cfg.pin_rs = LCD_RS;           

            cfg.pin_d0 = LCD_D0;
            cfg.pin_d1 = LCD_D1;
            cfg.pin_d2 = LCD_D2;
            cfg.pin_d3 = LCD_D3;
            cfg.pin_d4 = LCD_D4;
            cfg.pin_d5 = LCD_D5;
            cfg.pin_d6 = LCD_D6;
            cfg.pin_d7 = LCD_D7;
            cfg.pin_d8 = LCD_D8;
            cfg.pin_d9 = LCD_D9;
            cfg.pin_d10 = LCD_D10;
            cfg.pin_d11 = LCD_D11;
            cfg.pin_d12 = LCD_D12;
            cfg.pin_d13 = LCD_D13;
            cfg.pin_d14 = LCD_D14;
            cfg.pin_d15 = LCD_D15;

            _bus_instance.config(cfg);              
            _panel_instance.setBus(&_bus_instance); 
        }

        {                                        
            auto cfg = _panel_instance.config(); 

            cfg.pin_cs = LCD_CS;   
            cfg.pin_rst = LCD_RST;  
            cfg.pin_busy = LCD_BSY; 

            cfg.memory_width = TFT_WIDTH;   
            cfg.memory_height = TFT_HEIGHT;  
            cfg.panel_width = TFT_WIDTH;    
            cfg.panel_height = TFT_HEIGHT;   
            cfg.offset_x = 0;         
            cfg.offset_y = 0;         
            cfg.offset_rotation = 0;  
            cfg.dummy_read_pixel = 8; 
            cfg.dummy_read_bits = 1;  
            cfg.readable = true;      
            cfg.invert = false;       
            cfg.rgb_order = false;    
            cfg.dlen_16bit = true;    
            cfg.bus_shared = true;//false;    

            _panel_instance.config(cfg);
}

    {
      auto cfg = _light_instance.config();    

      cfg.pin_bl = LCD_BLK;              
      cfg.invert = false;           
      cfg.freq   = 44100;           
      cfg.pwm_channel = 7;          

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);  
    }

    { 
      auto cfg = _touch_instance.config();

      cfg.x_min      = 0;
      cfg.x_max      = TFT_WIDTH - 1;
      cfg.y_min      = 0;  
      cfg.y_max      = TFT_HEIGHT - 1;
      cfg.pin_int    = I2C_PIN_INT;  
      cfg.bus_shared = true; 
      cfg.offset_rotation = 0;

      cfg.i2c_port = I2C_PORT_NUM;     
      cfg.i2c_addr = I2C_ADR;  
      cfg.pin_sda  = I2C_PIN_SDA;   
      cfg.pin_scl  = I2C_PIN_SCL;   
      cfg.freq = 400000;  

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);  
    }
        setPanel(&_panel_instance); 
    }
};


extern "C" {
#endif

#include <lvgl.h>

/*********************
* ACCESSORY STRUCTS
*********************/
typedef enum{
    STEP_NODE,
    PROCESS_NODE
} NodeType_t;

typedef enum {
    BLACK_AND_WHITE_FILM,
    COLOR_FILM,
    BOTH_FILM,
    FILM_TYPE_NA
} filmType_t;

typedef enum {
    CELSIUS_TEMP,
    FAHRENHEIT_TEMP,
    TEMP_UNIT_NA
} tempUnit_t;

typedef enum {
    CHEMISTRY,
    RINSE,
    MULTI_RINSE,
    CHEMICAL_TYPE_NA
} chemicalType_t;

typedef enum {
	  C1,
    C2,
    C3,
    WB,
    CHEMICAL_SOURCE_NA
} chemicalSource_t;

typedef enum {
	  YES,
    NO,
} yesOrNo_t;

struct machineSettings {
  tempUnit_t 	        tempUnit; //0= C° 1= °F
  yesOrNo_t 	        waterInlet;
  float 	        	  calibratedTemp;
  uint8_t 	        	filmRotationSpeedSetpoint;
	uint8_t 	        	rotationIntervalSetpoint;
  uint8_t 	        	randomSetpoint;
  yesOrNo_t 	        isPersistentAlarm;
  yesOrNo_t 	        isProcessAutostart;
  uint8_t 	        	drainFillOverlapSetpoint;
};

typedef struct machineStatistics {
  uint32_t 	          completed;
  uint64_t 	          totalMins;
  uint32_t 	          stopped;
  uint32_t 	          clean;
}machineStatistics;


/*********************
* ELEMENTS STRUCTS
*********************/
#define MAX_PROC_NAME_LEN		  20
typedef struct processNode processNode;  // Forward declaration
//typedef struct sProcessDetail sProcessDetail;  // Forward declaration
//typedef struct sCheckup sCheckup;  // Forward declaration


typedef struct sStepDetail {
    /* LVGL objects */
	lv_obj_t 			      *stepDetailParent;
	lv_obj_t			      *mBoxStepPopupTitleLine;
	lv_style_t			    style_mBoxStepPopupTitleLine;
	lv_point_precise_t	titleLinePoints[2];


	lv_obj_t 	        	*stepDetailNameContainer;
	lv_obj_t 	        	*stepDetailContainer;
	lv_obj_t 	        	*stepDurationContainer;
	lv_obj_t 	        	*stepTypeContainer;
	lv_obj_t 	        	*stepSourceContainer;
	lv_obj_t 	        	*stepDiscardAfterContainer;

	lv_obj_t 	        	*stepDetailLabel;
	lv_obj_t 	        	*stepDetailNamelLabel;
	lv_obj_t 	        	*stepDurationLabel;
	lv_obj_t 	        	*stepDurationMinLabel;
	lv_obj_t 	        	*stepSaveLabel;
	lv_obj_t 	        	*stepCancelLabel;
	lv_obj_t 	        	*stepTypeLabel;
	lv_obj_t 	        	*stepSourceLabel;
	lv_obj_t 	        	*stepTypeHelpIcon;
	lv_obj_t 	        	*stepSourceTempLabel;
	lv_obj_t 	        	*stepDiscardAfterLabel;
	lv_obj_t 	        	*stepSourceTempHelpIcon;
	lv_obj_t 	        	*stepSourceTempValue;

	lv_obj_t 	        	*stepDiscardAfterSwitch;

	lv_obj_t 	        	*stepSaveButton;
	lv_obj_t 	        	*stepCancelButton;

	lv_obj_t 	        	*stepSourceDropDownList;
	lv_obj_t 	        	*stepTypeDropDownList;
	lv_style_t        	*dropDownListStyle;

	lv_obj_t	        	*stepDetailSecTextArea;
	lv_obj_t	        	*stepDetailMinTextArea;
	lv_obj_t	        	*stepDetailNamelTextArea;

	/* Params objects */

//	  processNode       *referenceProcess;  // Use a pointer instead of an instance
    char                stepNameString[MAX_PROC_NAME_LEN];
    uint8_t             timeMins;
    uint8_t             timeSecs;
    chemicalType_t      type;
    chemicalSource_t    source;
    uint8_t             discardAfterProc;
} sStepDetail;


typedef struct singleStep { //GRAPHIC ELEMENT IN THE STEPS LIST
    /* LVGL objects */
    lv_obj_t           *stepElement;
    lv_obj_t           *stepElementSummary;
    lv_style_t         stepStyle;
    lv_obj_t           *stepName;
    lv_obj_t           *stepTime;
    lv_obj_t           *stepTimeIcon;
    lv_obj_t           *stepTypeIcon;
    lv_obj_t           *discardAfterIcon;
    lv_obj_t           *sourceLabel;
    lv_coord_t         container_y;
    lv_obj_t          *deleteButton;
    lv_obj_t          *deleteButtonLabel;
    lv_obj_t          *editButton;
    lv_obj_t          *editButtonLabel;
    uint8_t            swipedLeft;
    uint8_t            swipedRight;
    /* Params objects */
    sStepDetail 	  *stepDetails;
} singleStep;

typedef struct stepNode {
    struct stepNode   *next;   /* Pointer to next element in list */
    struct stepNode   *prev;   /* Pointer to previous element in list */
    singleStep         step;   /* Step data */
} stepNode;

typedef struct stepList {
    stepNode          *start;  /* Start of list */
    stepNode          *end;    /* End of list */
    uint16_t           size;   /* Number of list entries currently */
} stepList;


typedef struct sCheckup{
    /* LVGL objects */
	lv_obj_t			*checkupParent;
	lv_style_t		    textAreaStyleCheckup;
	lv_obj_t			*checkupContainer;
	lv_obj_t			*checkupNextStepsContainer;
	lv_obj_t			*checkupProcessNameContainer;
	lv_obj_t			*checkupStepContainer;
	lv_obj_t			*checkupWaterFillContainer;
	lv_obj_t			*checkupReachTempContainer;
	lv_obj_t			*checkupTankAndFilmContainer;
	lv_obj_t			*checkupStepSourceContainer;
	lv_obj_t			*checkupTempControlContainer;
	lv_obj_t			*checkupWaterTempContainer;
	lv_obj_t			*checkupNextStepContainer;
	lv_obj_t			*checkupSelectTankChemistryContainer;
	lv_obj_t			*checkupFillWaterContainer;
	lv_obj_t			*checkupTargetTempsContainer;
	lv_obj_t			*checkupTargetTempContainer;
	lv_obj_t			*checkupTargetWaterTempContainer;
	lv_obj_t			*checkupTargetChemistryTempContainer;
	lv_obj_t			*checkupTankIsPresentContainer;
	lv_obj_t			*checkupFilmRotatingContainer;
	lv_obj_t			*checkupFilmInPositionContainer;
	lv_obj_t			*checkupProcessingContainer;


	lv_obj_t			*checkupTankSizeLabel;
	lv_obj_t			*checkupChemistryVolumeLabel;
	lv_obj_t			*checkupNextStepsLabel;
	lv_obj_t			*checkupWaterFillLabel;
	lv_obj_t			*checkupReachTempLabel;
	lv_obj_t			*checkupTankAndFilmLabel;
	lv_obj_t			*checkupMachineWillDoLabel;
	lv_obj_t			*checkupCloseButtonLabel;
	lv_obj_t			*checkupStepSourceLabel;
	lv_obj_t			*checkupTempControlLabel;
	lv_obj_t			*checkupWaterTempLabel;
	lv_obj_t			*checkupNextStepLabel;
	lv_obj_t			*checkupStopAfterButtonLabel;
	lv_obj_t			*checkupStopNowButtonLabel;
	lv_obj_t			*checkupStartButtonLabel;
	lv_obj_t			*checkupProcessReadyLabel;
	lv_obj_t			*checkupSelectBeforeStartLabel;
	lv_obj_t			*checkupFillWaterLabel;
	lv_obj_t			*checkupSkipButtonLabel;
	lv_obj_t			*checkupTargetTempLabel;
	lv_obj_t			*checkupTargetWaterTempLabel;
	lv_obj_t			*checkupTargetChemistryTempLabel;
	lv_obj_t			*checkupTankIsPresentLabel;
	lv_obj_t			*checkupFilmRotatingLabel;
	lv_obj_t			*checkupProcessCompleteLabel;

	lv_obj_t			*checkupTargetTempValue;
	lv_obj_t			*checkupTargetWaterTempValue;
	lv_obj_t			*checkupTargetChemistryTempValue;
	lv_obj_t			*checkupStepSourceValue;
	lv_obj_t			*checkupTempControlValue;
	lv_obj_t			*checkupWaterTempValue;
	lv_obj_t			*checkupNextStepValue;
	lv_obj_t			*checkupProcessNameValue;
	lv_obj_t			*checkupTankIsPresentValue;
	lv_obj_t			*checkupFilmRotatingValue;
	lv_obj_t			*checkupStepTimeLeftValue;
	lv_obj_t			*checkupProcessTimeLeftValue;
	lv_obj_t			*checkupStepNameValue;
	lv_obj_t			*checkupStepKindValue;

	lv_obj_t			*checkupWaterFillStatusIcon;
	lv_obj_t			*checkupReachTempStatusIcon;
	lv_obj_t			*checkupTankAndFilmStatusIcon;

	lv_obj_t			*lowVolumeChemRadioButton;
	lv_obj_t			*highVolumeChemRadioButton;


	lv_obj_t			*checkupSkipButton;
	lv_obj_t			*checkupStartButton;
	lv_obj_t			*checkupStopAfterButton;
	lv_obj_t			*checkupStopNowButton;
	lv_obj_t			*checkupCloseButton;
  lv_timer_t    *timer;

	uint8_t 			isProcessing; // 0 or 1
	uint8_t 			processStep;//0 or 1 or 2 or 3 or 4
	uint32_t			activeVolume_index;
	uint8_t 			tankSize;
	uint8_t 			stopAfter;

	uint8_t 			stepFillWaterStatus;
	uint8_t 			stepReachTempStatus;
	uint8_t 			stepCheckFilmStatus;

	lv_obj_t			*stepArc;
	lv_obj_t			*processArc;

	lv_obj_t			*checkupTankSizeTextArea;

	/* Params objects */
}sCheckup;

typedef struct sProcessDetail {
    /* LVGL objects */
	lv_obj_t			*processesContainer;
	lv_obj_t			*processDetailParent;
	lv_style_t		    textAreaStyle;

	lv_obj_t			*processDetailContainer;
	lv_obj_t			*processDetailNameContainer;
	lv_obj_t			*processStepsContainer;
	lv_obj_t			*processInfoContainer;
	lv_obj_t			*processTempControlContainer;
	lv_obj_t			*processTempContainer;
	lv_obj_t			*processToleranceContainer;
	lv_obj_t			*processColorOrBnWContainer;
	lv_obj_t			*processTotalTimeContainer;

	lv_obj_t			*processDetailNameTextArea;
	lv_obj_t			*processDetailStepsLabel;
	lv_obj_t			*processDetailInfoLabel;
	lv_obj_t			*processDetailCloseButtonLabel;
	lv_obj_t			*processTempControlLabel;
	lv_obj_t			*processTempLabel;
	lv_obj_t			*processTempControlSwitch;
	lv_obj_t			*processTempUnitLabel;
	lv_obj_t			*processToleranceLabel;
	lv_obj_t			*processColorLabel;
	lv_obj_t			*processBnWLabel;
	lv_obj_t			*processPreferredLabel;
	lv_obj_t			*processSaveLabel;
	lv_obj_t			*processRunLabel;
	lv_obj_t			*processNewStepLabel;
	lv_obj_t			*processTotalTimeLabel;
	lv_obj_t			*processTotalTimeValue;

	lv_obj_t			*processDetailCloseButton;
	lv_obj_t			*processRunButton;
	lv_obj_t			*processSaveButton;
	lv_obj_t			*processNewStepButton;

	lv_obj_t			*processTempTextArea;
	lv_obj_t			*processToleranceTextArea;

	/* Params objects */
    stepList          stepElementsList;  /* Process steps list */
	  sCheckup		      *checkup;
    char              processNameString[MAX_PROC_NAME_LEN];
    uint32_t          temp;
    uint8_t           tempTolerance;
    uint8_t           isTempControlled;
    uint8_t           isPreferred;
    uint8_t           somethingChanged;
    filmType_t        filmType;
    uint32_t          timeMins;
    uint8_t           timeSecs;
}sProcessDetail;


//GRAPHIC ELEMENT IN THE PROCESS LIST
typedef struct singleProcess { 
    /* LVGL objects */
    lv_obj_t          *processElement;
    lv_style_t        processStyle;
    lv_obj_t          *preferredIcon;
    lv_obj_t          *processElementSummary;
    lv_obj_t          *processName;
    lv_obj_t          *processTemp;
    lv_obj_t          *processTempIcon;
    lv_obj_t          *processTime;
    lv_obj_t          *processTimeIcon;
    lv_obj_t          *processTypeIcon;
    lv_obj_t          *deleteButton;
    lv_obj_t          *deleteButtonLabel;
    lv_coord_t         container_y;
    uint8_t            swipedLeft;
    uint8_t            swipedRight;
    uint8_t             isFiltered;

    sProcessDetail     *processDetails;  /* Process details, that includes all steps */
} singleProcess;

struct processNode {
    struct processNode *next;   /* Pointer to next element in list */
    struct processNode *prev;   /* Pointer to previous element in list */
    singleProcess       process; /* Process data */
};

typedef struct processList { // Linked list of processes
    processNode       *start;  /* Start of list */
    processNode       *end;    /* End of list */
    int32_t           size;   /* Number of list entries currently */
} processList;



struct sRollerPopup {
	/* LVGL objects */
	lv_obj_t			      *mBoxRollerParent;
	lv_obj_t			      *mBoxRollerTitleLine;
	lv_style_t			    style_mBoxRollerTitleLine;
	lv_point_precise_t	titleLinePoints[2];

	lv_obj_t	         	*roller;
	lv_style_t          style_roller;


	lv_obj_t 	         	*mBoxRollerContainer;
	lv_obj_t 	         	*mBoxRollerTitle;
	lv_obj_t 	         	*mBoxRollerButton;
	lv_obj_t 	         	*mBoxRollerButtonLabel;
	lv_obj_t 	         	*mBoxRollerRollerContainer;

  lv_obj_t 	         	*whoCallMe;

  char                *minutesOptions;
  char                *secondsOptions;  
  char                *tempCelsiusOptions;       
  char                *tempCelsiusToleranceOptions;  
	/* Params objects */
};


struct sFilterPopup {
	/* LVGL objects */
	lv_obj_t			      *mBoxFilterPopupParent;
	lv_obj_t			      *mBoxStepPopupTitleLine;
	lv_style_t			    style_mBoxTitleLine;
	lv_point_precise_t	titleLinePoints[2];

    
	lv_obj_t	      		*mBoxContainer;
	lv_obj_t	      		*mBoxTitle;
	lv_obj_t	      		*mBoxNameContainer;
	lv_obj_t	      		*mBoxNameLabel;
	lv_obj_t	      		*selectColorContainerRadioButton;
	lv_obj_t	      		*mBoxColorLabel;
	lv_obj_t	      		*mBoxBnWLabel;
	lv_obj_t	      		*mBoxPreferredContainer;
	lv_obj_t	      		*mBoxPreferredLabel;
	lv_obj_t	      		*mBoxResetFilterLabel;
	lv_obj_t	      		*mBoxApplyFilterLabel;


	lv_obj_t	      		*mBoxNameTextArea;
	lv_obj_t	      		*mBoxSelectColorRadioButton;
	lv_obj_t	      		*mBoxSelectBnWRadioButton;
	lv_obj_t	      		*mBoxOnlyPreferredSwitch;
	lv_obj_t	      		*mBoxResetFilterButton;
	lv_obj_t	      		*mBoxApplyFilterButton;

  
	/* Params objects */
  char                *filterName;
  uint8_t             isColorFilter;
  uint8_t             isBnWFilter;
  uint8_t             preferredOnly;
};


struct sMessagePopup {
	/* LVGL objects */
	lv_obj_t			      *mBoxPopupParent;
	lv_obj_t			      *mBoxPopupTitleLine;
	lv_style_t			    style_mBoxPopupTitleLine;
	lv_point_precise_t	titleLinePoints[2];

	lv_obj_t		      	*mBoxPopupContainer;
	lv_obj_t		      	*mBoxPopupTextContainer;


	lv_obj_t		      	*mBoxPopupTitle;
	lv_obj_t		      	*mBoxPopupText;
	lv_obj_t		      	*mBoxPopupButtonLabel;
	lv_obj_t		      	*mBoxPopupButton1Label;
	lv_obj_t		      	*mBoxPopupButton2Label;

	lv_obj_t		      	*mBoxPopupButtonClose;
	lv_obj_t		      	*mBoxPopupButton1;
	lv_obj_t		      	*mBoxPopupButton2;  

  void    	         	*whoCallMe;
	/* Params objects */
};


/*********************
* HOME PAGE STRUCT
*********************/
struct sHome {
    /* LVGL objects */
	lv_obj_t *screen_home;
  lv_obj_t *startButton;
  lv_obj_t *splashImage;
  lv_obj_t *errorButtonLabel;
  lv_obj_t *errorLabel;
	/* Params objects */
};


/*********************
* MENU STRUCT
*********************/
struct sMenu {
    /* LVGL objects */
	lv_obj_t 			*oldTabSelected;
	lv_obj_t 			*newTabSelected;
	lv_obj_t			*screen_mainMenu;
	lv_obj_t			*processesTab;
	lv_obj_t			*settingsTab;
	lv_obj_t			*toolsTab;
	lv_obj_t			*iconLabel;
	lv_obj_t			*label;

	/* Params objects */
	uint8_t				oldSelection;
	uint8_t				newSelection;
};


//THIS IS THE PROCESSES "PAGE" IN THE "PROCESS LIST" TAB
struct sProcesses {
  /* LVGL objects */
  lv_obj_t			    *processesSection;
  lv_obj_t			    *sectionTitleLine;
  lv_style_t			style_sectionTitleLine;
  lv_point_precise_t	titleLinePoints[2];


  lv_obj_t	        	*processesLabel;
  lv_obj_t	        	*iconFilterLabel;
  lv_obj_t	        	*iconNewProcessLabel;
  lv_obj_t	        	*processesListContainer;

  lv_obj_t	        	*processFilterButton;
  lv_obj_t	        	*newProcessButton;
  uint8_t             isFiltered;
  /* Params objects */
  processList           processElementsList;
};


struct sSettings {
    /* LVGL objects */
	lv_obj_t			      *settingsSection;
	lv_obj_t			      *sectionTitleLine;
	lv_style_t			    style_sectionTitleLine;
	lv_point_precise_t	titleLinePoints[2];
	lv_coord_t 			    pad;

	lv_obj_t 	        	*settingsLabel;
	lv_obj_t 	        	*tempUnitLabel;
	lv_obj_t 	        	*waterInletLabel;
	lv_obj_t 	        	*tempSensorTuneButtonLabel;
	lv_obj_t 	        	*tempSensorTuningLabel;
	lv_obj_t 	        	*filmRotationSpeedLabel;
	lv_obj_t 	        	*filmRotationInverseIntervalLabel;
	lv_obj_t 	        	*filmRotationRandomLabel;
	lv_obj_t 	        	*persistentAlarmLabel;
	lv_obj_t 	        	*autostartLabel;
	lv_obj_t 	        	*drainFillTimeLabel;

	lv_obj_t 	        	*drainFillTimeValueLabel;
	lv_obj_t 	        	*filmRotationInverseIntervalValueLabel;
	lv_obj_t 	        	*filmRotationRandomValueLabel;
	lv_obj_t 	        	*filmRotationSpeedValueLabel;

	lv_obj_t 	        	*settingsContainer;
	lv_obj_t 	        	*tempUnitContainer;
	lv_obj_t 	        	*waterInletContainer;
	lv_obj_t 	        	*tempTuningContainer;
	lv_obj_t 	        	*filmRotationSpeedContainer;
	lv_obj_t 	        	*filmRotationInverseIntervallContainer;
	lv_obj_t 	        	*randomContainer;
	lv_obj_t 	        	*persistentAlarmContainer;
	lv_obj_t 	        	*autostartContainer;
	lv_obj_t 	        	*drainFillTimeContainer;

	lv_obj_t 	        	*autostartSwitch;
	lv_obj_t 	        	*persistentAlarmSwitch;
	lv_obj_t 	        	*waterInletSwitch;

	lv_obj_t 	        	*filmRotationSpeedSlider;
	lv_obj_t 	        	*filmRotationInversionIntervalSlider;
	lv_obj_t 	        	*filmRandomlSlider;
	lv_obj_t 	        	*drainFillTimeSlider;

	lv_obj_t	        	*tempSensorTuneButton;
	lv_obj_t 	        	*tempUnitCelsiusRadioButton;
	lv_obj_t 	        	*tempUnitFahrenheitRadioButton;

	uint32_t 	        	active_index;


  /* Params objects */
  struct machineSettings   settingsParams;
};


struct sTools {
    /* LVGL objects */
	lv_obj_t			      *toolsSection;
	lv_obj_t			      *sectionTitleLine;
	lv_style_t			    style_sectionTitleLine;
	lv_point_precise_t	titleLinePoints[2];

	lv_obj_t 	        	*toolsCleaningContainer;
	lv_obj_t 	        	*toolsDrainingContainer;
	lv_obj_t 	        	*toolsImportContainer;
	lv_obj_t 	        	*toolsExportContainer;


	lv_obj_t 	        	*toolsCleaningLabel;
	lv_obj_t 	        	*toolsDrainingLabel;
	lv_obj_t 	        	*toolsImportLabel;
	lv_obj_t 	        	*toolsExportLabel;

	lv_obj_t 	        	*toolsCleaningIcon;
	lv_obj_t 	        	*toolsDrainingIcon;
	lv_obj_t 	        	*toolsImportIcon;
	lv_obj_t 	        	*toolsExportIcon;

	lv_obj_t 	        	*toolsMaintenanceLabel;
	lv_obj_t 	        	*toolsUtilitiesLabel;
	lv_obj_t 	        	*toolsStatisticsLabel;
	lv_obj_t 	        	*toolsSoftwareLabel;

	lv_obj_t 	        	*toolsStatCompleteProcessesContainer;
	lv_obj_t 	        	*toolsStatTotalTimeContainer;
	lv_obj_t 	        	*toolsStatCompleteCycleContainer;
	lv_obj_t 	        	*toolsStatStoppedProcessesContainer;

	lv_obj_t 	        	*toolStatCompletedProcessesLabel;
	lv_obj_t 	        	*toolStatCompletedProcessesValue;
	lv_obj_t 	        	*toolStatTotalTimeLabel;
	lv_obj_t 	        	*toolStatTotalTimeValue;
	lv_obj_t 	        	*toolStatCompleteCycleLabel;
	lv_obj_t 	        	*toolStatCompleteCycleValue;
	lv_obj_t 	        	*toolStatStoppedProcessesLabel;
	lv_obj_t 	        	*toolStatStoppedProcessesValue;


	lv_obj_t 	        	*toolsSoftwareVersionContainer;
	lv_obj_t 	        	*toolsSoftwareSerialContainer;

	lv_obj_t 	        	*toolSoftWareVersionLabel;
	lv_obj_t 	        	*toolSoftwareVersionValue;
	lv_obj_t 	        	*toolSoftwareSerialLabel;
	lv_obj_t 	        	*toolSoftwareSerialValue;

	lv_obj_t 	        	*toolCreditButton;
	lv_obj_t 	        	*toolCreditButtonLabel;

	/* Params objects */
  struct machineStatistics machineStats;
};

struct sKeyboardPopup {
	/* LVGL objects */
  lv_obj_t          *keyBoardParent;
	lv_obj_t 			    *keyboard;
	lv_obj_t			    *keyboardTextArea;
};


/*********************
* ELEMENTS STRUCT
*********************/
struct sElements {
	struct sFilterPopup			filterPopup;
	struct sMessagePopup 		messagePopup;
	struct sRollerPopup			rollerPopup;
  struct sKeyboardPopup   keyboardPopup;
};


/*********************
* PAGES STRUCT
*********************/
struct sPages {
	struct sHome				  home;
	struct sMenu				  menu;
	struct sProcesses			processes;
	struct sSettings			settings;
	struct sTools				  tools;
};


/*********************
* ALL ELEMENTS/PAGES GUI COMPONENT STRUCT
*********************/
struct gui_components {
	struct sElements	element;
	struct sPages		  page;
  QueueHandle_t			sysActionQ;
  processNode       *tempProcessNode;
  stepNode          *tempStepNode;
};

/*********************
* GLOBAL DEFINES
*********************/

/*********************
* System manager defines
*********************/
#define SAVE_PROCESS_CONFIG           0x0001
#define SAVE_MACHINE_STATS            0x0002

LV_FONT_DECLARE(FilMachineFontIcons_15);
LV_FONT_DECLARE(FilMachineFontIcons_20);
LV_FONT_DECLARE(FilMachineFontIcons_30);
LV_FONT_DECLARE(FilMachineFontIcons_40);
LV_FONT_DECLARE(FilMachineFontIcons_100);

LV_IMG_DECLARE(splash_img);


#define TAB_PROCESSES 3
#define TAB_SETTINGS  4
#define TAB_TOOLS     5

#define FILENAME_SAVE         "/FilMachine.json"

#define MAX_STEP_ELEMENTS		  10
#define MAX_PROC_ELEMENTS		  30


/*********************
* Button sizes
*********************/
#define BUTTON_PROCESS_HEIGHT     40
#define BUTTON_PROCESS_WIDTH      95

#define BUTTON_START_HEIGHT       90
#define BUTTON_START_WIDTH        140

#define BUTTON_MBOX_HEIGHT        45
#define BUTTON_MBOX_WIDTH         110

#define BUTTON_POPUP_CLOSE_HEIGHT 30
#define BUTTON_POPUP_CLOSE_WIDTH  30

#define BUTTON_TUNE_HEIGHT        35
#define BUTTON_TUNE_WIDTH         80

#define LOGO_HEIGHT               89
#define LOGO_WIDTH                102


/********************* APPLICATION DEFINES *********************/




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
#define GREY             0xd6d6d6
#define WHITE            0xffffff
#define BLACK            0x000000
#define LIGHT_BLUE       0x1fd3e0
#define LIGHT_BLUE_DARK  0x16939c


/*********************
* Icon chars
*********************/
#define oldCamera_icon      "\xEF\x82\x83" //never used, to add into font.c file in case need to be used
#define plusplus_icon       "\xEF\x84\x81" //never used, to add into font.c file in case need to be used
#define film_icon           "\xEF\x80\x88" //never used, to add into font.c file in case need to be used
#define image_icon          "\xEF\x80\xBE" //never used, to add into font.c file in case need to be used

#define tabProcess_label    "Process list"
#define tabProcess_icon     "\xEF\x85\xA0"
#define tabSetting_label    "Settings"
#define tabSetting_icon     "\xEF\x93\xBE"
#define tabTools_label      "Tools"
#define tabTools_icon       "\xEF\x9F\x99"

#define filter_icon         "\xEF\x87\x9E" //never used, to add into font.c file in case need to be used
#define funnel_icon         "\xEF\x82\xB0"
#define newProcess_icon     "\xEF\x83\xBE"
#define preferred_icon      "\xEF\x80\x84"
#define colorpalette_icon   "\xEF\x94\xBF"
#define temp_icon           "\xEF\x8B\x89"
#define blackwhite_icon     "\xEF\x81\x82"
#define questionMark_icon   "\xEF\x81\x99"
#define closePopup_icon     "\xEF\x80\x8D"
#define play_icon           "\xEF\x81\x8B"

#define save_icon           "\xEF\x83\x87"
#define trash_icon          "\xEF\x8B\xAD"
#define moveUp_icon         "\xEF\x81\xB7" //never used, to add into font.c file in case need to be used
#define moveDown_icon       "\xEF\x81\xB8" //never used, to add into font.c file in case need to be used
#define chemical_icon       "\xEF\x83\x83"
#define rinse_icon          "\xEF\x81\x83"
#define multiRinse_icon     "\xEF\x86\xB8"
#define edit_icon           "\xEF\x81\x84"
#define copy_icon           "\xEF\x83\x85" //never used, to add into font.c file in case need to be used
#define checkStep_icon      "\xEF\x80\x8C"
#define arrowStep_icon      "\xEF\x81\xA1"
#define dotStep_icon        "\xEF\x86\x92"
#define clock_icon          "\xEF\x80\x97"
#define chip_icon           "\xEF\x8B\x9B"
#define alert_icon          "\xEF\x81\xAA"
#define sdCard_icon         "\xEF\x9F\x82"
#define discardAfter_icon   "\xEF\x8B\xB5"

#define INIT_ERROR_SD   1
#define INIT_ERROR_WIRE 2
#define INIT_ERROR_I2C  3

#define initSDError_text "INITIALIZATION ERROR!\nSD-CARD FAILURE!\nFIX SD-CARD!\nTHEN CLICK ON ICON TO REBOOT!"
#define initI2CError_text "INITIALIZATION ERROR!\nI2C MODULE FAILURE\nFIX IT!\nTHEN CLICK ON ICON TO REBOOT!"
#define initWIREError_text "INITIALIZATION ERROR!\nINIT WIRE FAILURE\nFIX IT!\nTHEN CLICK ON ICON TO REBOOT!"

/*********************
* Process tab strings
*********************/
#define buttonFilter_text 				 			"Filters"
#define Processes_text 					 			"Processes"
#define keyboard_placeholder_text 		 			"Write film name..."
#define filterPopupTitle_text 			 			"Process list filter"
#define filterPopupNamePlaceHolder_text  			"Part of name to filter"
#define filterPopupName_text 			 			"Name"
#define filterPopupColor_text 			 			"Color"
#define filterPopupBnW_text 			 			"B&W"
#define filterPopupBoth_text 			 			"Both"
#define filterPopupPreferred_text 		 			"Preferred only"
#define filterPopupApplyButton_text 	 			"Apply"
#define filterPopupResetButton_text 	 			"Reset"


/*********************
* Config tab strings
*********************/
#define Settings_text 								 "Machine settings"
#define tempUnit_text 								 "Temp. unit:"
#define tempSensorTuning_text 						 "Calib. temp. sensor"
#define tuneButton_text 							 "TUNE"
#define tempAlertMBox_text 							 "Verify that the machine's temperature is steady, take a reading of the surrounding air, enter the value in the text field, and press 'Tune' button.To reset the tuning values, long-press the 'Tune' button."
#define soundAlertMBox_text 						 "Continue to sound the alert when a process is about to end or when the heating phase reaches the desired temperature."
#define autostartAlertMBox_text 					 "When the desired temperature is reached, automatically initiate the temperature-controlled process."
#define filmRotationSpeedAlertMBox_text 			 "Stated as rpm, with a range of 20 to 90 revolutions per minute."
#define rotationInverseIntervalAlertMBox_text 		 "The duration, which may be adjusted between 5 and 60 seconds, during which the film will spin in one direction before moving to the other."
#define filmRotationRandomAlertMBox_text 			 "To guarantee a perfectly equal development, you may add a random variation to the inversion time using this option. For instance, if the randomization value is set to ~10% and the inversion time is set to 10 seconds, each inversion will have a different real inversion time, ranging from 8 to 10 seconds."
#define drainFillTimeAlertMBox_text 				 "Both filling and emptying the tank need time and cannot be completed quickly. You can select how much of the filling and draining time has to be taken into account when calculating the processing time of each step (the amount of time you designate for each step of the process) using the overlap % between filling and draining time and processing time. "
#define rotationSpeed_text 							 "Film rotation speed"
#define rotationInversionInterval_text 				 "Rotation inv. interval"
#define rotationRandom_text 						 "Randomness"
#define celsius_text 								 "°C"
#define fahrenheit_text 							 "°F"
#define waterInlet_text 							 "Water inlet link"
#define persistentAlarm_text 						 "Persistent alarms"
#define autostart_text 								 "Process autostart"
#define drainFillTime_text 							 "Drain/fill time overlap"


/*********************
* Tools tab strings/vars
*********************/
#define Maintenance_text 							"Maintenance"
#define Utilities_text 								"Utilities"
#define Statistics_text 							"Statistics"
#define Software_text 								"Software"
#define cleanMachine_text 							"Clean machine"
#define drainMachine_text 							"Drain machine"
#define importConfigAndProcesses_text 				"Import"
#define importConfigAndProcessesMBox_text 			"Import configuration and processes from micro USB drive"
#define exportConfigAndProcesses_text 				"Export"
#define exportConfigAndProcessesMBox_text 			"Export configuration and processes from micro USB drive"
#define statCompleteProcesses_text 					"Completed processes"
#define statTotalProcessTime_text 					"Total process time"
#define statCompleteCleanProcess_text 				"Completed clean cycles"
#define statStoppedProcess_text 					"Stopped processes"
#define softwareVersion_text 						"Software version"
#define softwareVersionValue_text    				"v0.0.0.1"
#define softwareSerialNum_text 						"Serial number"
#define softwareSerialNumValue_text 				"1234567890"
#define softwareCredits_text 						"Credits"

#define softwareCreditsValue_text 					"Credit to Frank P. \nand \nPete B."

uint8_t completedProcesses;
uint8_t totalMinProcesses;
uint8_t completedClean;
uint8_t stoppedProcesses;
lv_obj_t * tempSensorTuneButton;


/*********************
* Process detail strings/vars
*********************/
#define processDetailStep_text 				 		"Steps"
#define processDetailInfo_text 				 		"Details"
#define processDetailIsColor_text 			 		"For color negative"
#define processDetailIsBnW_text 			 		"For b&w negative"
#define processDetailIsTempControl_text		 		"Temp. control"
#define processDetailTemp_text 				 		"Temperature:"
#define processDetailIsPreferred_text 		 		"Preferred:"
#define processDetailTotalTime_text 		 		"Total time:"
#define processDetailTempPlaceHolder_text 	 		"Tap"
#define processDetailTempTolerance_text 	 		"Tolerance:"
#define processDetailPlaceHolder_text 			   		"Enter name"

//uint8_t isPreferred;

//lv_obj_t * processTempTextArea;
//lv_obj_t * processToleranceTextArea;

/*********************
* Step detail strings/vars
*********************/
#define stepDetailTitle_text 						"New step"
#define stepDetailName_text 						"Name:"
#define stepDetailDuration_text 			   		"Duration:             :"
#define stepDetailDurationMinPlaceHolder_text  		"min"
#define stepDetailDurationSecPlaceHolder_text  		"sec"
#define stepDetailType_text 						"Type:"
#define stepDetailSource_text 						"Source:"
#define stepDetailDiscardAfter_text 		   		"Discard after:"
#define stepDetailPlaceHolder_text 			   		"Enter name for new step"
#define stepDetailSave_text 						"Save"
#define stepDetailCancel_text 						"Cancel"
#define stepDetailCurrentTemp_text 			   		"Now:"


static const char* stepTypeList = "Chemistry\n"
		                          "Rinse\n"
		                          "MultiRinse";

static const char* stepSourceList = "C1\n"
		                            "C2\n"
		                            "C3\n"
		                            "WB";

static const char* processSourceList[4] = {"C1", "C2", "C3", "WB"};
static const char* processTempControlList[3] = {"Off", "Run", "Susp."};
                        
uint8_t stepType;
uint8_t stepSource;
uint8_t discardAfter;
double stepSourceTemp;
lv_obj_t * stepDetailNamelTextArea;

/*********************
* Popup elements
*********************/

#define stopProcessPopupTitle_text 					"Stop process"
#define warningPopupTitle_text 						"Warning!"
#define setMinutesPopupTitle_text 					"Set minutes"
#define setSecondsPopupTitle_text 					"Set seconds"
#define tuneTempPopupTitle_text 					"Set temperature"
#define tuneRollerButton_text 						"SET"
#define messagePopupDetailTitle_text 				"Detail information"
#define deleteButton_text 							"Delete"
#define deletePopupTitle_text 						"Delete element"
#define deleteAllProcessPopupTitle_text 						"Delete all process"
#define deletePopupBody_text 						"Are you sure to delete the selected element?"
#define deleteAllProcessPopupBody_text 						"Are you sure to delete all the process created?"
#define warningPopupLowWaterTitle_text 		   		"The water level is too low!Temperature control has been suspended\nRefill the water bath immediately to correct resume the temperature control process"
#define stopNowProcessPopupBody_text			   		"Stopping a process will ruin the film inside the tank and leave the chemistry inside!\nDo you want to stop the process now?"
#define stopAfterProcessPopupBody_text			   		"Do you want to stop the process after this step is completed?"

static const char *currentStep[3] = {"Filling", "Draining", "Processing"};

char rollerElementSelected [10];
lv_obj_t * questionMark;
lv_obj_t * roller;
lv_style_t style_roller;
char tempBuffer [10];


/*********************
* Checkup strings/vars
*********************/
#define checkupNexStepsTitle_text 		"Next steps:"
#define checkupProcessReady_text 		"Process starting..."
#define checkupTheMachineWillDo_text 	"The machine will:"
#define checkupFillWater_text 			"Fill the water bath"
#define checkupTankRotation_text 		"Check tank presence and film rotation"
#define checkupReachTemp_text 			"Reach the chemistry target temperature"
#define checkupStop_text 				"Stop"
#define checkupStart_text 				"Start"
#define checkupSkip_text 				"Skip"
#define checkupStopNow_text 			"Stop now!"
#define checkupStopAfter_text 			"Stop after!"
#define checkupProcessingTitle_text 	"Processing:"
#define checkupStepSource_text 			"Step source:"
#define checkupTempControl_text 		"Temp. control:"
#define checkupWaterTemp_text 			"Water temp:"
#define checkupNextStep_text			"Next step:"
#define checkupSelectBeforeStart_text 	"Select the tank size and chemistry amount and click the 'Start' button"
#define checkupTankSize_text 			"Select tank size"
#define checkupChemistryVolume_text 	"Select chemistry volume"
#define checkupMinimumChemistry_text 	"Minimum required chemistry : 500ml"
#define checkupFillWaterMachine_text 	"The machine is not connected to a water source.\n\nFill the water bath manually up to the top water level sensor"
#define checkupTargetTemp_text 			"Target temperature"
#define checkupWater_text 				"Water"
#define checkupChemistry_text 			"Chemistry"
#define checkupTankPosition_text 		"Tank is in position:"
#define checkupFilmRotation_text 		"Film is rotating:"
#define checkupYes_text 				"Yes"
#define checkupNo_text 					"No"
#define checkupChecking_text 			"Checking..."
#define checkupProcessComplete_text 			"Process\nCOMPLETE!"
#define checkupProcessStopped_text 			"Process\nSTOPPED!"

static const char * checkupTankSizesList = "Small\n"
                                   		   "Medium\n"
                                    	   "Large";


//const char * checkupStepStatuses [3][14] = {"\xEF\x86\x92", "\xEF\x81\xA1", "\xEF\x80\x8C"};  

static const char *checkupStepStatuses[] = { dotStep_icon, arrowStep_icon, checkStep_icon };

static const uint16_t tankFullSizes [3] = {500, 700, 1000}; 
static const uint16_t tankLowSizes  [3] = {250, 350, 550}; 

static const uint8_t tankFullFillDrainTimes [3] = {15, 19, 25}; 
static const uint8_t tankLowFillDrainTimes  [3] = {8, 11, 16}; 

//lv_obj_t * checkupTankSizeTextArea;
#define checkupTankSizePlaceHolder_text "Size"
#define checkupChemistryLowVol_text 	"Low"
#define checkupChemistryHighVol_text 	"High"

//Test long string
#define testLongString "Cantami, o diva, del Pelìde Achille l'ira funesta che infiniti addusse lutti agli achei , molte anzi tempo all'orco generose travolse alme d'eroi, e di cani e d'augelli orrido pasto lor salme abbandonò (così di giove l'alto consiglio s'adempia ) , da quando primamente disgiunse aspra contesa il re de' prodi Atride e il divo Achille."

//processNode	* tempProcessNode;
//stepNode	  * tempStepNode;

extern uint8_t initErrors;
char formatted_string[20];

//RELAY STUFF
static const uint16_t developingRelays[RELAY_NUMBER] = {WATER_RLY,DEV_RLY,STOP_RLY,FIX_RLY,WASTE_RLY,PUMP_IN_RLY,PUMP_OUT_RLY,HEATER_RLY};

//MOTOR STUFF
static const bool rotationFW = true;
long motorIntervalTime;
uint8_t motorIntervalRotation;
static const uint8_t motorIntervalRotationDefault = 10;

uint8_t motorSpeed;
static const uint8_t motorSpeedDefault = 200;

static const uint16_t MotorPins[MOTOR_PIN_NUMBER] = {MOTOR_ENA_PIN,MOTOR_IN1_PIN,MOTOR_IN2_PIN};

//TEMPERATURE STUFF
static uint16_t timerReadTemp = 5000;
static long tempActualTime = 0;
static long tempTempTime = 0;
static int8_t tempValueCheck;
static uint8_t tempSensorResolution = 10;
static bool tempChemicalSensorAvailable = false;
static bool tempBathSensorAvailable = false;
static const float tempTolerance = 0.5;
static bool firstTimeInTemp = true;

static const uint8_t fahrenheitOffset = 32;
static const float lowestTemp = 16.0;
static const float stepTemp = 0.5;
static const float initialTemp = 20.0;
static float manualTemp = initialTemp;
static float DevTemp;
static float Temp_Bath;
static float Temp_Chemical;
static bool  tempReached = false;



/*********************
* ELEMENTS Function Prototypes
*********************/

// @file element_filterPopup.c
void event_filterMBox(lv_event_t *e);
void filterPopupCreate(void);

// @file element_messagePopup.c
void messagePopupCreate(const char *popupTitleText, const char *popupText, const char *textButton1, const char *textButton2, void *whoCallMe);
void event_messagePopup(lv_event_t *e);


// @file element_process.c
void event_processElement(lv_event_t *e);
void processElementCreate(processNode *newProcess, int32_t tempSize);
bool deleteProcessElement( processNode	*processToDelete );
processNode *getProcElementEntryByObject( lv_obj_t *obj );
processNode *addProcessElement(processNode	*processToAdd);

// @file element_rollerPopup.c
void event_Roller(lv_event_t *e);
void rollerPopupCreate(const char * tempOptions,const char * popupTitle, void *whoCallMe, uint32_t currentVal);

// @file element_step.c
void event_stepElement(lv_event_t *e);
void stepElementCreate(stepNode * newStep,processNode * processReference, int8_t tempSize);
bool deleteStepElement( stepNode	*stepToDelete, processNode * processReference );
stepNode *addStepElement(stepNode * stepToAdd, processNode * processReference);

/*********************
* PAGES Function Prototypes
*********************/
void init_globals( void );


// @file page_checkup.c
void event_checkup(lv_event_t *e);
void checkup(processNode *referenceProcess);


// @file page_home.c
void event_btn_start(lv_event_t *e);
void homePage(void);


// @file page_menu.c
void event_tab_switch(lv_event_t *e);
void menu(void);


// @file page_processDetail.c
void event_processDetail(lv_event_t *e);
void event_processDetail_style_delete(lv_event_t *e);
void processDetail(lv_obj_t *referenceProcess);


// @file page_processes.c
void event_processes_style_delete(lv_event_t *e);
void event_tabProcesses(lv_event_t *e);
void processes(void);


// @file page_settings.c
void event_settings_style_delete(lv_event_t *e);
void event_settingPopupMBox(lv_event_t *e);
void event_settings_handler(lv_event_t *e);
void initSettings(void);
void settings(void);


// @file page_stepDetail.c
void event_stepDetail(lv_event_t *e);
void stepDetail(processNode *referenceNode, stepNode *currentNode);

// @file page_tools.c
void event_toolsElement(lv_event_t *e);
void initTools(void);
void tools(void);


// @file accessory.c
uint8_t qSysAction( uint16_t msg );
lv_obj_t * create_radiobutton(lv_obj_t * mBoxParent, const char * txt, const int32_t x, const int32_t y, const int32_t size, const lv_font_t * font, const lv_color_t borderColor, const lv_color_t bgColor);
lv_obj_t * create_text(lv_obj_t * parent, const char * icon, const char * txt);
lv_obj_t * create_slider(lv_obj_t * parent, const char * icon, const char * txt, int32_t min, int32_t max,int32_t val);
lv_obj_t * create_switch(lv_obj_t * parent, const char * icon, const char * txt, bool chk);

void* isNodeInList(void* list, void* node, NodeType_t type);
void* allocateAndInitializeNode(NodeType_t type);

void event_cb(lv_event_t * e);
void event_checkbox_handler(lv_event_t * e);
void event_keyboard(lv_event_t* e);

void createQuestionMark(lv_obj_t * parent,lv_obj_t * element,lv_event_cb_t e, const int32_t x, const int32_t y);
void createMessageBox(char *title, char *text, char *button1Text, char *button2Text);
void create_keyboard();
void showKeyboard(lv_obj_t * whoCallMe);
void hideKeyboard(lv_obj_t * whoCallMe);
char *createRollerValues( uint32_t maxVal, const char* extra_str );
uint8_t SD_init();
void initSD_I2C_MCP23017();
void initMCP23017Pins();

void calcolateTotalTime(processNode *processNode);
uint8_t calcolatePercentage(uint32_t minutes, uint8_t seconds, uint32_t total_minutes, uint8_t total_seconds);
float convertCelsiusoToFahrenheit(uint32_t tempC);
void updateProcessElement(processNode *process);
void updateStepElement(processNode *referenceProcess, stepNode *step);
uint32_t loadSDCardProcesses();
char * generateRandomCharArray(int length);

void initializeRelayPins();
void sendValueToRelay(uint16_t pumpFrom, uint16_t pumpDir); 
void initializeMotorPins();
void stopMotor(uint8_t pin1, uint8_t pin2);
void runMotorFW(uint8_t pin1, uint8_t pin2);
void runMotorRV(uint8_t pin1, uint8_t pin2);
void setMotorSpeedFast(uint8_t pin,uint8_t spd);
void setMotorSpeedUp(uint8_t pin, uint8_t spd);
void setMotorSpeedDown(uint8_t pin, uint8_t spd);
void enableMotor(uint8_t pin);
void testPin(uint8_t pin);
//float getTemperature(DeviceAddress sensor);
void initializeTemperatureSensor();
void printTemperature(float temp);
//char* printAddressSensor(DeviceAddress deviceAddress);
void writeMachineStats(machineStatistics machineStats);
void readMachineStats(machineStatistics machineStats);

void toLowerCase(char *str);
int caseInsensitiveStrstr(const char *haystack, const char *needle);
void filterAndDisplayProcesses( void );
void removeFiltersAndDisplayAllProcesses( void );
void emptyList(void *list, NodeType_t type);

//@file initDisplay.c
void my_disp_flush(lv_display_t* display, const lv_area_t* area, unsigned char* data);
void my_touchpad_read(lv_indev_t* dev, lv_indev_data_t* data);

//@file THeBeginningObject.ino
//void eventSave(lv_event_t * e);

extern void (*rebootBoard)(void);
#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*DEFINITIONS_H*/