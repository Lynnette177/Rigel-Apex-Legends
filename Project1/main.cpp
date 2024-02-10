#include "offsets.h"
#include "Driver.h"
#include<thread>
#include <iostream>
#include <Windows.h>
#include <string>
#include"auth.hpp"
#include "utils.hpp"
#include <iomanip>
#include "apex.hpp"
#include"mapdriver.hpp"
#include "SignatureScanner.h"
std::string tm_to_readable_time(tm ctx) {
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%a %m/%d/%y %H:%M:%S %Z", &ctx);

	return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp) {
	auto cv = strtol(timestamp.c_str(), NULL, 10); // long

	return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp) {
	std::tm context;

	localtime_s(&context, &timestamp);

	return context;
}
using namespace KeyAuth;
std::string name = skCrypt("ApexLegend").decrypt(); // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = skCrypt("ss3JcGuVnA").decrypt(); // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = skCrypt("76bfa5eb68d6705367b62a0621232224cb5cdbe6079ba28130dbf15a13ffadd9").decrypt(); // app secret, the blurred text on licenses tab and other tabs
std::string version = skCrypt("3.0").decrypt(); // leave alone unless you've changed version on website
std::string url = skCrypt("https://keyauth.win/api/1.2/").decrypt(); // change if you're self-hosting
api KeyAuthApp(name, ownerid, secret, version, url);
std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);
const std::string compilation_date = (std::string)skCrypt(__DATE__);
const std::string compilation_time = (std::string)skCrypt(__TIME__);
int main()
{
	if (1) {
		std::string consoleTitle = (std::string)skCrypt("Loader - Ver:  ") + compilation_date;
		SetConsoleTitleA(consoleTitle.c_str());
		std::cout << skCrypt("\n\nConnecting..");
		KeyAuthApp.init();
		if (!KeyAuthApp.data.success)
		{
			std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
			Sleep(1500);
			exit(0);
		}
		if (KeyAuthApp.checkblack()) {
			abort();
		}
		std::cout << skCrypt("\n Version: ") << KeyAuthApp.data.version;
		//std::cout << skCrypt("\n Customer panel link: ") << KeyAuthApp.data.customerPanelLink;
		//std::cout << skCrypt("\n Checking session validation status (remove this if causing your loader to be slow)");
		KeyAuthApp.check();
		std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;

		if (std::filesystem::exists(".\\test.json")) //change test.txt to the path of your file :smile:
		{
			if (!CheckIfJsonKeyExists(".\\test.json", "username"))
			{
				std::string key = ReadFromJson(".\\test.json", "license");
				KeyAuthApp.license(key);
				if (!KeyAuthApp.data.success)
				{
					std::remove(".\\test.json");
					std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
					Sleep(1500);
					exit(0);
				}
				std::cout << skCrypt("\nAuto logged in...");
			}
			else
			{
				std::string username = ReadFromJson(".\\test.json", "username");
				std::string password = ReadFromJson(".\\test.json", "password");
				KeyAuthApp.login(username, password);
				if (!KeyAuthApp.data.success)
				{
					std::remove(".\\test.json");
					std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
					Sleep(1500);
					exit(0);
				}
				std::cout << skCrypt("\nSucceed");
			}
			KeyAuthApp.log("Somebody else is using");
		}
		else
		{
			std::string username;
			std::string password;
			std::string key;
			std::cout << skCrypt("\n Input Your License: ");
			std::cin >> key;
			KeyAuthApp.license(key);
			if (!KeyAuthApp.data.success)
			{
				std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
				Sleep(1500);
				exit(0);
			}
			if (username.empty() || password.empty())
			{
				WriteToJson(".\\test.json", "license", key, false, "", "");
				std::cout << skCrypt("Succeed");
			}
			else
			{
				WriteToJson(".\\test.json", "username", username, true, "password", password);
				std::cout << skCrypt("Succeed");
			}


		}

		std::cout << skCrypt("\nUser Data:");
		//std::cout << skCrypt("\n Username: ") << KeyAuthApp.data.username;
		std::cout << skCrypt("\n IP Addr: ") << KeyAuthApp.data.ip;
		std::cout << skCrypt("\n HWID: ") << KeyAuthApp.data.hwid;
		std::cout << skCrypt("\n Activated at: ") << tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.data.createdate)));
		std::cout << skCrypt("\n Last Log in: ") << tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.data.lastlogin)));
		std::cout << skCrypt("\n License Information: \n ");

		for (int i = 0; i < KeyAuthApp.data.subscriptions.size(); i++) { // Prompto#7895 was here
			auto sub = KeyAuthApp.data.subscriptions.at(i);
			//std::cout << skCrypt("\n name: ") << sub.name;
			std::cout << skCrypt("Expires:") << tm_to_readable_time(timet_to_tm(string_to_timet(sub.expiry)));
		}

		std::cout << skCrypt("\n Checking..");
		KeyAuthApp.check();
		std::cout << skCrypt("\n Status:") << KeyAuthApp.data.message;
	}
	int x;
	std::cout << "Load Driver?\nInput:1(Load) 0(Do not)\nInput:";
	std::cin >> x;
	if (x) {
		mmap_driver();
	}
	while (!hwnd)
	{
		hwnd = FindWindowA(NULL, ("Apex Legends"));
		Sleep(1000);
		std::cout << "[+]Finding apex...\n";
	}
	static RECT TempRect = { NULL };
	static POINT TempPoint;
	GetClientRect(hwnd, &TempRect);
	ClientToScreen(hwnd, &TempPoint);
	TempRect.left = TempPoint.x;
	TempRect.top = TempPoint.y;
	screenWeight = TempRect.right;
	screenHeight = TempRect.bottom;
	
	while (!oPID) // get the process id
	{
		oPID = GetPID("r5apex.exe");
		printf("[+] Status Apex:Found\n");
		Sleep(500);
	}

	while (!oBaseAddress) // request the module base from driver
	{
		oBaseAddress = GetModuleBaseAddress(oPID, "r5apex.exe");
		std::cout << "Driver May Failed...Still Checking...\n";
		Sleep(1000);
	}
	std::cout << "[+]Driver Loaded\n[+]Rigel Launched";
	_beginthread((_beginthread_proc_type)mainthread, 0, 0);
	Sleep(10);
	_beginthread((_beginthread_proc_type)aimbotthread, 0, 0);
	_beginthread((_beginthread_proc_type)overlaythread, 0, 0);
	Sleep(10);
	_beginthread((_beginthread_proc_type)skinthread, 0, 0);
	Sleep(-1);
}