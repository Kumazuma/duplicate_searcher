#pragma once
#include<wx/wx.h>
#include<wx/regex.h>
#include<vector>
#include<unordered_map>
#include<array>
#include<thread>
#include"hashkey.h"

class Searcher: public wxEvtHandler
{
	wxDECLARE_CLASS(Searcher);
public:
	Searcher(wxEvtHandler* parent);
	~Searcher();
	bool SetFileExtFilter(const wxArrayString& arr);
	void SetTargetPaths(const std::vector<wxString>& targetPaths);
	void SetTargetPaths(std::vector<wxString>&& targetPaths);
	void GetDuplicateFiles(std::unordered_map<HashKey, std::vector<wxString>>* ret);
	void Run();
protected:
	void Process();
private:
	bool isRunning;
	wxRegEx regex;
	std::thread thread;
	std::vector<wxString> targetPaths;
	std::unordered_map<HashKey, std::vector<wxString> > duplicatedList;
};

wxDECLARE_EVENT(EVT_SEARCHER_SUCCESS, wxThreadEvent);
wxDECLARE_EVENT(EVT_SEARCHER_PROCESS, wxThreadEvent);