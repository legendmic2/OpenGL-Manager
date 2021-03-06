#pragma once


// CBKDlg 대화 상자

class CBKDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBKDlg)

public:
	CBKDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBKDlg();
	int bk_num;
	CFont m_font;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	CBitmapButton OceanButton;
	CBitmapButton Dessert_Button;
	CBitmapButton Moon_Button;
	CBitmapButton DefaultBK;
	afx_msg void OnBnClickedDefault();
	afx_msg void OnBnClickedOcean();
	afx_msg void OnBnClickedDesert();
	afx_msg void OnBnClickedMoon();
	virtual BOOL OnInitDialog();
};
