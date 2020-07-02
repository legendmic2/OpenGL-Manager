#pragma once


// CTab1 대화 상자

class CTab1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTab1)

public:
	CTab1(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTab1();
	int handle_st;
	int n;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_X;
	CEdit m_Y;
	CEdit m_Z;

	CStatic char_name;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	CEdit width;
	CEdit height;
	CEdit depth;
	CComboBox obj_paint;
	int clicked;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnCloseupCombo1();
	afx_msg void OnCbnDropdownCombo1();
	CButton move;
	CButton rotate;
	CButton resize;
};
