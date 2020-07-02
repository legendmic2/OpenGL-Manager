
// MFCApplication1Dlg.cpp: 구현 파일
//


// 오브젝트 순서: 1번 괴물, 2번: 발, 3번: 큐브, 4번: 미라, 5번: 닌자 펭귄, 6번: 원
// 텍스처의 성능을 향상하기 위해 새로운 라이브러리인 soil을 추가시켰습니다! (PNG로드 용)
#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "targa.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include"CTab1.h"
#include "CTab2.h"
#include "CListCtrlEx.h"
#include "CBKDlg.h"
#include "my_obj.h"
#include "texture.h"
#include <algorithm>
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

float objcolor[3] = { 1,1,1 };
float obj_opacity = 1;
int handle_style = 0;	//핸들 스타일에 대한 변수(0일땐 기본, 1일땐 Rot,2일땐 Resize,3일땐 Move)

///////////////////////////////////////////////////// 장군우 추가 ///////////////////////////////////////////////////// 
const int objNum = 6;
// cube의 center position							-> 드래그 된 좌표로 Position이 할당될 것이므로 초기값은 모두 0
float xPos[objNum] = {};
// float yPos[objNum] = { 0.5, 0.5, 0.5, -1, -1, 0.5}; // 처음 load될 때부터 공중에 떠있는 애들이 있어서 y값을 조절했음
float yPos[objNum] = { -20, -20, -20, -20, -20, -20 }; // 처음 load될 때부터 공중에 떠있는 애들이 있어서 y값을 조절했음
float zPos[objNum] = {};
float planeSize = 50;

// 0 : 오우거 // 1 : 발바닥뼉따구 // 2 : 큐브 // 3 : 미이라 // 4 : 펭귄 // 5 : 트램펄린?
float objDefSize[6][3] = { {4, 7 ,6}, {2, 3 ,10}, {2, 2 ,2}, {6, 4 ,2}, {2, 4 ,2}, {10, 1 ,10} }; // 초기값 저장용
float objSize[6][3] = { {4, 7 ,6}, {2, 3 ,10}, {2, 2 ,2}, {6, 4 ,2}, {2, 4 ,2}, {10, 1 ,10} }; // {x축너비, y축너비, z축너비}

int clickedIdx = -1;  // 클릭된 object의 idx
bool isClicked = false; // Object가 클릭된 경우
bool isMouseDragOnGL = false; // 마우스가 OpenGL 화면 내부로 드래그되었는가
bool isObjDragOnGL[objNum] = {}; // 해당 object가 OpenGL화면으로 드래그되었는가
bool isObjLoaded[objNum] = {}; // 해당 object가 OpenGL 화면에 드래그를 통해 Load 완료되었는가
int colorclickedIdx = -1;

GLdouble wx, wy, wz = 0; // world-axis

////////////////test//////////////////////
int cnt = 0;

////////////////// 1202 For Test /////////////////////

float PerspAngle = 30.0; // Wheel Depth Control

float planeEdge = 5.0; // edge size for Setting position limit

float ambientLight[] = { 0.f, 0.f, 0.5f, 1.f };
float diffuseLight[] = { 0.5f, 0.5f, 0.5f, 1.f };
float specularLight[] = { 1, 1, 1, 1 };

float matAmbient[] = { .2f, .2f, .2f, 1.0f }; // 반사하는 양이 적으니 dark하게
float matDiff[] = { .8f, .8f, .8f, 1.f };
float matSpec[] = { 1.f, 1.f, 1.f, 1.f };
// float lightPosition[] = { planeSize, 10.f, planeSize, 1.0 };
float lightPosition[] = { 60.f, 50.f, 80.f, 0.8 };
float lightDirection[] = { 0, -1, 0, 1 };

float SpotambientLight[] = { 1.f, 1.f, 0.f, 1.f };
float SpotdiffuseLight[] = { 1.f, 1.f, 1.f, 1.f };
float spotlightDir[] = { 1, 1, 1 };
float spotPosition[] = { 5.f, 0.5f, -5.f, 1.0 }; // 나중에 정해줄 것

//라이트 추가 부분
float matdAmbient[] = { .2f, .2f, .2f, 1.0f };
float matdSpec[] = { 0.f, 0.f, 0.f, 1.f };
float matdDiff[] = { .8f, .8f, .8f, 1.f };
float shine = 0.f;


////////////////DongWoo//////////////////////

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 대화 상자



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	////////////////////// 장군우 추가 ///////////////////////

	theta = 0.0;
	m_xTran = m_yTran = m_zTran = m_xRot = m_yRot = 0;
	m_xTran = m_ycTran = m_zcTran = 0;
	m_sRot = 0.5f;
	m_sScl = 0.1f;
	m_sTran = 0.03f;
	m_Lmouse = m_Rmouse = false;

	CPoint WinPos = {}; // 윈도우의 x좌표, y좌표
	WinPosZ = 0; // ReadPixel로 구할 윈도우의Z좌표(깊이를 부여)
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMFCApplication1Dlg::OnTcnSelchangeTab1)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CMFCApplication1Dlg::OnNMDblclkList1)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST1, &CMFCApplication1Dlg::OnLvnBegindragList1)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_COMMAND(ID_BACKGROUND_BK, &CMFCApplication1Dlg::OnBackgroundBk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMFCApplication1Dlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	

	//탭 추가 code
	m_tab.InsertItem(0, _T("캐릭터"));		//캐릭터 탭 추가(캐릭터 탭은 유지, 캐릭터 선택이 바뀔때마다 값만 바뀜(탭은 유지)
	m_tab.InsertItem(1, _T("배경"));		//배경 탭 추가(디폴트 탭)

	m_tab.SetCurSel(1);	//처음에는 배경 탭 보여줌

	CRect rect;
	m_tab.GetWindowRect(&rect);
	
	//탭과 Dialog box를 연결
	pDlg1 = new CTab1;
	pDlg1->Create(IDD_DIALOG1, &m_tab);
	pDlg1->MoveWindow(0, 25, rect.Width(), rect.Height());
	pDlg1->ShowWindow(SW_HIDE);

	pDlg2 = new CTab2;
	pDlg2->Create(IDD_DIALOG2, &m_tab);
	pDlg2->MoveWindow(0, 25, rect.Width(), rect.Height());
	pDlg2->ShowWindow(SW_SHOW);

	//메뉴바와 다이어로그 박스 연결
	CMenu menu;
	menu.LoadMenuW(IDR_MENU1);
	SetMenu(&menu);

	/*w.nhn?blogId=chollikyo&logNo=10169470194&proxyReferer=https%3A
	%2F%2Fwww.google.co.kr%2F*/
	//리스트 컨트롤에 아이콘(icon)추가
	SetIcon(m_hIcon, TRUE);	//큰 아이콘을 설정합니다.

	//큰 아이콘 이미지 리스트 생성
	//첫번째 아이콘 생성
	static CImageList imgList;	//static필수!!!!
// 오브젝트 순서: 1번 괴물, 2번: 발, 3번: 큐브, 4번: 미라, 5번: 닌자 펭귄, 6번: 원
	imgList.Create(60, 60, ILC_COLOR8, 1, 1);
	imgList.Add(AfxGetApp()->LoadIconW(IDI_BEAST));	//0번 아이콘 추가
	imgList.Add(AfxGetApp()->LoadIconW(IDI_FOOT));	//1번 아이콘 추가
	imgList.Add(AfxGetApp()->LoadIconW(IDI_CUBE));	//2번 아이콘 추가
	imgList.Add(AfxGetApp()->LoadIconW(IDI_MUMMY));	//2번 아이콘 추가
	imgList.Add(AfxGetApp()->LoadIconW(IDI_PENGUIN));	//2번 아이콘 추가
	imgList.Add(AfxGetApp()->LoadIconW(IDI_TRAMPOLINE));	//2번 아이콘 추가


	//이미지 리스트를 리스트 컨트롤에 설정
	
	m_list.SetImageList(&imgList, LVSIL_NORMAL);
	
	// 오브젝트 순서: 1번 괴물, 2번: 발, 3번: 큐브, 4번: 미라, 5번: 닌자 펭귄, 6번: 원
	m_list.InsertItem(0, _T("Beast"),0);	//아이템 추가 0번 아이콘으로 설정
	m_list.InsertItem(1, _T("Foot bone"),1);	//아이템 추가 1번 아이콘으로 설정
	m_list.InsertItem(2, _T("Dice cube"),2);	//아이템 추가 2번 아이콘으로 설정
	m_list.InsertItem(3, _T("Mummy"),3);	//아이템 추가
	m_list.InsertItem(4, _T("ninJAPANguin"),4);	//아이템 추가
	m_list.InsertItem(5, _T("Trampoline"),5);	//아이템 추가
	/*총 6개의 아이템 추가*/
	
	m_bDrag = false;	//초기에는 아이템 드래그를 안하므로 false

	pDlg2->fog_check.SetCheck(false);	//초기에는 fog 없게
	pDlg2->m_light.SetCheck(true);	//초기에는 디폴트 light
	pDlg2->m_clight.SetCheck(false);	//초기에는 커스텀 light없이
	pDlg2->m_spot.SetCheck(false);	//초기에는 spcular light없이


	pDlg2->BKColorList.AddString(_T("Red"));	//배경 탭에 컬러 리스트 추가
	pDlg2->BKColorList.AddString(_T("Orange"));	//배경 탭에 컬러 리스트 추가
	pDlg2->BKColorList.AddString(_T("Yellow"));	//배경 탭에 컬러 리스트 추가
	pDlg2->BKColorList.AddString(_T("Green"));	//배경 탭에 컬러 리스트 추가
	pDlg2->BKColorList.AddString(_T("Blue"));	//배경 탭에 컬러 리스트 추가
	pDlg2->BKColorList.AddString(_T("Indigo"));	//배경 탭에 컬러 리스트 추가
	pDlg2->BKColorList.AddString(_T("Violet"));	//배경 탭에 컬러 리스트 추가
	pDlg2->BKColorList.AddString(_T("White"));	//배경 탭에 컬러 리스트 추가

	//초기 배경색 = 하얀색(하얀색일때 텍스쳐 색이 제대로 나옴!!!!)
	bkcolor[0] = 1;
	bkcolor[1] = 1;
	bkcolor[2] = 1;
	bkcolor[3] = 0.1;//약간 투명하게

	which_bk = 0;	//처음에는 기본배경화면
	
	pDlg1->char_name.SetWindowText(_T("치킨"));

	
	//OpenGL context 생성
	if (!GetRenderingContext())
	{
		AfxMessageBox(CString("OpenGL 초기화중 에러가 발생하여 프로그램을 실행할 수 없습니다."));
		return -1;
	}

	angle = 0;
	SetTimer(1000, 30, NULL);

	//서동우 추가부분.
	//obj 로드 미리:


	//1번 괴물
			loadOBJ("PrimalBeast.obj", objects[0].vertices, objects[0].uvs, objects[0].normals, k, y, z);
			objects[0].axas = loadPNG("PrimalBeast_Dif.bmp");//CreateTexture(g_Texture[i], Conversion);
			
	//2번 발
			loadOBJ("Foot.obj", objects[1].vertices, objects[1].uvs, objects[1].normals, k, y, z);
			objects[1].axas = loadBMP_custom("bone.bmp");//CreateTexture(g_Texture[i], Conversion);

	//3번 큐브
			loadOBJ("cube.obj", objects[2].vertices, objects[2].uvs, objects[2].normals, k, y, z);
			objects[2].axas = loadDDS("uvmap.DDS");//CreateTexture(g_Texture[i], Conversion);
	
	//4번 미라
			loadOBJ("Mummy.obj", objects[3].vertices, objects[3].uvs, objects[3].normals, k, y, z);
			objects[3].axas = loadPNG("parkwork_mummy_bmp.png");//CreateTexture(g_Texture[i], Conversion);

	//5번 펭귄
			loadOBJ("Ninja Penguin.obj", objects[4].vertices, objects[4].uvs, objects[4].normals, k, y, z);
			objects[4].axas = loadPNG("Ninja_Penguin_bmp.png");//CreateTexture(g_Texture[i], Conversion);

	//6번 원
			loadOBJ("MissionMarker.obj", objects[5].vertices, objects[5].uvs, objects[5].normals, k, y, z);
			objects[5].axas = loadPNG("target00.png");//CreateTexture(g_Texture[i], Conversion);



	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication1Dlg::OnPaint()
{
	LoadAllTextures();
		CDialogEx::OnPaint();
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int sel = m_tab.GetCurSel();

	switch (sel) {
	case 0:
		pDlg1->ShowWindow(SW_SHOW);
		pDlg2->ShowWindow(SW_HIDE);
		break;
	case 1:
		pDlg2->ShowWindow(SW_SHOW);
		pDlg1->ShowWindow(SW_HIDE);
		break;
	}
	*pResult = 0;
}


void CMFCApplication1Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (FALSE == ::wglDeleteContext(m_hRC))
	{
		AfxMessageBox(CString("wglDeleteContext failed"));
	}
}


void CMFCApplication1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 추가본
	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);
	glLoadIdentity();

	// 밑 부분은 LIGHT0이 겹쳐서 일단 주석처리
	
	glDisable(GL_LIGHT0);								// Turn on a light with defaults set
	glDisable(GL_LIGHTING);								// Turn on lighting
	glDisable(GL_COLOR_MATERIAL);						// Allow color
	
	////////////////////////핸들 스타일 번호 받아옴////////////////////////
	handle_style = pDlg1->handle_st;	//캐릭터 탭 다이얼로그에서 번호 받아옴

	GLMakeScene();	//Scene을 이루는 함수
	obj_opacity = (float)(1 - (float)pDlg1->n / 100);
	//화면 업데이트
	SwapBuffers(m_pDC->GetSafeHdc());
	CDialogEx::OnTimer(nIDEvent);
}

BOOL CMFCApplication1Dlg::GetRenderingContext()
{
	//픽처 컨트롤에만 그리도록 DC 생성
	//참고 https://goo.gl/CK36zE
	//glsl
	CWnd* pImage = GetDlgItem(IDC_PICTURE);
	CRect rc;
	pImage->GetWindowRect(rc);
	m_pDC = pImage->GetDC();


	if (NULL == m_pDC)
	{
		AfxMessageBox(CString("Unable to get a DC"));
		return FALSE;
	}


	if (!GetOldStyleRenderingContext())
	{
		return TRUE;
	}


	int pixelFormat;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size
		1,                          // version
		PFD_SUPPORT_OPENGL |        // OpenGL window
		PFD_DRAW_TO_WINDOW |        // render to window
		PFD_DOUBLEBUFFER,           // support double-buffering
		PFD_TYPE_RGBA,              // color type
		32,                         // prefered color depth
		0, 0, 0, 0, 0, 0,           // color bits (ignored)
		0,                          // no alpha buffer
		0,                          // alpha bits (ignored)
		0,                          // no accumulation buffer
		0, 0, 0, 0,                 // accum bits (ignored)
		16,                         // depth buffer
		0,                          // no stencil buffer
		0,                          // no auxiliary buffers
		PFD_MAIN_PLANE,             // main layer
		0,                          // reserved
		0, 0, 0,                    // no layer, visible, damage masks
	};



	//Get access to modern OpenGL functionality from this old style context.
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		AfxMessageBox(CString("GLEW could not be initialized!"));
		return FALSE;
	}


	//참고 http://gamedev.stackexchange.com/a/30443
	GLint attribs[] =
	{
		//OpenGL 2.0 사용
		WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
		WGL_CONTEXT_MINOR_VERSION_ARB, 0,
		// Uncomment this for forward compatibility mode
		//WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		// Uncomment this for Compatibility profile
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		// We are using Core profile here
		//WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};



	HGLRC CompHRC = wglCreateContextAttribsARB(m_pDC->GetSafeHdc(), 0, attribs);
	if (CompHRC && wglMakeCurrent(m_pDC->GetSafeHdc(), CompHRC))
		m_hRC = CompHRC;

	return TRUE;
}

BOOL CMFCApplication1Dlg::GetOldStyleRenderingContext()
{
	//A generic pixel format descriptor. This will be replaced with a more
	//specific and modern one later, so don't worry about it too much.
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |            // support OpenGL
		PFD_DOUBLEBUFFER,               // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		32,                             // 32-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                        // 24-bit z-buffer						// 동우씨랑 얘기하다가 이거 바꿨습니다
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	
	// Get the id number for the best match supported by the hardware device context
	// to what is described in pfd
	CClientDC dc(this); // 추가
	int pixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
	
	// 추가
	BOOL success = SetPixelFormat(dc.m_hDC, pixelFormat, &pfd);
	m_hRC = wglCreateContext(dc.m_hDC);  // Create RC compatible with the given DC
	wglMakeCurrent(dc.m_hDC, m_hRC); // Make the RC current (bind RC to DC)
	


	//If there's no match, report an error
	if (0 == pixelFormat)
	{
		AfxMessageBox(CString("ChoosePixelFormat failed"));
		return FALSE;
	}

	//If there is an acceptable match, set it as the current 
	if (FALSE == SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd))
	{
		AfxMessageBox(CString("SetPixelFormat failed"));
		return FALSE;
	}

	//Create a context with this pixel format
	if (0 == (m_hRC = wglCreateContext(m_pDC->GetSafeHdc())))
	{
		AfxMessageBox(CString("wglCreateContext failed"));
		return FALSE;
	}

	//Make it current. 
	if (FALSE == wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC))
	{
		AfxMessageBox(CString("wglMakeCurrent failed"));
		return FALSE;
	}
	return TRUE;
}

void CMFCApplication1Dlg::GLMakeScene()	// OpenGL Scene 그림 그리는곳
{
	// TODO: 여기에 구현 코드 추가.

	glViewport(0, 0, 1000, 500); // 동우씨와 해봤을 때 이 사이즈가 가장 적절했습니다 -> cx = 1000 // cy = 500

	glMatrixMode(GL_PROJECTION); // change the Matrix Mode
	glLoadIdentity();

	gluPerspective(PerspAngle, 1.0f*1000 /500, 1.0f, 100.0f); // cx, cy를 상수로 집어넣음


	gluLookAt(0, 5, 45, 0, 0, 0, 0, 3, 1); // 멀리서 보기위해 좀 떨어뜨려놓았음

	glMatrixMode(GL_MODELVIEW);  // go back to the default
	glLoadIdentity();

	glTranslatef(m_xTran, m_yTran, m_zTran); // 마우스 우클릭 & 휠로 카메라 이동

	glRotatef(m_xRot, 1, 0, 0); // 마우스 좌클릭드래그로 카메라 이동
	glRotatef(m_yRot, 0, 1, 0);

	if (isMouseDragOnGL) {
		if(!isObjLoaded[item_num] && isObjDragOnGL[item_num])
		DrawFramework(xPos[item_num], zPos[item_num], objSize[item_num]);
	}

	//fog관련 함수
	float fog_color[] = { 0.5,0.5,0.5 };
	if (pDlg2->fog_check.GetCheck()){
		glEnable(GL_FOG);
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogfv(GL_FOG_COLOR, fog_color);
		glFogf(GL_FOG_START, -50.0f);
		glFogf(GL_FOG_END, 50.0f);
	}
	else {
		glDisable(GL_FOG);
	}


	
	//배경색 바꾸는 스위치문
	switch (pDlg2->BKColorList.GetCurSel()) {
	case 0:
		bkcolor[0] = 1;	bkcolor[1] = 0; bkcolor[2] = 0; //0번째는 빨간색
		break;
	case 1:
		bkcolor[0] = 1;	bkcolor[1] = 0.5; bkcolor[2] = 0.1; //1번째는 주황색
		break;
	case 2:
		bkcolor[0] = 1;	bkcolor[1] = 1; bkcolor[2] = 0; //2번째는 노란색
		break;
	case 3:
		bkcolor[0] = 0;	bkcolor[1] = 1; bkcolor[2] = 0; //3번째는 초록색
		break;
	case 4:
		bkcolor[0] = 0;	bkcolor[1] = 0.3; bkcolor[2] = 1; //4번째는 파란색
		break;
	case 5:
		bkcolor[0] = 0;	bkcolor[1] = 0; bkcolor[2] = 0.5; //5번째는 남색
		break;
	case 6:
		bkcolor[0] =0.4;	bkcolor[1] = 0; bkcolor[2] = 0.6; //6번째는 보라색
		break;
	case 7:
		bkcolor[0] = 1;	bkcolor[1] = 1; bkcolor[2] = 1; bkcolor[3] = 1; //7번째는 흰색
		break;
		

	}
	
	//어떤 배경 텍스쳐 일까?
	switch (which_bk) {
	case 0://기본일때
		pDlg2->BK_name.SetWindowTextW(_T("Default"));
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, floorTex->texID);
		//glTranslatef(0, 5, 0);
		glRotatef(-90, 1, 0, 0);
		glColor4f(bkcolor[0], bkcolor[1], bkcolor[2], bkcolor[3]);
		DrawSphere(200, 50, 50);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, envTex->texID);
		glColor3f(1, 1, 1);
		DrawPlane();
		glPopMatrix();
		break;
	case 1://바다일때
		pDlg2->BK_name.SetWindowTextW(_T("Ocean"));
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, floorTex2->texID);
		//glTranslatef(0, 5, 0);
		glRotatef(-90, 1, 0, 0);
		glColor4f(bkcolor[0], bkcolor[1], bkcolor[2], bkcolor[3]);
		DrawSphere(200, 50, 50);
		glPopMatrix();


		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, envTex2->texID);
		glColor3f(1, 1, 1);
		DrawPlane();
		glPopMatrix();
		break;
	case 2://사막일때
		pDlg2->BK_name.SetWindowTextW(_T("Desert"));
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, floorTex3->texID);
		//glTranslatef(0, 5, 0);
		glRotatef(-90, 1, 0, 0);
		glColor4f(bkcolor[0], bkcolor[1], bkcolor[2], bkcolor[3]);
		DrawSphere(200, 50, 50);
		glPopMatrix();


		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, envTex3->texID);
		glColor3f(1, 1, 1);
		DrawPlane();
		glPopMatrix();
		break;
	case 3://달일때
		pDlg2->BK_name.SetWindowTextW(_T("Moon"));
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, floorTex4->texID);
		//glTranslatef(0, 5, 0);
		glRotatef(-90, 1, 0, 0);
		glColor4f(bkcolor[0], bkcolor[1], bkcolor[2], bkcolor[3]);
		DrawSphere(200, 50, 50);
		glPopMatrix();


		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, envTex4->texID);
		glColor3f(1, 1, 1);
		DrawPlane();
		glPopMatrix();
		break;

	}

	//색추가 서동우
	switch (pDlg1->obj_paint.GetCurSel()) {
	case 0:
		objcolor[0] = 1;	objcolor[1] = 0; objcolor[2] = 0; //0번째는 빨간색
		break;
	case 1:
		objcolor[0] = 1;	objcolor[1] = 0.5; objcolor[2] = 0; //1번째는 주황색
		break;
	case 2:
		objcolor[0] = 1;	objcolor[1] = 1; objcolor[2] = 0; //2번째는 노란색
		break;
	case 3:
		objcolor[0] = 0;	objcolor[1] = 1; objcolor[2] = 0; //3번째는 초록색
		break;
	case 4:
		objcolor[0] = 0;	objcolor[1] = 0.3; objcolor[2] = 1; //4번째는 파란색
		break;
	case 5:
		objcolor[0] = 0;	objcolor[1] = 0; objcolor[2] = 0.5; //5번째는 남색
		break;
	case 6:
		objcolor[0] = 0.4;	objcolor[1] = 0; objcolor[2] = 0.6; //6번째는 보라색
		break;
	case 7:
		objcolor[0] = 1;	objcolor[1] = 1; objcolor[2] = 1; //7번째는 흰색
		break;
	}

	if(!pDlg1->clicked)
	pDlg1->obj_paint.SetCurSel(objects[colorclickedIdx].colornum);

	if (colorclickedIdx > -1 && pDlg1->clicked) {
		objects[colorclickedIdx].objcolor[0] = objcolor[0];
		objects[colorclickedIdx].objcolor[1] = objcolor[1];
		objects[colorclickedIdx].objcolor[2] = objcolor[2];
		objects[colorclickedIdx].colornum = pDlg1->obj_paint.GetCurSel();
	}

	for (int i = 0; i < objNum; i++) {
		if (isObjLoaded[i]) {
			render(xPos[i], yPos[i], zPos[i], objState[i], isMouseDragOnGL, i);
		}
	}
	
}


void CMFCApplication1Dlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//리스트를 왼쪽 버튼으로 두번 클릭했을때 EVEnt
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CMFCApplication1Dlg::OnLvnBegindragList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	item_num = -1;	//클릭한 아이템 번호 초기화(아직 아무것도 클릭안했다)

	m_bDrag = true;	//드래그 시작!
	CPoint ptDrag, ptAction;

	m_pImgListDrag = m_list.CreateDragImage(pNMLV->iItem, &ptDrag); // 우선 드래그를 하긴 위한 이미지를 만듭니다.
	item_num = pNMLV->iItem; // 그릴때 선택된 번호
	m_pImgListDrag->SetBkColor(RGB(0, 0, 0)); // 드래그 이미지 색은 변함없이
	ptAction = pNMLV->ptAction; // 포인터 앱션
	SetCapture(); // 현재 이미지 움직임 캡처
	m_pImgListDrag->BeginDrag(0, ptAction - ptDrag); // 드래그 시작
	m_list.ClientToScreen(&ptAction); // 화면 맞추기
	m_pImgListDrag->DragEnter(NULL, ptAction);
	pDlg1->handle_st = handle_style = 0;

	
	*pResult = 0;
}

void CMFCApplication1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//////////////////////////////신은빈 추가(드래그앤 드롭 관련)//////////////
	if (m_bDrag) {


		if (m_pImgListDrag != NULL) {
			//좌표를 반환한다 '
			ClientToScreen(&point);
			// 변환된 좌표가 속한 윈도우 객체의 주소를 구한다 
			CWnd* pWnd = CWnd::WindowFromPoint(point);
			// 주소가 대화상자 자체나 자식 윈도우에 해당하는 주소이면 계속 드래그 상태를 유지하도록 한다
			if (pWnd != NULL)
			{
				if (this == pWnd || IsChild(pWnd))
				{

					m_pImgListDrag->DragEnter(NULL, point);
					//DragMove :  인자로 전달받은 좌표에 드래그 이미지를 출력
					m_pImgListDrag->DragMove(point);
					CRect rect, tab;

					GetClientRect(&rect);
					m_tab.GetWindowRect(&tab);
					m_list.GetWindowRect(&rect);
					if (!rect.PtInRect(point)) { //리스트 화면외에 나갈시에는 이미지 안보이기(앨리스 처럼)
						m_pImgListDrag->DragLeave(NULL);
						/////////////리스트 화면 나간 부분/////////////////////
						isMouseDragOnGL = true;
						isObjDragOnGL[item_num] = true; // obj가 화면 내부로 드래그 시작됨

						if (!isObjLoaded[item_num]) { // 드래그 한 아이템이 아직 openGL 화면에 upload가 완료되지 않았다면
							ScreenToClient(&point); // Screen좌표를 Client좌표로 재변환

							WinPos = point;
							ConvertWinToWorld(WinPos.x, WinPos.y); // 현재 마우스가 가리키는 world좌표를 wx wy wz에 저장

							

							xPos[item_num] = wx; // object의 고유좌표를 현재 마우스의 좌표로 설정 -> 마우스를 따라다니게 됨 (xz평면상에서)
							zPos[item_num] = wz;
							

						}
						
						//리스트 박스에서 오브젝트 처음 꺼낼때도 탭이 바뀜!!!!!!!!
						m_tab.SetCurSel(0);	//탭이 0번으로 바뀌고(0번이 캐릭터창)
						pDlg1->ShowWindow(SW_SHOW);	//캐릭터 설정 창을 보여줌
						pDlg2->ShowWindow(SW_HIDE);	//배경 설정 창을 가림
						switch (item_num) {
						case 0:
							pDlg1->char_name.SetWindowText(_T("괴물"));	//오브젝트 이름 변경!
							break;
						case 1:
							pDlg1->char_name.SetWindowText(_T("발뼈"));	//오브젝트 이름 변경!
							break;
						case 2:
							pDlg1->char_name.SetWindowText(_T("주사위"));	//오브젝트 이름 변경!
							break;
						case 3:
							pDlg1->char_name.SetWindowText(_T("미라"));	//오브젝트 이름 변경!
							break;
						case 4:
							pDlg1->char_name.SetWindowText(_T("닌자 펭귄"));	//오브젝트 이름 변경!
							break;
						case 5:
							pDlg1->char_name.SetWindowText(_T("트램펄린"));	//오브젝트 이름 변경!
							break;
						}
						colorclickedIdx = item_num;
						CString x;
						x.Format(_T("%.1f"), xPos[item_num]);
						pDlg1->m_X.SetWindowText(x);

						CString y;
						y.Format(_T("%.1f"), yPos[item_num]);
						pDlg1->m_Y.SetWindowText(y);

						CString z;
						z.Format(_T("%.1f"), zPos[item_num]);
						pDlg1->m_Z.SetWindowText(z);

						CString width;
						width.Format(_T("%.1f"), objSize[item_num][0]);
						pDlg1->width.SetWindowText(width);	//클릭된 자표의 width좌표 봐여줌

						CString height;
						height.Format(_T("%.1f"), objSize[item_num][1]);
						pDlg1->height.SetWindowText(height);	//클릭된 자표의 height좌표 봐여줌

						CString depth;
						depth.Format(_T("%.1f"), objSize[item_num][2]);
						pDlg1->depth.SetWindowText(depth);	//클릭된 자표의 depth좌표 봐여줌

					}

				}
				else
				{
					// DragEnter와 반대로 이미지를 지운다 
					m_pImgListDrag->DragLeave(NULL);
					
					
				}
			}
		}
	}

	/////////////////////////////// 장군우 추가 ///////////////////////////////

	if (m_Lmouse)
	{
		if (isMouseDragOnGL && clickedIdx != -1) { // Lmouse가 Down상태이고 object가 클릭된 상태라면
			if (handle_style == 0 || handle_style == 3) { // Handling Style = Default
				WinPos = point;
				ConvertWinToWorld(WinPos.x, WinPos.y); // 현재 마우스가 가리키는 world좌표를 wx wy wz에 저장

				if (wx > planeSize - planeEdge) wx = planeSize - planeEdge;
				if (wx < -(planeSize - planeEdge)) wx = -(planeSize - planeEdge);
				if (wz > planeSize - planeEdge) wz = planeSize - planeEdge;
				if (wz < -(planeSize - planeEdge)) wz = -(planeSize - planeEdge);
				xPos[clickedIdx] = wx; // object의 고유좌표를 현재 마우스의 좌표로 설정 -> 마우스를 따라다니게 됨 (xz평면상에서)
				zPos[clickedIdx] = wz;
				// 초기화
				wx = 0;
				wz = 0;

				CString x;
				x.Format(_T("%.1f"), xPos[clickedIdx]);
				pDlg1->m_X.SetWindowText(x);

				CString y;
				y.Format(_T("%.1f"), yPos[clickedIdx]);
				pDlg1->m_Y.SetWindowText(y);

				CString z;
				z.Format(_T("%.1f"), zPos[clickedIdx]);
				pDlg1->m_Z.SetWindowText(z);

				CString width;
				width.Format(_T("%.1f"), objSize[clickedIdx][0]);
				pDlg1->width.SetWindowText(width);	//클릭된 자표의 width좌표 봐여줌

				CString height;
				height.Format(_T("%.1f"), objSize[clickedIdx][1]);
				pDlg1->height.SetWindowText(height);	//클릭된 자표의 height좌표 봐여줌

				CString depth;
				depth.Format(_T("%.1f"), objSize[clickedIdx][2]);
				pDlg1->depth.SetWindowText(depth);	//클릭된 자표의 depth좌표 봐여줌

			}

			else if (handle_style == 1) { // Handling Style = Rot
				objState[clickedIdx].Rot += (float)(point.x - m_oldPos.x) * m_sRot;
				if (objState[clickedIdx].Rot == 360.0 || objState[clickedIdx].Rot == -360.0) objState[clickedIdx].Rot = 0.0;
				m_oldPos = point;
			}

			else { // Handling Style = Resize
				objState[clickedIdx].xScl += (float)(point.x - m_oldPos.x) * m_sScl;
				objState[clickedIdx].yScl += (float)(point.x - m_oldPos.x) * m_sScl;
				objState[clickedIdx].zScl += (float)(point.x - m_oldPos.x) * m_sScl;
				objSize[clickedIdx][0] = objDefSize[clickedIdx][0] * objState[clickedIdx].xScl;
				objSize[clickedIdx][1] = objDefSize[clickedIdx][1] * objState[clickedIdx].xScl;
				objSize[clickedIdx][2] = objDefSize[clickedIdx][2] * objState[clickedIdx].xScl;
				m_oldPos = point;

				CString width;
				width.Format(_T("%.1f"), objSize[clickedIdx][0]);
				pDlg1->width.SetWindowText(width);	//클릭된 자표의 width좌표 봐여줌

				CString height;
				height.Format(_T("%.1f"), objSize[clickedIdx][1]);
				pDlg1->height.SetWindowText(height);	//클릭된 자표의 height좌표 봐여줌

				CString depth;
				depth.Format(_T("%.1f"), objSize[clickedIdx][2]);
				pDlg1->depth.SetWindowText(depth);	//클릭된 자표의 depth좌표 봐여줌
			}
			
			//Invalidate(false);
		}
		else { // Camera Moving
			m_yRot += (float)(point.x - m_oldPos.x) * m_sRot; // 차이를 작게하기위해 speed를 곱해줌
			m_xRot += (float)(point.y - m_oldPos.y) * m_sRot;
			if (m_xRot >= 40) m_xRot = 40;
			if (m_xRot <= -5) m_xRot = -5;

			//Invalidate();
		}

	}

	if (m_Rmouse)
	{
		m_xTran -= (float)(m_oldPos.x - point.x) * m_sTran; // 차이를 작게하기위해 speed를 곱해줌
		m_yTran += (float)(m_oldPos.y - point.y) * m_sTran; // speed가 없으면 금방 휙휙움직여버림
	}
	m_oldPos = point;
	//Invalidate(false);

	CDialogEx::OnMouseMove(nFlags, point);
}

void CMFCApplication1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	if (m_bDrag) {
		ClientToScreen(&point);
		CRect rect, tab;
		GetClientRect(&rect);
		m_tab.GetWindowRect(&tab);
		m_pImgListDrag->DragLeave(NULL);
		if (rect.PtInRect(point) && !tab.PtInRect(point)) { //출력화면에 넣을시, 탭 이미지 겹치는 부분 방지, 그림 번호선택

		}
		if (item_num != 3 && item_num != 4) { // object를 xz평면 상단으로 이동 (그 전까지는 클릭했을 때 인식되지않도록 xz평면의 하단부에 놓여있음)
			yPos[item_num] = -1.0f;
		}
		else yPos[item_num] = 0.5f;
		isObjLoaded[item_num] = true; // object upload 완료
		
		// EndDrag() 함수가 호출되면 관련 객체는 드래그 앤 드랍 동작이 끝났다고 인식하게 됩니다.
		m_pImgListDrag->EndDrag();
		ReleaseCapture();
		m_bDrag = false;
	}

	m_Lmouse = false;

	isClicked = false;

	ReleaseCapture(); // 마우스 릴리스 (순서 바껴도 상관X)
	
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CMFCApplication1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{

	pDlg1->OnCbnSelchangeCombo1();
	pDlg1->OnCbnCloseupCombo1();

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_Lmouse = true;
	SetCapture(); // 마우스값 캡처 -> 창 밖으로 나가도 마우스값을 보내주세요
	// 클릭한 위치를 알려줘야. -> point
	m_oldPos = point;
	// L button move한 만큼 움직여야. -> 전에있는 oldPos와 지금의 oldPos와의 차이를 통해 움직이는것
	WinPos = point;
	ConvertWinToWorld(WinPos.x, WinPos.y); // 현재 마우스가 가리키는 world좌표를 wx wy wz에 저장
	//if (isNear(wx, wy, wz)) m_isClicked[clickedIdx] = true;
	if (isNear(wx, wy, wz)) {
		isClicked = true; // 마우스 클릭위치가 object와 근접해있다면 isClicked를 true로


		/////////////클릭되면 캐릭터 탭 인터페이스 부분에 변경점/////////////////////////
		//오브젝트를 클릭했을때
		m_tab.SetCurSel(0);	//탭이 0번으로 바뀌고(0번이 캐릭터창)
		pDlg1->ShowWindow(SW_SHOW);	//캐릭터 설정 창을 보여줌
		pDlg2->ShowWindow(SW_HIDE);	//배경 설정 창을 가림
		
		
		switch (clickedIdx) {
		case 0:
			pDlg1->char_name.SetWindowText(_T("괴물"));	//오브젝트 이름 변경!
			break;
		case 1:
			pDlg1->char_name.SetWindowText(_T("발뼈"));	//오브젝트 이름 변경!
			break;
		case 2:
			pDlg1->char_name.SetWindowText(_T("주사위"));	//오브젝트 이름 변경!
			break;
		case 3:
			pDlg1->char_name.SetWindowText(_T("미라"));	//오브젝트 이름 변경!
			break;
		case 4:
			pDlg1->char_name.SetWindowText(_T("닌자 펭귄"));	//오브젝트 이름 변경!
			break;
		case 5:
			pDlg1->char_name.SetWindowText(_T("트램펄린"));	//오브젝트 이름 변경!
			break;
		}
		colorclickedIdx = clickedIdx;
		CString x;
		x.Format(_T("%.1f"), xPos[clickedIdx]);
		pDlg1->m_X.SetWindowText(x);

		CString y;
		y.Format(_T("%.1f"), yPos[clickedIdx]);
		pDlg1->m_Y.SetWindowText(y);

		CString z;
		z.Format(_T("%.1f"), zPos[clickedIdx]);
		pDlg1->m_Z.SetWindowText(z);

		CString width;
		width.Format(_T("%.1f"), objSize[clickedIdx][0]);
		pDlg1->width.SetWindowText(width);	//클릭된 자표의 width좌표 봐여줌

		CString height;
		height.Format(_T("%.1f"), objSize[clickedIdx][1]);
		pDlg1->height.SetWindowText(height);	//클릭된 자표의 height좌표 봐여줌

		CString depth;
		depth.Format(_T("%.1f"), objSize[clickedIdx][2]);
		pDlg1->depth.SetWindowText(depth);	//클릭된 자표의 depth좌표 봐여줌
	}
	else {
		isClicked = false;
		//배경을 클릭했을때
		m_tab.SetCurSel(1);	//탭이 1번으로 바뀌고(1번이 배경창)
		pDlg2->ShowWindow(SW_SHOW);	//배경설정창을 보여줌
		pDlg1->ShowWindow(SW_HIDE);	//캐릭터 설정 창을 가려줌
		
		pDlg1->handle_st = handle_style = 0;
		
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMFCApplication1Dlg::ConvertWinToWorld(float x_cursor, float y_cursor)
{
	// TODO: 여기에 구현 코드 추가.
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	WinPos.x = (double)x_cursor;									// Window의 x,y좌표와 OpenGL화면에서의 x,y좌표는 시작점이 다르기 때문에 Window의 좌표를 OpenGL좌표로 바꾸어주는 과정
	WinPos.y = viewport[3] - (double)y_cursor;
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glReadPixels((int)WinPos.x, (int)WinPos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &WinPosZ); 
	gluUnProject(WinPos.x, WinPos.y, WinPosZ, modelview, projection, viewport, &wx, &wy, &wz);		// world x, y, z좌표를 wx, wy, wz에 저장
	//glClearBufferData();
	
	// 초기화
	WinPos.x = 0;
	WinPos.y = 0;
	WinPosZ = 0;
}


bool CMFCApplication1Dlg::isNear(float wx, float wy, float wz) {
	//for (int i = 0; i < objNum; i++) {
	for (int i = 0; i < objNum; i++) { // object 1개니까 [0]만 비교
		if (abs((long)(wx - xPos[i])) <= objSize[i][0]/2 && abs((long)(wy - yPos[i])) <= objSize[i][1] && abs((long)(wz - zPos[i])) <= objSize[i][2] / 2) {
			clickedIdx = i;
			return true;
		}
	}
	clickedIdx = -1;
	return false;
}



void CMFCApplication1Dlg::DrawPlane()
{
	// TODO: 여기에 구현 코드 추가.
	glPushMatrix();
		glBegin(GL_QUADS);
		//glColor3f(0.2, 0.2, 0.2);
		glNormal3f(0, 1, 0);
		glTexCoord2f(1, 0);glVertex3f(planeSize, 0, planeSize);
		glTexCoord2f(1, 1);glVertex3f(planeSize, 0, -planeSize);
		glTexCoord2f(0, 1);glVertex3f(-planeSize, 0, -planeSize);
		glTexCoord2f(0, 0);glVertex3f(-planeSize, 0, planeSize);

		glEnd();
	glPopMatrix();
}


void CMFCApplication1Dlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_Rmouse = true;
	SetCapture();
	m_oldPos = point;



	CDialogEx::OnRButtonDown(nFlags, point);
}


void CMFCApplication1Dlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_Rmouse = false;
	ReleaseCapture();



	CDialogEx::OnRButtonUp(nFlags, point);
}


BOOL CMFCApplication1Dlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	
	if (zDelta > 0)
		PerspAngle -= 10.0; // +면 (스크롤을 올리면) Zoom In
	else
		PerspAngle += 10.0; // -면 (스크롤을 내리면) Zoom Out

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CMFCApplication1Dlg::DrawSphere(int slices, int stacks, float radius)
{
	// TODO: 여기에 구현 코드 추가.
	//glColor3f(0, 0.5, 0.8);
	const float PI = 3.14159f;

	float x = 0.0f, y = 0.0f, z = 0.0f, s = 0.0f, t = 0.0f;

	float PIOverStacks = PI / float(stacks);
	float PI2OverSlices = 2.0f * PI / float(slices);

	glBegin(GL_TRIANGLE_STRIP);
	for (int stack = 0; stack < stacks; ++stack)
	{
		float Phi = float(stack) * PIOverStacks;
		float CosP = cosf(Phi);
		float SinP = sinf(Phi);
		for (int slice = 0; slice < slices; ++slice)
		{
			float Theta = float(slice) * PI2OverSlices;
			x = radius * cosf(Theta) * SinP;
			y = radius * sinf(Theta) * SinP;
			z = radius * CosP;
			s = 1.0f - (float)slice / (float)slices;
			t = (float)stack / (float)stacks;

			glTexCoord2f(s, t);
			glVertex3f(x, y, z);
			glEnable(GL_NORMALIZE);
			glNormal3f(x, y, z);

			float nextPhi = float(stack + 1) * PIOverStacks;
			float nextCosP = cosf(nextPhi);
			float nextSinP = sinf(nextPhi);
			x = radius * cosf(Theta) * nextSinP;
			y = radius * sinf(Theta) * nextSinP;
			z = radius * nextCosP;
			s = 1.0f - (float)slice / (float)slices;
			t = (float)(stack + 1.0f) / (float)stacks;

			glTexCoord2f(s, t);
			glVertex3f(x, y, z);
			glEnable(GL_NORMALIZE);
			glNormal3f(x, y, z);
		}
	}
	glEnd();
}


unsigned char *CMFCApplication1Dlg::LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;							// the file pointer
	BITMAPFILEHEADER	bitmapFileHeader;		// bitmap file header
	unsigned char		*bitmapImage;			// bitmap image data
	int					imageIdx = 0;		// image index counter
	unsigned char		tempRGB;				// swap variable

												// open filename in "read binary" mode
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// read the bitmap file header
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	// verify that this is a bitmap by checking for the universal bitmap id
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// read the bitmap information header
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// move file pointer to beginning of bitmap data
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// allocate enough memory for the bitmap image data
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	// verify memory allocation
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// read in the bitmap image data
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	// make sure bitmap image data was read
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// swap the R and B values to get RGB since the bitmap color format is in BGR
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// close the file and return the bitmap image data
	fclose(filePtr);
	return bitmapImage;
}

texture_t *CMFCApplication1Dlg::LoadTextureFile(char *filename)
{
	BITMAPINFOHEADER texInfo;
	texture_t *thisTexture;

	// allocate memory for the texture structure
	thisTexture = (texture_t*)malloc(sizeof(texture_t));
	if (thisTexture == NULL)
		return NULL;

	// load the texture data and check validity
	thisTexture->data = LoadBitmapFile(filename, &texInfo);
	if (thisTexture->data == NULL)
	{
		free(thisTexture);
		return NULL;
	}

	// set width and height info for this texture
	thisTexture->width = texInfo.biWidth;
	thisTexture->height = texInfo.biHeight;

	// generate the texture object for this texture
	glGenTextures(1, &thisTexture->texID);

	return thisTexture;
}

bool CMFCApplication1Dlg::LoadAllTextures()
{

	// load the floor texture
	

	envTex = LoadTextureFile("gras.bmp");	//기본 초원 땅
	if (envTex == NULL)
		return false;

	envTex2 = LoadTextureFile("waterenv.bmp");	//물 땅
	if (envTex2 == NULL)
		return false;

	envTex3 = LoadTextureFile("sand.bmp");	//달 표면
	if (envTex3 == NULL)
		return false;

	envTex4 = LoadTextureFile("moon.bmp");	//사막 땅
	if (envTex4 == NULL)
		return false;

	// load the sky texture
	floorTex = LoadTextureFile("sky.bmp");	//기본 화창한 하늘
	if (floorTex == NULL)
		return false;

	floorTex2 = LoadTextureFile("sky.bmp");
	if (floorTex2 == NULL)
		return false;

	floorTex3 = LoadTextureFile("sky.bmp");	//우주하늘
	if (floorTex3 == NULL)
			return false;

	floorTex4 = LoadTextureFile("universe.bmp");
	if (floorTex4 == NULL)
		return false;
		
	///////////////////////////여기부턴 땅///////////////////////////////////////
	// setup the torus' environment map       
	glBindTexture(GL_TEXTURE_2D, envTex->texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, envTex->width, envTex->height, GL_RGB, GL_UNSIGNED_BYTE, envTex->data);

	// setup the torus' environment map       
	glBindTexture(GL_TEXTURE_2D, envTex2->texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, envTex2->width, envTex2->height, GL_RGB, GL_UNSIGNED_BYTE, envTex2->data);

	// setup the torus' environment map       
	glBindTexture(GL_TEXTURE_2D, envTex3->texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, envTex3->width, envTex3->height, GL_RGB, GL_UNSIGNED_BYTE, envTex3->data);

	// setup the torus' environment map       
	glBindTexture(GL_TEXTURE_2D, envTex4->texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, envTex4->width, envTex4->height, GL_RGB, GL_UNSIGNED_BYTE, envTex4->data);

	///////////////////////////여기부턴 하늘///////////////////////////////////////
	// setup the floor texture
	glBindTexture(GL_TEXTURE_2D, floorTex->texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, floorTex->width, floorTex->height, GL_RGB, GL_UNSIGNED_BYTE, floorTex->data);

	glBindTexture(GL_TEXTURE_2D, floorTex2->texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, floorTex2->width, floorTex2->height, GL_RGB, GL_UNSIGNED_BYTE, floorTex2->data);
	

	glBindTexture(GL_TEXTURE_2D, floorTex3->texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, floorTex3->width, floorTex3->height, GL_RGB, GL_UNSIGNED_BYTE, floorTex3->data);

	glBindTexture(GL_TEXTURE_2D, floorTex4->texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, floorTex4->width, floorTex4->height, GL_RGB, GL_UNSIGNED_BYTE, floorTex4->data);
	return true;
}

int CMFCApplication1Dlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	
	return 0;
}


void CMFCApplication1Dlg::OnBackgroundBk()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	KillTimer(1000);
	CBKDlg bk;
	if (IDOK == bk.DoModal()) {
		which_bk = bk.bk_num;
	}
	SetTimer(1000, 30, NULL);
}


BOOL CMFCApplication1Dlg::CreateTexture(GLuint &textureID, LPTSTR szFileName)

{
	HBITMAP hBMP;                                                                 // Handle Of The Bitmap
	BITMAP  bitmap;																  // Bitmap Structure

	glGenTextures(1, &textureID);                                                 // Create The Texture
	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);


	if (!hBMP)                                                                    // Does The Bitmap Exist?
		return FALSE;                                                           // If Not Return False

	GetObject(hBMP, sizeof(bitmap), &bitmap);                                     // Get The Object
																				  // hBMP:        Handle To Graphics Object
																				  // sizeof(bitmap): Size Of Buffer For Object Information
																				  // &bitmap:        Buffer For Object Information

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);                                        // Pixel Storage Mode (Word Alignment / 4 Bytes)

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, textureID);                                      // Bind To The Texture ID
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);			  // Linear Min Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			  // Linear Mag Filter
	glTexImage2D(GL_TEXTURE_2D, 0, 3, bitmap.bmWidth, bitmap.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap.bmBits);
	// MUST NOT BE INDEX BMP, BUT RGB
	DeleteObject(hBMP);                                                           // Delete The Object

	return TRUE;
}

void CMFCApplication1Dlg::render(float x, float y, float z, objStateStruct objState, bool isMouseDragOnGL, int k) {
	
	if (isMouseDragOnGL) {

			glPushMatrix();
		{
					int   g_ViewMode = GL_TRIANGLES;					// We want the default drawing mode to be normal

					//모든 라이트에서 컬럭색 나오도록 수정.
					if (pDlg2->m_light.GetCheck()) {
						glDisable(GL_LIGHT1);
						glDisable(GL_LIGHT2);


						glMaterialfv(GL_FRONT, GL_AMBIENT, matdAmbient);
						glMaterialfv(GL_FRONT, GL_DIFFUSE, matdDiff);
						glMaterialfv(GL_FRONT, GL_SPECULAR, matdSpec);
						glMaterialf(GL_FRONT, GL_SHININESS, shine);




						glEnable(GL_LIGHT0);								// Turn on a light with defaults set
						glEnable(GL_LIGHTING);								// Turn on lighting
						glEnable(GL_COLOR_MATERIAL);						// Allow color
					}
					else if (pDlg2->m_clight.GetCheck()) {
						glDisable(GL_LIGHT0);
						glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
						glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);

						glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight);
						glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight);
					
						glLightfv(GL_LIGHT2, GL_POSITION, lightPosition);

						glEnable(GL_LIGHTING);
						glEnable(GL_LIGHT2);
						glEnable(GL_COLOR_MATERIAL);	
					}
					else {
						glDisable(GL_LIGHTING); glDisable(GL_LIGHT0);
						glDisable(GL_LIGHT2);
					}

					if (pDlg2->m_spot.GetCheck())
					{
						glLightfv(GL_LIGHT1, GL_AMBIENT, SpotambientLight);
						glLightfv(GL_LIGHT1, GL_DIFFUSE, SpotdiffuseLight);
						glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);

						glLightfv(GL_LIGHT1, GL_POSITION, spotPosition);
						glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10.0f);
						glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotlightDir);
						glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10.f);
						glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
						glMaterialf(GL_FRONT, GL_SHININESS, 10.f);
						glEnable(GL_LIGHT1);

					}

					else
					{

						glMaterialfv(GL_FRONT, GL_AMBIENT, matdAmbient);
						glMaterialfv(GL_FRONT, GL_DIFFUSE, matdDiff);
						glMaterialfv(GL_FRONT, GL_SPECULAR, matdSpec);
						glMaterialf(GL_FRONT, GL_SHININESS, shine);

						glDisable(GL_LIGHT1);
					}


					if (k == 3 || k == 4) y -= 1.5f;
					else if (k != 5) y += 1.0f;
					else y += 0.1f;
					if (k == 2)
					y += (objState.yScl -1);
					glTranslatef(x, y + 1.0, z);
					glRotatef(objState.Rot, 0, 1, 0); // 회전할때 사용할 것
					glScalef(objState.xScl, objState.yScl, objState.zScl);
					// obj 크기가 너무커서 스케일을 크게 해줄 필요가 있습니다.
					if(k == 3 || k == 4) // 사이즈 조정하니 마우스에서 에러가 발생합니다.
					glScalef(0.05, 0.05, 0.05);
					else if(k == 0)
						glScalef(0.02, 0.02, 0.02);

				
					glBindTexture(GL_TEXTURE_2D, objects[k].axas);
					//바뀐 obj선 그리기.
					glBegin(GL_TRIANGLES);
					
					// Since we know how many objects our model has, go through each of them.
					for (int i = 0; i < objects[k].vertices.size(); i++)
					{


						// Turn on texture mapping and turn off color
						glEnable(GL_TEXTURE_2D);

						// Reset the color to normal again
						glColor3ub(255, 255, 255);

						// Bind the texture map to the object by it's materialID (*ID Current Unused*)
					//
						 //foot의 경우는 새로바뀐 로더에서 노말 계산할 때 오래 걸리는 부작용이 있어 속도가 느립니다!
						glNormal3f(objects[k].normals.at(i).x, objects[k].normals.at(i).y, objects[k].normals.at(i).z);
						///////////////여기에 색 바뀔 switch문 넣자(#오브젝트색)////////////////
						glColor4f(objects[k].objcolor[0], objects[k].objcolor[1], objects[k].objcolor[2], obj_opacity);
						glTexCoord2f(objects[k].uvs.at(i).x, objects[k].uvs.at(i).y);
						glVertex3f(objects[k].vertices.at(i).x, objects[k].vertices.at(i).y, objects[k].vertices.at(i).z);

					}
					glEnd();
					glDisable(GL_BLEND);
					glPopMatrix();

				}


				
			}
		
	}
	
	// TODO: 여기에 구현 코드 추가.



void CMFCApplication1Dlg::DrawFramework(float xPos, float zPos, float objSize[3])
{
	// TODO: 여기에 구현 코드 추가.
	glPushMatrix();

		glTranslatef(xPos, 0.f, zPos);

		glBegin(GL_LINE_LOOP);
		glVertex3f(objSize[0] / 2, 0.f, objSize[2] / 2); // 1
		glVertex3f(objSize[0] / 2, 0.f, -objSize[2] / 2); // 2
		glVertex3f(-objSize[0] / 2, 0.f, -objSize[2] / 2); // 3
		glVertex3f(-objSize[0] / 2, 0.f, objSize[2] / 2); // 4
		glEnd();
		glBegin(GL_LINE_LOOP);
		glVertex3f(objSize[0] / 2, objSize[1], objSize[2] / 2); // 1^
		glVertex3f(objSize[0] / 2, objSize[1], -objSize[2] / 2); // 2^
		glVertex3f(-objSize[0] / 2, objSize[1], -objSize[2] / 2); // 3^
		glVertex3f(-objSize[0] / 2, objSize[1], objSize[2] / 2); // 4^
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(objSize[0] / 2, 0.f, objSize[2] / 2); // 1
		glVertex3f(objSize[0] / 2, objSize[1], objSize[2] / 2); // 1^
		glVertex3f(objSize[0] / 2, 0.f, -objSize[2] / 2); // 2
		glVertex3f(objSize[0] / 2, objSize[1], -objSize[2] / 2); // 2^
		glVertex3f(-objSize[0] / 2, 0.f, -objSize[2] / 2); // 3
		glVertex3f(-objSize[0] / 2, objSize[1], -objSize[2] / 2); // 3^
		glVertex3f(-objSize[0] / 2, 0.f, objSize[2] / 2); // 4
		glVertex3f(-objSize[0] / 2, objSize[1], objSize[2] / 2); // 4^
		glEnd();


	glPopMatrix();


	
}


void CMFCApplication1Dlg::OnCbnSelchangeCombo1()
{
	pDlg1->clicked = true;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
