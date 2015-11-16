
//���������Ҫ���ļ�

#include "stdafx.h"
#include "CImage.h"
#include "ChildView.h"
#include "InfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*------------------------------------------------------------------------------------------------------------
1.���������ǻ���MFC��������ʾ������ͼ���Ӧ�ó����ܣ�ͬѧ�ǿ����ڴ˿�ܵĻ����������ҵ����Ĺ��ܴ��롣
2.�����д���ͼ�������΢��MFC��������ṩ��CImage�࣬�����ṩ�˶�ȡ����ʾ���ָ�ʽͼ��ĺ��������Դ��򻯴�
  ��ͼ������еĹ�������ʹ��Ұ���Ҫ������������ҵ���еĹ���ʵ���ϡ�
3.ÿ����ҵ��Ӧ�Ų˵���Tools�µ�һ���˵��ÿ���˵����Ӧһ���հ׺���������ʵ��ÿ����ҵ�Ĺ��ܴ��롣
4.�����һ��ͼ���Ժ�ͼ�������洢��CImage���͵�imgOriginal�����У����CImage��ĳ�Ա������ǿ�󣬿�����MSDN
 ����F1�򿪣��в�ѯ�й����Ĳ�����imgOriginal�����Ǳ�������CChildView��˽�б��������������µĳ�Ա�����б����á�
  �й�ͼ���һЩ��������������imgOriginal.������()�ķ������ã������һ��ͼ�������imgOriginal.Load("x.jpg")��
  �������õĲ�������MSDN������CImage��
5.�����CChildView::OnToolsMakeBW��һ���򵥵����ӣ����ǲ˵���Tools�µĵ�һ���˵����Ӧ����Ϣ��Ӧ�������Ѳ�ɫͼ��
  תΪ�Ҷ�ͼ��
------------------------------------------------------------------------------------------------------------*/
void CChildView::OnToolsMakeBW(void)
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��

	if (!imgOriginal.IsIndexed()) //�����ǰ�򿪵�ͼ���ÿ�����ش���8λ����û��ʹ��ɫ���������󲿷�ͼ���������֣�
	{
		//��ÿ�����شӲ�ɫתΪ�Ҷ�
		COLORREF pixel;//����һ�����ص�ɫ����Ϣ
		int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
		byte r,g,b,avg;//r,g,bֵ��ƽ���Ҷ�ֵ����
		for (int x=0; x<maxX; x++) //��ѭ��
		{
			for (int y=0; y<maxY; y++) //��ѭ��
			{
				pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
				r = GetRValue(pixel);//ȡ��pixel��rֵ
				g = GetGValue(pixel);//ȡ��pixel��gֵ
				b = GetBValue(pixel);//ȡ��pixel��bֵ
				avg = ((r + g + b)/3);//�ô�ʽ��rgb��ɫֵתΪ�Ҷ�ֵ
				imgOriginal.SetPixelRGB(x,y,avg,avg,avg);//��λ��(x,y)λ���ϵ����ص�rgbɫ��ͨ����Ϊ(avg,avg,avg)����ת��Ϊ�Ҷ�
			}
		}
	} 
	//�°벿�ֵĴ����Ǵ���ͼ���ÿ������С��8λ�ģ�������ϸע��
	else
	{
		// the image uses an indexed palette, so we will just change the palette table entries to their B&W equivalents 
		int MaxColors = imgOriginal.GetMaxColorTableEntries();
		RGBQUAD* ColorTable;
		ColorTable = new RGBQUAD[MaxColors];

		imgOriginal.GetColorTable(0,MaxColors,ColorTable);
		for (int i=0; i<MaxColors; i++)
		{
			int avg = (ColorTable[i].rgbBlue + ColorTable[i].rgbGreen + ColorTable[i].rgbRed)/3;
			ColorTable[i].rgbBlue = (BYTE)avg;
			ColorTable[i].rgbGreen = (BYTE)avg;
			ColorTable[i].rgbRed = (BYTE)avg;
		}
		imgOriginal.SetColorTable(0,MaxColors,ColorTable);
	
		delete[] ColorTable;
	}

	//ˢ����ʾͼ��
	Invalidate();
	UpdateWindow();
}



/*-----------------------code for lab2----------------------------------
tasks:
1.Image binarization��
2.Binary image erosion
3.Binary image delation
4.Binary image opening
5.Binary image closing
6.Hit-or-miss transform (optional)
----------------------------------------------------------------------*/
void CChildView::OnHomeworkiiBinarization()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��
	COLORREF pixel;//����һ�����ص�ɫ����Ϣ
	int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
	int *pixelArray = new int[maxX*maxY];

	//Find the maximal and minimal pixel value in the image
	byte MAXavg = 0, MINavg = 255, temp;
	for (int x=0; x<maxX; x++) //��ѭ��
	{
		for (int y=0; y<maxY; y++) //��ѭ��
		{
			pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
			temp = GetRValue(pixel);//ȡ��pixel��rֵ
			MAXavg = MAXavg >= temp ? MAXavg:temp;
			MINavg = MINavg <= temp ? MINavg:temp;
			pixelArray[x*maxY+y] = temp;
		}
	}

	//Thresholdingthe image by ��minimal value +1�� to obtain the binary image;
	int threshold = 0;
	int allPix = maxX * maxY;
	double betweenVariance = 0;
	for(int i = MINavg+1; i < MAXavg; ++i)
	{
		int forePix = 0, backPix = 0;
		int foreSum = 0, backSum = 0;
		//Determine the foreground and background in terms of the binary image;
		for(int j = 0; j < allPix; ++j)
		{
			if(pixelArray[j] >= i)
			{
				++backPix;
				backSum += pixelArray[j];
			}
			else
			{
				++forePix;
				foreSum += pixelArray[j];
			}
		}
		//Compute the the between-variance;
		double uf = (double)foreSum/forePix;
		double ub = (double)backSum/backPix;
		double tempVariance = (double)backPix/allPix*forePix/allPix*(uf - ub)*(uf - ub);
		if(betweenVariance < tempVariance)
		{
			betweenVariance = tempVariance;
			threshold = i;
		}
	}

	//Obtain the threshold and do Image binarization
	OnToolsMakeBW();
	byte avg, r;
	for (int x=0; x<maxX; x++) //��ѭ��
	{
		for (int y=0; y<maxY; y++) //��ѭ��
		{
			pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
			r = GetRValue(pixel);//ȡ��pixel��rֵ
			avg = (r >= threshold)? 255:0;
			imgOriginal.SetPixelRGB(x,y,avg,avg,avg);//��λ��(x,y)λ���ϵ����ص�rgbɫ��ͨ����Ϊ(avg,avg,avg)����ת��Ϊ�Ҷ�
		}
	}
	delete[] pixelArray;
	Invalidate();
	UpdateWindow();
}

void CChildView::erosion()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��
	COLORREF pixel;//����һ�����ص�ɫ����Ϣ
	int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
	int *pixelArray = new int[maxX*maxY];

	for (int x=0; x<maxX; x++) //��ѭ��
	{
		for (int y=0; y<maxY; y++) //��ѭ��
		{
			pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
			pixelArray[x*maxY+y] = GetRValue(pixel);
		}
	}

	//ʹ��3*3�ĽṹԪ�ظ�ʴ

	for (int x=1; x<maxX-1; x++) //��ѭ��
	{
		for (int y=1; y<maxY-1; y++) //��ѭ��
		{
			int nErosion = 1;
			for(int i=-1; i<2; ++i)
			{
				for(int j=-1; j<2; ++j)
				{
					if(pixelArray[(x+i)*maxY+(y+j)] != 255)
					{
						nErosion = 0;
						break;
					}
				}
			}
			if(!nErosion)
			{
				imgOriginal.SetPixelRGB(x,y,0,0,0);
			}
		}
	}
	Invalidate();
	UpdateWindow();	
}
void CChildView::OnHomeworkiiErosion()
{
	OnHomeworkiiBinarization();
	erosion();
}

void CChildView::delation()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��
	COLORREF pixel;//����һ�����ص�ɫ����Ϣ
	int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
	int *pixelArray = new int[maxX*maxY];

	for (int x=0; x<maxX; x++) //��ѭ��
	{
		for (int y=0; y<maxY; y++) //��ѭ��
		{
			pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
			pixelArray[x*maxY+y] = GetRValue(pixel);
		}
	}

	//ʹ��3*3�ĽṹԪ������
	for (int x=1; x<maxX-1; x++) //��ѭ��
	{
		for (int y=1; y<maxY-1; y++) //��ѭ��
		{
			int nDeflation = 1;
			for(int i=-1; i<2; ++i)
			{
				for(int j=-1; j<2; ++j)
				{
					if(pixelArray[(x+i)*maxY+(y+j)] == 255)
					{
						nDeflation = 0;
						break;
					}
				}
			}
			if(!nDeflation)
			{
				imgOriginal.SetPixelRGB(x,y,255,255,255);
			}
		}
	}	
	Invalidate();
	UpdateWindow();
}
void CChildView::OnHomeworkiiDelation()
{
	OnHomeworkiiBinarization();
	delation();
}


void CChildView::OnHomeworkiiOpening()
{
	OnHomeworkiiBinarization();
	erosion();
	delation();	
}

void CChildView::OnHomeworkiiClosing()
{
	OnHomeworkiiBinarization();
	delation();
	erosion();	
}
void CChildView::OnToolsHomeworkII()
{
}

/*-----------------------code for lab2----------------------------------*/



//Ϊ�˵��������Ϣ��Ӧ���������ʵ�ִ��루ϸ�ڲμ�ppt��
void CChildView::OnToolsHomeworkIII()
{
	// TODO: Add your command handler code here
	::AfxMessageBox("Hello World!");
}

/*-------------------------------------------------------------------------------------------------
���´����Ǳ�������ܰ������ֳɵĺ�������򿪡���ʾ���洢ͼ���ļ��Ĵ��룬ͬѧ�ǿ��Բ�ȥ�����о�
--------------------------------------------------------------------------------------------------*/

// CChildView�Ĺ��캯�������������Լ���Ϣӳ���(����ϸ��������ο�C++��MFC�ľ����鼮)

CChildView::CChildView()
{
	m_nImageSize = SIZE_NONE;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_FILE_OPENIMAGE, OnFileOpenimage)
	ON_COMMAND(ID_FILE_SAVEIMAGE, OnFileSaveImage)
	ON_COMMAND(ID_TOOLS_MAKEBW, OnToolsMakeBW)
	ON_COMMAND(ID_FILE_IMAGEINFO, OnImageInfo)
	ON_UPDATE_COMMAND_UI(ID_SIZE_HALF,OnUpdateSizeHalf)
	ON_UPDATE_COMMAND_UI(ID_SIZE_ORIGINAL,OnUpdateSizeOriginal)
	ON_UPDATE_COMMAND_UI(ID_SIZE_DOUBLE,OnUpdateSizeDouble)
	ON_UPDATE_COMMAND_UI(ID_SIZE_FILL,OnUpdateSizeFill)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEIMAGE,OnUpdateImage)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMAGEINFO,OnUpdateImage)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_MAKEBW,OnUpdateImage)
	ON_COMMAND_RANGE(ID_SIZE_HALF,ID_SIZE_FILL,OnChangeSize)
	ON_COMMAND(ID_TOOLS_HOMEWORKIII, &CChildView::OnToolsHomeworkIII)
	ON_COMMAND(ID_TOOLS_HOMEWORKII, &CChildView::OnToolsHomeworkII)
	ON_COMMAND(ID_HOMEWORKII_EROSION, &CChildView::OnHomeworkiiErosion)
	ON_COMMAND(ID_HOMEWORKII_DELATION, &CChildView::OnHomeworkiiDelation)
	ON_COMMAND(ID_HOMEWORKII_OPENING, &CChildView::OnHomeworkiiOpening)
	ON_COMMAND(ID_HOMEWORKII_CLOSING, &CChildView::OnHomeworkiiClosing)
	ON_COMMAND(ID_HOMEWORKII_BINARIZATION, &CChildView::OnHomeworkiiBinarization)
END_MESSAGE_MAP()



// CChildView message handlers
// �����еĴ󲿷ְ�ť��Ӧ���ֳɵĺ���
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

//���δ�����ÿһ����Ϣѭ���б���������һ����С������ʾ��ǰͼƬ
void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if (!imgOriginal.IsNull()) 
	{

		switch (m_nImageSize)
		{
			case SIZE_HALF:
				imgOriginal.StretchBlt(dc,0,0,imgOriginal.GetWidth()/2,imgOriginal.GetHeight()/2,SRCCOPY);
				break;
			case SIZE_ORIGINAL:
				imgOriginal.StretchBlt(dc,0,0,imgOriginal.GetWidth(),imgOriginal.GetHeight(),SRCCOPY);
				break;
			case SIZE_DOUBLE:
				imgOriginal.StretchBlt(dc,0,0,imgOriginal.GetWidth()*2,imgOriginal.GetHeight()*2,SRCCOPY);
				break;
			case SIZE_FILL:
				CRect rctWindowSize;
				GetClientRect(rctWindowSize);
				imgOriginal.StretchBlt(dc,0,0,rctWindowSize.Width(),rctWindowSize.Height(),SRCCOPY);
		};
	}

}

//���δ���ʵ�ִ��ļ��Ի���ѡ��һ��ͼƬ�ļ�����ʾ
void CChildView::OnFileOpenimage(void)
{
	// TODO: Add your command handler code here
	CString strFilter;
	CSimpleArray<GUID> aguidFileTypes;
	HRESULT hResult;

	hResult = imgOriginal.GetExporterFilterString(strFilter,aguidFileTypes);
	if (FAILED(hResult)) {
		CString fmt;
		fmt.Format("GetExporterFilter failed:\n%x - %s", hResult, _com_error(hResult).ErrorMessage());
		::AfxMessageBox(fmt);
		return;
	}

	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	dlg.m_ofn.nFilterIndex = m_nFilterLoad;
	hResult = (int)dlg.DoModal();
	if(FAILED(hResult)) {
		return;
	}

	m_nFilterLoad = dlg.m_ofn.nFilterIndex;
	imgOriginal.Destroy();
	hResult = imgOriginal.Load(dlg.GetPathName());
	if (FAILED(hResult)) {
		CString fmt;
		fmt.Format("Load image failed:\n%x - %s", hResult, _com_error(hResult).ErrorMessage());
		::AfxMessageBox(fmt);
		return;
	}

	m_nImageSize = SIZE_ORIGINAL;
	Invalidate();
	UpdateWindow();

}
//���δ���ʵ�ֽ���ǰ��ͼƬ�洢������
void CChildView::OnFileSaveImage(void) 
{
	CString strFilter;
	CSimpleArray<GUID> aguidFileTypes;
	HRESULT hResult;


	strFilter = "Bitmap image|*.bmp|JPEG image|*.jpg|GIF image|*.gif|PNG image|*.png||";

	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
	dlg.m_ofn.nFilterIndex = m_nFilterLoad;
	hResult = (int)dlg.DoModal();
	if (FAILED(hResult)) {
		return;
	}

    // Add the appropriate extension if the user didn't type one

	CString strFileName;
	CString strExtension;

	strFileName = dlg.m_ofn.lpstrFile;


	// add the file extension if the user didn't supply one
	if (dlg.m_ofn.nFileExtension == 0) 
	{
		switch (dlg.m_ofn.nFilterIndex)
		{
		case 1:
			strExtension = "bmp";
			break;
		case 2:
			strExtension = "jpg";
			break;
		case 3:
			strExtension = "gif";
			break;
		case 4:
			strExtension = "png";
			break;
		default:
			break;
		}

		strFileName = strFileName + '.' + strExtension;

	}

	// the extension on the file name will determine the file type that is saved
	hResult = imgOriginal.Save(strFileName);
	if (FAILED(hResult)) {
		CString fmt;
		fmt.Format("Save image failed:\n%x - %s", hResult, _com_error(hResult).ErrorMessage());
		::AfxMessageBox(fmt);
		return;
	}

}



//���δ����ѯͼ�������Ϣ
void CChildView::OnImageInfo()
{
	CInfoDlg infoDlg(imgOriginal);
	infoDlg.DoModal();
}

//���δ���ı�ͼ����ʾ��С
void CChildView::OnChangeSize(UINT nID)
{
	m_nImageSize = (SizesEnum)(nID - ID_SIZE_BASE);
	Invalidate();
	UpdateWindow();
}

//�����Ķδ����Բ�ͬ�Ĵ�С��ʾͼƬ
void CChildView::OnUpdateSizeHalf(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!imgOriginal.IsNull());
	pCmdUI->SetCheck((UINT)(m_nImageSize == SIZE_HALF));
}

void CChildView::OnUpdateSizeOriginal(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!imgOriginal.IsNull());
	pCmdUI->SetCheck((UINT)(m_nImageSize == SIZE_ORIGINAL));
}

void CChildView::OnUpdateSizeDouble(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!imgOriginal.IsNull());
	pCmdUI->SetCheck((UINT)(m_nImageSize == SIZE_DOUBLE));
}

void CChildView::OnUpdateSizeFill(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!imgOriginal.IsNull());
	pCmdUI->SetCheck((UINT)(m_nImageSize == SIZE_FILL));
}

void CChildView::OnUpdateImage(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!imgOriginal.IsNull());
}





