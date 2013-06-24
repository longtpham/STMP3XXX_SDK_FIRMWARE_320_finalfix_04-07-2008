#pragma once
#include "atlimage.h"
#include "afxwin.h"

extern CString APP_CONFIG_BMP_FILE_PATH;

class CUserImage : public CImage
{
public:
    HRESULT Load( const char *pname )
    {
		if( pname =="" )
		{
			return 1;
		}

        char tmpdir[_MAX_PATH];
        char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];

        GetModuleFileName( NULL, tmpdir, _MAX_PATH );
        _splitpath( tmpdir, drive, dir, fname, ext );
        _makepath( tmpdir, drive, dir, "", "" );

        CString str = tmpdir;
        str+="skin\\";
        str+=APP_CONFIG_BMP_FILE_PATH;
        str+="\\";
        str += pname;

        HRESULT ret = CImage::Load( str );
        if( ret != 0 )
        {
#if _DEBUG
            AfxMessageBox( str );
#endif
        }

        return ret;
    }
};

