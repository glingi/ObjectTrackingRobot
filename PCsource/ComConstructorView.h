
// ComConstructorView.h : interface of the CComConstructorView class
//


#pragma once


class CComConstructorView : public CEditView
{
protected: // create from serialization only
	CComConstructorView();
	DECLARE_DYNCREATE(CComConstructorView)

// Attributes
public:
	CComConstructorDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CComConstructorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in ComConstructorView.cpp
inline CComConstructorDoc* CComConstructorView::GetDocument() const
   { return reinterpret_cast<CComConstructorDoc*>(m_pDocument); }
#endif

