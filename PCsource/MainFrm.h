
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "serialmfc.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//편의를 위해 public 으로 변경
public:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	
	afx_msg LRESULT OnSerialMsg ( WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	//선언한것
public:
	CSerialMFC m_Serial;
	int m_nPort;
	CSerial::EBaudrate m_eBaudrate;
public:
	

	//메시지 핸들러들
	afx_msg void OnConnect();
	afx_msg void OnUpdateDisconnect(CCmdUI *pCmdUI);
	
	afx_msg void OnDisconnect();
	afx_msg void OnUpdateConnect(CCmdUI *pCmdUI);
};


