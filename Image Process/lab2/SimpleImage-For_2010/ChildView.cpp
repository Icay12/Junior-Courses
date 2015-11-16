
//本框架最主要的文件

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
1.本程序框架是基于MFC开发的显示及处理图像的应用程序框架，同学们可以在此框架的基础上完成作业所需的功能代码。
2.程序中处理图像的类是微软MFC基础类库提供的CImage类，该类提供了读取和显示各种格式图像的函数，可以大大简化处
  理图像过程中的工作量，使大家把主要精力集中在作业所列的功能实现上。
3.每个作业对应着菜单上Tools下的一个菜单项，每个菜单项对应一个空白函数，用于实现每个作业的功能代码。
4.当你打开一幅图像以后，图像句柄被存储在CImage类型的imgOriginal对象中，这个CImage类的成员函数很强大，可以在MSDN
 （按F1打开）中查询有关它的操作。imgOriginal对象是本程序中CChildView的私有变量，可以在以下的成员函数中被调用。
  有关图像的一些基础操作都可用imgOriginal.函数名()的方法调用，比如打开一幅图像可以用imgOriginal.Load("x.jpg")，
  更多有用的操作请在MSDN中搜索CImage。
5.下面的CChildView::OnToolsMakeBW是一个简单的例子，它是菜单上Tools下的第一个菜单项对应的消息响应函数，把彩色图像
  转为灰度图。
------------------------------------------------------------------------------------------------------------*/
void CChildView::OnToolsMakeBW(void)
{
	CWaitCursor wait;//在函数执行过程中使鼠标图标变成等待图标

	if (!imgOriginal.IsIndexed()) //如果当前打开的图像的每个像素大于8位，即没有使用色彩索引表（大部分图像都属于这种）
	{
		//把每个像素从彩色转为灰度
		COLORREF pixel;//保存一个像素的色彩信息
		int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//从imgOriginal取得图像的长maxX和高maxY
		byte r,g,b,avg;//r,g,b值和平均灰度值变量
		for (int x=0; x<maxX; x++) //列循环
		{
			for (int y=0; y<maxY; y++) //行循环
			{
				pixel = imgOriginal.GetPixel(x,y);//取得位于(x,y)位置上的像素的色彩信息
				r = GetRValue(pixel);//取得pixel的r值
				g = GetGValue(pixel);//取得pixel的g值
				b = GetBValue(pixel);//取得pixel的b值
				avg = ((r + g + b)/3);//用此式将rgb彩色值转为灰度值
				imgOriginal.SetPixelRGB(x,y,avg,avg,avg);//将位于(x,y)位置上的像素的rgb色彩通道设为(avg,avg,avg)，即转换为灰度
			}
		}
	} 
	//下半部分的代码是处理图像的每个像素小于8位的，不再详细注解
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

	//刷新显示图像
	Invalidate();
	UpdateWindow();
}



/*-----------------------code for lab2----------------------------------
tasks:
1.Image binarization；
2.Binary image erosion
3.Binary image delation
4.Binary image opening
5.Binary image closing
6.Hit-or-miss transform (optional)
----------------------------------------------------------------------*/
void CChildView::OnHomeworkiiBinarization()
{
	CWaitCursor wait;//在函数执行过程中使鼠标图标变成等待图标
	COLORREF pixel;//保存一个像素的色彩信息
	int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//从imgOriginal取得图像的长maxX和高maxY
	int *pixelArray = new int[maxX*maxY];

	//Find the maximal and minimal pixel value in the image
	byte MAXavg = 0, MINavg = 255, temp;
	for (int x=0; x<maxX; x++) //列循环
	{
		for (int y=0; y<maxY; y++) //行循环
		{
			pixel = imgOriginal.GetPixel(x,y);//取得位于(x,y)位置上的像素的色彩信息
			temp = GetRValue(pixel);//取得pixel的r值
			MAXavg = MAXavg >= temp ? MAXavg:temp;
			MINavg = MINavg <= temp ? MINavg:temp;
			pixelArray[x*maxY+y] = temp;
		}
	}

	//Thresholdingthe image by “minimal value +1” to obtain the binary image;
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
	for (int x=0; x<maxX; x++) //列循环
	{
		for (int y=0; y<maxY; y++) //行循环
		{
			pixel = imgOriginal.GetPixel(x,y);//取得位于(x,y)位置上的像素的色彩信息
			r = GetRValue(pixel);//取得pixel的r值
			avg = (r >= threshold)? 255:0;
			imgOriginal.SetPixelRGB(x,y,avg,avg,avg);//将位于(x,y)位置上的像素的rgb色彩通道设为(avg,avg,avg)，即转换为灰度
		}
	}
	delete[] pixelArray;
	Invalidate();
	UpdateWindow();
}

void CChildView::erosion()
{
	CWaitCursor wait;//在函数执行过程中使鼠标图标变成等待图标
	COLORREF pixel;//保存一个像素的色彩信息
	int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//从imgOriginal取得图像的长maxX和高maxY
	int *pixelArray = new int[maxX*maxY];

	for (int x=0; x<maxX; x++) //列循环
	{
		for (int y=0; y<maxY; y++) //行循环
		{
			pixel = imgOriginal.GetPixel(x,y);//取得位于(x,y)位置上的像素的色彩信息
			pixelArray[x*maxY+y] = GetRValue(pixel);
		}
	}

	//使用3*3的结构元素腐蚀

	for (int x=1; x<maxX-1; x++) //列循环
	{
		for (int y=1; y<maxY-1; y++) //行循环
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
	CWaitCursor wait;//在函数执行过程中使鼠标图标变成等待图标
	COLORREF pixel;//保存一个像素的色彩信息
	int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//从imgOriginal取得图像的长maxX和高maxY
	int *pixelArray = new int[maxX*maxY];

	for (int x=0; x<maxX; x++) //列循环
	{
		for (int y=0; y<maxY; y++) //行循环
		{
			pixel = imgOriginal.GetPixel(x,y);//取得位于(x,y)位置上的像素的色彩信息
			pixelArray[x*maxY+y] = GetRValue(pixel);
		}
	}

	//使用3*3的结构元素膨胀
	for (int x=1; x<maxX-1; x++) //列循环
	{
		for (int y=1; y<maxY-1; y++) //行循环
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



//为菜单项添加消息响应函数并添加实现代码（细节参加ppt）
void CChildView::OnToolsHomeworkIII()
{
	// TODO: Add your command handler code here
	::AfxMessageBox("Hello World!");
}

/*-------------------------------------------------------------------------------------------------
以下代码是本基础框架包含的现成的函数，如打开、显示、存储图像文件的代码，同学们可以不去深入研究
--------------------------------------------------------------------------------------------------*/

// CChildView的构造函数和析构函数以及消息映射表(更详细的内容请参考C++及MFC的经典书籍)

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
// 程序中的大部分按钮对应的现成的函数
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

//本段代码在每一次消息循环中被调用来以一定大小比例显示当前图片
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

//本段代码实现打开文件对话框并选择一个图片文件并显示
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
//本段代码实现将当前的图片存储至本地
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



//本段代码查询图像相关信息
void CChildView::OnImageInfo()
{
	CInfoDlg infoDlg(imgOriginal);
	infoDlg.DoModal();
}

//本段代码改变图像显示大小
void CChildView::OnChangeSize(UINT nID)
{
	m_nImageSize = (SizesEnum)(nID - ID_SIZE_BASE);
	Invalidate();
	UpdateWindow();
}

//下面四段代码以不同的大小显示图片
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





