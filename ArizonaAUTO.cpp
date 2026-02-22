//Author: rukiamuq
//The author is not responsible for any damage caused by the script.
//A curriculum that does not have malicious intent.

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <shellapi.h>

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
		Sleep(160);
}


int main() {
	


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
			if (fileOut.rfind(u8"(( Администратор") == std::string::npos) {}
			else {
				SixSevenSend();
				Sleep(700);
				wchar_t text[] = L"е.и спасибо";

				for (size_t i = 0; i < 2; i++) {
					keybd_event(VK_RETURN, 0, 0, 0);
					keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
				}
				Sleep(200);
				for (size_t i = 0; i < std::size(text) - 1; i++) {
					ButtonEmulation(text[i]);
				}
				Sleep(80);

				keybd_event(VK_RETURN, 0, 0, 0);
				keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
			}
		}
		else file.clear();
	}
	
}







//github of author: https://github.com/rukiamuq-hard