﻿
// gppcDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "gppc.h"
#include "gppcDlg.h"
#include "afxdialogex.h"

#include ".\CGridListCtrlEx\CGridColumnTraitText.h"
#include ".\CGridListCtrlEx\CGridColumnTraitEdit.h"
#include ".\CGridListCtrlEx\CGridRowTraitXP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CgppcDlg 대화 상자



CgppcDlg::CgppcDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GPPC_DIALOG, pParent)
	, test_running(false)
	, carrier_speed(_T(""))
	, zStatus(kEventZReady)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CgppcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_1, listctrl_of_steptable[kStep1]);
	DDX_Control(pDX, IDC_LIST_2, listctrl_of_steptable[kStep2]);
	DDX_Control(pDX, IDC_LIST_3, listctrl_of_steptable[kStep3]);
	DDX_Control(pDX, IDC_LIST_4, listctrl_of_steptable[kStep4]);

	DDX_Control(pDX, IDC_COMBO_SERIAL1, combo_gpp[0]);
	DDX_Control(pDX, IDC_COMBO_SERIAL2, combo_gpp[1]);
	DDX_Control(pDX, IDC_COMBO_SERIAL3, combo_gpp[2]);
	DDX_Control(pDX, IDC_COMBO_SERIAL4, combo_gpp[3]);
	DDX_Control(pDX, IDC_COMBO_SERIAL5, combo_gpp[4]);
	DDX_Control(pDX, IDC_COMBO_SERIAL6, combo_gpp[5]);
	DDX_Control(pDX, IDC_COMBO_SERIAL7, combo_gpp[6]);
	DDX_Control(pDX, IDC_COMBO_SERIAL8, combo_gpp[7]);
	DDX_Control(pDX, IDC_COMBO_SERIAL9, combo_gpp[8]);
	DDX_Control(pDX, IDC_COMBO_SERIAL10, combo_gpp[9]);
	DDX_Control(pDX, IDC_COMBO_SERIAL11, combo_gpp[10]);
	DDX_Control(pDX, IDC_COMBO_SERIAL12, combo_gpp[11]);
	DDX_Control(pDX, IDC_COMBO_SERIAL13, combo_loadcell[0]);
	DDX_Control(pDX, IDC_COMBO_SERIAL14, combo_loadcell[1]);
	DDX_Control(pDX, IDC_COMBO_SERIAL15, combo_dongle);
	DDX_Text(pDX, IDC_EDIT_SPEED, carrier_speed);
	DDX_Text(pDX, IDC_EDIT_DISTANCE1, distance[kStep1]);
	DDX_Text(pDX, IDC_EDIT_DISTANCE2, distance[kStep2]);
	DDX_Text(pDX, IDC_EDIT_DISTANCE3, distance[kStep3]);
	DDX_Text(pDX, IDC_EDIT_DISTANCE4, distance[kStep4]);
	DDX_Text(pDX, IDC_EDIT_DELAY1, delay[kStep1]);
	DDX_Text(pDX, IDC_EDIT_DELAY2, delay[kStep2]);
	DDX_Text(pDX, IDC_EDIT_DELAY3, delay[kStep3]);
	DDX_Text(pDX, IDC_EDIT_DELAY4, delay[kStep4]);
}

BEGIN_MESSAGE_MAP(CgppcDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_USEREVENT, OnUserEvent)
	ON_BN_CLICKED(IDCANCEL, &CgppcDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_S1_ADD_GROUP, &CgppcDlg::OnBnClickedButtonS1AddGroup)
	ON_BN_CLICKED(IDC_BUTTON_S1_DEL_GROUP, &CgppcDlg::OnBnClickedButtonS1DelGroup)
	ON_BN_CLICKED(IDC_BUTTON_S2_ADD_GROUP, &CgppcDlg::OnBnClickedButtonS2AddGroup)
	ON_BN_CLICKED(IDC_BUTTON_S2_DEL_GROUP, &CgppcDlg::OnBnClickedButtonS2DelGroup)
	ON_BN_CLICKED(IDC_BUTTON_S3_ADD_GROUP, &CgppcDlg::OnBnClickedButtonS3AddGroup)
	ON_BN_CLICKED(IDC_BUTTON_S3_DEL_GROUP, &CgppcDlg::OnBnClickedButtonS3DelGroup)
	ON_BN_CLICKED(IDC_BUTTON_S4_ADD_GROUP, &CgppcDlg::OnBnClickedButtonS4AddGroup)
	ON_BN_CLICKED(IDC_BUTTON_S4_DEL_GROUP, &CgppcDlg::OnBnClickedButtonS4DelGroup)
	
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CgppcDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_SERIAL_ALL_CONNECT, &CgppcDlg::OnBnClickedButtonSerialAllConnect)
	ON_BN_CLICKED(IDC_BUTTON_SERIAL_ALL_DISCONNECT, &CgppcDlg::OnBnClickedButtonSerialAllDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_DELAY_CALC, &CgppcDlg::OnBnClickedButtonDelayCalc)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CgppcDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CgppcDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CgppcDlg 메시지 처리기

BOOL CgppcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	
	for (int i = 0; i < kSerialGppCount; i++)
	{
		power_controller[i].Init(i, (i % 6 == 5) ? 1 : 2, PowerContollerCB, this);
	}

	for (int i = 0; i < kSerialLoadcellCount; i++)
	{
		loadcell[i].Init(i/*index = id*/, LoadcellReceiveCB, this);
	}

	zdongle.Init(ZDongleReceiveCB, this);

	InitListCtrls();
	InitComboSerial();

	carrier_speed = _T("0");
	for (int i = 0 ; i < kStepMax; i++)
	{
		distance[i] = _T("0");
		delay[i] = _T("0");
	}

	LoadJSonOfSteps();
	UpdateData(FALSE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CgppcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CgppcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CgppcDlg::OnOK()
{
	Exit();
	CDialogEx::OnOK();
}

void CgppcDlg::OnBnClickedCancel()
{
	Exit();
	CDialogEx::OnCancel();
}

void CgppcDlg::Exit()
{
	if (test_running)
	{
		test_running = FALSE;
		TestStart(FALSE);
	}
	zdongle.DisconnectSerial();
	for (int i = 0; i < kSerialLoadcellCount; i++) { loadcell[i].DisconnectSerial(); }
	for (int i = 0; i < kSerialGppCount; i++) { power_controller[i].Deinit(); }
}

BOOL CgppcDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		// Ignore Enter, ESC Key
		if (VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam) { return TRUE; }
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CgppcDlg::InitComboSerial()
{
	HKEY h_CommKey;
	DWORD Size = MAX_PATH;
	TCHAR i_str[MAX_PATH];

	for (int i = 0; i < kSerialGppCount; i++) { combo_gpp[i].Clear(); }
	for (int i = 0; i < kSerialLoadcellCount; i++) { combo_loadcell[i].Clear(); }
	combo_dongle.Clear();

	LONG Reg_Ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ | KEY_QUERY_VALUE, &h_CommKey);
	if (Reg_Ret != ERROR_SUCCESS) { return; }

	for (int i = 0; Reg_Ret == ERROR_SUCCESS; i++)
	{
		Reg_Ret = RegEnumValue(h_CommKey, i, i_str, &Size, NULL, NULL, NULL, NULL);
		if (Reg_Ret == ERROR_SUCCESS)
		{
			DWORD dwType, dwSize = MAX_PATH;
			TCHAR szBuffer[MAX_PATH];

			RegQueryValueEx(h_CommKey, i_str, 0, &dwType, (LPBYTE)szBuffer, &dwSize);
			CString strPort(szBuffer);

			for (int i = 0; i < kSerialGppCount; i++) { combo_gpp[i].AddString(strPort); }
			for (int i = 0; i < kSerialLoadcellCount; i++) { combo_loadcell[i].AddString(strPort); }
			combo_dongle.AddString(strPort);
		}

		Size = MAX_PATH;
	}

	RegCloseKey(h_CommKey);

	for (int i = 0; i < kSerialGppCount; i++) { combo_gpp[i].SetCurSel(0); }
	for (int i = 0; i < kSerialLoadcellCount; i++) { combo_loadcell[i].SetCurSel(0); }
	combo_dongle.SetCurSel(0);
}

/***** Begin UI - Step ******/
void CgppcDlg::InitListCtrls()
{
	for (int i = 0; i < kStepMax; i++)
	{
		listctrl_of_steptable[i].SetExtendedStyle(listctrl_of_steptable[i].GetExtendedStyle() | LVS_EX_CHECKBOXES);

		image_in_steptable[i].Create(16, 16, ILC_COLOR16 | ILC_MASK, 1, 0);
		int imageindex = CGridColumnTraitImage::AppendStateImages(listctrl_of_steptable[i], image_in_steptable[i]);

		listctrl_of_steptable[i].SetImageList(&image_in_steptable[i], LVSIL_SMALL);

		// Give better margin to editors
		listctrl_of_steptable[i].SetCellMargin(1.2);
		CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;
		listctrl_of_steptable[i].SetDefaultRowTrait(pRowTrait);
		listctrl_of_steptable[i].EnableVisualStyles(true);

		// Create Columns
		int col = 0;
		listctrl_of_steptable[i].InsertHiddenLabelColumn();	// Requires one never uses column 0

		// GROUP
		listctrl_of_steptable[i].InsertColumnTrait(++col, _T("GROUP"), LVCFMT_CENTER, 52, -1, new CGridColumnTraitText);

		// M1 ~ M22
		for (int j = 1; j <= 22; j++)
		{
			CGridColumnTraitImage* trait = new CGridColumnTraitImage();
			trait->AddImageIndex(imageindex, _T(""), FALSE);		// Unchecked (and not editable)
			trait->AddImageIndex(imageindex + 1, _T(""), TRUE);	// Checked (and editable)
			trait->SetToggleSelection(true);
			CString numbering;
			numbering.Format(_T("M%d"), j);
			listctrl_of_steptable[i].InsertColumnTrait(++col, numbering, LVCFMT_CENTER, 36, -1, trait);
			
		}

		// START - END - INTERVAL
		listctrl_of_steptable[i].InsertColumnTrait(++col, _T("Start"), LVCFMT_RIGHT, 48, -1, new CGridColumnTraitEdit);
		listctrl_of_steptable[i].InsertColumnTrait(++col, _T("End"), LVCFMT_RIGHT, 48, -1, new CGridColumnTraitEdit);
		listctrl_of_steptable[i].InsertColumnTrait(++col, _T("Interval"), LVCFMT_RIGHT, 54, -1, new CGridColumnTraitEdit);
	}
}

void CgppcDlg::AddStepTableRow(const int step_number, const CString start_value, const CString end_value, const CString interval)
{
	int row_count = listctrl_of_steptable[step_number].GetItemCount();
	int row = listctrl_of_steptable[step_number].InsertItem(row_count + 1, _T(""));

	int col = 0;
	listctrl_of_steptable[step_number].SetItemData(row, col++);
	CString group_number;
	group_number.Format(_T("%d"), row + 1);
	listctrl_of_steptable[step_number].SetItemText(row, col++, group_number);

	for (int j = 0; j < 22; j++)
	{
		listctrl_of_steptable[step_number].SetCellImage(row, col++, FALSE);
	}

	listctrl_of_steptable[step_number].SetItemText(row, col++, start_value);
	listctrl_of_steptable[step_number].SetItemText(row, col++, end_value);
	listctrl_of_steptable[step_number].SetItemText(row, col, interval);
}

void CgppcDlg::AddStepTableRow(const int step_number, StepGroup* step_group)
{
	int row_count = listctrl_of_steptable[step_number].GetItemCount();
	int row = listctrl_of_steptable[step_number].InsertItem(row_count + 1, _T(""));

	int col = 0;
	listctrl_of_steptable[step_number].SetItemData(row, col++);
	CString group_number;
	group_number.Format(_T("%d"), row + 1);
	listctrl_of_steptable[step_number].SetItemText(row, col++, group_number);

	vector<int> port = step_group->GetOutputPorts();
	sort(port.begin(), port.end());
	
	int pos = 0;
	for (int j = 0; j < 22; j++)
	{
		BOOL check(FALSE);
		if (pos < port.size() && (j+1) == port[pos])
		{
			check = TRUE;
			pos++;
		}

		listctrl_of_steptable[step_number].SetCellImage(row, col++, check);
	}

	CString start_current, end_current, interval;
	start_current = step_group->GetStartCurrentText().c_str();
	end_current = step_group->GetEndCurrentText().c_str();
	interval = step_group->GetIntervalText().c_str();

	listctrl_of_steptable[step_number].SetItemText(row, col++, start_current);
	listctrl_of_steptable[step_number].SetItemText(row, col++, end_current);
	listctrl_of_steptable[step_number].SetItemText(row, col, interval);
}

void CgppcDlg::RemoveStepTableRow(const int step_number)
{
	int select_row = listctrl_of_steptable[step_number].GetNextItem(-1, LVNI_SELECTED);
	if (select_row < 0) { return; }

	listctrl_of_steptable[step_number].DeleteItem(select_row);

	// re-numbering
	int row_count = listctrl_of_steptable[step_number].GetItemCount();
	if (select_row < row_count)
	{
		const int col = 1;	// group number
		for (int row = select_row; row < row_count; row++)
		{
			CString group_number;
			group_number.Format(_T("%d"), row + 1);
			listctrl_of_steptable[step_number].SetItemText(row, col, group_number);
		}
	}
}

afx_msg void CgppcDlg::LoadJSonOfSteps()
{
	for (int i = 0; i < kStepMax; i++)
	{ 	
		listctrl_of_steptable[i].DeleteAllItems();
	}	// Remove All

	ifstream jsonFile(kJsonfile);

	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	Json::Value jsonValue;

	JSONCPP_STRING errs;
	bool ok = parseFromStream(builder, jsonFile, &jsonValue, &errs);
	if (!ok) { return; }

	const char key_stepinfo[] = "step_info";
	for (int step = kStep1; step < kStepMax; step++)
	{
		string str_step("step_" + to_string(step + 1));

		const char* key_step = str_step.c_str();

		int group_count = jsonValue[key_stepinfo][key_step].size();
		for (int index_group = 0; index_group < group_count; index_group++)
		{
			string start = jsonValue[key_stepinfo][key_step][index_group].get("start", "").asString();
			string end = jsonValue[key_stepinfo][key_step][index_group].get("end", "").asString();
			string interval = jsonValue[key_stepinfo][key_step][index_group].get("interval", "").asString();
			
			int member_count = jsonValue[key_stepinfo][key_step][index_group]["member"].size();
			vector<int> member;
			for (int index_member = 0; index_member < member_count; index_member++)
			{
				int member_number = jsonValue[key_stepinfo][key_step][index_group]["member"][index_member].asInt();
				member.push_back(member_number);
			}
						
			StepGroup step_group(start, end, interval, member);
			AddStepTableRow(step, &step_group);
		}

		int distance = jsonValue["delay_info"]["distance"][step].asInt();
		this->distance[step].Format(_T("%d"), distance);
	}

	int speed = jsonValue["delay_info"].get("carrier_speed", 0).asInt();
	this->carrier_speed.Format(_T("%d"), speed);
	UpdateData(FALSE);
	OnBnClickedButtonDelayCalc();
}

void CgppcDlg::SaveJSonOfSteps()
{
	ofstream jsonFile;
	jsonFile.open(kJsonfile);

	Json::Value root;
	
	Json::Value step_info;
	Json::Value delay_info;

	UpdateData(TRUE);
	for (int step = kStep1; step < kStepMax; step++)
	{	
		string str_step("step_" + to_string(step + 1));
		int row_count = listctrl_of_steptable[step].GetItemCount();
		for (int row = 0; row < row_count; row++)
		{
			Json::Value group_json;
			int col = 1;
			CString group_number = listctrl_of_steptable[step].GetItemText(row, col++);

			int mem_count(0);
			for (int j = 0; j < 22; j++)
			{
				BOOL m = listctrl_of_steptable[step].GetCellImage(row, col++);
				if (m) { group_json["member"][mem_count++] = j + 1; }
			}

			CString start_current = listctrl_of_steptable[step].GetItemText(row, col++);
			CString end_current   = listctrl_of_steptable[step].GetItemText(row, col++);
			CString interval	  = listctrl_of_steptable[step].GetItemText(row, col);

			group_json["start"] = (string)CT2CA(start_current);
			group_json["end"] = (string)CT2CA(end_current);
			group_json["interval"] = (string)CT2CA(interval);

			step_info[str_step.c_str()].append(group_json);
		}

		if (this->distance[step].GetLength() < 1) { this->distance[step] = _T("0"); }
		int distance = _ttoi(this->distance[step]);
		delay_info["distance"].append(distance);
	}

	if (this->carrier_speed.GetLength() < 1) { this->carrier_speed = _T("0"); }
	int speed = _ttoi(this->carrier_speed);
	delay_info["carrier_speed"] = speed;
	
	UpdateData(FALSE);

	root["step_info"] = step_info;
	root["delay_info"] = delay_info;

	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = "	"; // tab
	unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

	writer->write(root, &jsonFile);
	jsonFile.close();
}

void CgppcDlg::UpdateStepGroups()
{
	for (int step = kStep1; step < kStepMax; step++)
	{
		step_groups[step].clear();
		int row_count = listctrl_of_steptable[step].GetItemCount();
		for (int row = 0; row < row_count; row++)
		{
			int col = 1;
			CString group_number = listctrl_of_steptable[step].GetItemText(row, col++);

			vector<int> member;
			for (int j = 0; j < 22; j++)
			{
				BOOL m = listctrl_of_steptable[step].GetCellImage(row, col++);
				if (m) { member.push_back(j + 1); }
			}

			CString start_current = listctrl_of_steptable[step].GetItemText(row, col++);
			CString end_current = listctrl_of_steptable[step].GetItemText(row, col++);
			CString interval = listctrl_of_steptable[step].GetItemText(row, col);

			string start_text = (string)CT2CA(start_current);
			string end_text = (string)CT2CA(end_current);
			string interval_text = (string)CT2CA(interval);

			if (this->delay[step].GetLength() < 1) { this->delay[step] = _T("0"); }
			int delay = _ttoi(this->delay[step]);

			StepGroup step_group(start_text, end_text, interval_text, member, delay);
			step_groups[step].push_back(step_group);
		}
	}
}

void CgppcDlg::OnBnClickedButtonS1AddGroup() { AddStepTableRow(kStep1); }
void CgppcDlg::OnBnClickedButtonS2AddGroup() { AddStepTableRow(kStep2); }
void CgppcDlg::OnBnClickedButtonS3AddGroup() { AddStepTableRow(kStep3); }
void CgppcDlg::OnBnClickedButtonS4AddGroup() { AddStepTableRow(kStep4); }

void CgppcDlg::OnBnClickedButtonS1DelGroup() { RemoveStepTableRow(kStep1); }
void CgppcDlg::OnBnClickedButtonS2DelGroup() { RemoveStepTableRow(kStep2); }
void CgppcDlg::OnBnClickedButtonS3DelGroup() { RemoveStepTableRow(kStep3); }
void CgppcDlg::OnBnClickedButtonS4DelGroup() { RemoveStepTableRow(kStep4); }

/****** End UI - Step ******/

void CgppcDlg::OnBnClickedButtonDelayCalc()
{
	UpdateData(TRUE);
	
	if (this->carrier_speed.GetLength() < 1) { this->carrier_speed = _T("0"); }
	int speed = _ttoi(this->carrier_speed);

	
	for (int i = 0; i < kStepMax; i++)
	{
		if (this->distance[i].GetLength() < 1) { this->distance[i] = _T("0"); }
		int distance = _ttoi(this->distance[i]);
		

		unsigned int delay = (speed > 0) ? ((double)distance / (double)speed * 1000) : 0;
		this->delay[i].Format(_T("%d"), delay);
	}
	
	UpdateData(FALSE);
}

void CgppcDlg::OnBnClickedButtonSerialAllConnect()
{
	const int ids[] = {
		IDC_BUTTON_SERIAL1, IDC_BUTTON_SERIAL2, IDC_BUTTON_SERIAL3, IDC_BUTTON_SERIAL4, IDC_BUTTON_SERIAL5, IDC_BUTTON_SERIAL6,
		IDC_BUTTON_SERIAL7, IDC_BUTTON_SERIAL8, IDC_BUTTON_SERIAL9, IDC_BUTTON_SERIAL10, IDC_BUTTON_SERIAL11, IDC_BUTTON_SERIAL12,
		IDC_BUTTON_SERIAL13, IDC_BUTTON_SERIAL14, IDC_BUTTON_SERIAL15,
	};

	const int port[] = {
		IDC_COMBO_SERIAL1, IDC_COMBO_SERIAL2, IDC_COMBO_SERIAL3, IDC_COMBO_SERIAL4, IDC_COMBO_SERIAL5, IDC_COMBO_SERIAL6,
		IDC_COMBO_SERIAL7, IDC_COMBO_SERIAL8, IDC_COMBO_SERIAL9, IDC_COMBO_SERIAL10, IDC_COMBO_SERIAL11, IDC_COMBO_SERIAL12,
		IDC_COMBO_SERIAL13, IDC_COMBO_SERIAL14, IDC_COMBO_SERIAL15,
	};

	const int count = sizeof(ids) / sizeof(*ids);
	for (int i = 0; i < count; i++)
	{
		BOOL result(FALSE);
		CString strPort;
		GetDlgItem(port[i])->GetWindowTextW(strPort);
		
		if (i < kSerialGppCount)
		{
			if (!power_controller[i].IsOpen())
			{
				result = power_controller[i].ConnectSerial(strPort);
				if (result)
				{
					// 연결되면 모든 연결된 포트에 ALLOUTON 과 VSET 명령을 날린다
					power_controller[i].SendCommand((CString)_T("ALLOUTON\r\n"));
					for (int j = 0; j < power_controller[i].GetPortCount(); j++)
					{
						power_controller[i].SendCommand(j, (CString)_T("VSET"), (CString)_T("32.000"));
						power_controller[i].SendCommand(j, (CString)_T("ISET"), (CString)_T("0.300"));
					}
				}
			}
		}
		else if (i < kSerialGppCount + kSerialLoadcellCount)
		{
			if (!loadcell[i - kSerialGppCount].IsOpen())
			{
				result = loadcell[i - kSerialGppCount].ConnectSerial(strPort);
			}
		}
		else
		{
			if (!zdongle.IsOpen())
			{
				result = zdongle.ConnectSerial(strPort);
			}
		}
		
		//CT2CA pszConvertedAnsiString(strPort);
		//cout << pszConvertedAnsiString << " : " << result <<  endl;
		if (result) { GetDlgItem(ids[i])->SetWindowTextW(_T("연결됨")); }
	}
}

void CgppcDlg::OnBnClickedButtonSerialAllDisconnect()
{
	const int ids[] = {
		IDC_BUTTON_SERIAL1, IDC_BUTTON_SERIAL2, IDC_BUTTON_SERIAL3, IDC_BUTTON_SERIAL4, IDC_BUTTON_SERIAL5, IDC_BUTTON_SERIAL6,
		IDC_BUTTON_SERIAL7, IDC_BUTTON_SERIAL8, IDC_BUTTON_SERIAL9, IDC_BUTTON_SERIAL10, IDC_BUTTON_SERIAL11, IDC_BUTTON_SERIAL12,
		IDC_BUTTON_SERIAL13, IDC_BUTTON_SERIAL14, IDC_BUTTON_SERIAL15,
	};

	const int count = sizeof(ids) / sizeof(*ids);
	for (int i = 0; i < count; i++)
	{
		BOOL result(FALSE);
		if (i < kSerialGppCount)
		{
			result = power_controller[i].DisconnectSerial();
		}
		else if (i < kSerialGppCount + kSerialLoadcellCount)
		{
			result = loadcell[i - kSerialGppCount].DisconnectSerial();
		}
		else
		{
			result = zdongle.DisconnectSerial();
		}

		if (result) { GetDlgItem(ids[i])->SetWindowTextW(_T("끊어짐")); }
	}
}

void CgppcDlg::OnBnClickedButtonTest()
{
	test_running = !test_running;
	TestStart(test_running);
}

void CgppcDlg::TestStart(BOOL start)
{
	const int ids[] = {
		IDC_LIST_1, IDC_LIST_2, IDC_LIST_3, IDC_LIST_4,
		IDC_BUTTON_S1_ADD_GROUP, IDC_BUTTON_S2_ADD_GROUP, IDC_BUTTON_S3_ADD_GROUP, IDC_BUTTON_S4_ADD_GROUP,
		IDC_BUTTON_S1_DEL_GROUP, IDC_BUTTON_S2_DEL_GROUP, IDC_BUTTON_S3_DEL_GROUP, IDC_BUTTON_S4_DEL_GROUP,

		IDC_EDIT_SPEED, IDC_BUTTON_DELAY_CALC,
		IDC_EDIT_DISTANCE1, IDC_EDIT_DISTANCE2, IDC_EDIT_DISTANCE3, IDC_EDIT_DISTANCE4,
		IDC_EDIT_DELAY1, IDC_EDIT_DELAY2, IDC_EDIT_DELAY3, IDC_EDIT_DELAY4,

		IDC_COMBO_SERIAL1, IDC_COMBO_SERIAL2, IDC_COMBO_SERIAL3, IDC_COMBO_SERIAL4, IDC_COMBO_SERIAL5, IDC_COMBO_SERIAL6,
		IDC_COMBO_SERIAL7, IDC_COMBO_SERIAL8, IDC_COMBO_SERIAL9, IDC_COMBO_SERIAL10, IDC_COMBO_SERIAL11, IDC_COMBO_SERIAL12,
		IDC_COMBO_SERIAL13, IDC_COMBO_SERIAL14, IDC_COMBO_SERIAL15,

		IDC_BUTTON_SERIAL1, IDC_BUTTON_SERIAL2, IDC_BUTTON_SERIAL3, IDC_BUTTON_SERIAL4, IDC_BUTTON_SERIAL5, IDC_BUTTON_SERIAL6,
		IDC_BUTTON_SERIAL7, IDC_BUTTON_SERIAL8, IDC_BUTTON_SERIAL9, IDC_BUTTON_SERIAL10, IDC_BUTTON_SERIAL11, IDC_BUTTON_SERIAL12,
		IDC_BUTTON_SERIAL13, IDC_BUTTON_SERIAL14, IDC_BUTTON_SERIAL15,

		IDC_BUTTON_SERIAL_ALL_CONNECT, IDC_BUTTON_SERIAL_ALL_DISCONNECT
	};

	if (start)
	{
		trycount = 0;
		// update step_groups
		UpdateStepGroups();
		step_linear.clear();
		for (int step = kStep1; step < kStepMax; step++)
		{
			int row_count = step_groups[step].size();
			for (int row = 0; row < row_count; row++)
			{
				step_linear.push_back(step_groups[step][row]);
			}
		}

		csv.Open();
		string row;
		row = "index";
		row += ",time";
		row += ",m1,m2,m3,m4,m5,m6,m7,m8,m9,m10,m11";
		row += ",m12,m13,m14,m15,m16,m17,m18,m19,m20,m21,m22";
		row += ",w1,w2,w3,w4,w5,w6";
		row += ",w7,w8,w9,w10,w11,w12";
		row += "\n";
		csv.Write(row);
		zStatus = kEventZReady;
	}
	else
	{
		zStatus = kEventZReady;
		for (int i = 0; i < kSerialGppCount; i++)
		{
			for (int j = 0; j < power_controller[i].GetPortCount(); j++)
			{
				power_controller[i].ResetSchedule(j);
			}
		}
		csv.Close();
	}

	const int count = sizeof(ids) / sizeof(*ids);
	for (int i = 0; i < count; i++) { GetDlgItem(ids[i])->EnableWindow(!test_running); }
	
	GetDlgItem(IDC_BUTTON_TEST)->SetWindowTextW(test_running ? _T("시험 중지") : _T("시험 시작"));
}

void CgppcDlg::TestAddSchedule()
{
	static CString kISET = _T("ISET");
	static CString kDelay = _T("DELAY");

	int length = step_linear.size();
	for (int i = 0; i < length; i++)
	{
		float currnet = step_linear[i].GetCurrnet();
		int delayms = step_linear[i].GetDelayms();

		vector<int> port_number = step_linear[i].GetOutputPorts();
		for (int j = 0; j < port_number.size(); j++)
		{
			int ctrl_index(0);
			int port_index(0);

			int num = port_number[j];
			if (num < 12)
			{
				num--;
				ctrl_index = num / 2;
				port_index = num % 2;
			}
			else
			{
				num -= 12;
				ctrl_index = num / 2 + 6;
				port_index = num % 2;
			}

			if (power_controller[ctrl_index].IsOpen())
			{
				CString iset_value, delay_value;
				iset_value.Format(_T("%.2f"), currnet);
				delay_value.Format(_T("%d"), delayms);
				power_controller[ctrl_index].AddSchedule(port_index, kISET, iset_value);
				power_controller[ctrl_index].AddSchedule(port_index, kDelay, delay_value);
			}

			//cout << ctrl_index << "   " << port_index << endl;
			//cout << "try_idx: " << trycount << " group_idx: " << i + 1 << " currnet: " << currnet << " delay: " << delayms << endl;
		}
	}
}

BOOL CgppcDlg::TestNextGain()
{
	BOOL gain(FALSE);
	int length = step_linear.size();
	for (int i = 0; i < length && !gain; i++)
	{
		if (step_linear[i].IsOver() == FALSE)
		{
			step_linear[i].Gain();
			gain = TRUE;

			for (int j = 0; j < i; j++)
			{
				step_linear[j].ResetCurrent();
			}
		}
	}

	return gain;
}

void CgppcDlg::OnBnClickedButtonLoad()
{
	LoadJSonOfSteps();
}

void CgppcDlg::OnBnClickedButtonSave()
{
	SaveJSonOfSteps();
}

void CgppcDlg::ZDongleReceiveCB(void* data, void* context)
{
	CgppcDlg* ctx = (CgppcDlg*)context;
	if (!ctx->test_running) { return; }

	static ULONGLONG one, two;
	ZDongleData* zdata = (ZDongleData*)data;
	
	//cout << "id : " << zdata->GetId() << endl;
	switch (zdata->GetId())
	{
	case 1:	
		one = GetTickCount64();
		break;
	case 2:
		two = GetTickCount64();
		break;
	case 4:	// end
		one = two = 0;
		cout << endl << "Z : finish" << endl;
		::PostMessage(ctx->m_hWnd, WM_USEREVENT, (WPARAM)kEventZEnd, (LPARAM)NULL);
		break;
	}

	if (one > 0 && two > 0)
	{
		if (one < two)	// start
		{
			cout << endl << "Z : start" << endl;
			::PostMessage(ctx->m_hWnd, WM_USEREVENT, (WPARAM)kEventZStart, (LPARAM)NULL);
		}
		else if (one > two)	// ready
		{
			cout << endl << "Z : ready" << endl;
			::PostMessage(ctx->m_hWnd, WM_USEREVENT, (WPARAM)kEventZReady, (LPARAM)NULL);
		}
		else // error
		{
			cout << endl << "Z : error" << endl;
			//cout << "err" << endl;
		}

		one = two = 0;
	}
}

void CgppcDlg::LoadcellReceiveCB(void* data, void* context)
{
	CgppcDlg* ctx = (CgppcDlg*)context;
	if (!ctx->test_running || ctx->zStatus != kEventZStart) { return; }

	int id = *(int*)data;
	if (id == 0)
	{
		::PostMessage(ctx->m_hWnd, WM_USEREVENT, (WPARAM)kEventReceiveLoadcell, (LPARAM)NULL);
	}
}

void CgppcDlg::PowerContollerCB(void* data, void* context)
{
	CgppcDlg* ctx = (CgppcDlg*)context;
	if (!ctx->test_running) { return; }
}

afx_msg LRESULT CgppcDlg::OnUserEvent(WPARAM wParam, LPARAM lParam)
{
	int event = (int)wParam;

	if (event == kEventReceiveLoadcell)
	{
		if (zStatus != kEventZStart) { return 1; }
		//cout << "loadcell" << endl;

		SYSTEMTIME st;
		GetLocalTime(&st);

		string index = to_string(trycount) + ",";
		CString iset;
		for (int i = 0; i < kSerialGppCount; i++)
		{
			for (int j = 0; j < power_controller[i].GetPortCount(); j++)
			{
				CString c = power_controller[i].GetCurrent(j);
				iset += c;
				iset += _T(",");
			}
		}

		char buf[512] = { 0, };
		sprintf_s(buf, "%04d.%02d.%02d %02d:%02d:%02d.%d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		string time = buf;

		//(string)CT2CA(iset.operator LPCWSTR());
		CT2CA isetvalue(iset);
		string data = ",";
		data += isetvalue;

		string weight;// = ",";
		for (int i = 0; i < kSerialLoadcellCount; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				int w = loadcell[i].GetWeight(j);
				weight += to_string(w);
				if (j < 5) { weight += ","; }
			}

			if (i < kSerialLoadcellCount - 1) { weight += ","; }
		}

		string full = index + time + data + weight;
		cout << full << endl;
		if (csv.isOpen())
		{
			csv.Write(full + "\n");
		}
	}
	else if (event == kEventZStart)
	{
		zStatus = event;
		trycount++;
		TestAddSchedule();
		for (int i = 0; i < kSerialGppCount; i++)
		{
			for (int j = 0; j < power_controller[i].GetPortCount(); j++)
			{
				if (power_controller[i].IsOpen())
				{
					power_controller[i].StartScheduler(j);
				}
			}
		}
	}
	else if (event == kEventZReady)
	{
		zStatus = event;
		BOOL result = TestNextGain();
		if (!result)
		{
			cout << "Z : END" << endl;
			test_running = FALSE;
			TestStart(test_running);
		}

		for (int i = 0; i < 6; i++)
		{
			BOOL result = power_controller[i].IsOpen();
			if (result)
			{
				// 연결되면 모든 연결된 포트에 ALLOUTON 과 VSET 명령을 날린다
				for (int j = 0; j < power_controller[i].GetPortCount(); j++)
				{
					power_controller[i].SendCommand(j, (CString)_T("ISET"), (CString)_T("0.300"));
				}
			}
		}
	}
	else if (event == kEventZEnd)
	{
		if (zStatus == kEventZEnd) { return 1; }
		zStatus = event;

		for (int i = 0; i < kSerialGppCount; i++)
		{
			BOOL result = power_controller[i].IsOpen();
			if (result)
			{
				power_controller[i].ResetSchedule();
				// 연결되면 모든 연결된 포트에 ALLOUTON 과 VSET 명령을 날린다
				for (int j = 0; j < power_controller[i].GetPortCount(); j++)
				{
					power_controller[i].SendCommand(j, (CString)_T("ISET"), (CString)_T("0.000"));
				}
			}
		}
	}
	
	return 1;
}