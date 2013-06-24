//=============================================================================
//
// DESCRIPTION:
//
// Copyright (c) SigmaTel, Inc. Unpublished
// 
// SigmaTel, Inc.
// Proprietary  Confidential
// 
// This source code and the algorithms implemented therein constitute 
// confidential information and may comprise trade secrets of SigmaTel, Inc. 
// or its associates, and any unauthorized use thereof is prohibited.
//
//=============================================================================

#ifndef TASK_COPY
#define TASK_COPY
#endif

contiguous 
{               
    section .ptextdbwriterecord                     TASK_COPY;
    overlay 
    {
        contiguous
        {
            section .ptextdbopen                    TASK_COPY;
        }
        contiguous
        {
            section .ptextdbclose                   TASK_COPY;
        }
        contiguous
        {
            section .ptextdbgetnumrecords           TASK_COPY;
            section .ptextdbgetnumdeletedrecords    TASK_COPY;
            section .ptextdbnewrecord               TASK_COPY;
//            section .ptextdbgetstring               TASK_COPY;
            section .ptextdbsetstring               TASK_COPY;
//            section .ptextdbgetnumber               TASK_COPY;
            section .ptextdbsetnumber               TASK_COPY;
            section .ptextdbgetblock                TASK_COPY;
            section .ptextdbsetblock                TASK_COPY;
        }
        contiguous
        {
            section .ptextdbdeleterecord            TASK_COPY;
        }
    }
    contiguous
    {
        overlay {
        section .ptextstrebuild                 TASK_COPY;
        section .ptextstaddmetadata             TASK_COPY;
        }
    }
}
