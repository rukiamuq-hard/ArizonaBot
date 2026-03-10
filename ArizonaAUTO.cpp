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
#include <lmcons.h>

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

	//Получает username пользователя
	char username[UNLEN + 1] = { NULL };
	DWORD size = sizeof(username) / sizeof(char);
	GetUserNameA(username, &size);
	std::string startPath = "C:\\Users\\" + (std::string)username;

	if (fileSys::exists(startPath + "\\OneDrive") && fileSys::is_directory(startPath + "\\OneDrive")) {
		startPath = startPath + "\\OneDrive";
	}
	else if (fileSys::exists(startPath + "\\Documents") && fileSys::is_directory(startPath + "\\Documents")) {
		startPath = startPath + "\\Documents";
	}
	else { 
		MessageBox(NULL, L"Path to \"Documents\" can`t find, contact to developer!", L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	//находит путь к директории чат лога сампа
	std::string finalPath;
	if (fileSys::exists(startPath) && fileSys::is_directory(startPath)) {
		for (const auto& feach : fileSys::recursive_directory_iterator(startPath)) {
			if (feach.is_directory() && feach.path().filename() == "chatlog") {
					finalPath.append(feach.path().string());
			}
		}
	}
	
	
	std::ifstream TokenChatId("TokenChatId.txt");
	//проверяет есть ли такой файл, если нету - создаёт, и просит заполнить файл
	if (!TokenChatId) {
		MessageBox(NULL, L"Can`t open Token and Chat id`s file, creating file on programm catalog...", L"Error", MB_OK | MB_ICONERROR);
		std::ofstream ChatToken("TokenChatId.txt");
		MessageBox(NULL, L"Text file succesfully created, add your token and chat id", L"Error", MB_ICONERROR | MB_OK);
		MessageBox(NULL, L"First Line = Token, Second Line = ChatID.", L"Error", MB_ICONERROR | MB_OK);
		return 1;
	}

	std::string linkToken = "https://api.telegram.org/bot";
	fullLink = linkToken;
	
	//говнокод, прога парсит токен и чат айди, и соединяет ссылки для дальнейшего использования в функции SixSevenSound()
	std::getline(TokenChatId, linkToken);
	fullLink = fullLink + linkToken + "/sendMessage?chat_id=";
	std::getline(TokenChatId, linkToken);
	fullLink = fullLink + linkToken + "&text=!!!СРОЧНО ЗА КОМП, АДМИН ПИШЕТ!!!";

	TokenChatId.close();

	//тут цикл форич, который ищет файл по дате создания, соответственно, самый последний созданный
	fileSys::file_time_type FTT;
	bool first = true;
	for (const auto& l : fileSys::directory_iterator(finalPath)) {
		if (l.is_regular_file()) {
			auto time = fileSys::last_write_time(l);
			if (first || time > FTT) {
				FTT = time;
				finalPath = l.path().string();
				first = false;
			}
		}
	}

	//основной парсер логов, банальная проверка каждой строки на конкретные фразы, по другому не умею.
	//работает - не трогай :)
	std::fstream file(finalPath);
	if (!file) {
		MessageBox(NULL, L"Error, can`t open file", L"Error", MB_ICONERROR | MB_OK);
		return 1;
	}
	std::cout << "\nSuccesfully loaded log. Thank you for use!\n";
	std::string fileOut;
	while (true) {
		if (std::getline(file, fileOut)) {
			if (fileOut.rfind(u8"(( Администратор") && fileOut.rfind(u8"Вы тут?") != std::string::npos || fileOut.rfind(u8"(( Администратор") && fileOut.rfind(u8"вы тут?") != std::string::npos) {
				SendMsg(L"е.и да, я тут");
			}	
			else if(fileOut.rfind(u8"(( Администратор") && fileOut.rfind(u8"2 + 2") != std::string::npos || fileOut.rfind(u8"(( Администратор") && fileOut.rfind(u8"2+2=") != std::string::npos) {
				SendMsg(L"е.и 4");
			}
			else if (fileOut.rfind(u8"(( Администратор") && fileOut.rfind(u8"Столица") != std::string::npos) {
				SendMsg(L"е.и я не люблю географию");
			}
			else if (fileOut.rfind(u8"(( Администратор") && fileOut.rfind(u8"На каком вы сервере") != std::string::npos) {
				SendMsg(L"е.и аризона");
			}

		}
		else file.clear(); Sleep(50);
	}
	
}


//github of author: https://github.com/rukiamuq-hard