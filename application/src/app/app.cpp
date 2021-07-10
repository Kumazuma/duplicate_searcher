#include "app.h"
#include "../mainform/mainform.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	searcher = nullptr;
	frame = new MainForm{};
	frame->Show();
	frame->Bind(wxEVT_BUTTON, &App::OnClickStart, this);
	return true;
}

int App::OnExit()
{
	if (searcher != nullptr)
		delete searcher;
	return wxApp::OnExit();
}

void App::OnClickStart(wxCommandEvent& event)
{
	auto dirCtrl{ wxDynamicCast(frame->FindWindowById(ID_DIR_CTRL), wxGenericDirCtrl) };
	auto startBtn{ wxDynamicCast(frame->FindWindowById(ID_START_CTRL), wxButton) };

	wxArrayString filePaths;
	std::vector<wxString> arr;
	dirCtrl->GetPaths(filePaths);
	arr.reserve(filePaths.size());
	if (searcher != nullptr)
		delete searcher;
	searcher = new Searcher(this);
	searcher->Bind(EVT_SEARCHER_PROCESS, &App::OnProcessSearch, this);
	searcher->Bind(EVT_SEARCHER_SUCCESS, &App::OnProcessSuccess, this);
	for (auto& dir : filePaths)
		arr.emplace_back(std::move(dir));
	searcher->SetTargetPaths(std::move(arr));
	startBtn->Enable(false);
	searcher->Run();
}

void App::OnProcessSearch(wxThreadEvent& event)
{
	auto process{ event.GetPayload<std::pair<int, int>>() };
	auto msg{ wxString::Format(wxT("%d/%d ¿Ï·á\n"), process.first,process.second) };
	auto log{ wxDynamicCast(frame->FindWindowById(ID_LOG_CTRL), wxTextCtrl) };
	
	log->AppendText(msg);
}

void App::OnProcessSuccess(wxThreadEvent& event)
{
	auto startBtn{ wxDynamicCast(frame->FindWindowById(ID_START_CTRL), wxButton) };
	startBtn->Enable();

	std::unordered_map<HashKey, std::vector<wxString> > table;
	searcher->GetDuplicateFiles(&table);
	auto reportCtrl{ wxDynamicCast(frame->FindWindowById(ID_REPORT), wxListCtrl) };
	int n{};
	int groupId{};
	reportCtrl->Freeze();
	reportCtrl->DeleteAllItems();
	for (auto& pair : table)
	{
		auto groupIdStr{ wxString::Format(wxT("%d"), groupId) };
		for (auto& file : pair.second)
		{
			wxListItem item;
			item.SetId(n);
			item.SetText(file);
			reportCtrl->InsertItem(item);
			reportCtrl->SetItem(n, 0, file);
			reportCtrl->SetItem(n, 1, groupIdStr);

			wxString fileSizeStr;
			auto fileSize{ wxFileName::GetSize(file) };
			if (fileSize < 1024)
			{
				fileSizeStr = wxString::Format(wxT("%d bytes"), (int)fileSize.GetValue());
			}
			else if(fileSize < 1024 * 1024)
			{
				fileSizeStr = wxString::Format(wxT("%f KiB"), fileSize.GetValue() /1024.0);
			}
			else if (fileSize < 1024 * 1024 * 1024)
			{
				fileSizeStr = wxString::Format(wxT("%f MiB"), fileSize.GetValue() / (1024.0 * 1024.0));
			}
			else if (fileSize < 1024 * 1024 * 1024 * 1024)
			{
				fileSizeStr = wxString::Format(wxT("%f GiB"), fileSize.GetValue() / (1024.0 * 1024.0 * 1024.0));
			}
			reportCtrl->SetItem(n, 2, fileSizeStr);
			++n;
		}
		++groupId;
	}
	reportCtrl->Thaw();
}
