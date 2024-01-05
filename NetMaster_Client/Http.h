#pragma once
namespace Http {

	std::vector<char> HttpGet(char* url);
	BOOL DwnFile(char* url, char* path);
	std::string HttpPost(const char* host, const std::string& postData, const char* endpoint);
	BOOL HttpInit();
	std::string extractFileName(const std::string& url);
}