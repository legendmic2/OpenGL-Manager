#pragma once


// CListCtrlEx 대화 상자

class CListCtrlEx : public CDialogEx
{
	DECLARE_DYNAMIC(CListCtrlEx)

public:
	CListCtrlEx(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CListCtrlEx();

	bool m_lbDown;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	bool IsSelected(int index);
	bool SelectDropTarget(int item);
};
