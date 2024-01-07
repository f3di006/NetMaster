#pragma once
namespace Http {
	struct FileDownload {

		std::string url;
		std::string path;
		BOOL exec;

	};
	std::vector<char> HttpGet(char* url);
	void _stdcall DwnFile(FileDownload* file);
	std::string HttpPost(const char* host, const std::string& postData, const char* endpoint);
	BOOL HttpInit();
	std::string extractFileName(const std::string& url);

	

}