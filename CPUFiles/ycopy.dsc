/**************************************************************************
**
**  FILE        :  @(#)ycopy.dsc 
**
**  VERSION     :  05/21/05
**
**  DESCRIPTION :  This locator file combines all the copy sections.
**
**      NOTE    :  To add more copy sections, create a new cluster
**				   and a new copyX.dsc file.
**
**************************************************************************/

cluster Y_Copy1 
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy1.dsc"
            }
        }
    }
}

cluster Y_Copy2 
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy2.dsc"
            }
        }
    }
}

cluster Y_Copy3 
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy3.dsc"
            }
        }
    }
}

cluster Y_Copy4 
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy4.dsc"
            }
        }
    }
}

cluster Y_Copy5 
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy5.dsc"
            }
        }
    }
}

cluster Y_Copy6 
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy6.dsc"
            }
        }
    }
}

cluster Y_Copy7 
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy7.dsc"
            }
        }
    }
}

cluster Y_Copy8 
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy8.dsc"
            }
        }
    }
}

cluster Y_Copy9 
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy9.dsc"
            }
        }
    }
}

cluster Y_Copy10 
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy10.dsc"
            }
        }
    }
}

cluster Y_Copy11 
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy11.dsc"
            }
        }
    }
}

cluster Y_Copy12 
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy12.dsc"
            }
        }
    }
}

cluster Y_Copy13 
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy13.dsc"
            }
        }
    }
}

cluster Y_Copy14 
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy14.dsc"
            }
        }
    }
}

cluster Y_Copy15 
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy15.dsc"
            }
        }
    }
}

cluster Y_Copy16
{   amode Y_far
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy16.dsc"
            }
        }
    }
}

cluster Y_Copy17
{   amode Y_far
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy17.dsc"
            }
        }
    }
}

cluster Y_Copy18
{   amode Y_far
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy18.dsc"
            }
        }
    }
}

cluster Y_Copy19
{   amode Y_far
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy19.dsc"
            }
        }
    }
}

cluster Y_Copy20
{   amode Y_far
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy20.dsc"
            }
        }
    }
}

cluster Y_Copy21
{   amode Y_far
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copy21.dsc"
            }
        }
    }
}

/***********************************************************************
** This is the catch all file. Keep it always the last in the list
***********************************************************************/
cluster Y_CopyLast
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "..\..\..\..\..\CPUFiles\copylast.dsc"
            }
        }
    }
}
