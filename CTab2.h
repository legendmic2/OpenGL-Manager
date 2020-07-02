#pragma once


// CTab2 대화 상자

class CTab2 : public CDialogEx
{
	DECLARE_DYNAMIC(CTab2)

public:
	CTab2(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTab2();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CButton fog_check;
	afx_msg void OnBnClickedCheck2();
	CComboBox BKColorList;
	CStatic BK_name;
	afx_msg void OnBnClickedCheck1();
	CButton m_light;
	CButton m_clight;
	afx_msg void OnBnClickedCheck4();
	CButton m_spot;
	CButton move_clight;
	afx_msg void OnBnClickedCheck5();
};
