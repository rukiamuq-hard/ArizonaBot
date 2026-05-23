//Author: rukiamuq
//The author is not responsible for any damage caused by the script.
//A curriculum that does not have malicious intent.
#include <codecvt>
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
#include <curl/curl.h>
#include <thread>
#include "json.hpp"

#pragma comment(lib, "Shell32.lib")

namespace fileSys = std::filesystem;
using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
	userp->append((char*)contents, size * nmemb);
	return size * nmemb;
}

std::string toUTFeight(std::string system_str) {
	if (system_str.empty()) return "";

	int w_size = MultiByteToWideChar(CP_ACP, 0, system_str.c_str(), -1, nullptr, 0);
	std::wstring w_str(w_size, 0);
	MultiByteToWideChar(CP_ACP, 0, system_str.c_str(), -1, &w_str[0], w_size);

	int u8_size = WideCharToMultiByte(CP_UTF8, 0, w_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string u8_str(u8_size, 0);
	WideCharToMultiByte(CP_UTF8, 0, w_str.c_str(), -1, &u8_str[0], u8_size, nullptr, nullptr);

	if (!u8_str.empty() && u8_str.back() == '\0') {
		u8_str.pop_back();
	}

	return u8_str;
}

std::wstring UTF8ToWide(const std::string& utf8_str) {
	if (utf8_str.empty()) return L"";
	int w_size = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
	std::wstring w_str(w_size, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &w_str[0], w_size);
	if (!w_str.empty() && w_str.back() == L'\0') w_str.pop_back();
	return w_str;
}

int SixSevenSend() {
	setlocale(LC_ALL, "UKR");
	std::ifstream TokenChatId("TokenChatId.txt");

	std::string token, chat_id;
	std::getline(TokenChatId, token);
	std::getline(TokenChatId, chat_id);
	TokenChatId.close();

	std::string url = "https://api.telegram.org/bot" + token + "/sendMessage";
	std::string fields = "chat_id=" + chat_id + u8"&text=!!!СРОЧНО ЗА КОМП, АДМИН ПИШЕТ!!!";

	std::cout << url + chat_id;
	
	CURL* curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields.c_str());
		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			std::cerr << "Error sending msg, check chat_id or token";
		}

		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
}

std::string aiResponse(std::string vprs) {
		SetConsoleOutputCP(CP_UTF8);

		CURL* curl;
		CURLcode res;

		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();
		std::ifstream PROMPT("prompt.txt");
		if (!PROMPT)
		{
			std::cout << "file not opened\n";
		}

		std::string prompt((std::istreambuf_iterator<char>(PROMPT)), std::istreambuf_iterator<char>());
		PROMPT.close();

		std::string tokenApiGemini, readbuff;
		PROMPT.open("TokenChatId.txt");
		for (std::size_t i = 0; i < 3; i++) {
			std::getline(PROMPT, tokenApiGemini);
		}
		PROMPT.close();

		if (curl) {
			json send;
			send["contents"] = json::array();
			send["contents"].push_back({
				{"role", "user"},
				{"parts", json::array({
					{{"text", prompt + toUTFeight(vprs)}}
				})}
			});
			std::string jsonString = send.dump();
			std::string model = "gemini-3.1-flash-lite";
			std::string url = "https://generativelanguage.googleapis.com/v1beta/models/" + model + ":generateContent?key=" + tokenApiGemini;;
			std::cout << jsonString;

			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());

			struct curl_slist* headers = nullptr;
			headers = curl_slist_append(headers, "Content-Type: application/json");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readbuff);

			res = curl_easy_perform(curl);

			if (res != CURLE_OK) {
				std::cerr << "Curl ERROR!" << curl_easy_strerror(res);
			}
			else {
				try {
					json Response_json = json::parse(readbuff);
					std::string res_text = Response_json["candidates"][0]["content"]["parts"][0]["text"].get<std::string>().c_str();
					return res_text;
				}
				catch (...) {
					std::cout << "cant parse json:\n  " << readbuff << std::endl;
				}
			}
			curl_slist_free_all(headers);
			curl_easy_cleanup(curl);
		}
		curl_global_cleanup();
}

void SendUnicodeChar(wchar_t ch) {
	INPUT inputs[2] = {};

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = 0;
	inputs[0].ki.wScan = ch;
	inputs[0].ki.dwFlags = KEYEVENTF_UNICODE;

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = 0;
	inputs[1].ki.wScan = ch;
	inputs[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;

	SendInput(2, inputs, sizeof(INPUT));
	Sleep(50 + rand() % 150);
}

void SendEnter() {
	INPUT inputs[2] = {};

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = VK_RETURN;
	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = VK_RETURN;
	inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(2, inputs, sizeof(INPUT));
}

void SendMsg(const std::string& msgToFuncUTF8) {
	std::wstring wideMsg = UTF8ToWide(msgToFuncUTF8);

	Sleep(rand() % 500 + 300);

	SendEnter();
	Sleep(200);

	std::wstring tslB = L"t/b ";
	for (wchar_t ch : tslB) {
		SendUnicodeChar(ch);
	}
	for (wchar_t ch : wideMsg) {

		if (ch == L'\n' || ch == L'\r') continue;
		SendUnicodeChar(ch);
	}

	Sleep(200);

	SendEnter();
}


int main() {
	srand(0);
	
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

	std::string finalPath;
	if (fileSys::exists(startPath) && fileSys::is_directory(startPath)) {
		for (const auto& feach : fileSys::recursive_directory_iterator(startPath)) {
			if (feach.is_directory() && feach.path().filename() == "chatlog") {
					finalPath.append(feach.path().string());
			}
		}
	}
	
	
	std::ifstream TokenChatId("TokenChatId.txt");
	if (!TokenChatId) {
		MessageBox(NULL, L"Can`t open Token and Chat id`s file, creating file on programm catalog...", L"Error", MB_OK | MB_ICONERROR);
		std::ofstream ChatToken("TokenChatId.txt");
		MessageBox(NULL, L"Text file succesfully created, add your token and chat id", L"Error", MB_ICONERROR | MB_OK);
		MessageBox(NULL, L"First Line = Token, Second Line = ChatID.", L"Error", MB_ICONERROR | MB_OK);
		ChatToken.close();
		return 1;
	}

	TokenChatId.close();

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

	std::fstream file(finalPath);
	if (!file) {
		MessageBox(NULL, L"Error, can`t open file", L"Error", MB_ICONERROR | MB_OK);
		return 1;
	}
	std::cout << "\nSuccesfully loaded log. Thank you for use!\n";

	std::string fileOut;
	while (true) {
		//u8"(( Администратор")
		if (std::getline(file, fileOut)) {
			std::size_t pos = fileOut.rfind(u8"(( Администратор");
			if ( pos != std::string::npos ) {
				std::string otv = u8"\nВот вопрос: " + fileOut.substr(pos);
				std::thread sendTELEGRAMM(SixSevenSend);
				sendTELEGRAMM.detach();
				SendMsg(aiResponse(otv).c_str());
			}

		}
		else file.clear(); Sleep(125);
	}
}


//github of author: https://github.com/rukiamuq-hard