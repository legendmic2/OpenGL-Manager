// CBKDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "CBKDlg.h"
#include "afxdialogex.h"


// CBKDlg 대화 상자

IMPLEMENT_DYNAMIC(CBKDlg, CDialogEx)

CBKDlg::CBKDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{
	

	
}

CBKDlg::~CBKDlg()
{
}

void CBKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OCEAN, OceanButton);
	DDX_Control(pDX, IDC_DESERT, Dessert_Button);
	DDX_Control(pDX, IDC_MOON, Moon_Button);
	DDX_Control(pDX, IDC_DEFAULT, DefaultBK);
}


BEGIN_MESSAGE_MAP(CBKDlg, CDialogEx)
	ON_BN_CLICKED(IDC_DEFAULT, &CBKDlg::OnBnClickedDefault)
	ON_BN_CLICKED(IDC_OCEAN, &CBKDlg::OnBnClickedOcean)
	ON_BN_CLICKED(IDC_DESERT, &CBKDlg::OnBnClickedDesert)
	ON_BN_CLICKED(IDC_MOON, &CBKDlg::OnBnClickedMoon)
END_MESSAGE_MAP()


// CBKDlg 메시지 처리기


void CBKDlg::OnBnClickedDefault()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox(_T("기본 화면입니다"), _T("선택"));
	bk_num = 0;
	OnOK();
}


void CBKDlg::OnBnClickedOcean()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox(_T("바다 화면입니다"),_T("선택"));
	bk_num = 1;
	OnOK();
}


void CBKDlg::OnBnClickedDesert()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox(_T("사막 화면입니다"), _T("선택"));
	bk_num = 2;
	OnOK();
}


void CBKDlg::OnBnClickedMoon()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox(_T("달 화면입니다"), _T("선택"));
	bk_num = 3;
	OnOK();
}


BOOL CBKDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	DefaultBK.LoadBitmaps(IDB_DEFAULT, NULL, NULL, NULL);
	DefaultBK.SizeToContent();

	OceanButton.LoadBitmaps(IDB_OCEAN, NULL, NULL, NULL);
	OceanButton.SizeToContent();

	Dessert_Button.LoadBitmaps(IDB_DESERT, NULL, NULL, NULL);
	Dessert_Button.SizeToContent();

	Moon_Button.LoadBitmaps(IDB_MOON, NULL, NULL, NULL);
	Moon_Button.SizeToContent();

	//http://starfactory.tistory.com/85
	m_font.CreateFont(50, // nHeight 
		10, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		1, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		DEFAULT_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		_T("굴림")); // lpszFacename 

	GetDlgItem(IDC_STATIC)->SetFont(&m_font);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
