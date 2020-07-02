// CTab1.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "CTab1.h"
#include "afxdialogex.h"


// CTab1 대화 상자

IMPLEMENT_DYNAMIC(CTab1, CDialogEx)

CTab1::CTab1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, clicked(false)
{
	handle_st = 0;//핸들 스타일에 대한 변수(0일땐 기본, 1일땐 Rot,2일땐 Resize,3일땐 Move)
}

CTab1::~CTab1()
{
}

void CTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_X, m_X);
	DDX_Control(pDX, IDC_Y, m_Y);
	DDX_Control(pDX, IDC_Z, m_Z);
	DDX_Control(pDX, IDC_CHARNAME, char_name);
	DDX_Control(pDX, IDC_width, width);
	DDX_Control(pDX, IDC_HEIGHT, height);
	DDX_Control(pDX, IDC_DEPTH, depth);
	DDX_Control(pDX, IDC_COMBO1, obj_paint);
	DDX_CBIndex(pDX, IDC_COMBO1, clicked);
	DDX_Control(pDX, IDC_BUTTON1, move);
	DDX_Control(pDX, IDC_BUTTON3, rotate);
	DDX_Control(pDX, IDC_BUTTON2, resize);
}


BEGIN_MESSAGE_MAP(CTab1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTab1::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CTab1::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CTab1::OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CTab1::OnCbnSelchangeCombo1)
	ON_CBN_CLOSEUP(IDC_COMBO1, &CTab1::OnCbnCloseupCombo1)
	ON_CBN_DROPDOWN(IDC_COMBO1, &CTab1::OnCbnDropdownCombo1)
END_MESSAGE_MAP()


// CTab1 메시지 처리기


BOOL CTab1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//X,Y,Z축 초기화 (처음에 보여지는 값은 0)
	m_X.SetWindowText(_T("0"));
	m_Y.SetWindowText(_T("0"));
	m_Z.SetWindowText(_T("0"));

	//넓이 폭 높이 초기화(처음 보여지는 값은 0)
	GetDlgItem(IDC_width)->SetWindowText(_T("0"));
	GetDlgItem(IDC_HEIGHT)->SetWindowText(_T("0"));
	GetDlgItem(IDC_DEPTH)->SetWindowText(_T("0"));

	obj_paint.AddString(_T("Red"));	//paint 탭에 컬러 리스트 추가
	obj_paint.AddString(_T("Orange"));	//paint 탭에 컬러 리스트 추가
	obj_paint.AddString(_T("Yellow"));	//paint 탭에 컬러 리스트 추가
	obj_paint.AddString(_T("Green"));	//paint 탭에 컬러 리스트 추가
	obj_paint.AddString(_T("Blue"));	//paint 탭에 컬러 리스트 추가
	obj_paint.AddString(_T("Indigo"));	//paint 탭에 컬러 리스트 추가
	obj_paint.AddString(_T("Violet"));	//paint 탭에 컬러 리스트 추가
	obj_paint.AddString(_T("White"));	//paint 탭에 컬러 리스트 추가

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTab1::OnBnClickedButton1()	//default버튼 클릭했을때
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	handle_st = 0;
}


void CTab1::OnBnClickedButton3()	//rot버튼 클릭했을때
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	handle_st = 1;
}


void CTab1::OnBnClickedButton2()	//resize버튼 클릭했을때
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	handle_st = 2;
}



void CTab1::OnCbnSelchangeCombo1()
{
	//clicked = true;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTab1::OnCbnCloseupCombo1()
{
	clicked = false;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTab1::OnCbnDropdownCombo1()
{
	clicked = true;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
