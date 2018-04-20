
// ComConstructorDoc.cpp : implementation of the CComConstructorDoc class
//

#include "stdafx.h"
 

 
#include "ComConstructor.h"
#include "ComConstructorDoc.h"
//for imageprocessing
#include "VideoProcessor.h"
#include "ColorDetector.h"
#include "ColorDetectController.h"

using namespace cv;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CComConstructorDoc

IMPLEMENT_DYNCREATE(CComConstructorDoc, CDocument)

BEGIN_MESSAGE_MAP(CComConstructorDoc, CDocument)
	ON_COMMAND(ID_VIDEOPROCESSING_COLORDECTECT, &CComConstructorDoc::OnVideoprocessingColordectect)
END_MESSAGE_MAP()


// CComConstructorDoc construction/destruction

CComConstructorDoc::CComConstructorDoc()
{
	// TODO: add one-time construction code here

}

CComConstructorDoc::~CComConstructorDoc()
{
	
}

BOOL CComConstructorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	reinterpret_cast<CEditView*>(m_viewList.GetHead())->SetWindowText(NULL);

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CComConstructorDoc serialization

void CComConstructorDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	reinterpret_cast<CEditView*>(m_viewList.GetHead())->SerializeRaw(ar);
}


// CComConstructorDoc diagnostics

#ifdef _DEBUG
void CComConstructorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CComConstructorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CComConstructorDoc commands

void CComConstructorDoc::OnVideoprocessingColordectect()
{
		// Create instance
		VideoProcessor processor;
		// Open video file
		processor.setInput(1);

		processor.setDelay(33);
		// Set the frame processor callback function
		processor.setFrameProcessor(ColorBall);
	
		// Start the process
		processor.run();

		cv::waitKey();
}

