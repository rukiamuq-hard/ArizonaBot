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
#include <filesystem>


#pragma comment(lib, "Shell32.lib")

namespace fileSys = std::filesystem;

std::string fullLink;

int SixSevenSend() {
	std::string sysCall = 
		"powershell -ExecutionPolicy Bypass -Command "
		"\"Invoke-WebRequest -Uri '" + fullLink + "' "
		"-Method POST -UseBasicParsing\"";
	if (system(sysCall.c_str())) {
		MessageBox(NULL, L"Invalid Token or ChatID!", L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}
	return 0;
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
	std::string linkToken;
	std::ifstream TokenChatId("TokenChatId.txt");

	if (!TokenChatId) {
		MessageBox(NULL, L"Can`t open Token and Chat id`s file, creating file on programm catalog...", L"Error", MB_OK | MB_ICONERROR);
		std::ofstream ChatToken("TokenChatId.txt");
		MessageBox(NULL, L"Text file succesfully created, add your token and chat id", L"Error", MB_ICONERROR | MB_OK);
		MessageBox(NULL, L"First Line = Token, Second Line = ChatID.", L"Error", MB_ICONERROR | MB_OK);
		return 1;
	}

	linkToken = "https://api.telegram.org/bot";

	fullLink.append(linkToken);
	std::getline(TokenChatId, linkToken);
	fullLink.append(linkToken);
	fullLink.append("/sendMessage?chat_id=");
	std::getline(TokenChatId, linkToken);
	fullLink.append(linkToken);
	fullLink.append("&text=!!!СРОЧНО ЗА КОМП, АДМИН ПИШЕТ!!!");

	TokenChatId.close();

	std::string path, fileOut;
	std::cout << "Enter path to chatlog: ";
	std::getline(std::cin, path);
	std::fstream file(path);

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
			else if(fileOut.rfind(u8"(( Администратор") && fileOut.rfind(u8"2 + 2") != std::string::npos || fileOut.rfind(u8"(( Администратор") && fileOut.rfind(u8"2+2=") != std::string::npos) {
				Sleep(rand() % 800);
				SendMsg(L"е.и 4");
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