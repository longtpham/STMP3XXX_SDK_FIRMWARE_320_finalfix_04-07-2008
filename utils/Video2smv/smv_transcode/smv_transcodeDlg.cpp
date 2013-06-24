// smv_transcodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "smv.h"
#include "smv_transcode.h"
#include "smv_transcodeDlg.h"
#include ".\smv_transcodedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define VIDEO_BASE_TIME  (LONGLONG)10000000

#define LIST_BOX_DISTANCE_BODER 2

#define LIST_SEG_NAME    0
#define LIST_SEG_START   1
#define LIST_SEG_END     2
#define LIST_SEG_SIZE    3
#define LIST_SEG_STATUS  4

BOOL GetSkinProfile( char *name, int *pos_x, int *pos_y );
void WriteSkinProfile( char *name, int pos_x, int pos_y );

#define MAX_INI_LEN_BUFFER 300

ILenum gScalingFilter = ILU_LINEAR;
void ImageProcess( ILint image )
{
    ilBindImage( image );
    iluImageParameter( ILU_FILTER, gScalingFilter );
}

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    enum { IDD = IDD_ABOUTBOX };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
    DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Csmv_transcodeDlg dialog
Csmv_transcodeDlg::Csmv_transcodeDlg(CWnd* pParent /*=NULL*/)
    : CDialog(Csmv_transcodeDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Csmv_transcodeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_ADD_ITEM, m_btnAddItem);
    DDX_Control(pDX, IDC_DELETE_ITEM, m_btnDeleteItem);
    DDX_Control(pDX, IDC_TRANS_ITEM, m_btnTransItem);
    DDX_Control(pDX, IDC_BTN_ABOUT, m_btnAbout);
    DDX_Control(pDX, IDC_LIST_ITEM, m_listItems);
    DDX_Control(pDX, IDC_QUALITY, m_cmbQuality);
    DDX_Control(pDX, IDC_SAMPLE_RATE, m_cmbSampleRate);
    DDX_Control(pDX, IDC_GRAPHIC_RATE, m_cmbGraphicRate);
    DDX_Control(pDX, IDC_FRAME_RATE, m_cmbFrameRate);
    DDX_Control(pDX, IDC_ROTATE, m_cmbRotate);
    DDX_Control(pDX, IDC_PLAY_PREVIOUS, m_btnPrevious);
    DDX_Control(pDX, IDC_PLAY_NEXT, m_btnNext);
    DDX_Control(pDX, IDC_PLAY_PLAY, m_btnPlay);
    DDX_Control(pDX, IDC_PLAY_PAUSE, m_btnPause);
    DDX_Control(pDX, IDC_PLAY_STOP, m_btnStop);
    DDX_Control(pDX, IDC_FULL_IMAGE, m_btnFullImage);
    DDX_Control(pDX, IDC_FULL_SCREEN, m_btnFullScreen);
    DDX_Control(pDX, IDC_SCREEN_SIZE, m_cmbSize);
    DDX_Control(pDX, IDC_RANGE_SLIDER, m_Slider);
    DDX_Control(pDX, IDC_AS_START, m_btnSetAsStart);
    DDX_Control(pDX, IDC_AS_END, m_btnSetAsEnd);
    DDX_Control(pDX, IDC_HIDE_MENU, m_btnMenuBar);
    DDX_Control(pDX, IDC_MENU_MIN, m_btnMenuMin);
    DDX_Control(pDX, IDC_MENU_MAX, m_btnMenuMax);
    DDX_Control(pDX, IDC_MENU_CLOSE, m_btnMenuClose);
    DDX_Control(pDX, IDC_STATIC_SIZE, m_StaticSize);
    DDX_Control(pDX, IDC_STATIC_QUALITY, m_StaticQuality);
    DDX_Control(pDX, IDC_STATIC_SAMPLE_RATE, m_StaticSampleRate);
    DDX_Control(pDX, IDC_STATIC_AUDIO_CHANNEL, m_StaticAudioChannel);
    DDX_Control(pDX, IDC_STATIC_GRAPHIC_RATE, m_StaticGraphicRate);
    DDX_Control(pDX, IDC_STATIC_FRAME_RATE, m_StaticFrameRate);
    DDX_Control(pDX, IDC_STATIC_ROTATE, m_StaticRotate);
    DDX_Control(pDX, IDC_STATIC_GROUP, m_StaticGroupBox);
    DDX_Control(pDX, IDC_STATIC_PROGRESS, m_StaticProgress);
    DDX_Control(pDX, IDC_STATIC_PLAYBACK, m_StaticPlayGround);
    DDX_Control(pDX, IDC_AUDIO_MONO, m_btnAudioMono);
    DDX_Control(pDX, IDC_AUDIO_STEREO, m_btnAudioStereo);
    DDX_Control(pDX, IDC_SETTING, m_btnSetting);
    DDX_Control(pDX, IDC_IDLE_FOCUS, m_btnIdleFocus);
}

BEGIN_MESSAGE_MAP(Csmv_transcodeDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_QUERYDRAGICON()
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_HSCROLL()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_BN_CLICKED(IDC_PLAY_PREVIOUS, OnBnClickedPlayPrevious)
    ON_BN_CLICKED(IDC_PLAY_NEXT, OnBnClickedPlayNext)
    ON_BN_CLICKED(IDC_PLAY_PLAY, OnBnClickedPlayPlay)
    ON_BN_CLICKED(IDC_PLAY_PAUSE, OnBnClickedPlayPause)
    ON_BN_CLICKED(IDC_PLAY_STOP, OnBnClickedPlayStop)
    ON_BN_CLICKED(IDC_ADD_ITEM, OnBnClickedAddItem)
    ON_BN_CLICKED(IDC_DELETE_ITEM, OnBnClickedDeleteItem)
    ON_BN_CLICKED(IDC_TRANS_ITEM, OnBnClickedTransItem)
    ON_BN_CLICKED(IDC_BTN_ABOUT, OnBnClickedAbout)
    ON_BN_CLICKED(IDC_AS_START, OnBnClickedAsStart)
    ON_BN_CLICKED(IDC_AS_END, OnBnClickedAsEnd)
    ON_BN_CLICKED(IDC_HIDE_MENU, OnBnClickedHideMenu)
    ON_BN_CLICKED(IDC_FULL_IMAGE, OnBnClickedFullImage)
    ON_BN_CLICKED(IDC_FULL_SCREEN, OnBnClickedFullScreen)
    ON_BN_CLICKED(IDC_MENU_MIN, OnBnClickedMenuMin)
    ON_BN_CLICKED(IDC_MENU_MAX, OnBnClickedMenuMax)
    ON_BN_CLICKED(IDC_MENU_CLOSE, OnBnClickedMenuClose)
    ON_BN_CLICKED(IDC_AUDIO_MONO, &Csmv_transcodeDlg::OnBnClickedAudioMono)
    ON_BN_CLICKED(IDC_AUDIO_STEREO, &Csmv_transcodeDlg::OnBnClickedAudioStereo)
    ON_BN_CLICKED(IDC_SETTING, OnBnClickedSetting)
    ON_COMMAND(ID_LISTMENU_INSERT, OnListmenuInsert)
    ON_COMMAND(ID_LISTMENU_DELETE, OnListmenuDelete)
    ON_COMMAND(ID_LISTMENU_DELETEALL, OnListmenuDeleteall)
    ON_COMMAND(ID_LISTMENU_SELECT, OnListmenuSelect)
    ON_COMMAND(ID_LISTMENU_SELECTALL, OnListmenuSelectall)
    ON_COMMAND(ID_LISTMENU_DESELECT, OnListmenuDeselect)
    ON_COMMAND(ID_LISTMENU_DESELECTALL, OnListmenuDeselectall)
    ON_COMMAND(ID_FILE_ADD, OnFileAdd)
    ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectall)
    ON_COMMAND(ID_EDIT_DESELECTALL, OnEditDeselectall)
    ON_COMMAND(ID_FILE_SAVEDIRECTORY, OnFileSavedirectory)
    ON_COMMAND(ID_EDIT_DELETEALL, OnEditDeleteall)
    ON_COMMAND(ID_FILE_CONVERT, OnFileConvert)
    ON_COMMAND(ID_FILE_STOP, OnFileStop)
    ON_COMMAND(ID_SKIN_SKIN1, OnSkinSkin1)
    ON_COMMAND(ID_SKIN_SKIN2, OnSkinSkin2)
    ON_COMMAND(ID_SKIN_SKIN3, OnSkinSkin3)
    ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
    ON_CBN_SELCHANGE(IDC_QUALITY, OnCbnSelchangeQuality)
    ON_CBN_SELCHANGE(IDC_SAMPLE_RATE, OnCbnSelchangeSampleRate)
    ON_CBN_SELCHANGE(IDC_AUDIO_CHANNEL, OnCbnSelchangeAudioChannel)
    ON_CBN_SELCHANGE(IDC_GRAPHIC_RATE, OnCbnSelchangeGraphicRate)
    ON_CBN_SELCHANGE(IDC_FRAME_RATE, OnCbnSelchangeFrameRate)
    ON_CBN_SELCHANGE(IDC_ROTATE, OnCbnSelchangeRotate)
    ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_ITEM, OnLvnKeydownListItem)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ITEM, OnLvnItemchangedListItem)
    ON_MESSAGE(WM_USER_LIST_CONTEXT, OnListRButton)
    ON_MESSAGE(WM_USER_REFRESH_SKIN, OnRefreshSkin)
    ON_MESSAGE(WM_UPDATE,OnProgress)
    ON_MESSAGE(WM_SHOWMESSAGE,OnShowMessage)
    ON_MESSAGE(WM_USER_CONTINUE_CONVERT,ConvertNext)
END_MESSAGE_MAP()


// Csmv_transcodeDlg message handlers

BOOL Csmv_transcodeDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // TODO: Add extra initialization here
    m_hPreviewBitmap = NULL;

    char tmpdir[_MAX_PATH];
    char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
    GetModuleFileName( NULL, tmpdir, _MAX_PATH );
    _splitpath( tmpdir, drive, dir, fname, ext );
    _makepath( tmpdir, drive, dir, "", "" );
    APP_CONFIG_INI_FILE_NAME = tmpdir;
    APP_CONFIG_INI_FILE_NAME += "config.ini";

    g_SavedDirectory = "";
    m_blNeedNext = FALSE;
	m_blRePreviewRefresh = FALSE;
    RefreshSkin();
    LoadDefaultData();

    char buffer[ MAX_INI_LEN_BUFFER ];
    GetPrivateProfileString( "FreeEdge", "Support", "NO", buffer, MAX_INI_LEN_BUFFER, APP_SKIN_INI_FILE_NAME );

    if( strcmpi( buffer, "YES" ) == 0 )
    {
        g_blTitleShow = FALSE;
    }
    else
    {
        GetPrivateProfileString( "MenuBar", "Hide", "TRUE", buffer, MAX_INI_LEN_BUFFER, APP_SKIN_INI_FILE_NAME );

        if( stricmp( buffer, "TRUE" ) == 0 )
        {
            g_blTitleShow = FALSE;
        }
        else
        {
            g_blTitleShow = TRUE;
        }
    }

    OnShowMainInterface();

    return TRUE;
}

void Csmv_transcodeDlg::LoadDefaultData()
{
    m_listItems.SetExtendedStyle( LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
    m_listItems.InsertColumn(0,_T("Files") );
    m_listItems.InsertColumn(1,_T("Start") );
    m_listItems.InsertColumn(2,_T("End") );
    m_listItems.InsertColumn(3,_T("Size(KB)") );
    m_listItems.InsertColumn(4,_T("Status") );

    const int colwid[ 5 ] = { 250, 50, 50, 80, 250 };
    for( int i = 0; i < 5; i++ )
    {
        m_listItems.SetColumnWidth( i, colwid[ i ] );
    }

    m_cmbSize.InsertString( 0, _T("220X176") );
    m_cmbSize.InsertString( 0, _T("176X144") );
    m_cmbSize.InsertString( 0, _T("160X128") );
    m_cmbSize.InsertString( 0, _T("128X160") );
    m_cmbSize.InsertString( 0, _T("128X128") );
    m_cmbSize.InsertString( 0, _T("128X96") );
    m_cmbSize.InsertString( 0, _T("96X64") );
    m_cmbSize.SetCurSel( 0 );

    m_cmbQuality.InsertString( 0, _T("Customize...") );
    m_cmbQuality.InsertString( 0, _T("83MB/Hour") );
    m_cmbQuality.InsertString( 0, _T("128MB/Hour") );
    m_cmbQuality.InsertString( 0, _T("200MB/Hour") );
    m_cmbQuality.SetCurSel( 1 );

    m_cmbSampleRate.InsertString( 0, _T("22 KB/Sec") );
    m_cmbSampleRate.InsertString( 0, _T("11 KB/Sec") );
    m_cmbSampleRate.InsertString( 0, _T(" 8 KB/Sec") );
    m_cmbSampleRate.SetCurSel( 1 );

    const TCHAR *G_Rate[ 58 ] = {
            _T("60 KB/Sec"),   _T("59 KB/Sec"),   _T("58 KB/Sec"),   _T("57 KB/Sec"),
            _T("56 KB/Sec"),   _T("55 KB/Sec"),   _T("54 KB/Sec"),   _T("53 KB/Sec"),
            _T("52 KB/Sec"),   _T("51 KB/Sec"),   _T("50 KB/Sec"),   _T("49 KB/Sec"),
            _T("48 KB/Sec"),   _T("47 KB/Sec"),   _T("46 KB/Sec"),   _T("45 KB/Sec"),
            _T("44 KB/Sec"),   _T("43 KB/Sec"),   _T("42 KB/Sec"),   _T("41 KB/Sec"),
            _T("40 KB/Sec"),   _T("39 KB/Sec"),   _T("38 KB/Sec"),   _T("37 KB/Sec"),
            _T("36 KB/Sec"),   _T("35 KB/Sec"),   _T("34 KB/Sec"),   _T("33 KB/Sec"),
            _T("32 KB/Sec"),   _T("31 KB/Sec"),   _T("30 KB/Sec"),   _T("29 KB/Sec"),
            _T("28 KB/Sec"),   _T("27 KB/Sec"),   _T("26 KB/Sec"),   _T("25 KB/Sec"),
            _T("24 KB/Sec"),   _T("23 KB/Sec"),   _T("22 KB/Sec"),   _T("21 KB/Sec"),
            _T("20 KB/Sec"),   _T("19 KB/Sec"),   _T("18 KB/Sec"),   _T("17 KB/Sec"),
            _T("16 KB/Sec"),   _T("15 KB/Sec"),   _T("14 KB/Sec"),   _T("13 KB/Sec"),
            _T("12 KB/Sec"),   _T("11 KB/Sec"),   _T("10 KB/Sec"),   _T(" 9 KB/Sec"),
            _T(" 8 KB/Sec"),   _T(" 7 KB/Sec"),   _T(" 6 KB/Sec"),   _T(" 5 KB/Sec"),
            _T(" 4 KB/Sec"),   _T(" 3 KB/Sec")    };

    for( int i = 0; i < 58; i++ )
    {
        m_cmbGraphicRate.InsertString( 0, G_Rate[ i ] );
    }

    m_cmbGraphicRate.SetCurSel( 22 );

    m_cmbFrameRate.InsertString( 0, _T("25 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T("24 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T("23 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T("22 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T("21 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T("20 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T("19 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T("18 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T("17 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T("16 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T("15 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T("14 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T("13 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T("12 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T("11 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T("10 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T(" 9 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T(" 8 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T(" 7 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T(" 6 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T(" 5 Frame/Sec") );
    m_cmbFrameRate.InsertString( 0, _T(" 4 Frame/Sec") );
    m_cmbFrameRate.SetCurSel( 6 );

    m_cmbRotate.InsertString( 0, _T("Rotate Right 90") );
    m_cmbRotate.InsertString( 0, _T("Rotate 180") );
    m_cmbRotate.InsertString( 0, _T("Rotate Left 90") );
    m_cmbRotate.InsertString( 0, _T("N/A") );
    m_cmbRotate.SetCurSel( 0 );

    m_btnFullImage.SetCheck( TRUE );
    m_btnFullScreen.SetCheck( FALSE );

    m_btnAudioMono.SetCheck( TRUE );
    m_btnAudioStereo.SetCheck( FALSE );
}

void Csmv_transcodeDlg::LoadSkin( void )
{
    if( !img_Outline.IsNull() )
    {
        img_Outline.Destroy();
    }

    if( !img_background.IsNull() )
    {
        img_background.Destroy();
    }

    if( !img_TitleBarMid.IsNull() )
    {
        img_TitleBarMid.Destroy();
    }

    if( !img_TitleBarLeftTop.IsNull() )
    {
        img_TitleBarLeftTop.Destroy();
    }

    if( !img_TitleBarRightTop.IsNull() )
    {
        img_TitleBarRightTop.Destroy();
    }

    if( !img_LeftBar.IsNull() )
    {
        img_LeftBar.Destroy();
    }

    if( !img_RightBar.IsNull() )
    {
        img_RightBar.Destroy();
    }

    if( !img_BottomBar.IsNull() )
    {
        img_BottomBar.Destroy();
    }

    if( !img_LeftBottom.IsNull() )
    {
        img_LeftBottom.Destroy();
    }

    if( !img_RightBottom.IsNull() )
    {
        img_RightBottom.Destroy();
    }

    // Transparent color
    {
        CUserImage img;
        img.Load( "transparent.bmp" );
        if( !img.IsNull() )
        {
            g_TransparentColor = img.GetPixel( 1, 1 );
        }
        else
        {
            g_TransparentColor = RGB( 0, 0, 0 );
        }
    }

    m_listItems.InitHeaderImage( "LstHeader.bmp", "LstHeaderSep.bmp", "LstTextColor.bmp" );
    m_listItems.InitItemImage( "lstbk.bmp", "lstfr.bmp" );

    m_btnFullImage.InitImage( "FullImage_N.bmp", "FullImage_NF.bmp", "FullImage_C.bmp", "FullImage_CF.bmp", "", "" );
    m_btnFullScreen.InitImage( "FullScreen_N.bmp", "FullScreen_NF.bmp", "FullScreen_C.bmp", "FullScreen_CF.bmp", "", "" );

    m_btnFullImage.SetWindowTip( "Full image" );
    m_btnFullScreen.SetWindowTip( "Full screen" );

    img_background.Load("mainback.bmp");
    pimg_background = &img_background;
    img_Outline.Load( "OutLine.bmp" );
    img_TitleBarMid.Load( "TitleBarMid.bmp" );
    img_TitleBarLeftTop.Load( "TitleLeftTop.bmp" );
    img_TitleBarRightTop.Load( "TitleRightTop.bmp" );
    img_LeftBar.Load( "LeftBar.bmp" );
    img_RightBar.Load( "RightBar.bmp" );
    img_BottomBar.Load( "BottomBar.bmp" );
    img_LeftBottom.Load( "BottomLeft.bmp" );
    img_RightBottom.Load( "BottomRight.bmp" );

    m_btnAddItem.InitImage("Add_N.bmp","Add_D.bmp","Add_F.bmp","Add_P.bmp");
    m_btnDeleteItem.InitImage("Delete_N.bmp","Delete_D.bmp","Delete_F.bmp","Delete_P.bmp");
    m_btnTransItem.InitImage("Convert_N.bmp","Convert_D.bmp","Convert_F.bmp","Convert_P.bmp");
    m_btnSetting.InitImage("setting_N.bmp","setting_D.bmp","setting_F.bmp","setting_P.bmp");
    m_btnAbout.InitImage("About_N.bmp","About_D.bmp","About_F.bmp","About_P.bmp");

    m_btnAddItem.SetWindowTip( "Add file(s)" );
    m_btnDeleteItem.SetWindowTip( "Delete file(s)" );
    m_btnTransItem.SetWindowTip( "Transcode" );
    m_btnSetting.SetWindowTip( "Set saved directory" );
    m_btnAbout.SetWindowTip( "About" );

#if SUPPORT_SMV_PLAY
    m_btnPrevious.InitImage("Previous_N.bmp","Previous_D.bmp","Previous_F.bmp","Previous_P.bmp");
    m_btnNext.InitImage("Next_N.bmp","Next_D.bmp","Next_F.bmp","Next_P.bmp");
    m_btnPlay.InitImage("Play_N.bmp","Play_D.bmp","Play_F.bmp","Play_P.bmp");
    m_btnPause.InitImage("Pause_N.bmp","Pause_D.bmp","Pause_F.bmp","Pause_P.bmp");
    m_btnStop.InitImage("Stop_N.bmp","Stop_D.bmp","Stop_F.bmp","Stop_P.bmp");

    m_btnPrevious.SetWindowTip( "Previous file" );
    m_btnNext.SetWindowTip( "Next file" );
    m_btnPlay.SetWindowTip( "Play" );
    m_btnPause.SetWindowTip( "Pause" );
    m_btnStop.SetWindowTip( "Stop" );
#else
    m_btnPrevious.InitImage("","","","");
    m_btnNext.InitImage("","","","");
    m_btnPlay.InitImage("","","","");
    m_btnPause.InitImage("","","","");
    m_btnStop.InitImage("","","","");
#endif

    m_btnSetAsStart.InitImage( "Start_N.bmp", "Start_D.bmp", "Start_F.bmp", "Start_P.bmp" );
    m_btnSetAsEnd.InitImage( "End_N.bmp", "End_D.bmp", "End_F.bmp", "End_P.bmp" );

    m_btnSetAsStart.SetWindowTip( "Set as start point" );
    m_btnSetAsEnd.SetWindowTip( "Set as end point" );

    m_cmbSize.InitImageOutline( "CmbOut_N.bmp", "CmbOut_D.bmp", "CmbOut_F.bmp" );
    m_cmbSize.InitImageItem( "ListBk_N.bmp", "ListBk_F.bmp", "ListBk_D.bmp", "ListText_N.bmp", "ListText_F.bmp", "ListText_D.bmp" );
    m_cmbSize.InitImageBtn( "CmbBtn_N.bmp", "CmbBtn_P.bmp", "CmbBtn_F.bmp", "CmbBtn_D.bmp" );

    m_cmbQuality.InitImageOutline( "CmbOut_N.bmp", "CmbOut_D.bmp", "CmbOut_F.bmp" );
    m_cmbQuality.InitImageItem( "ListBk_N.bmp", "ListBk_F.bmp", "ListBk_D.bmp", "ListText_N.bmp", "ListText_F.bmp", "ListText_D.bmp" );
    m_cmbQuality.InitImageBtn( "CmbBtn_N.bmp", "CmbBtn_P.bmp", "CmbBtn_F.bmp", "CmbBtn_D.bmp" );

    m_cmbSampleRate.InitImageOutline( "CmbOut_N.bmp", "CmbOut_D.bmp", "CmbOut_F.bmp" );
    m_cmbSampleRate.InitImageItem( "ListBk_N.bmp", "ListBk_F.bmp", "ListBk_D.bmp", "ListText_N.bmp", "ListText_F.bmp", "ListText_D.bmp" );
    m_cmbSampleRate.InitImageBtn( "CmbBtn_N.bmp", "CmbBtn_P.bmp", "CmbBtn_F.bmp", "CmbBtn_D.bmp" );

    m_btnAudioMono.InitImage( "Mono_N.bmp", "Mono_NF.bmp", "Mono_C.bmp", "Mono_CF.bmp", "Mono_ND.bmp", "Mono_CD.bmp" );
    m_btnAudioStereo.InitImage( "Stereo_N.bmp", "Stereo_NF.bmp", "Stereo_C.bmp", "Stereo_CF.bmp", "Stereo_ND.bmp", "Stereo_CD.bmp" );

    m_btnAudioMono.SetWindowTip( "Mono" );
    m_btnAudioStereo.SetWindowTip( "Stereo" );

    m_cmbGraphicRate.InitImageOutline( "CmbOut_N.bmp", "CmbOut_D.bmp", "CmbOut_F.bmp" );
    m_cmbGraphicRate.InitImageItem( "ListBk_N.bmp", "ListBk_F.bmp", "ListBk_D.bmp", "ListText_N.bmp", "ListText_F.bmp", "ListText_D.bmp" );
    m_cmbGraphicRate.InitImageBtn( "CmbBtn_N.bmp", "CmbBtn_P.bmp", "CmbBtn_F.bmp", "CmbBtn_D.bmp" );

    m_cmbFrameRate.InitImageOutline( "CmbOut_N.bmp", "CmbOut_D.bmp", "CmbOut_F.bmp" );
    m_cmbFrameRate.InitImageItem( "ListBk_N.bmp", "ListBk_F.bmp", "ListBk_D.bmp", "ListText_N.bmp", "ListText_F.bmp", "ListText_D.bmp" );
    m_cmbFrameRate.InitImageBtn( "CmbBtn_N.bmp", "CmbBtn_P.bmp", "CmbBtn_F.bmp", "CmbBtn_D.bmp" );

    m_cmbRotate.InitImageOutline( "CmbOut_N.bmp", "CmbOut_D.bmp", "CmbOut_F.bmp" );
    m_cmbRotate.InitImageItem( "ListBk_N.bmp", "ListBk_F.bmp", "ListBk_D.bmp", "ListText_N.bmp", "ListText_F.bmp", "ListText_D.bmp" );
    m_cmbRotate.InitImageBtn( "CmbBtn_N.bmp", "CmbBtn_P.bmp", "CmbBtn_F.bmp", "CmbBtn_D.bmp" );

    m_btnMenuMin.InitImage( "min_N.bmp", "min_N.bmp", "", "min_P.bmp" );
    m_btnMenuMax.InitImage( "max_N.bmp", "max_N.bmp", "", "max_P.bmp" );
    m_btnMenuClose.InitImage( "close_N.bmp", "close_N.bmp", "", "close_P.bmp" );
    m_btnMenuBar.InitImage( "showmenu_N.bmp", "showmenu_N.bmp", "", "showmenu_P.bmp" );

    m_btnMenuMin.SetWindowTip( "Minimize window" );
    m_btnMenuMax.SetWindowTip( "Maximize window" );
    m_btnMenuClose.SetWindowTip( "Close" );
    m_btnMenuBar.SetWindowTip( "Hide menu bar" );

    m_Slider.InitImage( "SliderAxis.bmp", "SilderAxisSel.bmp", "SliderBtn.bmp" );
    m_Slider.SetRange( 0, 100 );

    m_StaticSize.InitImage( "StaticSize.bmp" );
    m_StaticQuality.InitImage( "StaticQuality.bmp" );
    m_StaticSampleRate.InitImage( "StaticSampleRate.bmp" );
    m_StaticAudioChannel.InitImage( "StaticAudioChannel.bmp" );
    m_StaticGraphicRate.InitImage( "StaticGraphicRate.bmp" );
    m_StaticFrameRate.InitImage( "StaticFrameRate.bmp" );
    m_StaticRotate.InitImage( "StaticRotate.bmp" );
    m_StaticGroupBox.InitImage( "StaticGroup.bmp" );

    m_StaticProgress.InitImage( "ProgressBar.bmp", "ProgressSel.bmp" );
    m_StaticPlayGround.InitImage( "playback.bmp" );
}

void Csmv_transcodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        if( nID == SC_CLOSE )
        {
            CloseTranscoder();
        }
	    WriteConfig();

        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Csmv_transcodeDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        RedrawScreen();
    }
}

void Csmv_transcodeDlg::RedrawScreen()
{
    CPaintDC dc(this);
    if( img_background.IsNull() )
    {
        CDialog::OnEraseBkgnd( &dc );
    }

    RECT rect;
    GetClientRect( &rect );

    if( !img_background.IsNull() )
    {
        for( int i = 0; i <= rect.right / img_background.GetWidth(); i++ )
        {
            for( int k = 0; k <= rect.bottom / img_background.GetHeight(); k++ )
            {
//              img_background.BitBlt( dc, rect.left + i*img_background.GetWidth(),
//                      rect.top + k*img_background.GetHeight(), SRCCOPY );
                dc.TransparentBlt( rect.left + i*img_background.GetWidth(),
                    rect.top + k*img_background.GetHeight(),
                    img_background.GetWidth(),
                    img_background.GetHeight(),
                    CDC::FromHandle(img_background.GetDC()),0,0,img_background.GetWidth(),
                    img_background.GetHeight(), g_TransparentColor );

                img_background.ReleaseDC();
            }
        }
    }

    if( !img_TitleBarLeftTop.IsNull()
        && !img_TitleBarRightTop.IsNull()
        && !img_TitleBarMid.IsNull()
        && !img_LeftBar.IsNull()
        && !img_RightBar.IsNull()
        && !img_LeftBottom.IsNull()
        && !img_RightBottom.IsNull()
        && !img_BottomBar.IsNull() )
    {
        dc.TransparentBlt( 0, 0,
            img_TitleBarLeftTop.GetWidth(),
            img_TitleBarLeftTop.GetHeight(),
            CDC::FromHandle(img_TitleBarLeftTop.GetDC()),0,0,img_TitleBarLeftTop.GetWidth(),
            img_TitleBarLeftTop.GetHeight(), g_TransparentColor );

        img_TitleBarLeftTop.ReleaseDC();

        dc.TransparentBlt( rect.right - img_TitleBarRightTop.GetWidth() + 1,
            0,
            img_TitleBarRightTop.GetWidth(),
            img_TitleBarRightTop.GetHeight(),
            CDC::FromHandle(img_TitleBarRightTop.GetDC()),0,0,img_TitleBarRightTop.GetWidth(),
            img_TitleBarRightTop.GetHeight(), g_TransparentColor );

        img_TitleBarRightTop.ReleaseDC();

        img_TitleBarMid.StretchBlt( dc, img_TitleBarLeftTop.GetWidth(), 0,
            rect.right - img_TitleBarLeftTop.GetWidth() - img_TitleBarRightTop.GetWidth() + 1,
            img_TitleBarMid.GetHeight(), SRCCOPY );

        dc.TransparentBlt( 0,
            img_TitleBarLeftTop.GetHeight(),
            img_LeftBar.GetWidth(),
            rect.bottom - img_LeftBottom.GetHeight() - img_TitleBarLeftTop.GetHeight(),
            CDC::FromHandle(img_LeftBar.GetDC()),0,0,img_LeftBar.GetWidth(),
            img_LeftBar.GetHeight(), g_TransparentColor );

        img_LeftBar.ReleaseDC();

        dc.TransparentBlt( rect.right - img_RightBar.GetWidth() + 1,
            img_TitleBarRightTop.GetHeight(),
            img_RightBar.GetWidth(),
            rect.bottom - img_RightBottom.GetHeight() - img_TitleBarRightTop.GetHeight(),
            CDC::FromHandle(img_RightBar.GetDC()),0,0,img_RightBar.GetWidth(),
            img_RightBar.GetHeight(), g_TransparentColor );

        img_RightBar.ReleaseDC();

        dc.TransparentBlt( 0,
            rect.bottom - img_LeftBottom.GetHeight(),
            img_LeftBottom.GetWidth(),
            img_LeftBottom.GetHeight(),
            CDC::FromHandle(img_LeftBottom.GetDC()),0,0,img_LeftBottom.GetWidth(),
            img_LeftBottom.GetHeight(), g_TransparentColor );

        img_LeftBottom.ReleaseDC();

        dc.TransparentBlt( rect.right - img_RightBottom.GetWidth() + 1,
            rect.bottom - img_RightBottom.GetHeight(),
            img_RightBottom.GetWidth(),
            img_RightBottom.GetHeight(),
            CDC::FromHandle(img_RightBottom.GetDC()),0,0,img_RightBottom.GetWidth(),
            img_RightBottom.GetHeight(), g_TransparentColor );

        img_RightBottom.ReleaseDC();

        dc.TransparentBlt( img_LeftBottom.GetWidth(),
            rect.bottom - img_BottomBar.GetHeight(),
            rect.right - img_LeftBottom.GetWidth() - img_RightBottom.GetWidth() + 1,
            img_BottomBar.GetHeight(),
            CDC::FromHandle(img_BottomBar.GetDC()),0,0,img_BottomBar.GetWidth(),
            img_BottomBar.GetHeight(), g_TransparentColor );

        img_BottomBar.ReleaseDC();
    }

    m_StaticGroupBox.RedrawWindow();
    m_StaticSize.RedrawWindow();
    m_StaticQuality.RedrawWindow();
    m_StaticSampleRate.RedrawWindow();
    m_StaticAudioChannel.RedrawWindow();
    m_StaticGraphicRate.RedrawWindow();
    m_StaticFrameRate.RedrawWindow();
    m_StaticRotate.RedrawWindow();

    m_cmbSize.RedrawWindow();
    m_cmbQuality.RedrawWindow();
    m_cmbSampleRate.RedrawWindow();
    m_btnAudioMono.RedrawWindow();
    m_btnAudioStereo.RedrawWindow();

    m_cmbGraphicRate.RedrawWindow();
    m_cmbFrameRate.RedrawWindow();
    m_cmbRotate.RedrawWindow();

    m_btnFullImage.RedrawWindow();
    m_btnFullScreen.RedrawWindow();

    m_StaticPlayGround.Invalidate();
    m_StaticPlayGround.RedrawWindow();

#if SUPPORT_SMV_PLAY
    m_btnPrevious.RedrawWindow();
    m_btnNext.RedrawWindow();
    m_btnPlay.RedrawWindow();
    m_btnPause.RedrawWindow();
    m_btnStop.RedrawWindow();
#endif

    m_Slider.RedrawWindow();

    m_btnAddItem.RedrawWindow();
    m_btnDeleteItem.RedrawWindow();
    m_btnTransItem.RedrawWindow();
    m_btnAbout.RedrawWindow();
    m_btnMenuMin.RedrawWindow();
    m_btnMenuMax.RedrawWindow();
    m_btnMenuClose.RedrawWindow();
    m_btnMenuBar.RedrawWindow();

#if SUPPORT_SMV_PLAY
    m_btnPrevious.RedrawWindow();
    m_btnNext.RedrawWindow();
    m_btnPlay.RedrawWindow();
    m_btnPause.RedrawWindow();
    m_btnStop.RedrawWindow();
#endif

    m_btnSetAsStart.RedrawWindow();
    m_btnSetAsEnd.RedrawWindow();

    m_listItems.RedrawWindow();

    PutPreviewImage();
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Csmv_transcodeDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

BOOL Csmv_transcodeDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    return TRUE;
    //return CDialog::OnEraseBkgnd(pDC);
}

BOOL Csmv_transcodeDlg::PreTranslateMessage(MSG* pMsg)
{
    if ( (pMsg->message == WM_KEYDOWN))
    {
        if( pMsg->wParam == VK_RETURN
            || pMsg->wParam == VK_ESCAPE )
        {
            return TRUE;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void Csmv_transcodeDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    if( m_btnAddItem.m_hWnd == NULL
        || m_btnDeleteItem.m_hWnd == NULL
        || m_btnTransItem.m_hWnd == NULL
        || m_btnAbout.m_hWnd == NULL
        || m_btnMenuMin.m_hWnd == NULL
        || m_btnMenuMax.m_hWnd == NULL
        || m_btnMenuClose.m_hWnd == NULL
        || m_btnMenuBar.m_hWnd == NULL
#if SUPPORT_SMV_PLAY
        || m_btnPrevious.m_hWnd == NULL
        || m_btnNext.m_hWnd == NULL
        || m_btnPlay.m_hWnd == NULL
        || m_btnPause.m_hWnd == NULL
        || m_btnStop.m_hWnd == NULL
#endif
        || m_btnSetAsStart.m_hWnd == NULL
        || m_btnSetAsEnd.m_hWnd == NULL
        || m_btnFullImage.m_hWnd == NULL
        || m_btnFullScreen.m_hWnd == NULL
        || m_cmbSize.m_hWnd == NULL
        || m_cmbQuality.m_hWnd == NULL
        || m_cmbSampleRate.m_hWnd == NULL
        || m_btnAudioMono.m_hWnd == NULL
        || m_btnAudioStereo.m_hWnd == NULL
        || m_cmbGraphicRate.m_hWnd == NULL
        || m_cmbFrameRate.m_hWnd == NULL
        || m_cmbRotate.m_hWnd == NULL
        || m_listItems.m_hWnd == NULL
        || m_Slider.m_hWnd == NULL )
    {
        return;
    }

    Invalidate();

    if( !img_LeftBar.IsNull()
        && !img_RightBar.IsNull() )
    {
        RECT rect;
        GetClientRect( &rect );

        RECT tmp;
        m_listItems.GetWindowRect( &tmp );

        m_listItems.SetWindowPos( NULL, img_LeftBar.GetWidth() + LIST_BOX_DISTANCE_BODER, 56,
            rect.right - img_LeftBar.GetWidth() - img_LeftBar.GetWidth() + 1 - 2*LIST_BOX_DISTANCE_BODER,
            tmp.bottom - tmp.top,
            SWP_NOZORDER );

        m_listItems.RedrawWindow();
    }

    m_StaticGroupBox.RedrawWindow();
    m_StaticSize.RedrawWindow();
    m_StaticQuality.RedrawWindow();
    m_StaticSampleRate.RedrawWindow();
    m_StaticAudioChannel.RedrawWindow();
    m_StaticGraphicRate.RedrawWindow();
    m_StaticFrameRate.RedrawWindow();
    m_StaticRotate.RedrawWindow();

    m_cmbQuality.RedrawWindow();
    m_cmbSampleRate.RedrawWindow();
    m_cmbGraphicRate.RedrawWindow();
    m_cmbFrameRate.RedrawWindow();
    m_cmbRotate.RedrawWindow();
    m_cmbSize.RedrawWindow();

    m_listItems.RedrawWindow();

    m_btnAddItem.RedrawWindow();
    m_btnAddItem.RedrawWindow();
    m_btnDeleteItem.RedrawWindow();
    m_btnTransItem.RedrawWindow();
    m_btnAbout.RedrawWindow();
#if SUPPORT_SMV_PLAY
    m_btnPrevious.RedrawWindow();
    m_btnNext.RedrawWindow();
    m_btnPlay.RedrawWindow();
    m_btnPause.RedrawWindow();
    m_btnStop.RedrawWindow();
#endif
    m_btnFullImage.RedrawWindow();
    m_btnFullScreen.RedrawWindow();
    m_Slider.RedrawWindow();
}

void Csmv_transcodeDlg::OnBnClickedPlayPrevious()
{
    // TODO: Add your control notification handler code here
}

void Csmv_transcodeDlg::OnBnClickedPlayNext()
{
    // TODO: Add your control notification handler code here
}

void Csmv_transcodeDlg::OnBnClickedPlayPlay()
{
    // TODO: Add your control notification handler code here
    LoadSkin();
    Invalidate();
}

void Csmv_transcodeDlg::OnBnClickedPlayPause()
{
    // TODO: Add your control notification handler code here
}

void Csmv_transcodeDlg::OnBnClickedPlayStop()
{
    // TODO: Add your control notification handler code here
}

void Csmv_transcodeDlg::OnCbnSelchangeQuality()
{
    // TODO: Add your control notification handler code here
    BOOL blEnable = FALSE;

    if( m_cmbQuality.GetCurSel() == 3 )
    {
        blEnable = TRUE;
    }

    m_cmbSampleRate.EnableWindow( blEnable );
    m_btnAudioStereo.EnableWindow( blEnable );
    m_btnAudioMono.EnableWindow( blEnable );
    m_cmbGraphicRate.EnableWindow( blEnable );
    m_cmbFrameRate.EnableWindow( blEnable );
    m_cmbRotate.EnableWindow( blEnable );

    if( -1 != m_listItems.GetFocusItem() )
    {
        int pos = m_Slider.GetPos();
        int start, end;
        m_Slider.GetRange( start, end );

        SIZE sz;
		int nRotation;
		if( blEnable == FALSE )
			nRotation = 0;
		else
		{
			switch( m_cmbRotate.GetCurSel() )
			{
				case 1:
					nRotation = 1;
					break;
				case 2:
					nRotation = 2;
					break;
				case 3:
					nRotation = 3;
					break;
				default:
					nRotation = 0;
					break;
			}
		}
        sz.cx = m_recPlayerScreen.right - m_recPlayerScreen.left;
        sz.cy = m_recPlayerScreen.bottom - m_recPlayerScreen.top;

		if( m_hPreviewBitmap != NULL )
			DeleteObject( m_hPreviewBitmap );
        m_hPreviewBitmap = Preview( pos*VIDEO_BASE_TIME, GetWindowDC()->m_hDC, sz, nRotation, 0 );
        PutPreviewImage();
    }

}

void Csmv_transcodeDlg::OnCbnSelchangeSampleRate()
{
    // TODO: Add your control notification handler code here
}

void Csmv_transcodeDlg::OnCbnSelchangeAudioChannel()
{
    // TODO: Add your control notification handler code here
}

void Csmv_transcodeDlg::OnCbnSelchangeGraphicRate()
{
    // TODO: Add your control notification handler code here
}

void Csmv_transcodeDlg::OnCbnSelchangeFrameRate()
{
    // TODO: Add your control notification handler code here
}

void Csmv_transcodeDlg::OnCbnSelchangeRotate()
{
    // TODO: Add your control notification handler code here
    if( -1 != m_listItems.GetFocusItem() )
    {
        int pos = m_Slider.GetPos();
        int start, end;
        m_Slider.GetRange( start, end );

        SIZE sz;
		int nRotation;
		switch( m_cmbRotate.GetCurSel() )
		{
			case 1:
				nRotation = 1;
				break;
			case 2:
				nRotation = 2;
				break;
			case 3:
				nRotation = 3;
				break;
			default:
				nRotation = 0;
				break;
		}
        sz.cx = m_recPlayerScreen.right - m_recPlayerScreen.left;
        sz.cy = m_recPlayerScreen.bottom - m_recPlayerScreen.top;

		if( m_hPreviewBitmap != NULL )
			DeleteObject( m_hPreviewBitmap );
        m_hPreviewBitmap = Preview( pos*VIDEO_BASE_TIME, GetWindowDC()->m_hDC, sz, nRotation, 0 );
        PutPreviewImage();
    }
}

void Csmv_transcodeDlg::OnBnClickedFullImage()
{
    // TODO: Add your control notification handler code here
    m_btnFullImage.SetCheck( TRUE );
    m_btnFullScreen.SetCheck( FALSE );
}

void Csmv_transcodeDlg::OnBnClickedFullScreen()
{
    // TODO: Add your control notification handler code here
    m_btnFullImage.SetCheck( FALSE );
    m_btnFullScreen.SetCheck( TRUE );
}

void Csmv_transcodeDlg::OnLvnKeydownListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    switch( pLVKeyDow->wVKey )
    {
        case VK_DELETE:
            OnListmenuDelete();
            break;

        case VK_INSERT:
            OnBnClickedAddItem();
            break;
    }

    *pResult = 0;
}

LRESULT Csmv_transcodeDlg::OnListRButton( WPARAM WParam, LPARAM LParam )
{
    CMenu menu, *psubmenu;
    menu.LoadMenu( IDR_LISTMENU );
    psubmenu = menu.GetSubMenu( 0 );
    psubmenu->TrackPopupMenu( TPM_LEFTALIGN |TPM_RIGHTBUTTON, (int)WParam, (int)LParam, this );
    return EXIT_SUCCESS;
}

LRESULT Csmv_transcodeDlg::OnRefreshSkin( WPARAM WParam, LPARAM LParam )
{
    char buffer[ MAX_INI_LEN_BUFFER ];
    GetPrivateProfileString( "FreeEdge", "Support", "NO", buffer, MAX_INI_LEN_BUFFER, APP_SKIN_INI_FILE_NAME );

    if( strcmpi( buffer, "YES" ) == 0 )
    {
        g_blTitleShow = FALSE;
    }
    else
    {
        GetPrivateProfileString( "MenuBar", "Hide", "TRUE", buffer, MAX_INI_LEN_BUFFER, APP_SKIN_INI_FILE_NAME );

        if( stricmp( buffer, "TRUE" ) == 0 )
        {
            g_blTitleShow = FALSE;
        }
        else
        {
            g_blTitleShow = TRUE;
        }
    }

    OnShowMainInterface();

    return EXIT_SUCCESS;
}

void Csmv_transcodeDlg::OnListmenuInsert()
{
    // TODO: Add your command handler code here
    OnBnClickedAddItem();
}

void Csmv_transcodeDlg::OnListmenuDelete()
{
    // TODO: Add your command handler code here
    int count = m_listItems.GetItemCount();
    for( int i = count; i >= 0 ; i-- )
    {
        if( m_listItems.GetCheck(i) )
        {
            m_listItems.DeleteItem( i );
        }
    }

    if( m_hPreviewBitmap != NULL )
		DeleteObject( m_hPreviewBitmap );
    m_hPreviewBitmap = NULL;
    Invalidate();

    ::PostMessage( m_listItems.m_hWnd, WM_KEYDOWN, VK_RIGHT, 0 );
    ::PostMessage( m_listItems.m_hWnd, WM_KEYDOWN, VK_LEFT, 0 );

    if( m_blConverting == TRUE )
    {
        if( -1 == GetCurrentConvertItem())
        {
            StopConvert();
            m_blNeedNext = TRUE;
        }
        else
        {
            m_blNeedNext = FALSE;
        }
    }
}

void Csmv_transcodeDlg::OnListmenuDeleteall()
{
    // TODO: Add your command handler code here
    if( m_blConverting == TRUE )
    {
        StopConvert();
    }

    int count = m_listItems.GetItemCount();
    for( int i = count; i >= 0 ; i-- )
    {
        m_listItems.DeleteItem( i );
    }

	if( m_hPreviewBitmap != NULL )
		DeleteObject( m_hPreviewBitmap );
    m_hPreviewBitmap = NULL;
    Invalidate();

    ::PostMessage( m_listItems.m_hWnd, WM_KEYDOWN, VK_RIGHT, 0 );
    ::PostMessage( m_listItems.m_hWnd, WM_KEYDOWN, VK_LEFT, 0 );
}

void Csmv_transcodeDlg::OnListmenuSelect()
{
    // TODO: Add your command handler code here
    int count = m_listItems.GetItemCount();
    for( int i = count; i >= 0 ; i-- )
    {
        if( m_listItems.GetItemState( i, LVIS_SELECTED ))
        {
            m_listItems.SetCheck( i, TRUE );
        }
    }
}

void Csmv_transcodeDlg::OnListmenuSelectall()
{
    // TODO: Add your command handler code here
    int count = m_listItems.GetItemCount();
    for( int i = count; i >= 0 ; i-- )
    {
        m_listItems.SetCheck( i, TRUE );
    }
}

void Csmv_transcodeDlg::OnListmenuDeselect()
{
    // TODO: Add your command handler code here
    int count = m_listItems.GetItemCount();
    for( int i = count; i >= 0 ; i-- )
    {
        if( m_listItems.GetItemState( i, LVIS_SELECTED ))
        {
            m_listItems.SetCheck( i, FALSE );
        }
    }
}

void Csmv_transcodeDlg::OnListmenuDeselectall()
{
    // TODO: Add your command handler code here
    int count = m_listItems.GetItemCount();
    for( int i = count; i >= 0 ; i-- )
    {
        m_listItems.SetCheck( i, FALSE );
    }
}

void Csmv_transcodeDlg::OnFileAdd()
{
    // TODO: Add your command handler code here
    OnListmenuInsert();
}

void Csmv_transcodeDlg::OnEditSelectall()
{
    // TODO: Add your command handler code here
    OnListmenuSelectall();
}

void Csmv_transcodeDlg::OnEditDeselectall()
{
    // TODO: Add your command handler code here
    OnListmenuDeselectall();
}

void Csmv_transcodeDlg::OnEditDeleteall()
{
    // TODO: Add your command handler code here
    OnListmenuDeleteall();
}

void Csmv_transcodeDlg::OnFileSavedirectory()
{
    // TODO: Add your command handler code here
    OnBnClickedSetting();
}

void Csmv_transcodeDlg::OnFileConvert()
{
    // TODO: Add your command handler code here
    OnBnClickedTransItem();
}

void Csmv_transcodeDlg::OnFileStop()
{
    // TODO: Add your command handler code here
    OnBnClickedTransItem();
}

void Csmv_transcodeDlg::SkinChange( UINT nCheckID )
{
    CMenu *MainMenu = GetMenu();
    CMenu *pMenu;
    pMenu = MainMenu->GetSubMenu(2);
    if( pMenu == NULL )
    {
        return;
    }

    pMenu->CheckMenuItem(ID_SKIN_SKIN1, MF_BYCOMMAND|MF_UNCHECKED);
    pMenu->CheckMenuItem(ID_SKIN_SKIN2, MF_BYCOMMAND|MF_UNCHECKED);
    pMenu->CheckMenuItem(ID_SKIN_SKIN3, MF_BYCOMMAND|MF_UNCHECKED);

    pMenu->CheckMenuItem(nCheckID, MF_BYCOMMAND|MF_CHECKED);

    WritePrivateProfileString( "Skin Check", "Skin1", "Unselected", APP_CONFIG_INI_FILE_NAME );
    WritePrivateProfileString( "Skin Check", "Skin2", "Unselected", APP_CONFIG_INI_FILE_NAME );
    WritePrivateProfileString( "Skin Check", "Skin3", "Unselected", APP_CONFIG_INI_FILE_NAME );

    switch( nCheckID )
    {
        case ID_SKIN_SKIN1:
            WritePrivateProfileString( "Skin Check", "Skin1", "Selected", APP_CONFIG_INI_FILE_NAME );
            break;

        case ID_SKIN_SKIN2:
            WritePrivateProfileString( "Skin Check", "Skin2", "Selected", APP_CONFIG_INI_FILE_NAME );
            break;

        case ID_SKIN_SKIN3:
            WritePrivateProfileString( "Skin Check", "Skin3", "Selected", APP_CONFIG_INI_FILE_NAME );
            break;
    }
}

void Csmv_transcodeDlg::OnSkinSkin1()
{
    // TODO: Add your command handler code here
    SkinChange( ID_SKIN_SKIN1 );
    RefreshSkin();
    ::PostMessage( this->m_hWnd, WM_USER_REFRESH_SKIN, 0, 0 );
}

void Csmv_transcodeDlg::OnSkinSkin2()
{
    // TODO: Add your command handler code here
    SkinChange( ID_SKIN_SKIN2 );
    RefreshSkin();
    ::PostMessage( this->m_hWnd, WM_USER_REFRESH_SKIN, 0, 0 );
}

void Csmv_transcodeDlg::OnSkinSkin3()
{
    // TODO: Add your command handler code here
    SkinChange( ID_SKIN_SKIN3 );
    RefreshSkin();
    ::PostMessage( this->m_hWnd, WM_USER_REFRESH_SKIN, 0, 0 );
}

void Csmv_transcodeDlg::OnBnClickedAddItem()
{
    // TODO: Add your control notification handler code here
    CFileDialog openDlg( TRUE, NULL, NULL, OFN_ALLOWMULTISELECT,
		_T("Media File|*.avi;*.mpg;*.wmv;*.rm;*.ram;*.rmvb;*.mov;*.vob;*.mp4;*.m4v|AVI File|*.avi|MPG File|*.mpg|WMV File|*.wmv|Real Player file|*.rm;*.ram;*.rmvb|QuickTime file|*.mov|DVD file|*.vob|MP4 file|*.mp4;*.m4v|SMV File|*.smv|All|*.*|"),
        this, 0);

    DWORD MAXFILE = 2000;
    openDlg.m_ofn.nMaxFile = MAXFILE;
    char *filename_buf = new char[ MAXFILE ];
    openDlg.m_ofn.lpstrFile = filename_buf;
    openDlg.m_ofn.lpstrFile[0] = NULL;

    switch( openDlg.DoModal() )
    {
        case IDOK:
            POSITION pos;
            pos = openDlg.GetStartPosition();

            BOOL skip;
            for( ;; )
            {
                CString file;
                file = openDlg.GetNextPathName( pos );

                skip = FALSE;
                int count = m_listItems.GetItemCount();
                for( int i = 0; i < count ; i++ )
                {
                    char name[ MAX_PATH ];
                    m_listItems.GetItemText( i, LIST_SEG_NAME, name, MAX_PATH );

                    if( stricmp( name, file.GetBuffer() ) == 0 )
                    {
                        skip = TRUE;
                        break;
                    }
                }

                if( skip == FALSE )
                {
                    int item = m_listItems.InsertItem(LIST_SEG_NAME,file);
                    if( -1 != item )
                    {
                        m_listItems.SetCheck( item, TRUE );

                        if( m_blConverting != TRUE )
                        {
                            //LONGLONG len;
                            //LONG err =  NewTranscoder( this->m_hWnd, file.GetBuffer(), "", &len );
                            //if( err == 0 )
                            //{
                            //    SetStartEndTime( item, 0, UINT(len / VIDEO_BASE_TIME) );
                            //}
                            //else
                            //{
                            //    SetStartEndTime( item, 0, 0 );
                            //    SetItemErrorMessage( item, err );
                            //}
                        }
                    }
                }

                if( pos == NULL )
                {
                    break;
                }
            }
            break;

        case IDCANCEL:
            break;
    }

    delete []filename_buf;
    return;
}

void Csmv_transcodeDlg::OnBnClickedDeleteItem()
{
    // TODO: Add your control notification handler code here
    OnListmenuDelete();
}

void Csmv_transcodeDlg::GetConvertPara(TRANSCODER_PARAM &ChgPara, int item )
{
    switch( m_cmbQuality.GetCurSel() )
    {
        case 0:
            gScalingFilter = ILU_SCALE_BSPLINE;
            ChgPara.compressionlevel = 35;
            ChgPara.samplingfrquency = 22050;
            ChgPara.framerate = 14;
            ChgPara.nchannel = 2;
            break;

        case 1:
            gScalingFilter = ILU_LINEAR;
            ChgPara.compressionlevel = 25;
            ChgPara.samplingfrquency = 11025;
            ChgPara.framerate = 14;
            ChgPara.nchannel = 2;
            break;

        case 2:
            gScalingFilter = ILU_LINEAR;
            ChgPara.compressionlevel = 16;
            ChgPara.samplingfrquency = 8000;
            ChgPara.framerate = 14;
            ChgPara.nchannel = 2;
            break;

        case 3:
            ChgPara.compressionlevel = m_cmbGraphicRate.GetCurSel() + 3;
            if( ChgPara.compressionlevel > 25 )
            {
                gScalingFilter = ILU_SCALE_BSPLINE;
            }
            else
            {
                gScalingFilter = ILU_LINEAR;
            }

            switch( m_cmbSampleRate.GetCurSel() )
            {
                case 0:
                    ChgPara.samplingfrquency = 8000;
                    break;

                case 1:
                    ChgPara.samplingfrquency = 11025;
                    break;

                case 2:
                default:
                    ChgPara.samplingfrquency = 22050;
                    break;
            }

            {
                const int rate[] = { 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 
									 18, 19, 20, 21, 22, 23, 24, 25 };
                ChgPara.framerate = rate[ m_cmbFrameRate.GetCurSel() ];
            }

            if( m_btnAudioStereo.GetCheck() == TRUE )
            {
                ChgPara.nchannel = 2; // stereo
            }
            else
            {
                ChgPara.nchannel = 1; // mono
            }
            break;
    }

    switch( m_cmbSize.GetCurSel() )
    {
        case 0:
            ChgPara.width = 96;
            ChgPara.height = 64;
            break;

        case 1:
            ChgPara.width = 128;
            ChgPara.height = 96;
            break;

        case 2:
            ChgPara.width = 128;
            ChgPara.height = 128;
            break;

        case 3:
            ChgPara.width = 128;
            ChgPara.height = 159;
            break;

        case 4:
            ChgPara.width = 160;
            ChgPara.height = 128;
            break;

        case 5:
            ChgPara.width = 176;
            ChgPara.height = 144;
            break;

		case 6:
            ChgPara.width = 220;
            ChgPara.height = 176;
            break;
    }

    if( m_btnFullImage.GetCheck() == TRUE )
    {
        ChgPara.bFullImage = TRUE;
    }
    else
    {
        ChgPara.bFullImage = FALSE;
    }

    ChgPara.hWnd = this->m_hWnd;
    ChgPara.hDC = GetWindowDC()->m_hDC;

    ChgPara.nPreviewInternal = 1; // every 1%

    UINT Start, End;
    GetStartEndTime( item, Start, End );
    ChgPara.markA = (LONGLONG)Start*VIDEO_BASE_TIME;
    ChgPara.markB = (LONGLONG)End*VIDEO_BASE_TIME;

    switch( m_cmbRotate.GetCurSel() )
    {
        case 1:
            ChgPara.nRotation = 1;
            break;
        case 2:
            ChgPara.nRotation = 2;
            break;
        case 3:
            ChgPara.nRotation = 3;
            break;
        default:
            ChgPara.nRotation = 0;
            break;
    }

    ChgPara.nMirror = 0;
    ChgPara.pxImageProcess = ImageProcess;
	ChgPara.bGraphicOptimizer = GetPrivateProfileInt( "Graphic Setting", "optimizer", 0, APP_CONFIG_INI_FILE_NAME ) == 0 ? false : true;
}

void Csmv_transcodeDlg::OnBnClickedTransItem()
{
    // TODO: Add your control notification handler code here
    if( m_blConverting == TRUE )
    {
        StopConvert();
    }
    else
    {
        StartConvert();
    }
}

void Csmv_transcodeDlg::OnBnClickedSetting()
{
    // TODO: Add your control notification handler code here
    CBrowseDialog dlg;

    dlg.hwndOwner = this->m_hWnd;
    dlg.m_Title = "Select a folder to save destination files";
    dlg.m_SelDir = g_SavedDirectory;

    if( dlg.DoModal() == IDOK )
    {
        g_SavedDirectory = dlg.m_Path;
        g_SavedDirectory.TrimRight( '\\' );
        g_SavedDirectory += '\\';
    }
}

void Csmv_transcodeDlg::OnBnClickedAbout()
{
    // TODO: Add your control notification handler code here
    CAboutDlg dlg;
    dlg.DoModal();
}

void Csmv_transcodeDlg::OnBnClickedAsStart()
{
    // TODO: Add your control notification handler code here
    int min = 0, max = 0;
    int pos = m_Slider.GetPos();
    int item = m_listItems.GetFocusItem();

    if( -1 == item )
    {
        return;
    }

    m_Slider.GetSelection( min, max );

    if( pos < max || max == 0 )
    {
        m_Slider.SetSelection( pos, max );
    }

    m_Slider.GetSelection( min, max );
    SetStartEndTime( item, min, max );
}

void Csmv_transcodeDlg::OnBnClickedAsEnd()
{
    // TODO: Add your control notification handler code here
    int min, max;
    int pos = m_Slider.GetPos();
    int item = m_listItems.GetFocusItem();
    if( -1 == item )
    {
        return;
    }

    m_Slider.GetSelection( min, max );

    if( pos > min || min == 0 )
    {
        m_Slider.SetSelection( min, pos );
    }

    m_Slider.GetSelection( min, max );
    SetStartEndTime( item, min, max );
}

void Csmv_transcodeDlg::OnBnClickedHideMenu()
{
    if( g_blTitleShow == FALSE )
    {
        g_blTitleShow = TRUE;
    }
    else
    {
        g_blTitleShow = FALSE;
    }

    OnShowMainInterface();
}

void Csmv_transcodeDlg::OnShowMainInterface()
{
    // TODO: Add your control notification handler code here
    if( g_blTitleShow == TRUE )
    {
        m_btnMenuBar.SetWindowTip( "Hide menu bar" );

        SetWindowRgn( 0, TRUE );
    }
    else
    {
        m_btnMenuBar.SetWindowTip( "Show menu bar" );

        if( !img_Outline.IsNull() )
        {
            CRgn rgnWnd;

            int diff_x = GetSystemMetrics( SM_CXEDGE );
            int diff_y = GetSystemMetrics( SM_CYCAPTION )
                + GetSystemMetrics( SM_CYMENU )
                + GetSystemMetrics( SM_CYEDGE );

            rgnWnd.CreateRectRgn( diff_x + 1, diff_y + 1,
                img_Outline.GetWidth() + diff_x - 1,
                img_Outline.GetHeight() + diff_y - 1 );

            int bits = img_Outline.GetBPP();
            if( bits != 8 )
            {
                return;
            }

            int byteline = img_Outline.GetPitch();
            char *p = (char*)img_Outline.GetBits();

            for( int Height = 1; Height < img_Outline.GetHeight() - 1; Height++ )
            {
                for( int Width = 1; Width < img_Outline.GetWidth() - 1; Width++ )
                {
                    if( *(p + Width)  == g_TransparentColor )
                    {
                        CRgn rgnTemp;
                        rgnTemp.CreateRectRgn( diff_x + Width, diff_y + Height, diff_x + Width + 1, diff_y + Height + 1 );
                        rgnWnd.CombineRgn( &rgnWnd, &rgnTemp, RGN_XOR );
                    }
                }

                p += byteline;
            }

            SetWindowRgn( rgnWnd, TRUE );
        }
    }

}

void Csmv_transcodeDlg::OnLvnItemchangedListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here

    if( m_blConverting == TRUE )
    {
        *pResult = 0;
        return;
    }

    if( pNMLV->uNewState == ( LVIS_SELECTED | LVIS_FOCUSED )  )
    {
        int item = m_listItems.GetFocusItem();
        if( -1 != item )
        {
            if( m_hPreviewBitmap != NULL )
            {
				DeleteObject( m_hPreviewBitmap );
                m_hPreviewBitmap = NULL;
            }

            char name[ MAX_PATH ];
            m_listItems.GetItemText( item, LIST_SEG_NAME, name, MAX_PATH );
            if( name[0] != '\0' )
            {
                m_StaticProgress.SetRange( 0, 100 );
                m_StaticProgress.SetPos(0);

                LONGLONG len;

                LONG err = NewTranscoder( this->m_hWnd, name, "", &len );
				m_blRePreviewRefresh = TRUE;
                if( err == 0 )
                {
                    m_listItems.SetItemText( item, LIST_SEG_STATUS, "" );

                    UINT start = 0, end = (UINT)(len / VIDEO_BASE_TIME);
                    m_Slider.SetRange( start, end );
                    m_Slider.SetPageSize( ( end - start + 4 )/5 );

                    if( FALSE == GetStartEndTime( item, start, end ))
                    {
                        SetStartEndTime( item, 0, UINT(len / VIDEO_BASE_TIME) );
                    }

                    GetStartEndTime( item, start, end );

                    m_Slider.SetSelection( start, end );
                    m_Slider.SetPos( start );

                    SIZE sz;
					int nRotation;
					if( m_cmbQuality.GetCurSel() == 3 )
						nRotation = 0;
					else
					{
						switch( m_cmbRotate.GetCurSel() )
						{
							case 1:
								nRotation = 1;
								break;
							case 2:
								nRotation = 2;
								break;
							case 3:
								nRotation = 3;
								break;
							default:
								nRotation = 0;
								break;
						}
					}
                    sz.cx = m_recPlayerScreen.right - m_recPlayerScreen.left;
                    sz.cy = m_recPlayerScreen.bottom - m_recPlayerScreen.top;
                    m_hPreviewBitmap = Preview( start * VIDEO_BASE_TIME, GetWindowDC()->m_hDC, sz, nRotation, 0 );
                    PutPreviewImage();
                }
                else
                {
                    m_listItems.SetItemText( item, LIST_SEG_START, "" );
                    m_listItems.SetItemText( item, LIST_SEG_END, "" );
                    SetItemErrorMessage( item, err );
                }
            }
        }
    }

    *pResult = 0;
}

void Csmv_transcodeDlg::SetItemErrorMessage( int item, LONG err )
{
    CString str;

    switch( err )
    {
        case 0:
            break;

        case 0x80040265L:
			// SMV_MESSAGE_NO_CODEC
            str = _T("No codec is available for this video format. Please install the codec and the latest Microsoft DirectX(TM)");
            break;

        case 0x8004025CL:
			// SMV_MESSAGE_NO_AUDIO_CODEC
            str = _T("No audio codec is available for this file. Please install a correct codec");
            break;

        case 0x8004025DL:
			//SMV_MESSAGE_NO_VIDEO_CODEC
            str = _T("No video codec is available for this file. Please install a correct codec");
            break;

        default:
			// SMV_MESSAGE_HRESULT
            str.Format( _T("Unknown error = 0x%lx"), err );
            break;
    }

    m_listItems.SetItemText( item, LIST_SEG_STATUS, str );
}

void Csmv_transcodeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default

    if( -1 != m_listItems.GetFocusItem() )
    {
        int pos = m_Slider.GetPos();
        int start, end;
        m_Slider.GetRange( start, end );

        SIZE sz;
		int nRotation;
		if( m_cmbQuality.GetCurSel() == 3 )
			nRotation = 0;
		else
		{
			switch( m_cmbRotate.GetCurSel() )
			{
				case 1:
					nRotation = 1;
					break;
				case 2:
					nRotation = 2;
					break;
				case 3:
					nRotation = 3;
					break;
				default:
					nRotation = 0;
					break;
			}
		}
        sz.cx = m_recPlayerScreen.right - m_recPlayerScreen.left;
        sz.cy = m_recPlayerScreen.bottom - m_recPlayerScreen.top;

		if( m_hPreviewBitmap != NULL )
			DeleteObject( m_hPreviewBitmap );
        m_hPreviewBitmap = Preview( pos*VIDEO_BASE_TIME, GetWindowDC()->m_hDC, sz, nRotation, 0 );
        PutPreviewImage();
    }

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void Csmv_transcodeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CRect rect;
/*
    if( !img_RightBottom.IsNull() )
    {
        GetClientRect( rect );
        rect.left = rect.right - img_RightBottom.GetWidth();
        rect.top = rect.bottom - img_RightBottom.GetHeight();

        if( rect.PtInRect(point) )
        {
            ::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));

            if(nFlags & MK_LBUTTON )
            {
                ReleaseCapture();
                ::SendMessage( this->m_hWnd, WM_NCLBUTTONDOWN, HTBOTTOMRIGHT, 0);
            }
        }
    }
*/
#if _DEBUG
    char aaa[100];
    sprintf( aaa, "x = %d, y = %d", point.x, point.y );
    SetWindowText( aaa );
#endif

    if( !img_TitleBarMid.IsNull() )
    {
        GetClientRect( rect );
        rect.bottom = img_TitleBarMid.GetHeight();

        if( rect.PtInRect(point) )
        {
            ::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));

            if(nFlags & MK_LBUTTON )
            {
                ReleaseCapture();
                ::SendMessage( this->m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
                CDialog::OnMouseMove(nFlags, point);
                return;
            }
        }
    }

    m_btnIdleFocus.SetFocus();
    CDialog::OnMouseMove(nFlags, point);
}

void Csmv_transcodeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    OnMouseMove( nFlags, point );
    CDialog::OnLButtonDown(nFlags, point);
}

void Csmv_transcodeDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CRect rect;
    if( !img_TitleBarMid.IsNull() )
    {
        GetClientRect( rect );
        rect.bottom = img_TitleBarMid.GetHeight();

        if( rect.PtInRect(point) )
        {
            CMenu *MainMenu = GetMenu();
            MainMenu = MainMenu->GetSubMenu(2);

            CPoint tmp = point;
            ClientToScreen( &tmp );
            MainMenu->TrackPopupMenu( TPM_LEFTALIGN |TPM_RIGHTBUTTON, tmp.x, tmp.y, this );
        }
    }

    CDialog::OnRButtonDown(nFlags, point);
}

void Csmv_transcodeDlg::OnBnClickedMenuMin()
{
    // TODO: Add your control notification handler code here
    ::PostMessage( this->m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0 );
}

void Csmv_transcodeDlg::OnBnClickedMenuMax()
{
    // TODO: Add your control notification handler code here
    //if( m_MenuMin.IsWindowVisible() == TRUE )
    //{
    //    m_MenuMin.ShowWindow(SW_HIDE);
    //}

    //if( m_MenuMax.IsWindowVisible() == TRUE )
    //{
    //    m_MenuMax.ShowWindow(SW_HIDE);
    //}

    //if( m_MenuClose.IsWindowVisible() == TRUE )
    //{
    //    m_MenuClose.ShowWindow(SW_HIDE);
    //}

    //::PostMessage( this->m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0 );
#if _DEBUG
    RefreshSkin();
#endif
}

void Csmv_transcodeDlg::OnBnClickedMenuClose()
{
    // TODO: Add your control notification handler code here
    ::PostMessage( this->m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0 );
}

void Csmv_transcodeDlg::OnBnClickedAudioMono()
{
    // TODO: Add your control notification handler code here
    m_btnAudioMono.SetCheck( TRUE );
    m_btnAudioStereo.SetCheck( FALSE );
}

void Csmv_transcodeDlg::OnBnClickedAudioStereo()
{
    // TODO: Add your control notification handler code here
    m_btnAudioMono.SetCheck( FALSE );
    m_btnAudioStereo.SetCheck( TRUE );
}

void Csmv_transcodeDlg::OnHelpAbout()
{
    // TODO: Add your command handler code here
    CAboutDlg dlg;
    dlg.DoModal();
}

BOOL GetSkinProfile( char *name, int *pos_x, int *pos_y )
{
    char buffer[100];

    GetPrivateProfileString( name, "position", "", buffer, 100, APP_SKIN_INI_FILE_NAME );
    if( 2 != sscanf( buffer, "%d, %d", pos_x, pos_y ))
    {
#if _DEBUG
        static BOOL first = TRUE;

        if( first == TRUE )
        {
            AfxMessageBox( name );
            first = FALSE;
        }
#endif
        return FALSE;
    }

    return TRUE;
}

#if _DEBUG
void WriteSkinProfile( char *name, int pos_x, int pos_y )
{
    char buffer[100];
    int diff_y = ::GetSystemMetrics( SM_CYCAPTION ) + ::GetSystemMetrics( SM_CYMENU ) + ::GetSystemMetrics( SM_CYEDGE ) + 1;
    int diff_x = ::GetSystemMetrics( SM_CXEDGE ) + 1;

    sprintf( buffer, "%d, %d", pos_x - diff_x, pos_y - diff_y );
    WritePrivateProfileString( name, "position", buffer, APP_SKIN_INI_FILE_NAME );
}

void Csmv_transcodeDlg::WriteBtnPosition()
{
    CRect winrect;
    this->GetWindowRect( winrect );

    CRect rect;
    GetClientRect( &rect );

    WriteSkinProfile("ScreenSize", rect.right - rect.left, rect.bottom - rect.top );

    m_btnAddItem.GetWindowRect( rect );
    WriteSkinProfile( "m_btnAddItem", rect.left - winrect.left, rect.top - winrect.top );

    m_btnDeleteItem.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnDeleteItem", rect.left - winrect.left, rect.top - winrect.top );

    m_btnTransItem.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnTransItem", rect.left - winrect.left, rect.top - winrect.top );

    m_btnSetting.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnSetting", rect.left - winrect.left, rect.top - winrect.top );

    m_btnAbout.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnAbout", rect.left - winrect.left, rect.top - winrect.top );

    m_btnMenuMin.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnMenuMin", rect.left - winrect.left, rect.top - winrect.top );

    m_btnMenuMax.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnMenuMax", rect.left - winrect.left, rect.top - winrect.top );

    m_btnMenuClose.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnMenuClose", rect.left - winrect.left, rect.top - winrect.top );

    m_btnMenuBar.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnMenuBar", rect.left - winrect.left, rect.top - winrect.top );

    m_btnPrevious.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnPrevious", rect.left - winrect.left, rect.top - winrect.top );

    m_btnNext.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnNext", rect.left - winrect.left, rect.top - winrect.top );

    m_btnPlay.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnPlay", rect.left - winrect.left, rect.top - winrect.top );

    m_btnPause.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnPause", rect.left - winrect.left, rect.top - winrect.top );

    m_btnStop.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnStop", rect.left - winrect.left, rect.top - winrect.top );

    m_btnSetAsStart.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnSetAsStart", rect.left - winrect.left, rect.top - winrect.top );

    m_btnSetAsEnd.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnSetAsEnd", rect.left - winrect.left, rect.top - winrect.top );

    m_btnIdleFocus.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnIdleFocus", rect.left - winrect.left, rect.top - winrect.top );

    m_btnFullImage.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnFullImage", rect.left - winrect.left, rect.top - winrect.top );

    m_btnFullScreen.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnFullScreen", rect.left - winrect.left, rect.top - winrect.top );

    m_btnAudioStereo.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnAudioStereo", rect.left - winrect.left, rect.top - winrect.top );

    m_btnAudioMono.GetWindowRect( &rect );
    WriteSkinProfile( "m_btnAudioMono", rect.left - winrect.left, rect.top - winrect.top );

    m_cmbSize.GetWindowRect( &rect );
    WriteSkinProfile( "m_cmbSize", rect.left - winrect.left, rect.top - winrect.top );

    m_cmbQuality.GetWindowRect( &rect );
    WriteSkinProfile( "m_cmbQuality", rect.left - winrect.left, rect.top - winrect.top );

    m_cmbSampleRate.GetWindowRect( &rect );
    WriteSkinProfile( "m_cmbSampleRate", rect.left - winrect.left, rect.top - winrect.top );

    m_cmbGraphicRate.GetWindowRect( &rect );
    WriteSkinProfile( "m_cmbGraphicRate", rect.left - winrect.left, rect.top - winrect.top );

    m_cmbFrameRate.GetWindowRect( &rect );
    WriteSkinProfile( "m_cmbFrameRate", rect.left - winrect.left, rect.top - winrect.top );

    m_cmbRotate.GetWindowRect( &rect );
    WriteSkinProfile( "m_cmbRotate", rect.left - winrect.left, rect.top - winrect.top );

    m_listItems.GetWindowRect( &rect );
    WriteSkinProfile( "m_listItems", rect.left - winrect.left, rect.top - winrect.top );

    m_Slider.GetWindowRect( &rect );
    WriteSkinProfile( "m_Slider", rect.left - winrect.left, rect.top - winrect.top );

    m_StaticSize.GetWindowRect( &rect );
    WriteSkinProfile( "m_StaticSize", rect.left - winrect.left, rect.top - winrect.top );

    m_StaticQuality.GetWindowRect( &rect );
    WriteSkinProfile( "m_StaticQuality", rect.left - winrect.left, rect.top - winrect.top );

    m_StaticSampleRate.GetWindowRect( &rect );
    WriteSkinProfile( "m_StaticSampleRate", rect.left - winrect.left, rect.top - winrect.top );

    m_StaticAudioChannel.GetWindowRect( &rect );
    WriteSkinProfile( "m_StaticAudioChannel", rect.left - winrect.left, rect.top - winrect.top );

    m_StaticGraphicRate.GetWindowRect( &rect );
    WriteSkinProfile( "m_StaticGraphicRate", rect.left - winrect.left, rect.top - winrect.top );

    m_StaticFrameRate.GetWindowRect( &rect );
    WriteSkinProfile( "m_StaticFrameRate", rect.left - winrect.left, rect.top - winrect.top );

    m_StaticRotate.GetWindowRect( &rect );
    WriteSkinProfile( "m_StaticRotate", rect.left - winrect.left, rect.top - winrect.top );

    m_StaticGroupBox.GetWindowRect( &rect );
    WriteSkinProfile( "m_StaticGroupBox", rect.left - winrect.left, rect.top - winrect.top );

    m_StaticPlayGround.GetWindowRect( &rect );
    WriteSkinProfile( "m_StaticPlayGround", rect.left - winrect.left, rect.top - winrect.top );

    m_StaticProgress.GetWindowRect( &rect );
    WriteSkinProfile( "m_StaticProgress", rect.left - winrect.left, rect.top - winrect.top );
}
#endif

void Csmv_transcodeDlg::GetBtnPosition()
{
    // TODO: Add your control notification handler code here
    int scr_width = 200;
    int scr_height = 200;

    if( !img_Outline.IsNull() )
    {
        scr_width = img_Outline.GetWidth();
        scr_height = img_Outline.GetHeight();
    }

    ::SetWindowPos( this->m_hWnd, NULL, 0, 0, scr_width + 2*GetSystemMetrics( SM_CXEDGE ),
            scr_height + GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYMENU ) + 2*GetSystemMetrics( SM_CYEDGE ),
            SWP_NOMOVE | SWP_NOZORDER );

    int x, y;
    if( TRUE == GetSkinProfile( "m_btnAddItem", &x, &y ))
    {
        m_btnAddItem.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnDeleteItem", &x, &y ))
    {
        m_btnDeleteItem.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnTransItem", &x, &y ))
    {
        m_btnTransItem.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnSetting", &x, &y ))
    {
        m_btnSetting.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnAbout", &x, &y ))
    {
        m_btnAbout.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnMenuMin", &x, &y ))
    {
        m_btnMenuMin.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnMenuMax", &x, &y ))
    {
        m_btnMenuMax.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnMenuClose", &x, &y ))
    {
        m_btnMenuClose.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnMenuBar", &x, &y ))
    {
        m_btnMenuBar.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

#if SUPPORT_SMV_PLAY
    if( TRUE == GetSkinProfile( "m_btnPrevious", &x, &y ))
    {
        m_btnPrevious.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnNext", &x, &y ))
    {
        m_btnNext.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnPlay", &x, &y ))
    {
        m_btnPlay.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnPause", &x, &y ))
    {
        m_btnPause.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnStop", &x, &y ))
    {
        m_btnStop.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }
#endif

    if( TRUE == GetSkinProfile( "m_btnSetAsStart", &x, &y ))
    {
        m_btnSetAsStart.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnSetAsEnd", &x, &y ))
    {
        m_btnSetAsEnd.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnIdleFocus", &x, &y ))
    {
        m_btnIdleFocus.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnFullImage", &x, &y ))
    {
        m_btnFullImage.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnFullScreen", &x, &y ))
    {
        m_btnFullScreen.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnAudioStereo", &x, &y ))
    {
        m_btnAudioStereo.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_btnAudioMono", &x, &y ))
    {
        m_btnAudioMono.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_cmbSize", &x, &y ))
    {
        m_cmbSize.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_cmbQuality", &x, &y ))
    {
        m_cmbQuality.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_cmbSampleRate", &x, &y ))
    {
        m_cmbSampleRate.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_cmbGraphicRate", &x, &y ))
    {
        m_cmbGraphicRate.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_cmbFrameRate", &x, &y ))
    {
        m_cmbFrameRate.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_cmbRotate", &x, &y ))
    {
        m_cmbRotate.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_listItems", &x, &y ))
    {
        m_listItems.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_Slider", &x, &y ))
    {
        m_Slider.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_StaticSize", &x, &y ))
    {
        m_StaticSize.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_StaticQuality", &x, &y ))
    {
        m_StaticQuality.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_StaticSampleRate", &x, &y ))
    {
        m_StaticSampleRate.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_StaticAudioChannel",  &x, &y ))
    {
        m_StaticAudioChannel.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_StaticGraphicRate", &x, &y ))
    {
        m_StaticGraphicRate.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_StaticFrameRate", &x, &y ))
    {
        m_StaticFrameRate.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_StaticRotate", &x, &y ))
    {
        m_StaticRotate.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_StaticGroupBox", &x, &y ))
    {
        m_StaticGroupBox.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_StaticPlayGround", &x, &y ))
    {
        m_StaticPlayGround.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "m_StaticProgress", &x, &y ))
    {
        m_StaticProgress.SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    if( TRUE == GetSkinProfile( "PlayerScreenLeftTop", &x, &y ))
    {
        m_recPlayerScreen.left = x;
        m_recPlayerScreen.top = y;
    }
    else
    {
        m_recPlayerScreen.left = 0;
        m_recPlayerScreen.top = 0;
    }

    if( TRUE == GetSkinProfile( "PlayerScreenRightBottom", &x, &y ))
    {
        m_recPlayerScreen.right = x;
        m_recPlayerScreen.bottom = y;
    }
    else
    {
        m_recPlayerScreen.right = 100;
        m_recPlayerScreen.bottom = 100;
    }
}

void Csmv_transcodeDlg::GetConfig()
{
    char buffer[ MAX_INI_LEN_BUFFER ];
    char sk1[ MAX_INI_LEN_BUFFER ];
    char sk2[ MAX_INI_LEN_BUFFER ];
    char sk3[ MAX_INI_LEN_BUFFER ];

    GetPrivateProfileString( "Saved Directory", "Dir", "", buffer, MAX_INI_LEN_BUFFER, APP_CONFIG_INI_FILE_NAME );
    g_SavedDirectory = buffer;

    // menu
    CMenu *MainMenu = GetMenu();

    char tmpdir[_MAX_PATH];
    char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];

    GetModuleFileName( NULL, tmpdir, _MAX_PATH );
    _splitpath( tmpdir, drive, dir, fname, ext );
    _makepath( tmpdir, drive, dir, "", "" );

    CString str = tmpdir;
    str += "skin\\";

    CMenu *pMenu;
    pMenu = MainMenu->GetSubMenu(2);
    if( pMenu == NULL )
    {
        return;
    }

    GetPrivateProfileString( "Skin Define", "Skin1", "Default", sk1, MAX_INI_LEN_BUFFER, APP_CONFIG_INI_FILE_NAME );
    if( strlen( sk1 ) != 0 )
    {
        pMenu->ModifyMenu( ID_SKIN_SKIN1, MF_STRING, ID_SKIN_SKIN1, sk1 );
    }

    GetPrivateProfileString( "Skin Define", "Skin2", "", sk2, MAX_INI_LEN_BUFFER, APP_CONFIG_INI_FILE_NAME );
    if( strlen( sk2 ) != 0 )
    {
        pMenu->ModifyMenu( ID_SKIN_SKIN2, MF_STRING, ID_SKIN_SKIN2, sk2 );
    }
    else
    {
        pMenu->RemoveMenu( ID_SKIN_SKIN2, MF_BYCOMMAND);
    }

    GetPrivateProfileString( "Skin Define", "Skin3", "", sk3, MAX_INI_LEN_BUFFER, APP_CONFIG_INI_FILE_NAME );
    if( strlen( sk3 ) != 0 )
    {
        pMenu->ModifyMenu( ID_SKIN_SKIN3, MF_STRING, ID_SKIN_SKIN3, sk3 );
    }
    else
    {
        pMenu->RemoveMenu( ID_SKIN_SKIN3, MF_BYCOMMAND);
    }

    GetPrivateProfileString( "Skin Check", "Skin1", "Selected", buffer, MAX_INI_LEN_BUFFER, APP_CONFIG_INI_FILE_NAME );
    if( stricmp( buffer, "Selected" ) == 0 )
    {
        str += sk1;
        APP_CONFIG_BMP_FILE_PATH = sk1;
    }
    else
    {
        GetPrivateProfileString( "Skin Check", "Skin2", "Unselected", buffer, MAX_INI_LEN_BUFFER, APP_CONFIG_INI_FILE_NAME );
        if( stricmp( buffer, "Selected" ) == 0 )
        {
            str += sk2;
            APP_CONFIG_BMP_FILE_PATH = sk2;
        }
        else
        {
            GetPrivateProfileString( "Skin Check", "Skin3", "Unselected", buffer, MAX_INI_LEN_BUFFER, APP_CONFIG_INI_FILE_NAME );
            if( stricmp( buffer, "Selected" ) == 0 )
            {
                str += sk3;
                APP_CONFIG_BMP_FILE_PATH = sk3;
            }
        }
    }

    str += "\\";
    str += "skin.ini";
    APP_SKIN_INI_FILE_NAME = str;
}

void Csmv_transcodeDlg::WriteConfig()
{
    if( g_blTitleShow == TRUE )
    {
        WritePrivateProfileString( "MenuBar", "Hide", "FALSE", APP_SKIN_INI_FILE_NAME );
    }
    else
    {
        WritePrivateProfileString( "MenuBar", "Hide", "TRUE", APP_SKIN_INI_FILE_NAME );
    }

    WritePrivateProfileString( "Saved Directory", "Dir", g_SavedDirectory, APP_CONFIG_INI_FILE_NAME );
}

void Csmv_transcodeDlg::RefreshSkin()
{
#define MAX_SKIN_LEN_BUFFER 300
    char buffer[ MAX_SKIN_LEN_BUFFER ];

    GetConfig();
    LoadSkin();
    GetBtnPosition();
    ::PostMessage( this->m_hWnd, WM_SIZE, 0, 0 );

    CMenu *MainMenu = GetMenu();
    CMenu *pMenu;
    pMenu = MainMenu->GetSubMenu(2);
    if( pMenu == NULL )
    {
        return;
    }

    GetPrivateProfileString( "Skin Check", "Skin1", "Selected", buffer, MAX_SKIN_LEN_BUFFER, APP_CONFIG_INI_FILE_NAME );
    if( stricmp( buffer, "Selected" ) == 0 )
    {
        pMenu->CheckMenuItem(ID_SKIN_SKIN1, MF_BYCOMMAND|MF_CHECKED);
    }
    else
    {
        GetPrivateProfileString( "Skin Check", "Skin2", "Unselected", buffer, MAX_SKIN_LEN_BUFFER, APP_CONFIG_INI_FILE_NAME );
        if( stricmp( buffer, "Selected" ) == 0 )
        {
            pMenu->CheckMenuItem(ID_SKIN_SKIN2, MF_BYCOMMAND|MF_CHECKED);
        }
        else
        {
            GetPrivateProfileString( "Skin Check", "Skin3", "Unselected", buffer, MAX_SKIN_LEN_BUFFER, APP_CONFIG_INI_FILE_NAME );
            if( stricmp( buffer, "Selected" ) == 0 )
            {
                pMenu->CheckMenuItem(ID_SKIN_SKIN3, MF_BYCOMMAND|MF_CHECKED);
            }
        }
    }
}

void Csmv_transcodeDlg::PutPreviewImage()
{
    if( m_hPreviewBitmap )
    {
        CBitmap *bmp;
        CClientDC Client( this );

        bmp = CBitmap::FromHandle( m_hPreviewBitmap );

        BITMAP bmpInfo;
        bmp->GetBitmap(&bmpInfo);

        CDC dcMemory;
        dcMemory.CreateCompatibleDC(&Client);

        CBitmap* pOldBitmap = dcMemory.SelectObject(bmp);

		if( m_blRePreviewRefresh )
		{
			m_blRePreviewRefresh = FALSE;
			Client.FillSolidRect( &m_recPlayerScreen, 0 );
		}
        if( m_blConverting == TRUE )
        {
            Client.BitBlt(
                m_recPlayerScreen.left + ( m_recPlayerScreen.right - m_recPlayerScreen.left - bmpInfo.bmWidth )/2,
                m_recPlayerScreen.top + ( m_recPlayerScreen.bottom - m_recPlayerScreen.top - bmpInfo.bmHeight )/2,
                bmpInfo.bmWidth, bmpInfo.bmHeight,
                &dcMemory, 0, 0, SRCCOPY );
        }
        else
        {
            Client.StretchBlt( m_recPlayerScreen.left, m_recPlayerScreen.top,
                m_recPlayerScreen.right - m_recPlayerScreen.left,
                m_recPlayerScreen.bottom - m_recPlayerScreen.top,
                &dcMemory, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY );
        }

        dcMemory.SelectObject(pOldBitmap);
    }
}

void Csmv_transcodeDlg::StartConvert()
{
    m_btnTransItem.InitImage( "CvtEnd_N.bmp", "CvtEnd_D.bmp", "CvtEnd_F.bmp", "CvtEnd_P.bmp" );
	m_btnTransItem.RedrawWindow();

    CMenu *MainMenu = GetMenu();

    if( MainMenu != NULL )
    {
        MainMenu  = MainMenu->GetSubMenu(0);
        if( MainMenu != NULL )
        {
            MainMenu->EnableMenuItem( ID_FILE_CONVERT, MF_DISABLED | MF_GRAYED );
            MainMenu->EnableMenuItem( ID_FILE_STOP, MF_ENABLED );
        }
    }

#if SUPPORT_SMV_PLAY
    m_btnPrevious.EnableWindow( FALSE );
    m_btnNext.EnableWindow( FALSE );
    m_btnPlay.EnableWindow( FALSE );
    m_btnPause.EnableWindow( FALSE );
    m_btnStop.EnableWindow( FALSE );
#endif

    m_btnSetAsStart.EnableWindow( FALSE );
    m_btnSetAsEnd.EnableWindow( FALSE );
    m_Slider.EnableWindow( FALSE );

    m_StaticPlayGround.RedrawWindow();
    m_btnSetAsStart.RedrawWindow();
    m_btnSetAsEnd.RedrawWindow();
    m_Slider.RedrawWindow();

    m_blConverting = FALSE;
    m_blStopping = FALSE;
    ::PostMessage( this->m_hWnd, WM_USER_CONTINUE_CONVERT, 0, 0 );
}

void Csmv_transcodeDlg::StopConvert()
{
    m_btnTransItem.InitImage("Convert_N.bmp","Convert_D.bmp","Convert_F.bmp","Convert_P.bmp");
	m_btnTransItem.RedrawWindow();

    CMenu *MainMenu = GetMenu();

    if( MainMenu != NULL )
    {
        MainMenu  = MainMenu->GetSubMenu(0);
        if( MainMenu != NULL )
        {
            MainMenu->EnableMenuItem( ID_FILE_CONVERT, MF_ENABLED );
            MainMenu->EnableMenuItem( ID_FILE_STOP, MF_DISABLED | MF_GRAYED );
        }
    }
#if SUPPORT_SMV_PLAY
    m_btnPrevious.EnableWindow( TRUE );
    m_btnNext.EnableWindow( TRUE );
    m_btnPlay.EnableWindow( TRUE );
    m_btnPause.EnableWindow( TRUE );
    m_btnStop.EnableWindow( TRUE );
#endif

    m_btnSetAsStart.EnableWindow( TRUE );
    m_btnSetAsEnd.EnableWindow( TRUE );
    m_Slider.EnableWindow( TRUE );

    m_StaticPlayGround.RedrawWindow();
    m_btnSetAsStart.RedrawWindow();
    m_btnSetAsEnd.RedrawWindow();
    m_Slider.RedrawWindow();

    if( m_blConverting == TRUE )
    {
		m_listItems.SetItemText( GetCurrentConvertItem(), LIST_SEG_STATUS, "Cancel" );
        StopTranscode();
        m_blStopping = TRUE;
    }
}

int Csmv_transcodeDlg::GetCurrentConvertItem()
{
    int count = m_listItems.GetItemCount();
    for( int i = 0; i < count ; i++ )
    {
        char name[ MAX_PATH ];
        m_listItems.GetItemText( i, LIST_SEG_NAME, name, MAX_PATH );

        if( stricmp( name, m_CurrentConvert.GetBuffer() ) == 0 )
        {
            return i;
        }
    }

    return -1;
}

LRESULT Csmv_transcodeDlg::ConvertNext(WPARAM wParam, LPARAM lParam)
{
    int count = m_listItems.GetItemCount();
    int curItem = GetCurrentConvertItem();

    for( int i = 0; i < count ; i++ )
    {
        if( m_listItems.GetCheck( i ) == TRUE )
        //if( m_listItems.GetItemState( i, LVIS_SELECTED ))
        {
            char name[ MAX_PATH ];
            m_listItems.GetItemText( i, LIST_SEG_NAME, name, MAX_PATH );

            char drive[_MAX_DRIVE];
            char dir[_MAX_DIR];
            char fname[_MAX_FNAME];
            char ext[_MAX_EXT];
            _splitpath( name, drive, dir, fname, ext );

            CString str = g_SavedDirectory;
            str += fname;
            str += ".SMV";

            LONGLONG len;
            LONG err = NewTranscoder( this->m_hWnd, name, str.GetBuffer(), &len );
			m_blRePreviewRefresh = TRUE;
            if( 0 == err )
            {
                m_listItems.SetItemText( i, LIST_SEG_STATUS, "" );

                UINT start = 0, end = 0;
                if( FALSE == GetStartEndTime( i, start, end ))
                {
                    start = 0;
                    end = (UINT)(len / VIDEO_BASE_TIME);
                    SetStartEndTime( i, 0, UINT(len / VIDEO_BASE_TIME) );
                }

                TRANSCODER_PARAM ChgPara;
                GetConvertPara( ChgPara, i );

                int tmp_start, tmp_end;
                int range_min, range_end;
                m_Slider.GetSelection( tmp_start, tmp_end );
                m_Slider.GetRange( range_min, range_end );
                if( tmp_end ==  range_end )
                {
                    ChgPara.markB = len;
                }

                m_blConverting = TRUE;
                m_CurrentConvert = name;
                m_CurrentConvertResult = str;
                Transcode( &ChgPara );
				m_listItems.SetItemText( i, LIST_SEG_STATUS, "Converting" );
                return 0;
            }
            else
            {
                SetItemErrorMessage( i, err );
            }
        }
    }

    StopConvert();
    return 0;
}

LRESULT Csmv_transcodeDlg::OnProgress(WPARAM wParam, LPARAM lParam)
{
    if( wParam > 100 )
    {
        wParam = 100;
    }

    m_StaticProgress.SetPos( (UINT)wParam );
	if( m_hPreviewBitmap != NULL )
		DeleteObject( m_hPreviewBitmap );
	m_hPreviewBitmap = NULL;
    if( lParam != 0 )
    {
        m_hPreviewBitmap = (HBITMAP)lParam;
    }

    if( wParam == 100 )
    {
		if( m_hPreviewBitmap != NULL )
			DeleteObject( m_hPreviewBitmap );
        m_hPreviewBitmap = NULL;
        m_blConverting = FALSE;

        if( m_blStopping == FALSE )
        {
            ::PostMessage( this->m_hWnd, WM_USER_CONTINUE_CONVERT, 0, 0 );

		    int curItem = GetCurrentConvertItem();

		    if( -1 != curItem )
		    {
		        CFile fileext;
		        if( TRUE == fileext.Open( m_CurrentConvertResult.GetBuffer(), CFile::modeRead ) )
		        {
		            LONGLONG len = fileext.GetLength();
		            CString tmp;
		            tmp.Format( "%d", len/1024 );
		            m_listItems.SetItemText( curItem, LIST_SEG_SIZE, tmp.GetBuffer() );
		            fileext.Close();
		        }

		        m_listItems.SetCheck( curItem, FALSE );
				m_listItems.SetItemText( curItem, LIST_SEG_STATUS, "Finish" );
		    }
        }

        if( m_blNeedNext == TRUE )
        {
            m_blNeedNext = FALSE;
            StartConvert();
        }
    }

    PutPreviewImage();
    return 0;
}

LRESULT Csmv_transcodeDlg::OnShowMessage( WPARAM wParam, LPARAM lParam)
{
    int curItem = GetCurrentConvertItem();

	if( -1 == curItem )
    {
		curItem = m_listItems.GetFocusItem();
		if( -1 == curItem )
			return -1;
    }

    CString str;
    switch( wParam )
    {
        case SMV_MESSAGE_HRESULT:
            str.Format( _T("Failed result = 0x%x"), lParam );
            break;

        case SMV_MESSAGE_INVALID_GRAPHIC_DATA_SIZE:
            str = _T("Please select larger graphic data size or smaller frame rate.");
            break;

        case SMV_MESSAGE_NO_CODEC:
            str = _T("No codec is available for this video format. Please install the codec and the latest Microsoft DirectX(TM)");
            break;

        case SMV_MESSAGE_NO_AUDIO_CODEC:
            str = _T("No audio codec is available for this file. Please install a correct codec");
            break;

        case SMV_MESSAGE_NO_VIDEO_CODEC:
            str = _T("No video codec is available for this file. Please install a correct codec");
            break;

        default:
            str.Format( _T("Unknown error = 0x%x, 0x%0x"), wParam, lParam );
            break;
    }

    m_listItems.SetItemText( curItem, LIST_SEG_STATUS, str );
    return 0;
}

void Csmv_transcodeDlg::SetStartEndTime( UINT item, UINT start, UINT end )
{
    char tmp[20];
    sprintf( tmp, "%0d:%02d:%02d", start/3600, start/60, start%60 );
    m_listItems.SetItemText( item, LIST_SEG_START, tmp );

    sprintf( tmp, "%0d:%02d:%02d", end/3600, end/60, end%60 );
    m_listItems.SetItemText( item, LIST_SEG_END, tmp );
}

BOOL Csmv_transcodeDlg::GetStartEndTime( UINT item, UINT &start, UINT &end )
{
    start = 0;
    end = 0;

    char tmp[20];
    int hour = 0, minute = 0, second = 0;

    m_listItems.GetItemText( item, LIST_SEG_START, tmp, 20 );
    if( sscanf( tmp, "%d:%2d:%2d", &hour, &minute, &second ) == 3 )
    {
        start = hour * 3600 + minute * 60 + second;
    }
    else
    {
        return FALSE;
    }

    m_listItems.GetItemText( item, LIST_SEG_END, tmp, 20 );
    if( sscanf( tmp, "%d:%2d:%2d", &hour, &minute, &second ) == 3 )
    {
        end = hour * 3600 + minute * 60 + second;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

