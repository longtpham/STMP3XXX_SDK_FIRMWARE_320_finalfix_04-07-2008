////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: version.c
// Description: Accessor functions for version information
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "project.h"
#include "version.h"

////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Externs
////////////////////////////////////////////////////////////////////////////////

    extern WORD g_wVersionMajor;
    extern WORD g_wVersionMinor;

////////////////////////////////////////////////////////////////////////////////
// defs
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
#if 0 // Mar 1 2007: historical code, not in use anymore and so comment it out
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SysGetComponentVersionDecimal
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant SysGetComponentVersionDecimal(VERSION_COMPONENT * pVersion)
{
    pVersion->Decimal.Major = g_wVersionMajor;
    pVersion->Decimal.Minor = g_wVersionMinor;
    
    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SysGetComponentVersionAscii
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant SysGetComponentVersionAscii(VERSION_COMPONENT * pVersion)
{
    WORD wTemp;
    
    if(!(g_wVersionMajor < 10) || !(g_wVersionMinor < 1000))
    {
        pVersion->Ascii.Major_Char = 0x39;
        pVersion->Ascii.Minor_Char_0 = 0x39;
        pVersion->Ascii.Minor_Char_1 = 0x39;
        pVersion->Ascii.Minor_Char_2 = 0x39;
        return VERSION_ERROR_NON_STANDARD_VERSION;
    }

    pVersion->Ascii.Major_Char = g_wVersionMajor + 0x30;

    // Get the hundreds
    wTemp = g_wVersionMinor;
    pVersion->Ascii.Minor_Char_2 = 0;
    while(1)
    {
        if(wTemp >= 100)
        {
            pVersion->Ascii.Minor_Char_2++;
            wTemp -= 100;
        }
        else
        {
            break;
        }
    }
    
    // Convert it to ASCII
    pVersion->Ascii.Minor_Char_2 += 0x30;

    // Get the tens
    pVersion->Ascii.Minor_Char_1 = 0;
    while(1)
    {
        if(wTemp >= 10)
        {
            pVersion->Ascii.Minor_Char_1++;
            wTemp -= 10;
        }
        else
        {
            break;
        }
    }
    
    // Convert it to ASCII
    pVersion->Ascii.Minor_Char_1 += 0x30;

    // Get the ones
    pVersion->Ascii.Minor_Char_0 = wTemp + 0x30;

    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SysGetComponentVersionBcd
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant SysGetComponentVersionBcd(VERSION_COMPONENT * pVersion)
{
    WORD wTemp;
    
    if(!(g_wVersionMajor < 10) || !(g_wVersionMinor < 1000))
    {
        pVersion->Bcd.Major_Digit = 9;
        pVersion->Bcd.Minor_Digit_0 = 9;
        pVersion->Bcd.Minor_Digit_1 = 9;
        pVersion->Bcd.Minor_Digit_2 = 9;
        return VERSION_ERROR_NON_STANDARD_VERSION;
    }

    pVersion->Bcd.Major_Digit = g_wVersionMajor;

    // Get the hundreds
    wTemp = g_wVersionMinor;
    pVersion->Bcd.Minor_Digit_2 = 0;
    while(1)
    {
        if(wTemp >= 100)
        {
            pVersion->Bcd.Minor_Digit_2++;
            wTemp -= 100;
        }
        else
        {
            break;
        }
    }
    
    // Get the tens
    pVersion->Bcd.Minor_Digit_1 = 0;
    while(1)
    {
        if(wTemp >= 10)
        {
            pVersion->Bcd.Minor_Digit_1++;
            wTemp -= 10;
        }
        else
        {
            break;
        }
    }
    
    // Get the ones
    pVersion->Bcd.Minor_Digit_0 = wTemp;

    return SUCCESS;
}
#endif
