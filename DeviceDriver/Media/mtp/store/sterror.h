/**
***
*** Filename: StError.h
*** Description: list of all error codes used by store 
***     
***
*** Copyright (c) SigmaTel, Inc. Unpublished
***
*** SigmaTel, Inc.
*** Proprietary & Confidential
***
*** This source code and the algorithms implemented therein constitute
*** confidential information and may comprise trade secrets of SigmaTel, Inc.
*** or its associates, and any use thereof is subject to the terms and
*** conditions of the Confidential Disclosure Agreement pursuant to which this
*** source code was originally received.
**/
#ifndef __STERROR_H_
#define __STERROR_H_

//! \file StError.h
//! header to define all error values.

#define STOR_ERROR_NONE                                      0
#define STOR_ERROR_INVALID_OBJECT_HANDLE                    (DB_ERROR_LAST-1)       //0xFFFF00
#define STOR_ERROR_EMPTY                                    (DB_ERROR_LAST-2)       //0xFFFEFF
#define STOR_ERROR_OPENING_FILE                             (DB_ERROR_LAST-3)       //0xFFFEFE
#define STOR_ERROR_CHDIR                                    (DB_ERROR_LAST-4)       //0xFFFEFD  
#define STOR_ERROR_CREATING_FILE                            (DB_ERROR_LAST-5)       //0xFFFEFC
#define STOR_ERROR_WRITING_FILE                             (DB_ERROR_LAST-6)       //0xFFFEFB
#define STOR_ERROR_MORE_DATA                                (DB_ERROR_LAST-7)       //0xFFFEFA  
#define STOR_ERROR_READING_FILE                             (DB_ERROR_LAST-8)       //0xFFFEF9
#define STOR_ERROR_HANDLEARRAY_OVERFLOW                     (DB_ERROR_LAST-9)       //0xFFFEF8  
#define STOR_ERROR_FILESYSTEM                               (DB_ERROR_LAST-10)      //0xFFFEF7
#define STOR_ERROR_METADATA_NOT_FOUND                       (DB_ERROR_LAST-11)      //0xFFFEF6  
#define STOR_ERROR_OBJECT_READONLY                          (DB_ERROR_LAST-12)      //0xFFFEF5
#define STOR_ERROR_DUPLICATE_NAME                           (DB_ERROR_LAST-13)      //0xFFFEF4
#define STOR_ERROR_RMDIR                                    (DB_ERROR_LAST-14)       //0xFFFEFD  

#endif //!__STERROR_H_