/*
*
*	Megan Grass
*	October 13, 2024
*
*/


#pragma once

#include <resource.h>

#include <std_window.h>

std::unique_ptr<Standard_Window> Window = std::make_unique<Standard_Window>();

enum class AudioEncoding
{
	WAVE, PCM, FLAC
};

struct tagSplash {
	HWND hWnd = nullptr;                                 // Handle to the splash gui window
};

struct tagDumpISO {
	HWND hWnd = nullptr;                                 // Handle to the dumpsxiso gui window
	std::filesystem::path OutputPath;                    // -x
	bool b_CreateXml = false;                            // -s
	bool b_SortByDir = false;                            // -S
	AudioEncoding Encoding = AudioEncoding::WAVE;        // -e
	bool b_PathTable = false;                            // -pt
	std::filesystem::path Filename;                      // ISO filename
	std::filesystem::path XmlFilename;                   // XML filename
};

struct tagMakeISO {
	HWND hWnd = nullptr;                                  // Handle to the mkpsxiso gui window
	bool b_Overwrite = false;                             // -y
	bool b_OverrideImageName = false;                     // -o
	std::filesystem::path BinFilename;                    // -o
	bool b_OverrideCueName = false;                       // -c
	std::filesystem::path CueFilename;                    // -c
	bool b_OverrideVolumeLabel = false;                   // -l
	std::filesystem::path VolumeLabel;                    // -l
	bool b_GenLBA = false;                                // -lba
	bool b_GenHeader = false;                             // -lbahead
	bool b_GenFilecode = false;                           // -filecode
	bool b_GenRE1LBA = false;                             // -re1lba
	bool b_GenRE2LBA = false;                             // -re2lba
	bool b_GenRETEXT = false;                             // -retext
	bool b_GenNoISO = false;                              // -noisogen
	bool b_GenNoCDXA = false;                             // -noxa
	bool b_RebuildXML = false;                            // -rebuildxml
	std::filesystem::path Filename;                       // XML filename
};

extern int dumpsxiso(char* commandline);                  // dumpsxiso.lib
extern int mkpsxiso(char* commandline);                   // mkpsxiso.lib

std::unique_ptr<tagSplash> Splash = std::make_unique<tagSplash>();
std::unique_ptr<tagDumpISO> DumpISO = std::make_unique<tagDumpISO>();
std::unique_ptr<tagMakeISO> MakeISO = std::make_unique<tagMakeISO>();

BOOL CALLBACK SplashProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DumpProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK MakeProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);