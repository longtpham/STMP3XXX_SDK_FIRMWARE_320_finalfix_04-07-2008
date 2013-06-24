// smv_transcodeDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// Csmv_transcodeDlg dialog
class Csmv_transcodeDlg : public CDialog
{
// Construction
public:
    Csmv_transcodeDlg(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
    enum { IDD = IDD_SMV_TRANSCODE_DIALOG };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

private:
    void RedrawScreen();
    void LoadSkin();
    void LoadDefaultData();
#if _DEBUG
    void WriteBtnPosition();
#endif
    void GetBtnPosition();
    void GetConfig();
    void WriteConfig();
    void SkinChange( UINT nCheckID );
    void RefreshSkin();
    void PutPreviewImage();
    void OnShowMainInterface();
    void GetConvertPara(TRANSCODER_PARAM &ChgPara, int item );
    void SetStartEndTime( UINT item, UINT start, UINT end );
    BOOL GetStartEndTime( UINT item, UINT &start, UINT &end );
    LRESULT OnProgress(WPARAM wParam, LPARAM lParam);
    LRESULT OnShowMessage( WPARAM wParam, LPARAM lParam);
    LRESULT OnListRButton( WPARAM WParam, LPARAM LParam );
    LRESULT OnRefreshSkin( WPARAM WParam, LPARAM LParam );
    LRESULT ConvertNext(WPARAM wParam, LPARAM lParam);
    void StartConvert();
    void StopConvert();
    int GetCurrentConvertItem();
    void SetItemErrorMessage( int item, LONG err );

public:
    RECT m_recPlayerScreen; // use for preview window

private:
    BOOL m_blConverting;
    BOOL m_blStopping;
    BOOL m_blNeedNext;
    BOOL m_blRePreviewRefresh;
    CString m_CurrentConvert;
    CString m_CurrentConvertResult;
    HBITMAP m_hPreviewBitmap;

    CUserImage img_Outline;
    CUserImage img_background;
    CUserImage img_TitleBarMid;
    CUserImage img_TitleBarLeftTop;
    CUserImage img_TitleBarRightTop;
    CUserImage img_LeftBar;
    CUserImage img_RightBar;
    CUserImage img_BottomBar;
    CUserImage img_LeftBottom;
    CUserImage img_RightBottom;

    CImgButton m_btnAddItem;
    CImgButton m_btnDeleteItem;
    CImgButton m_btnTransItem;
    CImgButton m_btnSetting;
    CImgButton m_btnAbout;

    CImgButton m_btnMenuMin;
    CImgButton m_btnMenuMax;
    CImgButton m_btnMenuClose;
    CImgButton m_btnMenuBar;

    CImgButton m_btnPrevious;
    CImgButton m_btnNext;
    CImgButton m_btnPlay;
    CImgButton m_btnPause;
    CImgButton m_btnStop;

    CImgButton m_btnSetAsStart;
    CImgButton m_btnSetAsEnd;

    CButton m_btnIdleFocus;

    CImageCheckBox m_btnFullImage;
    CImageCheckBox m_btnFullScreen;

    CImageCheckBox m_btnAudioStereo;
    CImageCheckBox m_btnAudioMono;

    CImgComboBox m_cmbSize;
    CImgComboBox m_cmbQuality;
    CImgComboBox m_cmbSampleRate;
    CImgComboBox m_cmbGraphicRate;
    CImgComboBox m_cmbFrameRate;
    CImgComboBox m_cmbRotate;

    CImgListCtrl m_listItems;
    CImgSlider m_Slider;

    CImgStatic m_StaticSize;
    CImgStatic m_StaticQuality;
    CImgStatic m_StaticSampleRate;
    CImgStatic m_StaticAudioChannel;
    CImgStatic m_StaticGraphicRate;
    CImgStatic m_StaticFrameRate;
    CImgStatic m_StaticRotate;
    CImgStatic m_StaticGroupBox;
    CImgStatic m_StaticPlayGround;

    CImgProgressCtrl m_StaticProgress;

// Generated message map functions
private:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnBnClickedPlayPrevious();
    afx_msg void OnBnClickedPlayNext();
    afx_msg void OnBnClickedPlayPlay();
    afx_msg void OnBnClickedPlayPause();
    afx_msg void OnBnClickedPlayStop();
    afx_msg void OnCbnSelchangeQuality();
    afx_msg void OnCbnSelchangeSampleRate();
    afx_msg void OnCbnSelchangeAudioChannel();
    afx_msg void OnCbnSelchangeGraphicRate();
    afx_msg void OnCbnSelchangeFrameRate();
    afx_msg void OnCbnSelchangeRotate();
    afx_msg void OnBnClickedFullImage();
    afx_msg void OnBnClickedFullScreen();
    afx_msg void OnLvnKeydownListItem(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnListmenuInsert();
    afx_msg void OnListmenuDelete();
    afx_msg void OnListmenuDeleteall();
    afx_msg void OnListmenuSelect();
    afx_msg void OnListmenuSelectall();
    afx_msg void OnListmenuDeselect();
    afx_msg void OnListmenuDeselectall();
    afx_msg void OnFileAdd();
    afx_msg void OnEditSelectall();
    afx_msg void OnEditDeselectall();
    afx_msg void OnFileSavedirectory();
    afx_msg void OnEditDeleteall();
    afx_msg void OnFileConvert();
    afx_msg void OnFileStop();
    afx_msg void OnSkinSkin1();
    afx_msg void OnSkinSkin2();
    afx_msg void OnSkinSkin3();
    afx_msg void OnBnClickedAddItem();
    afx_msg void OnBnClickedDeleteItem();
    afx_msg void OnBnClickedTransItem();
    afx_msg void OnBnClickedAbout();
    afx_msg void OnBnClickedAsStart();
    afx_msg void OnBnClickedAsEnd();
    afx_msg void OnBnClickedHideMenu();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnBnClickedMenuMin();
    afx_msg void OnBnClickedMenuMax();
    afx_msg void OnBnClickedMenuClose();
    afx_msg void OnBnClickedAudioMono();
    afx_msg void OnBnClickedAudioStereo();
    afx_msg void OnHelpAbout();
    afx_msg void OnBnClickedSetting();
public:
    afx_msg void OnLvnItemchangedListItem(NMHDR *pNMHDR, LRESULT *pResult);
};

