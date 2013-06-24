/**************************************************************************
**
**  FILE        :  @(#)ycopy.dsc 
**
**  VERSION     :  05/21/05
**
**  DESCRIPTION :  This locator file combines all the copy sections.
**
**      NOTE    :  To add more copy sections, create a new cluster
**				   and a new local_copyX.dsc file.
**
**************************************************************************/

cluster Y_LocalCopy1 
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "local_copy1.dsc"
            }
        }
    }
}

cluster Y_LocalCopy2 
{   amode Y_far 
    {   overlay
        {   contiguous
            {
                #include "local_copy2.dsc"
            }
        }
    }
}
