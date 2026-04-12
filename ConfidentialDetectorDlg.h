
// ConfidentialDetectorDlg.h: файл заголовка
//

#pragma once
#include <vector>
#include "Detector.h"

// Диалоговое окно CConfidentialDetectorDlg
class CConfidentialDetectorDlg : public CDialogEx
{
// Создание
public:
	CConfidentialDetectorDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIDENTIALDETECTOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	DECLARE_MESSAGE_MAP()

public:
	CEdit m_editText;
	CListBox m_listResults;
private:
	std::vector<Match> m_lastMatches;
};
