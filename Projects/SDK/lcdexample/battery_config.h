///////////////////////////////////////////////////////////////////////////////
// Sigmatel, Inc  copyright (C) 2003-2004
// Filename: battery_config.h
// Battery Low Resolution ADC values & recharge values 
///////////////////////////////////////////////////////////////////////////////

#ifdef BATTERY_TYPE_LI_ION
    //these are 3500 only
    #define VBATT_REF 	        REF_5120MV
    #define VBATT_MIN_MV        3000
    #define VBATT_MAX_MV        4200

    #define VBATT_BO_MV         3000

    #define BATT_BROWNOUT       VBATT_BO_MV

    #ifndef LIION_BATTERY_VOLTAGE
    //this is in mV
    //The reference design uses a 4.2V LiIon battery
    #define LIION_BATTERY_VOLTAGE LIION_BATTERY_VOLTAGE_4_2
    #endif 

    #ifndef LIION_CONDITIONING_VOLTAGE_THRESHHOLD
    //This is in millivolts
    #define LIION_CONDITIONING_VOLTAGE_THRESHHOLD 3000
    #endif

    #ifndef LIION_CONDITIONING_CURRENT_LIMIT
    //This is in mA.   
    //Refer to the battery data sheet for the proper value
    //the reference design uses a 400mAh battery that suggests 40mA is a good conditioning current
    #define LIION_CONDITIONING_CURRENT_LIMIT 40
    #endif 

    #ifndef LIION_CHARGING_TIMEOUT
    //the default is 4 hours
    #define LIION_CHARGING_TIMEOUT  ((DWORD)(DWORD)4*(DWORD)60*(DWORD)60*(DWORD)1000)
    #endif 

    #ifndef LIION_CHARGING_DIFFERENTIAL_THRESHHOLD
    //We want to enter the 'top-off' mode when the voltage differential between
    //charging on and off is 20 mV
    #define LIION_CHARGING_DIFFERENTIAL_THRESHHOLD  20
    #endif 

    #ifndef LIION_CHARGING_DIFFERENTIAL_THRESHHOLD_COUNT
    //we want 10 samples of the non-charging current to be within the 
    //differential threshhold.  

#ifdef STMP_BUILD_PLAYER
    #define LIION_CHARGING_DIFFERENTIAL_THRESHHOLD_COUNT 5
#else
    #define LIION_CHARGING_DIFFERENTIAL_THRESHHOLD_COUNT 10
#endif
    #endif 

    #ifndef LIION_CHARGING_CURRENT_LIMIT
    //This is in mA.   
    //Refer to the battery data sheet for the proper value
    //the reference design uses a 400mAh battery that suggests 400mA is a good charging current
    #define LIION_CHARGING_CURRENT_LIMIT 400
    #endif 

    #ifndef LIION_TOPOFF_TIMEOUT
    //After we reach that limit, we want to run for a set amount of time to 'top off' the charging
    //in this case, 2 minutes (was 30)
#ifdef STMP_BUILD_PLAYER
    #define LIION_TOPOFF_TIMEOUT ((DWORD)(DWORD)15*(DWORD)60*(DWORD)1000)
#else
    #define LIION_TOPOFF_TIMEOUT ((DWORD)(DWORD)2*(DWORD)60*(DWORD)1000)
#endif
    #endif

	#ifndef POWER_OFF_FREQUENCY
	//Set the Power Off Frequency.  Determines how frequently charging is turned-off.  
#ifdef STMP_BUILD_PLAYER
	#define POWER_OFF_FREQUENCY 40
#else
	#define POWER_OFF_FREQUENCY 500
#endif
	#endif
#endif

////////////////////////////////////////////////////////////////////////////////
#ifdef BATTERY_TYPE_NIMH
    #define ONE  1
    #define ZERO 0

    //NOTE:  Battery is currently Hardcoded to REF_5120MV in USBMSC
#ifdef STMP_BUILD_PLAYER
    #define VBATT_REF 	       REF_2700MV
    #define VBATT_MIN_MV       900
    #define VBATT_MAX_MV       1500
    #define VBATT_BO_MV        840
#else
    #define VBATT_REF 	       REF_5120MV
    #define VBATT_MIN_MV       900
    #define VBATT_MAX_MV       1200
    #define VBATT_BO_MV        850
#endif

    #ifdef DEVICE_3500
        #define BATT_BROWNOUT				VBATT_BO_MV
    #else
        #define BATT_BROWNOUT               HW_LRADC_BO_TRIP_830MV_SETMASK
    #endif
    // nimh settings above this line are similar to alkaline values below except v max.
    
    #ifndef NIMH_BATTERY_VOLTAGE
    //Units: mV. This is the rated battery voltage.
    //The reference design uses a 1.5V NIMH battery
    #define NIMH_BATTERY_VOLTAGE NIMH_BATTERY_VOLTAGE_1_2
    #endif 

    #ifndef NIMH_VOLTAGE_THRESHHOLD_MV
    //Units: mV      Stop charging if this ADC battery voltage is measured. This can occur if an alkaline is inserted.
    #define NIMH_VOLTAGE_THRESHHOLD_MV 1800
    #endif
    // 1 enables alkaline detection via brief charge current & voltage threshold check.
    #define ALKALINE_DETECTION_IMPULSE_CHARGE_ENABLE 0 /* LEAVE DISABLED. Unimplemented. */

    //------- NIMH Detection via exposed batt case check w/ gpio. You must configure a gpio reg & pin num also below.
    #define BATTERY_CASE_POLARITY_DETECT_ENABLE 0 // Verified. GPIO pin setup in nimh.c init, External pullup R (47kOhm)
    // Explanation: General Purpose Input pin can connect to cylindrical batt end corner. 
    // NiMH AAA batteries have ground case exposed at corner, else casing insulation 
    //   causes signal to float & external pullup R gives a logic 1 indicating non-nimh. 
    // NiMH AA  batteries instead have additional positive terminal exposure (opposite polarity). 
    //   Ext transistor can Vlevel shift for gpio logic-1 voltage. 
    //   non-NIMH casing will allow gpio pin to stay grounded & gives logic 0.

    // SET this to ZERO if your NiMH is AAA or has ground case contact (detects NiMH via case (-) contact)
    // SET this to ONE if your NiMH battery is AA (detects NiMH via (+) terminal contact). Requires ext transistor V level shift.
    #define NIMH_BATTERY_CASE_LOGIC_LEVEL ZERO
    // *** IF YOU HAVE ENABLED, battery case polarity detection (used with nimh), 
    //  NOTE: The GP number and bit number in the four macros below must be set to match your input PIN number. 
    //    LAYOUT SPECIFIC, but all 3550/3560 qfp and bga can use a single GPIO register & bit num as below. 
    //    SigmaTel HW apps recommends pin GP37= pin 17 (100qfp)= pin j7 (bga)= cpu card pin24= GP1 Bit13 per 35xx datasheet.
    #define CONFIGURED_GPIO_INPUT_BIT_FOR_BATTERY_CASE_POLARITY ( HW_GP1DIR.B.B13 )
    // GPIO Enable register number as GPIO function.
    #define CONFIGURED_BATT_CASE_GP_NUM_ENR (HW_GP1ENR)
    // Data output enable reg num to use (will be set as input pin in this case)
    #define CONFIGURED_BATT_CASE_GP_NUM_DOER  (HW_GP1DOER)
    // bit num to use with desired input pin above
    #define CONFIGURED_BATT_CASE_GP_BIT  (HW_GP_B13_SETMASK)
    
    // Error code returned during charging init when case detect feature enabled but cell case not detected.
    #define NIMH_POLARITY_NOT_DETECTED (-2)
    //------- end batt case polarity detection

    //------------------------------------
    #ifndef NIMH_EQUILIBRIUM_CHARGING_TIMEOUT_MS
    // 0.1C = 10 hr charge, but use 12 hours to more fully charge larger cells. (units: msec)
    #define NIMH_EQUILIBRIUM_CHARGING_TIMEOUT_MS ((DWORD)((DWORD)12*(DWORD)60*(DWORD)60*(DWORD)1000))
    //#define NIMH_EQUILIBRIUM_CHARGING_TIMEOUT_MS ((DWORD)((DWORD)10*(DWORD)1000)) //10 sec test mode to easily observe transition
    #endif 
    
    #ifndef NIMH_EQUILIBRIUM_CHARGING_CURRENT_LIMIT
    // Units: mA. Refer to the battery data sheet for the proper value. 
    // Usually 1 C AAA means a charge of 700 mAh, 750 mAh for a few AAA cells (energizer).
    // Assume 700mAh battery so EQUILIBRIUM charge at 0.1C rate (1/10th charge/hr) = 70mA. 
    // Register options are 60 or 75mA. Suggest 75mA for AAA. AA's have wide mAh range. Use 1/10th of mAh here:
    #define NIMH_EQUILIBRIUM_CHARGING_CURRENT_LIMIT 75
    #endif   
    #ifndef
    #define NIMH_TRICKLE_CHARGE_ENABLE 1
    #endif
    #ifndef NIMH_TRICKLE_CHARGE_CURRENT
    #define NIMH_TRICKLE_CHARGE_CURRENT 10  
    #endif //25mA is 0.035C rate for a 700mAh cell. 10mA recommended by 35xx charger designer.
    //-------------------------------------
    #ifndef NIMH_QUICK_CHARGING_TIMEOUT_MS   // NOTE: C/3 quick charging (3.6hrs) is not implemented.
    // ~4 hr charge rate of C/3 terminated by one of 3 events: (per Duracell whitepaper) 
    //   -dV/minute of 10mV, or 3.6hr timeout (216 minutes), or real temperature>=(60celcius|140F).
    // Thermal warning: at this C/3 charge rate, all 3 of the above termination events must be checked.
    #define NIMH_QUICK_CHARGING_TIMEOUT_MS ((DWORD)(DWORD)216*(DWORD)60*(DWORD)1000) 
    #endif
    #ifndef NIMH_QUICK_CHARGING_MV_PER_MINUTE_THRESHOLD
    #define NIMH_QUICK_CHARGING_MV_PER_MINUTE_THRESHOLD 10
    #endif
    #ifndef NIMH_QUICK_CHARGING_TEMPERATURE_LIMIT_C
    // For temp check to function:    10celcius|50F < Ambient Temperature < 45celcius|113F
    #define NIMH_QUICK_CHARGING_TEMPERATURE_LIMIT_C 60
    #endif
    #ifndef NIMH_QUICK_CHARGING_TEMPERATURE_LIMIT_F
    #define NIMH_QUICK_CHARGING_TEMPERATURE_LIMIT_F 140
    #endif
    // C/3 = 700mA/3 = 233 mA, HW supports 235mA. For C=750mAh: 750mA/3= 250mA. 
    #ifndef NIMH_QUICK_CHARGING_CURRENT_LIMIT
    #define NIMH_QUICK_CHARGING_CURRENT_LIMIT 235        
    #endif    
    //-------------------------------------
    #ifndef NIMH_FAST_CHARGING_CURRENT_LIMIT  // NOTE: C/2 Fast charging (2hrs) is not implemented. 
    // in units of mA.   Temperature measurement required with this mode. 
    #define NIMH_FAST_CHARGING_CURRENT_LIMIT 400
    #endif      
    #ifndef NIMH_FAST_CHARGING_TOPOFF_TIMEOUT
    //After we reach that limit, we may want to run for a set amount of time to 'top off'
    //the charging in this case,                       n minutes
    #define NIMH_FAST_CHARGING_TOPOFF_TIMEOUT ((DWORD)(DWORD)30*(DWORD)60*(DWORD)1000)
    #endif

	#ifndef POWER_OFF_FREQUENCY
	//Set the Power Off Frequency Count.  Determines how frequently charging is turned-off. 
#ifdef STMP_BUILD_PLAYER
	#define POWER_OFF_FREQUENCY 40
#else
	#define POWER_OFF_FREQUENCY 500
#endif
	#endif
    
#endif // BATTERY_TYPE_NIMH
////////////////////////////////////////////////////////////////////////////////

#ifdef BATTERY_TYPE_ALKALINE
#ifdef STMP_BUILD_PLAYER
    #define VBATT_REF 	       REF_2700MV
    #define VBATT_MIN_MV       900
    #define VBATT_MAX_MV       1500
    #define VBATT_BO_MV        840
#else
    #define VBATT_REF 	       REF_5120MV
    #define VBATT_MIN_MV       900
    #define VBATT_MAX_MV       1500
    #define VBATT_BO_MV        850
#endif

    #define BATT_BROWNOUT				VBATT_BO_MV
    
    #define OXYRIDE_THRESHOLD_MV  1550     // Battery considered Oxyride if avg voltage exceeds 1.55v
    #define OXYRIDE_SETBACK_MV     200     // Battery Voltage - VDDD < 200mv for VDDIO stability
#endif 
