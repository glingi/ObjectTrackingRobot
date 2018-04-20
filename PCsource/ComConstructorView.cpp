
// ComConstructorView.cpp : implementation of the CComConstructorView class
//

#include "stdafx.h"

#include "ComConstructor.h"

#include "MainFrm.h"
#include "ComConstructorDoc.h"
#include "ComConstructorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CComConstructorView

IMPLEMENT_DYNCREATE(CComConstructorView, CEditView)

BEGIN_MESSAGE_MAP(CComConstructorView, CEditView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CComConstructorView::OnFilePrintPreview)
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CComConstructorView construction/destruction

CComConstructorView::CComConstructorView()
{
	// TODO: add construction code here

}

CComConstructorView::~CComConstructorView()
{
}

BOOL CComConstructorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}


// CComConstructorView printing


void CComConstructorView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CComConstructorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CComConstructorView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CComConstructorView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

void CComConstructorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CComConstructorView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CComConstructorView diagnostics

#ifdef _DEBUG
void CComConstructorView::AssertValid() const
{
	CEditView::AssertValid();
}

void CComConstructorView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CComConstructorDoc* CComConstructorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CComConstructorDoc)));
	return (CComConstructorDoc*)m_pDocument;
}
#endif //_DEBUG


// CComConstructorView message handlers
// 키보드로부터 키입력이 들어올때 윈도우 메시지 WM_CHAR발생
void CComConstructorView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd();
	CEdit& editView = GetEditCtrl();
	char szChar[4];
	char *pChar;
	pChar = szChar;
	CString szString;
	szString.Format(_T("%c"),szChar);

	// Send the data through the serial connection (if it's open)
	if (pMainFrame->m_Serial.IsOpen())
	{
		char ch = static_cast<char>(nChar);
		pMainFrame->m_Serial.Write(&ch,1);
		*pChar++ = ch;
		if(ch == '\r')
			*pChar++ ='\n';
		*pChar++ = 0x00;
		editView.SetSel(-1, 0);
		editView.ReplaceSel(szString);
	} //EDITVIEW는 시리얼 포트로 수신된 데이터를 출력해 주는 역할

	CEditView::OnChar(nChar, nRepCnt, nFlags);
}
