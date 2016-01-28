// This is a part of the Microsoft Foundation Classes C++ library. 
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

// ChildView.h : interface of the CChildView class
//


#pragma once
#include <afxstr.h>
#include <atlimage.h>
#include <comdef.h>


enum SizesEnum
{
	SIZE_HALF,
	SIZE_ORIGINAL,
	SIZE_DOUBLE,
	SIZE_FILL,
	SIZE_NONE
};

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions

private:
	CImage imgOriginal;
	int	m_nFilterLoad;
	SizesEnum m_nImageSize;

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	void OnFileOpenimage(void);
	void OnFileSaveImage(void);
	void OnToolsMakeBW(void);
	void OnImageInfo(void);
	void OnUpdateSizeHalf(CCmdUI* pCmdUI);
	void OnUpdateSizeOriginal(CCmdUI* pCmdUI);
	void OnUpdateSizeDouble(CCmdUI* pCmdUI);
	void OnUpdateSizeFill(CCmdUI* pCmdUI);
	void OnChangeSize(UINT nID);
	void OnUpdateImage(CCmdUI* pCmdUI);

public:
	void OnToolsHomeworkII();
	void OnHomeworkiiErosion();
	void OnHomeworkiiDelation();
	void OnHomeworkiiOpening();
	void OnHomeworkiiClosing();
	void OnHomeworkiiBinarization();
	void erosion();
	void delation();

	void OnToolsHomeworkIII();

	void OnHomeworkiiiImagelogarithmic();
	void OnHomeworkiiiHistogramequalization();
	void OnHomeworkivTranslation();
	void OnHomeworkivRotation();
	void OnHomeworkivScale();
	void OnScaleSameratio();
	void OnScaleDifferentratio();
	void OnHomeworkivShear();
	void OnShearOnxaxis();
	void OnShearOnyaxis();
	void OnHomeworkivMirror();
	void OnScaleSameratiolarger();
	void OnScaleDifferentratiolarger();
	void OnHomeworkvMeanfiltering();
	void OnHomeworkvLaplacian();
	void OnMeanfilteringSimplemean();
	void OnMeanfilteringWeightedmean();

	void OnLaplacianenhancementLaplacianresult();
	void OnLaplacianenhancementRearrangedlaplacianresult();
	void OnLaplacianenhancementAfterfusion();
	void OnHomeworkviBilateralfiltering();
};
