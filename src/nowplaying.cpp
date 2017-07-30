#include <windows.h>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>


// DWORD pID;
//pID = GetWindowThreadProcessId(hwnd, NULL);

char title[256];
std::string s_title;
HWND hwnd;
MSG msg = {0};

// Function for copying into clipboard
void copyToClipboard(const std::string &s_title) {
	OpenClipboard(NULL);
	EmptyClipboard();
	HGLOBAL handle = GlobalAlloc(GMEM_MOVEABLE, s_title.length()+1);
	// Abort if there is nothing to be copied into the clipboard (allocated memory is 0)
	if (!handle) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(handle), s_title.c_str(), s_title.length()+1);
	GlobalUnlock(handle);
	SetClipboardData(CF_TEXT, handle);
	CloseClipboard();
	GlobalFree(handle);
}


int main() {
	// Check every 5 seconds for the window name "Spotify" in order to get a handle to it
	while (1) {
		std::cout << "Waiting for Spotify..." << std::endl;
		hwnd = FindWindowA(NULL, "Spotify");
		if (hwnd) break;
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
	std::cout << "Spotify found!" << std::endl;
	// Register trigger hotkeys
	if (RegisterHotKey(NULL, 1, MOD_ALT | MOD_NOREPEAT, 0x51)) std::cout << (("Copy song to clipboard: ALT+Q")) << std::endl; // 0x51 = Q
	if (RegisterHotKey(NULL, 2, MOD_ALT | MOD_NOREPEAT, 0x58)) std::cout << (("Exit: ALT+X")) << std::endl; // 0x58 = X
	// Wait for trigger combination to be pressed
	while (GetMessage(&msg, NULL, 0, 0) != 0) {
		// Trigger is activated
		if (msg.message == WM_HOTKEY) {
			// Copy song name to clipboard on hotkey 1
			if (msg.wParam == 1) {
				GetWindowText(hwnd, title, sizeof(title));
				s_title = title;
				if (s_title == "Spotify") s_title = "No song is being played right now!";
				else s_title = "Now Playing: " + s_title;
				copyToClipboard(s_title);
			}
			// Exit application on hotkey 2
			 if (msg.wParam == 2) {
				break;
			}
		}
		}
	return 0;
	}