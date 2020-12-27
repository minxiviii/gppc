
// gppcDlg.h: 헤더 파일
//

#pragma once

#include ".\CGridListCtrlEx\CGridListCtrlGroups.h"
#include ".\json\include\json.h"
#include "PowerController.h"
#include "ZDongle.h"
#include "Loadcell.h"
#include "StepInfo.h"

enum eStepTableIndex {
	kStep1 = 0,
	kStep2,
	kStep3,
	kStep4,
	kStepMax
};

enum eCheckImageCode {
	kUnchecked = 0,
	kChecked
};

const int kSerialGppCount = 12;
const int kSerialLoadcellCount = 2;

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
	afx_msg void OnBnClickedButtonSerialAllConnect();
	afx_msg void OnBnClickedButtonSerialAllDisconnect();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonDelayCalc();
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
	PowerController power_countroller[kSerialGppCount];
	static void PowerContollerCB(void* data, void* context);
};
