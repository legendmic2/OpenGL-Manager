// CListCtrlEx.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "CListCtrlEx.h"
#include "afxdialogex.h"


// CListCtrlEx 대화 상자

IMPLEMENT_DYNAMIC(CListCtrlEx, CDialogEx)

CListCtrlEx::CListCtrlEx(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_lbDown = false;
}

CListCtrlEx::~CListCtrlEx()
{
}

void CListCtrlEx::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CListCtrlEx 메시지 처리기


void CListCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_lbDown = true;
	Invalidate();
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CListCtrlEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_lbDown = false;
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CListCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_lbDown && ((nFlags & MK_LBUTTON) == 0)) {
		PostMessage(WM_LBUTTONUP,
			MAKEWPARAM(LOWORD(nFlags), HIWORD(nFlags)),
			MAKELPARAM(point.x, point.y));
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


bool CListCtrlEx::IsSelected(int index)
{
	// TODO: 여기에 구현 코드 추가.
	return false;
}


bool CListCtrlEx::SelectDropTarget(int item)
{
	// TODO: 여기에 구현 코드 추가.
	return false;
}
