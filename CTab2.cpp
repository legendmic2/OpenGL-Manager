// CTab2.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "CTab2.h"
#include "afxdialogex.h"


// CTab2 대화 상자

IMPLEMENT_DYNAMIC(CTab2, CDialogEx)

CTab2::CTab2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
	
}

CTab2::~CTab2()
{
}

void CTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK2, fog_check);
	DDX_Control(pDX, IDC_COMBO2, BKColorList);
	DDX_Control(pDX, IDC_BKNAME, BK_name);
	DDX_Control(pDX, IDC_CHECK1, m_light);
	DDX_Control(pDX, IDC_CHECK4, m_clight);
	DDX_Control(pDX, IDC_CHECK3, m_spot);
}


BEGIN_MESSAGE_MAP(CTab2, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK2, &CTab2::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK1, &CTab2::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK4, &CTab2::OnBnClickedCheck4)
END_MESSAGE_MAP()


// CTab2 메시지 처리기


void CTab2::OnBnClickedCheck2()
{
	
}


void CTab2::OnBnClickedCheck1()
{
	if (m_clight.GetCheck())
		m_clight.SetCheck(false);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTab2::OnBnClickedCheck4()
{
	if (m_light.GetCheck())
		m_light.SetCheck(false);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTab2::OnBnClickedCheck5()
{
	if (!m_clight.GetCheck())
		move_clight.SetCheck(false);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
