#pragma once
#include <string>
namespace Mon {

	void initMon();
	void CheckBandwidth(double& mbrecv, double& mbsent);
	double CpuCurrentLoad();
	std::wstring GetWindowTxt();
	bool IsSessionLocked();
}