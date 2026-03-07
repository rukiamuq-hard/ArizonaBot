//Author: rukiamuq
//The author is not responsible for any damage caused by the script.
//A curriculum that does not have malicious intent.

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <shellapi.h>
#include <cstdlib>
#include <ctime>
#include <cwchar>

#pragma comment(lib, "Shell32.lib")

void SixSevenSend() {
	system(
		"powershell -ExecutionPolicy Bypass -Command "
		"\"Invoke-WebRequest -Uri 'https://api.telegram.org/botTOKEN/sendMessage?chat_id=CHAT_ID&text=!!!!!!ADMIN IS /b TO YOU!!!!!' "
		"-Method POST -UseBasicParsing\""
	);
}

void ButtonEmulation(wchar_t text) {
		BYTE vkCode = VkKeyScanW(text);
		keybd_event(vkCode, 0, 0, 0);
		keybd_event(vkCode, 0, KEYEVENTF_KEYUP, 0);
		Sleep(rand() % 300);
}

void SendMsg(const wchar_t msgToFunc[]) {
	SixSevenSend();
	Sleep(rand() % 800);
	for (size_t i = 0; i < 2; i++) {
		keybd_event(VK_RETURN, 0, 0, 0);
		keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
	}
	Sleep(rand() % 200);
	for (size_t i = 0; i < std::wcslen(msgToFunc); i++) {
		ButtonEmulation(msgToFunc[i]);
	}
	Sleep(rand() % 75);

	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
}

int main() {
	srand(0);

	std::string path;
	std::cout << "Enter path to chat log in active game: ";
	std::getline(std::cin, path);
	

	if (path.empty()) {
		MessageBox(NULL, L"Error, empty path", L"Error", MB_ICONERROR | MB_OK);
		return 1;
	}

	std::fstream file(path);
	std::string fileOut;
	
	if (!file) {
		MessageBox(NULL, L"Error, can`t open file", L"Error", MB_ICONERROR | MB_OK);
		return 1; 
	}

	std::cout << "\nSuccesfully loaded log. Thank you for use!\n";

	while (true) {
		if (std::getline(file, fileOut)) {
			if (fileOut.rfind(u8"(( Администратор") && fileOut.rfind(u8"Вы тут?") != std::string::npos || fileOut.rfind(u8"(( Администратор") && fileOut.rfind(u8"вы тут?") != std::string::npos) {
				SendMsg(L"е.и да, я тут");
			}	
			else if(fileOut.rfind(u8"(( Администратор") && fileOut.rfind(u8"2 + 2 ?") != std::string::npos || fileOut.rfind(u8"(( Администратор") && fileOut.rfind(u8"2 + 2 = ?") != std::string::npos) {
				Sleep(rand() % 800);
				SendMsg(L"е4");
			}
			else if (fileOut.rfind(u8"(( Администратор") && fileOut.rfind(u8"Столица") != std::string::npos) {
				SendMsg(L"е.и я не люблю географию");
			}
			else if (fileOut.rfind(u8"(( Администратор") && fileOut.rfind(u8"На каком вы сервере") != std::string::npos) {
				SendMsg(L"е.и аризона");
			}

		}
		else file.clear();
	}
	
}







//github of author: https://github.com/rukiamuq-hard