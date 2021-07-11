#include "app.h"
#include "mainform.h"
#include "duplicate_file_view_model.h"
wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	searcher = nullptr;
	frame = new MainForm{};
	frame->Show();
	frame->Bind(wxEVT_BUTTON, &App::OnClickStart, this, ID_START_CTRL);
	frame->Bind(wxEVT_BUTTON, &App::OnClickDelectSelctedFiles, this, ID_DEL_SEL_FILES);
	frame->Bind(wxEVT_BUTTON, &App::OnClickMoveFilesTo, this, ID_MOVE_SEL_FILES_TO);

	viewModel = new DuplicateFileViewModel{ table, selectedFilesTable };
	auto reportCtrl{ wxDynamicCast(frame->FindWindowById(ID_REPORT), wxDataViewCtrl) };
	reportCtrl->AssociateModel(viewModel);

	return true;
}

int App::OnExit()
{
	if (searcher != nullptr)
		delete searcher;
	viewModel->DecRef();
	return wxApp::OnExit();
}

void App::OnClickStart(wxCommandEvent& event)
{
	auto dirCtrl{ wxDynamicCast(frame->FindWindowById(ID_DIR_CTRL), wxGenericDirCtrl) };
	auto startBtn{ wxDynamicCast(frame->FindWindowById(ID_START_CTRL), wxButton) };
	auto filterCtrl{ wxDynamicCast(frame->FindWindowById(ID_EXT_FILTER_CTRL), wxTextCtrl) };
	auto filter{ filterCtrl->GetValue().Trim() };
	auto exts{ wxSplit(filter, wxT(' ')) };

	wxArrayString filePaths;
	std::vector<wxString> arr;
	dirCtrl->GetPaths(filePaths);
	arr.reserve(filePaths.size());
	if (searcher != nullptr)
		delete searcher;
	searcher = new Searcher(this);
	if (exts.size() != 0 && !searcher->SetFileExtFilter(exts))
	{
		wxMessageBox(wxT("Failed to generate filter"));
		delete searcher;
		searcher = nullptr;
		return;
	}

	searcher->Bind(EVT_SEARCHER_PROCESS, &App::OnProcessSearch, this);
	searcher->Bind(EVT_SEARCHER_SUCCESS, &App::OnProcessSuccess, this);
	for (auto& dir : filePaths)
		arr.emplace_back(std::move(dir));
	searcher->SetTargetPaths(std::move(arr));
	startBtn->Enable(false);
	searcher->Run();
}

void App::OnClickDelectSelctedFiles(wxCommandEvent& event)
{
	for (auto& pair : selectedFilesTable)
	{
		auto& selectedFiles{ pair.second };
		auto it{ selectedFiles.begin() };
		const auto end{ selectedFiles.end() };
		auto& files{ this->table[pair.first] };
		if (files.size() == selectedFiles.size())
			++it;

		if (it == end)
			continue;

		for (; it != end; ++it)
		{
			wxRemoveFile(*it);
			auto filesIt{ std::find(files.begin(), files.end(), *it) };
			files.erase(filesIt);
		}
		selectedFiles.clear();
	}
	UpdateViewModel();
}

void App::OnClickMoveFilesTo(wxCommandEvent& event)
{
	wxDirDialog dirDialog{ frame };
	if (dirDialog.ShowModal() != wxID_OK)
		return;
	wxString dirMoveTo{ dirDialog.GetPath() };
	for (auto& pair : selectedFilesTable)
	{
		auto& files{ pair.second };
		for (auto& file : files)
		{
			wxFileName fileName{ file };
			wxFileName movedFileName;
			movedFileName.SetFullName(fileName.GetFullPath());
			movedFileName.SetPath(dirMoveTo);
			wxString fullPath{ movedFileName.GetFullPath() };
			if (wxRenameFile(fileName.GetFullPath(), fullPath))
			{
				auto& files{ table[pair.first] };
				auto it{ std::find(files.begin(), files.end(), file) };
				if (it != files.end())
					it->swap(fullPath);
			}
		}
		selectedFilesTable[pair.first].clear();
	}
	UpdateViewModel();
}

void App::OnProcessSearch(wxThreadEvent& event)
{
	auto process{ event.GetPayload<std::pair<int, int>>() };
	auto msg{ wxString::Format(wxT("%d/%d ¿Ï·á\n"), process.first,process.second) };
	auto gauge{ wxDynamicCast(frame->FindWindowById(ID_PROGRESS), wxGauge) };
	gauge->Freeze();
	if (gauge->GetRange() != process.second)
	{
		gauge->SetRange(process.second);
	}
	gauge->SetValue(process.first);
	gauge->Thaw();
	wxPrintf(msg);
}

void App::OnProcessSuccess(wxThreadEvent& event)
{
	auto startBtn{ wxDynamicCast(frame->FindWindowById(ID_START_CTRL), wxButton) };

	startBtn->Enable();

	table.clear();
	searcher->GetDuplicateFiles(&table);
	UpdateViewModel();
}

void App::UpdateViewModel()
{
	auto reportCtrl{ wxDynamicCast(frame->FindWindowById(ID_REPORT), wxDataViewCtrl) };

	viewModel->DecRef();
	viewModel = new DuplicateFileViewModel{ table, selectedFilesTable };
	reportCtrl->Freeze();
	reportCtrl->ClearColumns();

	reportCtrl->AssociateModel(viewModel);
	wxDataViewColumn* const colCheckIconText = new wxDataViewColumn
	(
		L"\u2714 Path",
		new wxDataViewCheckIconTextRenderer(wxDATAVIEW_CELL_ACTIVATABLE),
		0,
		wxCOL_WIDTH_AUTOSIZE
	);
	reportCtrl->AppendColumn(colCheckIconText);
	reportCtrl->AppendTextColumn(wxT("Size"), 1, wxDATAVIEW_CELL_ACTIVATABLE, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE);
	reportCtrl->AppendTextColumn(wxT("Modified"), 2, wxDATAVIEW_CELL_ACTIVATABLE, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE);

	wxDataViewItemArray arr;
	viewModel->GetChildren(wxDataViewItem{ nullptr }, arr);
	for (auto& it : arr)
	{
		reportCtrl->ExpandChildren(it);
	}

	reportCtrl->Thaw();
}
