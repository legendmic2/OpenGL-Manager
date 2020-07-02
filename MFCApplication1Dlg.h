#define BITMAP_ID 0x4D42		// the universal bitmap ID
////// Types
typedef struct
{
	int width;					// width of texture
	int height;					// height of texture
	unsigned int texID;			// the texture object id of this texture
	unsigned char *data;		// the texture data
} texture_t;



// MFCApplication1Dlg.h: 헤더 파일
//

#pragma once
class CTab1;
class CTab2;
class CListCtrlEx;
class CBKDlg;

#include "./include/GL/glew.h"
#include "./include/GL/wglew.h"
#include <gl/GL.h>
#include <gl\GLU.h>
#include <gl/glut.h>


#include "main.h"
#include "Obj.h"
#include "my_obj.h"
#include "objloader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



#pragma comment(lib, "OpenGL32.Lib")
#pragma comment(lib, "./lib/glew32.lib")

// CMFCApplication1Dlg 대화 상자
class CMFCApplication1Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	CTab1 *pDlg1;
	CTab2 *pDlg2;
	CBKDlg *pDlg3;

	CListCtrlEx *test;

	HGLRC m_hRC;
	CDC* m_pDC;


	CImageList* m_pImgListDrag;
	int item_num;
	
	bool m_light = true;
	bool m_spot = false;
	bool m_clight = false;

	//OpenGL Setup
	BOOL GetRenderingContext();
	//Rendering Context and Device Context Pointers
	
	float angle;

	
	//배경을 위한 변수
	texture_t *envTex;				// environment map
	texture_t *floorTex;			// floor texture

	texture_t *envTex2;				// environment map
	texture_t *floorTex2;			// floor texture

	texture_t *envTex3;				// environment map
	texture_t *floorTex3;			// floor texture

	texture_t *envTex4;				// environment map
	texture_t *floorTex4;			// floor texture
	float bkcolor[4];	//배경을 위한 색
	int which_bk;	//무슨 bk가 선택됐는지 받아오는 변수

	CBitmap hBitmap;
	//////////////////// 장군우 추가 /////////////////////

	bool m_bDrag;

	GLfloat theta;

	float m_xTran, m_yTran, m_zTran;
	float m_xcTran, m_ycTran, m_zcTran;
	float m_xRot, m_yRot;
	float m_sRot, m_sTran, m_sScl; // speed

	bool m_Lmouse, m_Rmouse;
	CPoint m_oldPos;
	CPoint WinPos;
	float WinPosZ;

	struct objStateStruct {
		// obj[i]의 y축 Rot값
		float Rot = 0;
		// obj[i]의 scale값
		float xScl = 1; 
		float yScl = 1;
		float zScl = 1;
		// obj[i]의 size값
		float xSize = 1;
		float ySize = 1;
		float zSize = 1;
	};
	
	objStateStruct objState[6];

	//서동우 추가 한 부분
	vector < GLfloat > k;
	vector < GLfloat > y;
	vector < GLfloat > z;
	bool checked = false;
	//object 로드 하기


	my_obj objects[6];





// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	virtual BOOL GetOldStyleRenderingContext(void);

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_list;
	CStatic m_picture;
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void GLMakeScene();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBegindragList1(NMHDR *pNMHDR, LRESULT *pResult);
	void OnBegindrag(CListCtrl *list, NMHDR *pNMHDR);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	
	
	////////////// OpenGL Scene Function //////////////
	//void DrawCube(float x, float y, float z, float Rot, float Scl, bool isMouseDragOnGL); // 11/30 Scl 매개변수 추가
//	void DrawCube(float x, float y, float z, objStateStruct objState, bool isMouseDragOnGL);
	void ConvertWinToWorld(float x_cursor, float y_cursor);
	bool isNear(float wx, float wy, float wz);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void DrawPlane();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void DrawSphere(int slices, int stacks, float radius);
	unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);
	texture_t *LoadTextureFile(char *filename);
	bool LoadAllTextures();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBackgroundBk();
//	void SpotLightFunc();
	BOOL CreateTexture(GLuint &textureID, LPTSTR szFileName);
	void render(float x, float y, float z, objStateStruct objState, bool isMouseDragOnGL, int k);
	void DrawFramework(float xPos, float zPos, float objSize[3]);
	afx_msg void OnCbnSelchangeCombo1();
};
