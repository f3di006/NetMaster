#pragma once
namespace Utils {

	void SendText(const wchar_t*);
	BOOL spawnLogOn(PWSTR path);
	void CreateProc(wchar_t* path, wchar_t* args, BOOL wait = FALSE);
	void runAsUser(wchar_t* path, wchar_t* arg, BOOL wait = FALSE,BOOL retry=FALSE);
	void UnlockScreen(std::string password);
	void LockScreen();
	void blockHost(char* host);
	void CleanUp();
	std::vector<std::string> splitLines(const std::string& input, char lineSeparator);
	void UpdateHosts();
	void commandrun(char* command);
}
