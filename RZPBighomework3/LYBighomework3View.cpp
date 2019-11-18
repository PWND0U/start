
// LYBighomework3View.cpp: CLYBighomework3View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "LYBighomework3.h"
#endif

#include "LYBighomework3Doc.h"
#include "LYBighomework3View.h"

#include "CFace.h"
#include "CP3.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const double PI = 3.1415926;


// CLYBighomework3View

IMPLEMENT_DYNCREATE(CLYBighomework3View, CView)

BEGIN_MESSAGE_MAP(CLYBighomework3View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CLYBighomework3View 构造/析构

CLYBighomework3View::CLYBighomework3View() noexcept
{
	// TODO: 在此处添加构造代码
	V = NULL; 
	F = NULL; 
	R = 1000;
	d = 1000;
	Phi = 30;
	Theta = 30;
}

CLYBighomework3View::~CLYBighomework3View()
{
	if (V != NULL)
	{
		delete[] V;
		V = NULL;
	}
	for (int n = 0; n < N1; n++)//注意撤销次序,先列后行,与设置相反
	{
		delete[] F[n];
		F[n] = NULL;
	}
	delete[] F;
	F = NULL;
}

BOOL CLYBighomework3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CLYBighomework3View 绘图

void CLYBighomework3View::OnDraw(CDC* pDC)
{
	CLYBighomework3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect rect;//定义客户区
	GetClientRect(&rect);//获得客户区的大小
	pDC->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
	pDC->SetWindowExt(rect.Width(), rect.Height());//设置窗口范围
	pDC->SetViewportExt(rect.Width(), -rect.Height());//x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);//屏幕中心为原点
	ReadPoint();
	ReadFace();
	InitParameter();
	DrawObject(pDC);
	// TODO: 在此处为本机数据添加绘制代码
}


// CLYBighomework3View 打印

BOOL CLYBighomework3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CLYBighomework3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CLYBighomework3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CLYBighomework3View 诊断

#ifdef _DEBUG
void CLYBighomework3View::AssertValid() const
{
	CView::AssertValid();
}

void CLYBighomework3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLYBighomework3Doc* CLYBighomework3View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLYBighomework3Doc)));
	return (CLYBighomework3Doc*)m_pDocument;
}
#endif //_DEBUG

void CLYBighomework3View::ReadPoint()
{
	// TODO: 在此处添加实现代码.
	double gAlpha = 1, gBeta = 1;//面片夹角
	N1 = 180 / gAlpha, N2 = 360 / gBeta;//N1为纬度区域,N2为经度区域
	V = new CP3[(N1 - 1) * N2 + 2];//V为球的顶点
	//纬度方向除南北极点外有"N1－1"个点，"2"代表南北极两个点
	double gAlpha1, gBeta1, r = 300;//r为球体半径
	//计算北极点坐标
	V[0].x = 0, V[0].y = r, V[0].z = 0;
	//按行循环计算球体上的点坐标
	for (int i = 0; i < N1 - 1; i++)
	{
		gAlpha1 = (i + 1) * gAlpha * PI / 180;
		for (int j = 0; j < N2; j++)
		{
			gBeta1 = j * gBeta * PI / 180;
			V[i * N2 + j + 1].x = r * sin(gAlpha1) * sin(gBeta1);
			V[i * N2 + j + 1].y = r * cos(gAlpha1);
			V[i * N2 + j + 1].z = r * sin(gAlpha1) * cos(gBeta1);
		}
	}
	//计算南极点坐标
	V[(N1 - 1) * N2 + 1].x = 0, V[(N1 - 1) * N2 + 1].y = -r, V[(N1 - 1) * N2 + 1].z = 0;
}

void CLYBighomework3View::ReadFace()
{
	// TODO: 在此处添加实现代码.
	F = new CFace *[N1];//设置行
	for (int n = 0; n < N1; n++)
	{
		F[n] = new CFace[N2];//设置列
	}
	for (int j = 0; j < N2; j++)//构造北极三角形面片
	{
		int tempj = j + 1;
		if (tempj == N2) tempj = 0;//面片的首尾连接
		int NorthIndex[3];//北极三角形面片索引号数组
		NorthIndex[0] = 0;
		NorthIndex[1] = j + 1;
		NorthIndex[2] = tempj + 1;
		F[0][j].SetNum(3);
		for (int k = 0; k < F[0][j].vN; k++)
		{
			F[0][j].vI[k] = NorthIndex[k];
		}
	}
	for (int i = 1; i < N1 - 1; i++)//构造球面四边形面片
	{
		for (int j = 0; j < N2; j++)
		{
			int tempi = i + 1;
			int tempj = j + 1;
			if (tempj == N2) tempj = 0;
			int BodyIndex[4];//球面四边形面片索引号数组
			BodyIndex[0] = (i - 1) * N2 + j + 1;
			BodyIndex[1] = (tempi - 1) * N2 + j + 1;
			BodyIndex[2] = (tempi - 1) * N2 + tempj + 1;
			BodyIndex[3] = (i - 1) * N2 + tempj + 1;
			F[i][j].SetNum(4);
			for (int k = 0; k < F[i][j].vN; k++)
			{
				F[i][j].vI[k] = BodyIndex[k];
			}
		}
	}
	for (int j = 0; j < N2; j++)//构造南极三角形面片
	{
		int tempj = j + 1;
		if (tempj == N2) tempj = 0;
		int SouthIndex[3];//南极三角形面片索引号数组
		SouthIndex[0] = (N1 - 2) * N2 + j + 1;
		SouthIndex[1] = (N1 - 1) * N2 + 1;
		SouthIndex[2] = (N1 - 2) * N2 + tempj + 1;
		F[N1 - 1][j].SetNum(3);
		for (int k = 0; k < F[N1 - 1][j].vN; k++)
		{
			F[N1 - 1][j].vI[k] = SouthIndex[k];
		}
	}
}

void CLYBighomework3View::InitParameter()
{
	// TODO: 在此处添加实现代码.
	k[1] = sin(PI * Theta / 180);
	k[2] = sin(PI * Phi / 180);
	k[3] = cos(PI * Theta / 180);
	k[4] = cos(PI * Phi / 180);
	k[5] = k[2] * k[3];
	k[6] = k[2] * k[1];
	k[7] = k[4] * k[3];
	k[8] = k[4] * k[1];
	ViewPoint.x = R * k[6];
	ViewPoint.y = R * k[4];
	ViewPoint.z = R * k[5];
}

void CLYBighomework3View::PerProject(CP3 P)
{
	// TODO: 在此处添加实现代码.
	CP3 ViewP;
	ViewP.x = P.x * k[3] - P.z * k[1];//观察坐标系三维坐标
	ViewP.y = -P.x * k[8] + P.y * k[2] - P.z * k[7];
	ViewP.z = -P.x * k[6] - P.y * k[4] - P.z * k[5] + R;
	ScreenP.x = d * ViewP.x / ViewP.z;//屏幕二维坐标系
	ScreenP.y = d * ViewP.y / ViewP.z;
}

void CLYBighomework3View::DrawObject(CDC* pDC)
{
	// TODO: 在此处添加实现代码.
	CPen NewPen, *pOldPen;
	NewPen.CreatePen(PS_SOLID, 10, RGB(0, 255, 0));
	pOldPen = pDC->SelectObject(&NewPen);
	CP2 Point3[3], t3;//南北极顶点数组
	CP2 Point4[4], t4;//球体顶点数组
	for (int i = 0; i < N1; i++)
	{
		for (int j = 0; j < N2; j++)
		{
			CVector ViewVector(V[F[i][j].vI[0]], ViewPoint);//面的视矢量
			ViewVector = ViewVector.Normalize();//单位化视矢量
			F[i][j].SetFaceNormal(V[F[i][j].vI[0]], V[F[i][j].vI[1]], V[F[i][j].vI[2]]);
			F[i][j].fNormal.Normalize();//单位化法矢量
			if (Dot(ViewVector, F[i][j].fNormal) >= 0)//背面剔除
			{
				if (3 == F[i][j].vN)//三角形面片
				{
					for (int m = 0; m < F[i][j].vN; m++)
					{
						PerProject(V[F[i][j].vI[m]]);
						Point3[m] = ScreenP;
					}
					for (int n = 0; n < 3; n++)
					{
						if (0 == n)
						{
							pDC->MoveTo( Point3[n].x,Point3[n].y);
							t3 = Point3[n];
						}
						else
							pDC->LineTo(Point3[n].x, Point3[n].y);
					}
					pDC->LineTo(t3.x,t3.y);//闭合多边形
				}
				else//四边形面片
				{
					for (int m = 0; m < F[i][j].vN; m++)
					{
						PerProject(V[F[i][j].vI[m]]);
						Point4[m] = ScreenP;
					}
					for (int n = 0; n < 4; n++)
					{
						if (0 == n)
						{
							pDC->MoveTo(Point4[n].x,Point4[n].y);
							t4 = Point4[n];
						}
						else
							pDC->LineTo(Point4[n].x, Point4[n].y);
					}
					pDC->LineTo(t4.x,t4.y);//闭合多边形
				}
			}
		}
	}
	pDC->SelectObject(pOldPen);
}
// CLYBighomework3View 消息处理程序
