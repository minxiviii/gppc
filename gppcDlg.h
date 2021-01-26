
// gppcDlg.h: 헤더 파일
//

#pragma once

#include ".\CGridListCtrlEx\CGridListCtrlGroups.h"
#include ".\json\include\json.h"
#include "PowerController.h"
#include "ZDongle.h"
#include "Loadcell.h"
#include "StepInfo.h"
#include "csv.h"
#include "IniConfig.h"
#include "UDP_HallSensor.h"
#include "UDP_Analyzer.h"


const int WM_USEREVENT = (WM_USER + 1);
const int USER_TIMER = 1000;

enum eUserEvent {
	kEventReceiveLoadcell = 0,
	kEventZStart,
	kEventZReady,
	kEventZFinish,
	kEventZIdle,
};

enum eStepTableIndex {
	kStep1 = 0,
	kStep2,
	kStep3,
	kStep4,
	kStep5,
	kStep6,
	kStep7,
	kStep8,
	kStepMax,
};

enum eCheckImageCode {
	kUnchecked = 0,
	kChecked
};

const int kSerialGppCount = 18;
const int kSerialLoadcellCount = 3;

const int kPortPerChamber = 11;
const int kChamberCount = 3;
const int kTotalPort = kPortPerChamber * kChamberCount;

const string kJsonfile = ".\\gppc.json";

// CgppcDlg 대화 상자
class CgppcDlg : public CDialogEx
{
// 생성입니다.
public:
	CgppcDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GPPC_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonS1AddGroup();
	afx_msg void OnBnClickedButtonS2AddGroup();
	afx_msg void OnBnClickedButtonS3AddGroup();
	afx_msg void OnBnClickedButtonS4AddGroup();
	afx_msg void OnBnClickedButtonS1DelGroup();
	afx_msg void OnBnClickedButtonS2DelGroup();
	afx_msg void OnBnClickedButtonS3DelGroup();
	afx_msg void OnBnClickedButtonS4DelGroup();
	afx_msg void OnBnClickedButtonS5AddGroup();
	afx_msg void OnBnClickedButtonS5DelGroup();
	afx_msg void OnBnClickedButtonS6AddGroup();
	afx_msg void OnBnClickedButtonS6DelGroup();
	afx_msg void OnBnClickedButtonS7AddGroup();
	afx_msg void OnBnClickedButtonS7DelGroup();
	afx_msg void OnBnClickedButtonS8AddGroup();
	afx_msg void OnBnClickedButtonS8DelGroup();
	afx_msg void OnBnClickedButtonSerialAllConnect();
	afx_msg void OnBnClickedButtonSerialAllDisconnect();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonDelayCalc();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnCbnSelchangeComboSerialGpp01();
	afx_msg void OnCbnSelchangeComboSerialGpp02();
	afx_msg void OnCbnSelchangeComboSerialGpp03();
	afx_msg void OnCbnSelchangeComboSerialGpp04();
	afx_msg void OnCbnSelchangeComboSerialGpp05();
	afx_msg void OnCbnSelchangeComboSerialGpp06();
	afx_msg void OnCbnSelchangeComboSerialGpp07();
	afx_msg void OnCbnSelchangeComboSerialGpp08();
	afx_msg void OnCbnSelchangeComboSerialGpp09();
	afx_msg void OnCbnSelchangeComboSerialGpp10();
	afx_msg void OnCbnSelchangeComboSerialGpp11();
	afx_msg void OnCbnSelchangeComboSerialGpp12();
	afx_msg void OnCbnSelchangeComboSerialGpp13();
	afx_msg void OnCbnSelchangeComboSerialGpp14();
	afx_msg void OnCbnSelchangeComboSerialGpp15();
	afx_msg void OnCbnSelchangeComboSerialGpp16();
	afx_msg void OnCbnSelchangeComboSerialGpp17();
	afx_msg void OnCbnSelchangeComboSerialGpp18();
	afx_msg void OnCbnSelchangeComboSerialLoadcell1();
	afx_msg void OnCbnSelchangeComboSerialLoadcell2();
	afx_msg void OnCbnSelchangeComboSerialLoadcell3();
	afx_msg void OnCbnSelchangeComboSerialDongle();
	afx_msg LRESULT OnUserEvent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	
	void Exit();

	// UI - Steps
	CImageList image_in_steptable[kStepMax];
	CGridListCtrlGroups listctrl_of_steptable[kStepMax];
	void InitListCtrls();	// Step Group ListCtrl 초기화 
	void AddStepTableRow(const int step_number, const CString start_value = _T("0.0"), const CString end_value = _T("0.0"), const CString interval = _T("0.1"));
	void AddStepTableRow(const int step_number, StepGroup* step_group);
	void RemoveStepTableRow(const int step_number);
	void LoadJSonOfSteps();
	void SaveJSonOfSteps();
	void UpdateStepGroups();

	// UI - Serial
	CComboBox combo_gpp[kSerialGppCount];
	CComboBox combo_loadcell[kSerialLoadcellCount];
	CComboBox combo_dongle;
	void InitComboSerial();	// Serial Combobox 초기화


	// UI - calcurate delay
	CString carrier_speed;
	CString distance[kStepMax];
	CString delay[kStepMax];

	// test flag
	bool test_running;

	// Zigbee Dongle
	ZDongle zdongle;
	static void ZDongleReceiveCB(void* data, void* context);

	// Loadcell
	Loadcell loadcell[kSerialLoadcellCount];
	static void LoadcellReceiveCB(void* data, void* context);

	// gpp
	PowerController power_controller[kSerialGppCount];
	static void PowerContollerCB(void* data, void* context);

	// step
	vector<StepGroup> step_groups[kStepMax];
	vector<StepGroup*> step_linear;

	// UDP
	WSADATA wsa;
	UDP_HallSensor udp_hallsensor;
	UDP_Analyzer udp_analyzer;

	int zStatus;

	void TestStart(BOOL start);
	void TestAddSchedule(int step);
	BOOL TestNextGain();

	CSV csv;
	unsigned int trycount;
	map<CString, int> discovered_serial;
	void SelchangeComboSerial(const int combobox_number);
};
