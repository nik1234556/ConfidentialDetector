// ConfidentialDetectorDlg.cpp : файл реализации
//

#include "pch.h"
#include "framework.h"
#include "ConfidentialDetector.h"
#include "ConfidentialDetectorDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <sstream>
#include "Detector.h"        

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Диалоговое окно CAboutDlg используется для описания сведений о приложении
class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
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

// Диалоговое окно CConfidentialDetectorDlg

CConfidentialDetectorDlg::CConfidentialDetectorDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_CONFIDENTIALDETECTOR_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConfidentialDetectorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_editText);
    DDX_Control(pDX, IDC_LIST1, m_listResults);
}

BEGIN_MESSAGE_MAP(CConfidentialDetectorDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CConfidentialDetectorDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CConfidentialDetectorDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &CConfidentialDetectorDlg::OnBnClickedButton3)
END_MESSAGE_MAP()

BOOL CConfidentialDetectorDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);
    return TRUE;
}

void CConfidentialDetectorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CConfidentialDetectorDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CConfidentialDetectorDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CConfidentialDetectorDlg::OnBnClickedButton1()
{
    CString strText;
    m_editText.GetWindowTextW(strText);
    CT2A ascii(strText, CP_UTF8);
    std::string text(ascii);

    m_listResults.ResetContent();
    m_lastMatches = analyzeText(text);

    if (m_lastMatches.empty()) {
        m_listResults.AddString(L"Конфиденциальные данные не найдены.");
        MessageBox(L"Конфиденциальные данные не найдены.", L"Результат", MB_OK | MB_ICONINFORMATION);
    }
    else {
        for (size_t i = 0; i < m_lastMatches.size(); ++i) {
            const auto& m = m_lastMatches[i];

            // Преобразуем тип в юникод вручную
            CStringW typeW;
            CStringW valueW;

            if (m.type == "Банковская карта") {
                typeW = L"Банковская карта";
            }
            else if (m.type == "Паспортные данные") {
                typeW = L"Паспортные данные";
            }
            else if (m.type == "Номер договора") {
                typeW = L"Номер договора";
            }
            else {
                typeW = CA2W(m.type.c_str(), CP_UTF8);
            }

            valueW = CA2W(m.value.c_str(), CP_UTF8);

            CString item;
            item.Format(L"%d. %s\n   Значение: \"%s\"\n   Позиция: %d, длина: %d",
                (int)(i + 1), (LPCWSTR)typeW, (LPCWSTR)valueW, m.position, m.length);
            m_listResults.AddString(item);
        }
        CString msg;
        msg.Format(L"Обнаружено %d потенциально конфиденциальных фрагментов.", (int)m_lastMatches.size());
        MessageBox(msg, L"Внимание", MB_OK | MB_ICONWARNING);
    }
}

void CConfidentialDetectorDlg::OnBnClickedButton2()
{
    CFileDialog dlg(TRUE, L"txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        L"Текстовые файлы (*.txt)|*.txt|Все файлы (*.*)|*.*||");
    if (dlg.DoModal() == IDOK) {
        CString filePath = dlg.GetPathName();
        std::ifstream file(CW2A(filePath), std::ios::binary);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string content = buffer.str();
            m_editText.SetWindowTextW(CA2W(content.c_str(), CP_UTF8));
            file.close();
        }
        else {
            MessageBox(L"Не удалось открыть файл.", L"Ошибка", MB_OK | MB_ICONERROR);
        }
    }
}

void CConfidentialDetectorDlg::OnBnClickedButton3()
{
    m_editText.SetWindowTextW(L"");
    m_listResults.ResetContent();
    m_lastMatches.clear();
}
