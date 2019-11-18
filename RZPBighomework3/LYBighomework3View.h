
// LYBighomework3View.h: CLYBighomework3View 类的接口
//

#pragma once
#include "CP3.h"
#include "CFace.h"


class CLYBighomework3View : public CView
{
protected: // 仅从序列化创建
	CLYBighomework3View() noexcept;
	DECLARE_DYNCREATE(CLYBighomework3View)

// 特性
public:
	CLYBighomework3Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CLYBighomework3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	double R, Theta, Phi, d;//视点在用户坐标系中的球坐标
	double k[9];
	CP3 ViewPoint;//视点球坐标位置
	CP2 ScreenP;//屏幕坐标系的二维坐标点
	CP3* V;//球面顶点一维数组
	CFace** F;//小面的二维数组
	int N1, N2;//N1为纬度区域,N2为经度区域
	double Alpha, Beta;//x方向旋转α角,y方向旋转β角

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	void ReadPoint();
	void ReadFace();
	void InitParameter();
	void PerProject(CP3);
	void DrawObject(CDC* pDC);
};

#ifndef _DEBUG  // LYBighomework3View.cpp 中的调试版本
inline CLYBighomework3Doc* CLYBighomework3View::GetDocument() const
   { return reinterpret_cast<CLYBighomework3Doc*>(m_pDocument); }
#endif

