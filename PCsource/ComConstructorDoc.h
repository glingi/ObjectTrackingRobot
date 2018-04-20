
// ComConstructorDoc.h : interface of the CComConstructorDoc class
//


#pragma once

#include "stdafx.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

class CComConstructorDoc : public CDocument
{
protected: // create from serialization only
	CComConstructorDoc();
	DECLARE_DYNCREATE(CComConstructorDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CComConstructorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnVideoprocessingColordectect();
};


