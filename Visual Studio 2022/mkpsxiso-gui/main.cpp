/*
*
*	Megan Grass
*	October 13, 2024
*
*/

#include "main.h"

void DragAndDrop(StrVecW Files)
{
    Standard_String Str;

    Standard_FileSystem FS;

    for (std::size_t i = 0; i < Files.size(); i++)
    {
		StringW FileExtension = FS.GetFileExtension(Files[i]).wstring();

        Str.ToUpper(FileExtension);

		if (FileExtension == L".ISO" || FileExtension == L".BIN")
		{
            DumpISO->Filename = Files[i];
			DumpISO->b_CreateXml = true;
            DumpISO->OutputPath = Window->GetDirectory(DumpISO->Filename);
            DumpISO->OutputPath += "\\" + DumpISO->Filename.stem().string();
            DumpISO->XmlFilename = DumpISO->OutputPath / DumpISO->Filename.stem().string() += ".xml";

            SendMessage(Window->GetHandle(), WM_COMMAND, IDM_DUMPSXISO, 0);
            SetDlgItemText(DumpISO->hWnd, IDC_DUMPSXISO_FILE, Files[i].c_str());
            SetDlgItemText(DumpISO->hWnd, IDC_DUMPSXISO_OUTPUT, DumpISO->OutputPath.c_str());
            SetDlgItemText(DumpISO->hWnd, IDC_DUMPSXISO_XML_FILE, DumpISO->XmlFilename.c_str());
            SendMessage(DumpISO->hWnd, WM_COMMAND, IDC_DUMPSXISO_DUMP, 0);
		}

        if (FileExtension == L".XML")
        {
			MakeISO->Filename = Files[i];
			MakeISO->b_Overwrite = true;
			MakeISO->b_OverrideImageName = true;
			MakeISO->b_OverrideCueName = true;
            MakeISO->b_OverrideVolumeLabel = false;
			MakeISO->b_GenLBA = false;
			MakeISO->b_GenHeader = false;
			MakeISO->b_GenFilecode = false;
			MakeISO->b_GenRE1LBA = false;
			MakeISO->b_GenRE2LBA = false;
			MakeISO->b_GenRETEXT = false;
            MakeISO->b_GenNoISO = false;

            MakeISO->BinFilename = Window->GetDirectory(MakeISO->Filename) / MakeISO->Filename.stem().string() += ".bin";
			MakeISO->CueFilename = Window->GetDirectory(MakeISO->Filename) / MakeISO->Filename.stem().string() += ".cue";

            SendMessage(Window->GetHandle(), WM_COMMAND, IDM_MKPSXISO, 0);
            SetDlgItemText(MakeISO->hWnd, IDC_MKPSXISO_FILE, MakeISO->Filename.c_str());
            SetDlgItemText(MakeISO->hWnd, IDC_MKPSXISO_IMAGE, MakeISO->BinFilename.c_str());
			SetDlgItemText(MakeISO->hWnd, IDC_MKPSXISO_CUE, MakeISO->CueFilename.c_str());
			SendMessage(MakeISO->hWnd, WM_COMMAND, IDC_MKPSXISO_MAKE, 0);
        }
    }

    Window->EmptyDroppedFiles();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    WinSystem->InitCommonControls();
    WinSystem->SetTimer(30);

    Window->PresetCommandLine(lpCmdLine);
    Window->PresetStyle(WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
    Window->PresetStyleEx(WS_EX_ACCEPTFILES | WS_EX_APPWINDOW);
    Window->PresetClassName(hInstance, IDR_WINDOW);
    Window->PresetWindowName(hInstance, IDS_APP_TITLE);
    Window->PresetWindowColor(0, 0, 0);
    Window->PresetIcon(hInstance, IDI_WINDOW);
    Window->PresetIconSm(hInstance, IDI_SMALL);
    Window->PresetMenu(hInstance, IDR_WINDOW);
    Window->PresetStatusBar(3);
    Window->PresetOptions(WindowOptions::StatusBar);
    Window->Create(1920, 1080, hInstance, nCmdShow, WndProc);
    Window->SetAcceleratorTable(hInstance, IDR_WINDOW);
    Window->Status(0, "%ws", VER_INTERNAL_NAME_STR);
    Window->Status(1, "v%ws", VER_PRODUCT_VERSION_STR);
    Window->Status(2, "Status: Mode Not Set\0");

    Main->hWnd = nullptr;

    DumpISO->hWnd = nullptr;
    DumpISO->b_CreateXml = true;
    DumpISO->b_SortByDir = false;
    DumpISO->Encoding = AudioEncoding::WAVE;
    DumpISO->b_PathTable = false;

    MakeISO->hWnd = nullptr;
    MakeISO->b_Overwrite = true;
	MakeISO->b_OverrideImageName = false;
	MakeISO->b_OverrideCueName = false;
	MakeISO->b_OverrideVolumeLabel = false;
    MakeISO->b_GenLBA = false;
    MakeISO->b_GenHeader = false;
    MakeISO->b_GenFilecode = false;
    MakeISO->b_GenRE1LBA = false;
    MakeISO->b_GenRE2LBA = false;
    MakeISO->b_GenRETEXT = false;
    MakeISO->b_GenNoISO = false;
    MakeISO->b_GenNoCDXA = false;
    MakeISO->b_RebuildXML = false;

    CreateDialog(Window->GetInstance(), MAKEINTRESOURCE(IDD_MAIN), Window->GetHandle(), (DLGPROC)SplashProc);

	HWND hButton = GetDlgItem(Main->hWnd, IDC_MAIN_DUMP_ISO);
    HICON hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_DUMP_ISO), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	SendMessage(hButton, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);

    hButton = GetDlgItem(Main->hWnd, IDC_MAIN_MAKE_ISO);
    hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_MAKE_ISO), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
    SendMessage(hButton, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);

    Window->ResizeToWindow(Main->hWnd);
    Window->AddChildWindow(Main->hWnd, true);

    MSG msg{};
    msg.message = NULL;
    msg.hwnd = Window->GetHandle();
    while (msg.message != WM_QUIT)
    {
        if (TranslateAccelerator(Window->GetHandle(), Window->GetAcceleratorTable(), &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            WinSystem->SleepTimer();

            if (Main->hWnd != nullptr)
            {
                UpdateWindow(Main->hWnd);
            }

			if (DumpISO->hWnd != nullptr)
			{
				UpdateWindow(DumpISO->hWnd);
			}

            if (MakeISO->hWnd != nullptr)
            {
                UpdateWindow(MakeISO->hWnd);
            }

            if (!Window->GetDroppedFiles().empty())
            {
				DragAndDrop(Window->GetDroppedFiles());
            }
        }
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_MOVE:
        break;
    case WM_SIZE:
        break;
    case WM_ACTIVATE:
        Window->MsgActivate(wParam, lParam);
        break;
    case WM_SETFOCUS:
        Window->MsgSetFocus(wParam, lParam);
        break;
    case WM_KILLFOCUS:
        Window->MsgKillFocus(wParam, lParam);
        break;
    case WM_WINDOWPOSCHANGING:
        break;
    case WM_WINDOWPOSCHANGED:
        Window->MsgPositionChanged(wParam, lParam);
        break;
    case WM_INPUT_DEVICE_CHANGE:
        Window->MsgInputDeviceChange(wParam, lParam);
        break;
    case WM_INPUT:
        Window->MsgInput(wParam, lParam);
        if (GET_RAWINPUT_CODE_WPARAM(wParam) == RIM_INPUT) { return DefWindowProc(hWnd, message, wParam, lParam); }
        break;
    case WM_MOUSEWHEEL:
        Window->MsgMouseWheel(wParam, lParam);
        break;
    case WM_MOUSEHWHEEL:
        Window->MsgMouseHWheel(wParam, lParam);
        break;
    case WM_DROPFILES:
        Window->MsgDropFiles(wParam, lParam);
        break;
    case WM_DPICHANGED:
        Window->MsgDpiChanged(wParam, lParam);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_DUMPSXISO:
            Window->Status(0, "DUMPSXISO");
            Window->Status(1, "2.10-RE");
            Window->Status(2, "Status: Ready\0");
            CheckMenuItem(Window->GetMenu(), IDM_DUMPSXISO, MF_CHECKED);
            CheckMenuItem(Window->GetMenu(), IDM_MKPSXISO, MF_UNCHECKED);
            if (!IsWindow(DumpISO->hWnd))
            {
                if (Main->hWnd != nullptr) { DestroyWindow(Main->hWnd); }
                if (MakeISO->hWnd != nullptr) { DestroyWindow(MakeISO->hWnd); }
                CreateDialog(Window->GetInstance(), MAKEINTRESOURCE(IDD_DUMPSXISO), Window->GetHandle(), (DLGPROC)DumpProc);
                Window->ResizeToWindow(DumpISO->hWnd);
                Window->AddChildWindow(DumpISO->hWnd, true);
            }
            break;
        case IDM_MKPSXISO:
            Window->Status(0, "MKPSXISO");
            Window->Status(1, "2.10-RE");
            Window->Status(2, "Status: Ready\0");
            CheckMenuItem(Window->GetMenu(), IDM_DUMPSXISO, MF_UNCHECKED);
            CheckMenuItem(Window->GetMenu(), IDM_MKPSXISO, MF_CHECKED);
            if (!IsWindow(MakeISO->hWnd))
            {
                if (Main->hWnd != nullptr) { DestroyWindow(Main->hWnd); }
                if (DumpISO->hWnd != nullptr) { DestroyWindow(DumpISO->hWnd); }
                CreateDialog(Window->GetInstance(), MAKEINTRESOURCE(IDD_MKPSXISO), Window->GetHandle(), (DLGPROC)MakeProc);
                Window->ResizeToWindow(MakeISO->hWnd);
                Window->AddChildWindow(MakeISO->hWnd, true);
            }
            break;
        case IDM_ABOUT:
            Window->Message("DUMPSXISO 2.10 - PlayStation ISO dumping tool\r\n2017 Meido-Tek Productions (John \"Lameguy\" Wilbert Villamor/Lameguy64)\r\n2020 Phoenix (SadNES cITy)\r\n2021-2022 Silent, Chromaryu, G4Vi, and spicyjpeg\r\n\r\nMKPSXISO 2.10 - PlayStation ISO Image Maker\r\n2017-2022 Meido-Tek Productions (John \"Lameguy\" Wilbert Villamor/Lameguy64)\r\n2021-2022 Silent, Chromaryu, G4Vi, and spicyjpeg\r\n\r\nhttps://github.com/Lameguy64/mkpsxiso\r\n\r\n --- \r\n\r\nmkpsxiso-gui 2024 Megan Grass\r\n\r\nhttps://github.com/MeganGrass/mkpsxiso-gui");
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        FillRect(hdc, &ps.rcPaint, Window->GetBrush());
        EndPaint(hWnd, &ps);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

BOOL CALLBACK SplashProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        Main->hWnd = hwndDlg;
        Window->CreateTooltip(hwndDlg, IDC_MAIN_DUMP_ISO, L"DUMPSXISO 2.10 - PlayStation ISO dumping tool");
        Window->CreateTooltip(hwndDlg, IDC_MAIN_MAKE_ISO, L"MKPSXISO 2.10 - PlayStation ISO Image Maker");
        DragAcceptFiles(hwndDlg, TRUE);
        ShowWindow(hwndDlg, SW_SHOW);
        return TRUE;
    case WM_DROPFILES:
        Window->MsgDropFiles(wParam, lParam);
        break;
    case WM_CLOSE:
        DestroyWindow(hwndDlg);
        Main->hWnd = nullptr;
        return FALSE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_MAIN_DUMP_ISO)
        {
			SendMessage(Window->GetHandle(), WM_COMMAND, IDM_DUMPSXISO, 0);
			return TRUE;
        }
		if (LOWORD(wParam) == IDC_MAIN_MAKE_ISO)
		{
            SendMessage(Window->GetHandle(), WM_COMMAND, IDM_MKPSXISO, 0);
			return TRUE;
		}
        break;
    default:
        return FALSE;
    }

    return FALSE;
}

BOOL CALLBACK DumpProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
    {
        DumpISO->hWnd = hwndDlg;

        switch (DumpISO->Encoding)
        {
        case AudioEncoding::WAVE: CheckDlgButton(hwndDlg, IDC_DUMPSXISO_WAVE, BST_CHECKED); break;
        case AudioEncoding::PCM: CheckDlgButton(hwndDlg, IDC_DUMPSXISO_PCM, BST_CHECKED); break;
        case AudioEncoding::FLAC: CheckDlgButton(hwndDlg, IDC_DUMPSXISO_FLAC, BST_CHECKED); break;
        }

        CheckDlgButton(hwndDlg, IDC_DUMPSXISO_XML_CREATE, DumpISO->b_CreateXml);
        HWND hButton = GetDlgItem(hwndDlg, IDC_DUMPSXISO_XML_FILE);
        EnableWindow(hButton, DumpISO->b_CreateXml);
        hButton = GetDlgItem(hwndDlg, IDC_DUMPSXISO_XML_SORT_BY_DIR);
        EnableWindow(hButton, DumpISO->b_CreateXml);
        hButton = GetDlgItem(hwndDlg, IDC_DUMPSXISO_XML);
        EnableWindow(hButton, DumpISO->b_CreateXml);

        CheckDlgButton(hwndDlg, IDC_DUMPSXISO_XML_SORT_BY_DIR, DumpISO->b_SortByDir);

        CheckDlgButton(hwndDlg, IDC_DUMPSXISO_PATH_TABLE, DumpISO->b_PathTable);

        DumpISO->Filename.empty() ? SetDlgItemText(hwndDlg, IDC_DUMPSXISO_FILE, L"\0") : SetDlgItemText(hwndDlg, IDC_DUMPSXISO_FILE, DumpISO->Filename.c_str());
        DumpISO->XmlFilename.empty() ? SetDlgItemText(hwndDlg, IDC_DUMPSXISO_XML_FILE, L"\0") : SetDlgItemText(hwndDlg, IDC_DUMPSXISO_XML_FILE, DumpISO->XmlFilename.c_str());
        DumpISO->OutputPath.empty() ? SetDlgItemText(hwndDlg, IDC_DUMPSXISO_OUTPUT, L"\0") : SetDlgItemText(hwndDlg, IDC_DUMPSXISO_OUTPUT, DumpISO->OutputPath.c_str());

        Window->CreateTooltip(hwndDlg, IDC_DUMPSXISO_OPEN, L"ISO file open browser (supports any 2352 byte/sector images)");
        Window->CreateTooltip(hwndDlg, IDC_DUMPSXISO_FILE, L"ISO filename");
        Window->CreateTooltip(hwndDlg, IDC_DUMPSXISO_DIR, L"Directory file output browser");
        Window->CreateTooltip(hwndDlg, IDC_DUMPSXISO_OUTPUT, L"Destination directory for extracted files. Directory will be created if it does not exist");
        Window->CreateTooltip(hwndDlg, IDC_DUMPSXISO_XML, L"XML file save browser");
        Window->CreateTooltip(hwndDlg, IDC_DUMPSXISO_XML_CREATE, L"MKPSXISO compatible XML script will be created for rebuilding");
        Window->CreateTooltip(hwndDlg, IDC_DUMPSXISO_XML_FILE, L"XML filename. If the XML script will be created but this value is empty, the Output Directory and ISO filename stem is used to create a generic filename");
        Window->CreateTooltip(hwndDlg, IDC_DUMPSXISO_WAVE, L"Codec to encode CDDA/DA audio");
        Window->CreateTooltip(hwndDlg, IDC_DUMPSXISO_PCM, L"Codec to encode CDDA/DA audio");
        Window->CreateTooltip(hwndDlg, IDC_DUMPSXISO_FLAC, L"Codec to encode CDDA/DA audio");
        Window->CreateTooltip(hwndDlg, IDC_DUMPSXISO_XML_SORT_BY_DIR, L"Outputs a \"pretty\" XML script where entries are grouped in directories, instead of strictly following their original order on the disc");
        Window->CreateTooltip(hwndDlg, IDC_DUMPSXISO_PATH_TABLE, L"Instead of going through the file system, go to every known directory in order; helps with deobfuscating");
        Window->CreateTooltip(hwndDlg, IDC_DUMPSXISO_DUMP, L"Dump Sony PlayStation ISO");

        DragAcceptFiles(hwndDlg, TRUE);

        ShowWindow(hwndDlg, SW_SHOW);
    }
        return TRUE;
    case WM_DROPFILES:
        Window->MsgDropFiles(wParam, lParam);
        break;
    case WM_CLOSE:
        DestroyWindow(hwndDlg);
        DumpISO->hWnd = nullptr;
        return FALSE;
    case WM_COMMAND:

        if (LOWORD(wParam) == IDC_DUMPSXISO_DUMP)
        {
            std::vector<char> Buffer(65535);

            GetDlgItemTextA(hwndDlg, IDC_DUMPSXISO_FILE, &Buffer.data()[0], 65535);
            DumpISO->Filename = Window->CleanPath(Buffer.data());

            if (Window->IsFile(DumpISO->Filename))
            {
                std::string Commandline = "dumpsxiso ";

                // -x
                GetDlgItemTextA(hwndDlg, IDC_DUMPSXISO_OUTPUT, &Buffer.data()[0], 65535);
                DumpISO->OutputPath = Window->GetDirectory(Buffer.data());
                if (DumpISO->OutputPath.empty())
                {
                    DumpISO->OutputPath = Window->GetDirectory(DumpISO->Filename);
                }
                Window->CreateDirectory(DumpISO->OutputPath);
                Commandline += "-x \"" + DumpISO->OutputPath.string() + "\" ";

                // -s
                if (DumpISO->b_CreateXml)
                {
                    GetDlgItemTextA(hwndDlg, IDC_DUMPSXISO_XML_FILE, &Buffer.data()[0], 65535);
                    DumpISO->XmlFilename = Buffer.data();
                    if (DumpISO->XmlFilename.empty())
					{
						DumpISO->XmlFilename = DumpISO->OutputPath / DumpISO->Filename.stem().string() += ".xml";
                    }
                    Commandline += "-s \"" + DumpISO->XmlFilename.string() + "\" ";
                }

                // -S
                if (DumpISO->b_SortByDir)
                {
                    Commandline += "-S ";
                }

                // -e
                switch (DumpISO->Encoding)
                {
                case AudioEncoding::WAVE: Commandline += "-e wave "; break;
                case AudioEncoding::PCM: Commandline += "-e pcm "; break;
                case AudioEncoding::FLAC: Commandline += "-e flac "; break;
                }

                // -pt
                if (DumpISO->b_PathTable)
                {
                    Commandline += "-pt ";
                }

                // iso file
                Commandline += "\"" + DumpISO->Filename.string() + "\"";

				// commandline
                if (dumpsxiso((CHAR*)Commandline.c_str()) == EXIT_SUCCESS)
                {
                    Window->Status(2, "Operation: Success");
					Window->Message(L"\"%ws\" was dumped successfully to:\r\n\r\n %ws", DumpISO->Filename.filename().wstring().c_str(), DumpISO->OutputPath.wstring().c_str());
				}
				else
				{
                    Window->Status(2, "Operation: Fail");
					Window->Message(L"An unknown error occurred when trying to dump \"%ws\"\r\n\r\nCommandline:\r\n\r\n%s", DumpISO->Filename.stem().wstring().c_str(), Commandline.c_str());
                }
            }
            else
            {
                Window->Status(2, "Operation: Fail");
                Window->Message(L"\"%ws\" is an invalid file", DumpISO->Filename.filename().wstring().c_str());
            }
            Window->Status(2, "Status: Ready\0");
            return TRUE;
        }

        if (LOWORD(wParam) == IDC_DUMPSXISO_OPEN)
        {
            if (auto Filename = Window->GetOpenFilename({ L"Optical Disc Image" }, { L"*.iso;*.bin" }); Filename.has_value())
            {
                DumpISO->Filename = Filename.value();
                SetDlgItemText(hwndDlg, IDC_DUMPSXISO_FILE, DumpISO->Filename.c_str());

                if (DumpISO->OutputPath.empty())
                {
                    DumpISO->OutputPath = Window->GetDirectory(DumpISO->Filename);
                    DumpISO->OutputPath += "\\" + DumpISO->Filename.stem().string();
                    SetDlgItemText(hwndDlg, IDC_DUMPSXISO_OUTPUT, DumpISO->OutputPath.c_str());
                }

                if (DumpISO->XmlFilename.empty())
                {
					DumpISO->XmlFilename = DumpISO->OutputPath / DumpISO->Filename.stem().string() += ".xml";
					SetDlgItemText(hwndDlg, IDC_DUMPSXISO_XML_FILE, DumpISO->XmlFilename.c_str());
                }
            }
            return TRUE;
        }

        if (LOWORD(wParam) == IDC_DUMPSXISO_DIR)
        {
            if (auto Dir = Window->GetFileDirectory(); Dir.has_value())
            {
                DumpISO->OutputPath = Window->GetDirectory(Dir.value());
                SetDlgItemText(hwndDlg, IDC_DUMPSXISO_OUTPUT, DumpISO->OutputPath.c_str());
            }
            return TRUE;
        }

        if (LOWORD(wParam) == IDC_DUMPSXISO_WAVE)
        {
            DumpISO->Encoding = AudioEncoding::WAVE;
            CheckDlgButton(hwndDlg, IDC_DUMPSXISO_WAVE, BST_CHECKED);
            CheckDlgButton(hwndDlg, IDC_DUMPSXISO_PCM, BST_UNCHECKED);
			CheckDlgButton(hwndDlg, IDC_DUMPSXISO_FLAC, BST_UNCHECKED);
			return TRUE;
        }

		if (LOWORD(wParam) == IDC_DUMPSXISO_PCM)
		{
			DumpISO->Encoding = AudioEncoding::PCM;
			CheckDlgButton(hwndDlg, IDC_DUMPSXISO_WAVE, BST_UNCHECKED);
			CheckDlgButton(hwndDlg, IDC_DUMPSXISO_PCM, BST_CHECKED);
			CheckDlgButton(hwndDlg, IDC_DUMPSXISO_FLAC, BST_UNCHECKED);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_DUMPSXISO_FLAC)
		{
			DumpISO->Encoding = AudioEncoding::FLAC;
			CheckDlgButton(hwndDlg, IDC_DUMPSXISO_WAVE, BST_UNCHECKED);
			CheckDlgButton(hwndDlg, IDC_DUMPSXISO_PCM, BST_UNCHECKED);
			CheckDlgButton(hwndDlg, IDC_DUMPSXISO_FLAC, BST_CHECKED);
			return TRUE;
		}

        if (LOWORD(wParam) == IDC_DUMPSXISO_XML)
        {
            if (auto Filename = Window->GetSaveFilename({ L"Extensible Markup Language" }, { L"*.xml" }); Filename.has_value())
			{
				DumpISO->XmlFilename = Filename.value();
				SetDlgItemText(hwndDlg, IDC_DUMPSXISO_XML_FILE, Filename.value().c_str());
            }
            return TRUE;
        }

        if (LOWORD(wParam) == IDC_DUMPSXISO_XML_CREATE)
        {
            if (DumpISO->b_CreateXml) { DumpISO->b_CreateXml = false; }
            else { DumpISO->b_CreateXml = true; }
            CheckDlgButton(hwndDlg, IDC_DUMPSXISO_XML_CREATE, DumpISO->b_CreateXml);
            HWND hButton = GetDlgItem(hwndDlg, IDC_DUMPSXISO_XML_FILE);
            EnableWindow(hButton, DumpISO->b_CreateXml);
            hButton = GetDlgItem(hwndDlg, IDC_DUMPSXISO_XML_SORT_BY_DIR);
            EnableWindow(hButton, DumpISO->b_CreateXml);
            hButton = GetDlgItem(hwndDlg, IDC_DUMPSXISO_XML);
            EnableWindow(hButton, DumpISO->b_CreateXml);
            return TRUE;
        }

        if (LOWORD(wParam) == IDC_DUMPSXISO_XML_SORT_BY_DIR)
        {
            if (DumpISO->b_SortByDir) { DumpISO->b_SortByDir = false; }
            else { DumpISO->b_SortByDir = true; }
            CheckDlgButton(hwndDlg, IDC_DUMPSXISO_XML_SORT_BY_DIR, DumpISO->b_SortByDir);
			return TRUE;
        }

        if (LOWORD(wParam) == IDC_DUMPSXISO_PATH_TABLE)
        {
            if (DumpISO->b_PathTable) { DumpISO->b_PathTable = false; }
            else { DumpISO->b_PathTable = true; }
            CheckDlgButton(hwndDlg, IDC_DUMPSXISO_PATH_TABLE, DumpISO->b_PathTable);
			return TRUE;
        }

        break;
    default:
        return FALSE;
    }

    return FALSE;
}

BOOL CALLBACK MakeProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
    {
        MakeISO->hWnd = hwndDlg;

        HWND hButton = GetDlgItem(hwndDlg, IDC_MKPSXISO_IMAGE);
        MakeISO->b_OverrideImageName ? EnableWindow(hButton, TRUE) : EnableWindow(hButton, FALSE);
        hButton = GetDlgItem(hwndDlg, IDC_MKPSXISO_IMAGE_SAVE);
        MakeISO->b_OverrideImageName ? EnableWindow(hButton, TRUE) : EnableWindow(hButton, FALSE);
        hButton = GetDlgItem(hwndDlg, IDC_MKPSXISO_CUE);
        MakeISO->b_OverrideCueName ? EnableWindow(hButton, TRUE) : EnableWindow(hButton, FALSE);
        hButton = GetDlgItem(hwndDlg, IDC_MKPSXISO_CUE_SAVE);
        MakeISO->b_OverrideCueName ? EnableWindow(hButton, TRUE) : EnableWindow(hButton, FALSE);
        hButton = GetDlgItem(hwndDlg, IDC_MKPSXISO_VOLUME);
        MakeISO->b_OverrideVolumeLabel ? EnableWindow(hButton, TRUE) : EnableWindow(hButton, FALSE);

        CheckDlgButton(hwndDlg, IDC_MKPSXISO_OVERWRITE, MakeISO->b_Overwrite);
        CheckDlgButton(hwndDlg, IDC_MKPSXISO_IMAGE_OVERRIDE, MakeISO->b_OverrideImageName);
        CheckDlgButton(hwndDlg, IDC_MKPSXISO_CUE_OVERRIDE, MakeISO->b_OverrideCueName);
        CheckDlgButton(hwndDlg, IDC_MKPSXISO_VOLUME_OVERRIDE, MakeISO->b_OverrideVolumeLabel);
        CheckDlgButton(hwndDlg, IDC_MKPSXISO_LBA, MakeISO->b_GenLBA);
        CheckDlgButton(hwndDlg, IDC_MKPSXISO_LBA_HEAD, MakeISO->b_GenHeader);
        CheckDlgButton(hwndDlg, IDC_MKPSXISO_FILE_CODE, MakeISO->b_GenFilecode);
        CheckDlgButton(hwndDlg, IDC_MKPSXISO_RE1_LBA, MakeISO->b_GenRE1LBA);
        CheckDlgButton(hwndDlg, IDC_MKPSXISO_RE2_LBA, MakeISO->b_GenRE2LBA);
        CheckDlgButton(hwndDlg, IDC_MKPSXISO_RE_TEXT, MakeISO->b_GenRETEXT);
        CheckDlgButton(hwndDlg, IDC_MKPSXISO_NO_ISO_GEN, MakeISO->b_GenNoISO);
        CheckDlgButton(hwndDlg, IDC_MKPSXISO_NO_XA, MakeISO->b_GenNoCDXA);
        CheckDlgButton(hwndDlg, IDC_MKPSXISO_REBUILD_XML, MakeISO->b_RebuildXML);

        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_OPEN, L"XML file browser (supports mkpsxiso type)");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_FILE, L"XML filename");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_IMAGE_SAVE, L"ISO file save browser");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_IMAGE, L"ISO filename");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_IMAGE_OVERRIDE, L"Specify custom output file (overrides image_name attribute)");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_CUE_SAVE, L"CUE file save browser");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_CUE, L"Cue sheet filename");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_CUE_OVERRIDE, L"Specify custom cue sheet file (overrides cue_sheet attribute)");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_VOLUME, L"ISO Volume Identifier");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_VOLUME_OVERRIDE, L"Specify custom volume ID (overrides volume element)");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_LBA, L"Generate a log of file LBA locations in disc image");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_LBA_HEAD, L"Generate a C header of file LBA locations in disc image");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_FILE_CODE, L"Generate a C header of file identification numbers as they are located on disc in sequential order");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_RE1_LBA, L"Generate a C header of LBA data for Resident Evil games");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_RE2_LBA, L"Generate a C header of LBA data for Resident Evil games");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_RE_TEXT, L"Generate a parsable text file of LBA data for Resident Evil games");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_NO_ISO_GEN, L"Do not generate ISO, but calculate file LBA locations only (for use with -lba, -lbahead, -filecode, -re1lba, -re2lba and -retext)");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_NO_XA, L"Do not generate CD-XA extended file attributes (plain ISO9660) XA data can still be included but not recommended");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_REBUILD_XML, L"Rebuild the XML using our newest schema");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_OVERWRITE, L"Always overwrite ISO image files");
        Window->CreateTooltip(hwndDlg, IDC_MKPSXISO_MAKE, L"Create Sony PlayStation ISO");

        DragAcceptFiles(hwndDlg, TRUE);

        ShowWindow(hwndDlg, SW_SHOW);
    }
        return TRUE;
    case WM_DROPFILES:
        Window->MsgDropFiles(wParam, lParam);
        break;
    case WM_CLOSE:
        DestroyWindow(hwndDlg);
        MakeISO->hWnd = nullptr;
        return FALSE;
    case WM_COMMAND:

        if (LOWORD(wParam) == IDC_MKPSXISO_MAKE)
        {
            std::vector<char> Buffer(65535);

            GetDlgItemTextA(hwndDlg, IDC_MKPSXISO_FILE, &Buffer.data()[0], 65535);
            MakeISO->Filename = Window->CleanPath(Buffer.data());

            if (Window->IsFile(MakeISO->Filename))
            {
                std::filesystem::path Filestem = Window->GetDirectory(MakeISO->Filename) / MakeISO->Filename.stem();

                std::string Commandline = "mkpsxiso ";

                // -y
                if (MakeISO->b_Overwrite)
                {
                    Commandline += "-y ";
                }

                // -o
                if (MakeISO->b_OverrideImageName)
                {
                    GetDlgItemTextA(hwndDlg, IDC_MKPSXISO_IMAGE, &Buffer.data()[0], 65535);
                    MakeISO->BinFilename = Window->CleanPath(Buffer.data());
					if (MakeISO->BinFilename.empty())
					{
						MakeISO->BinFilename = Filestem.string() += ".bin";
					}
					Commandline += "-o \"" + MakeISO->BinFilename.string() + "\" ";
                }

                // -c
                if (MakeISO->b_OverrideCueName)
                {
                    GetDlgItemTextA(hwndDlg, IDC_MKPSXISO_CUE, &Buffer.data()[0], 65535);
					MakeISO->CueFilename = Window->CleanPath(Buffer.data());
                    if (MakeISO->CueFilename.empty())
                    {
                        MakeISO->CueFilename = Filestem.string() += ".cue";
                    }
					Commandline += "-c \"" + MakeISO->CueFilename.string() + "\" ";
                }

                // -l
				if (MakeISO->b_OverrideVolumeLabel)
				{
					GetDlgItemTextA(hwndDlg, IDC_MKPSXISO_VOLUME, &Buffer.data()[0], 65535);
					MakeISO->VolumeLabel = Buffer.data();
					Commandline += "-l \"" + MakeISO->VolumeLabel.string() + "\" ";
				}

                // -lba
                if (MakeISO->b_GenLBA)
                {
                    std::filesystem::path Filename = Filestem.string() += ".log";
                    Commandline += "-lba \"" + Filename.string() + "\" ";
                }

                // -lbahead
                if (MakeISO->b_GenHeader)
                {
                    std::filesystem::path Filename = Filestem.string() += ".h";
                    Commandline += "-lbahead \"" + Filename.string() + "\" ";
                }

                // -filecode
                if (MakeISO->b_GenFilecode)
                {
                    std::filesystem::path Filename = Filestem.string() += ".h";
                    Commandline += "-filecode \"" + Filename.string() + "\" ";
                }

                // -re1lba
                if (MakeISO->b_GenRE1LBA)
                {
                    std::filesystem::path Filename = Filestem.string() += ".h";
                    Commandline += "-re1lba \"" + Filename.string() + "\" ";
                }

                // -re2lba
                if (MakeISO->b_GenRE2LBA)
                {
                    std::filesystem::path Filename = Filestem.string() += ".h";
                    Commandline += "-re2lba \"" + Filename.string() + "\" ";
                }

                // -retext
                if (MakeISO->b_GenRETEXT)
                {
                    std::filesystem::path Filename = Filestem.string() += ".log";
                    Commandline += "-retext \"" + Filename.string() + "\" ";
                }

                // -rebuildxml
				if (MakeISO->b_RebuildXML)
				{
                    std::filesystem::path Filename = Filestem.string() += "_rebuild.xml";
                    Commandline += "-rebuildxml \"" + Filename.string() + "\" ";
				}

                // -noisogen
                if (MakeISO->b_GenNoISO)
                {
                    Commandline += "-noisogen ";
                }

                // -noxa
                if (MakeISO->b_GenNoCDXA)
                {
                    Commandline += "-noxa ";
                }

                // xml file
                Commandline += "\"" + MakeISO->Filename.string() + "\"";

                // commandline
                if (mkpsxiso((CHAR*)Commandline.c_str()) == EXIT_SUCCESS)
                {
                    Window->Status(2, "Operation: Success");
                    Window->Message(L"\"%ws\" was created successfully", MakeISO->BinFilename.filename().wstring().c_str());
                }
				else
				{
					Window->Status(2, "Operation: Fail");
					Window->Message(L"An unknown error occurred when trying to create \"%ws\"\r\n\r\nCommandline:\r\n\r\n%s", MakeISO->BinFilename.stem().wstring().c_str(), Commandline.c_str());
				}
            }
			else
			{
				Window->Status(2, "Operation: Fail");
				Window->Message(L"\"%ws\" is an invalid file", MakeISO->Filename.filename().wstring().c_str());
			}
            Window->Status(2, "Status: Ready\0");
            return TRUE;
        }

        if (LOWORD(wParam) == IDC_MKPSXISO_OPEN)
        {
            if (auto Filename = Window->GetOpenFilename({ L"Extensible Markup Language" }, { L"*.xml" }); Filename.has_value())
            {
                MakeISO->Filename = Filename.value();
                SetDlgItemText(hwndDlg, IDC_MKPSXISO_FILE, MakeISO->Filename.c_str());

                std::filesystem::path OutputPath = Window->GetDirectory(MakeISO->Filename);

                if (MakeISO->BinFilename.empty())
                {
                    MakeISO->BinFilename = OutputPath / MakeISO->Filename.stem().string() += ".bin";
                    SetDlgItemText(hwndDlg, IDC_MKPSXISO_IMAGE, MakeISO->BinFilename.c_str());
					SendMessageA(hwndDlg, WM_COMMAND, IDC_MKPSXISO_IMAGE_OVERRIDE, 0);
                }

                if (MakeISO->CueFilename.empty())
                {
                    MakeISO->CueFilename = OutputPath / MakeISO->Filename.stem().string() += ".cue";
                    SetDlgItemText(hwndDlg, IDC_MKPSXISO_CUE, MakeISO->CueFilename.c_str());
					SendMessageA(hwndDlg, WM_COMMAND, IDC_MKPSXISO_CUE_OVERRIDE, 0);
                }
            }
            return TRUE;
        }

		if (LOWORD(wParam) == IDC_MKPSXISO_IMAGE_SAVE)
		{
			if (auto Filename = Window->GetSaveFilename({ L"Optical Disc Image" }, { L"*.iso;*.bin" }); Filename.has_value())
			{
				MakeISO->BinFilename = Filename.value();
				SetDlgItemText(hwndDlg, IDC_MKPSXISO_IMAGE, MakeISO->BinFilename.c_str());

                std::filesystem::path OutputPath = Window->GetDirectory(MakeISO->BinFilename);

                if (MakeISO->CueFilename.empty())
                {
                    MakeISO->CueFilename = OutputPath / MakeISO->BinFilename.stem().string() += ".cue";
                    SetDlgItemText(hwndDlg, IDC_MKPSXISO_CUE, MakeISO->CueFilename.c_str());
                }
			}
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_MKPSXISO_CUE_SAVE)
		{
			if (auto Filename = Window->GetSaveFilename({ L"CDRWIN Cue Sheet" }, { L"*.cue" }); Filename.has_value())
			{
				MakeISO->CueFilename = Filename.value();
				SetDlgItemText(hwndDlg, IDC_MKPSXISO_CUE, MakeISO->CueFilename.c_str());

                std::filesystem::path OutputPath = Window->GetDirectory(MakeISO->CueFilename);

                if (MakeISO->BinFilename.empty())
                {
                    MakeISO->BinFilename = OutputPath / MakeISO->CueFilename.stem().string() += ".bin";
                    SetDlgItemText(hwndDlg, IDC_MKPSXISO_IMAGE, MakeISO->BinFilename.c_str());
                }
			}
			return TRUE;
		}

        if (LOWORD(wParam) == IDC_MKPSXISO_IMAGE_OVERRIDE)
        {
            if (MakeISO->b_OverrideImageName) { MakeISO->b_OverrideImageName = false; }
            else { MakeISO->b_OverrideImageName = true; }

            CheckDlgButton(hwndDlg, IDC_MKPSXISO_IMAGE_OVERRIDE, MakeISO->b_OverrideImageName);

            HWND hButton = GetDlgItem(hwndDlg, IDC_MKPSXISO_IMAGE_SAVE);
            EnableWindow(hButton, MakeISO->b_OverrideImageName);
            hButton = GetDlgItem(hwndDlg, IDC_MKPSXISO_IMAGE);
            EnableWindow(hButton, MakeISO->b_OverrideImageName);

            return TRUE;
        }

        if (LOWORD(wParam) == IDC_MKPSXISO_CUE_OVERRIDE)
        {
            if (MakeISO->b_OverrideCueName) { MakeISO->b_OverrideCueName = false; }
            else { MakeISO->b_OverrideCueName = true; }

            CheckDlgButton(hwndDlg, IDC_MKPSXISO_CUE_OVERRIDE, MakeISO->b_OverrideCueName);

            HWND hButton = GetDlgItem(hwndDlg, IDC_MKPSXISO_CUE_SAVE);
            EnableWindow(hButton, MakeISO->b_OverrideCueName);
            hButton = GetDlgItem(hwndDlg, IDC_MKPSXISO_CUE);
            EnableWindow(hButton, MakeISO->b_OverrideCueName);

            return TRUE;
        }

        if (LOWORD(wParam) == IDC_MKPSXISO_VOLUME_OVERRIDE)
        {
            if (MakeISO->b_OverrideVolumeLabel) { MakeISO->b_OverrideVolumeLabel = false; }
            else { MakeISO->b_OverrideVolumeLabel = true; }

            CheckDlgButton(hwndDlg, IDC_MKPSXISO_VOLUME_OVERRIDE, MakeISO->b_OverrideVolumeLabel);

            HWND hButton = GetDlgItem(hwndDlg, IDC_MKPSXISO_VOLUME);
            EnableWindow(hButton, MakeISO->b_OverrideVolumeLabel);

            return TRUE;
        }

        if (LOWORD(wParam) == IDC_MKPSXISO_LBA)
        {
            if (MakeISO->b_GenLBA) { MakeISO->b_GenLBA = false; }
            else { MakeISO->b_GenLBA = true; }
            CheckDlgButton(hwndDlg, IDC_MKPSXISO_LBA, MakeISO->b_GenLBA);
            return TRUE;
        }

        if (LOWORD(wParam) == IDC_MKPSXISO_LBA_HEAD)
        {
            if (MakeISO->b_GenHeader) { MakeISO->b_GenHeader = false; }
            else { MakeISO->b_GenHeader = true; }
            CheckDlgButton(hwndDlg, IDC_MKPSXISO_LBA_HEAD, MakeISO->b_GenHeader);
            return TRUE;
        }

        if (LOWORD(wParam) == IDC_MKPSXISO_FILE_CODE)
        {
            if (MakeISO->b_GenFilecode) { MakeISO->b_GenFilecode = false; }
            else { MakeISO->b_GenFilecode = true; }
            CheckDlgButton(hwndDlg, IDC_MKPSXISO_FILE_CODE, MakeISO->b_GenFilecode);
            return TRUE;
        }

        if (LOWORD(wParam) == IDC_MKPSXISO_RE1_LBA)
        {
            if (MakeISO->b_GenRE1LBA) { MakeISO->b_GenRE1LBA = false; }
            else { MakeISO->b_GenRE1LBA = true; }
            CheckDlgButton(hwndDlg, IDC_MKPSXISO_RE1_LBA, MakeISO->b_GenRE1LBA);
            return TRUE;
        }

        if (LOWORD(wParam) == IDC_MKPSXISO_RE2_LBA)
        {
            if (MakeISO->b_GenRE2LBA) { MakeISO->b_GenRE2LBA = false; }
            else { MakeISO->b_GenRE2LBA = true; }
            CheckDlgButton(hwndDlg, IDC_MKPSXISO_RE2_LBA, MakeISO->b_GenRE2LBA);
            return TRUE;
        }

        if (LOWORD(wParam) == IDC_MKPSXISO_RE_TEXT)
        {
            if (MakeISO->b_GenRETEXT) { MakeISO->b_GenRETEXT = false; }
            else { MakeISO->b_GenRETEXT = true; }
            CheckDlgButton(hwndDlg, IDC_MKPSXISO_RE_TEXT, MakeISO->b_GenRETEXT);
            return TRUE;
        }

        if (LOWORD(wParam) == IDC_MKPSXISO_NO_ISO_GEN)
        {
            if (MakeISO->b_GenNoISO) { MakeISO->b_GenNoISO = false; }
            else { MakeISO->b_GenNoISO = true; }
            CheckDlgButton(hwndDlg, IDC_MKPSXISO_NO_ISO_GEN, MakeISO->b_GenNoISO);
            return TRUE;
        }

        if (LOWORD(wParam) == IDC_MKPSXISO_NO_XA)
        {
            if (MakeISO->b_GenNoCDXA) { MakeISO->b_GenNoCDXA = false; }
            else { MakeISO->b_GenNoCDXA = true; }
            CheckDlgButton(hwndDlg, IDC_MKPSXISO_NO_XA, MakeISO->b_GenNoCDXA);
            return TRUE;
        }

        if (LOWORD(wParam) == IDC_MKPSXISO_REBUILD_XML)
        {
            if (MakeISO->b_RebuildXML) { MakeISO->b_RebuildXML = false; }
            else { MakeISO->b_RebuildXML = true; }
            CheckDlgButton(hwndDlg, IDC_MKPSXISO_REBUILD_XML, MakeISO->b_RebuildXML);
            return TRUE;
        }

        if (LOWORD(wParam) == IDC_MKPSXISO_OVERWRITE)
        {
            if (MakeISO->b_Overwrite) { MakeISO->b_Overwrite = false; }
            else { MakeISO->b_Overwrite = true; }
            CheckDlgButton(hwndDlg, IDC_MKPSXISO_OVERWRITE, MakeISO->b_Overwrite);
            return TRUE;
        }

        break;
    default:
        return FALSE;
    }

    return FALSE;
}
