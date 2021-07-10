#include"searcher.h"
#include <wx/dir.h>
#include<lsh/lsh.h>
#include <wx/file.h>

wxDEFINE_EVENT(EVT_SEARCHER_SUCCESS, wxThreadEvent);
wxDEFINE_EVENT(EVT_SEARCHER_PROCESS, wxThreadEvent);

class TravellerGetDir : public wxDirTraverser
{
public:
    TravellerGetDir(std::vector<wxString>& directory) : directory{ directory } { }
    virtual wxDirTraverseResult OnFile(const wxString&)
    {
        return wxDIR_CONTINUE;
    }
    virtual wxDirTraverseResult OnDir(const wxString& dirname)
    {
        directory.push_back(dirname);
        return wxDIR_CONTINUE;
    }
private:
	std::vector<wxString>& directory;
};

wxIMPLEMENT_CLASS(Searcher, wxEvtHandler);

Searcher::Searcher(wxEvtHandler* parent):
    isRunning{ false }
{
    this->SetNextHandler(parent);
}

Searcher::~Searcher()
{
    isRunning = false;
    if (thread.joinable())
        thread.join();
}

void Searcher::SetTargetPaths(const std::vector<wxString>& targetPaths)
{
    SetTargetPaths(std::vector<wxString>{targetPaths});
}

void Searcher::SetTargetPaths(std::vector<wxString>&& targetPaths)
{
    this->targetPaths.swap(targetPaths);
}

void Searcher::GetDuplicateFiles(std::unordered_map<HashKey, std::vector<wxString>>* ret)
{
    for (auto& it : duplicatedList)
    {
        if (it.second.size() >= 2)
        {
            (*ret)[it.first] = it.second;
        }
    }
}

void Searcher::Run()
{
    isRunning = true;
    thread = std::thread{ &Searcher::Process, this };
}

void Searcher::Process()
{
    std::vector<wxString> directories;
    TravellerGetDir traveller{ directories };
    for (const auto& path : targetPaths)
    {
        if (!isRunning)
            return;
        if (!wxDir::Exists(path))
            continue;
        
        directories.push_back(path);
        wxDir dir{ path };
        dir.Traverse(traveller);
    }
    wxArrayString files;
    for (const auto& path : directories)
    {
        if (!isRunning)
            return;
        wxDir::GetAllFiles(path, &files, wxEmptyString, wxDIR_FILES | wxDIR_HIDDEN | wxDIR_NO_FOLLOW);
    }
    int i{};
    for (auto& filePath : files)
    {
        if (!isRunning)
            return;
        wxFile file;
        if (!file.Open(filePath))
            continue;
        uint8_t chunk[1024 * 8];
        LSH_Context context;
        lsh_init(&context, LSH_TYPE_256);
        while (!file.Eof())
        {
            auto size = file.Read(chunk, sizeof(chunk));
            if (size != 0)
                lsh_update(&context, chunk, size);
            if (!isRunning)
                return;
        }
        std::array<uint8_t, 256 / 8> hash;
        lsh_final(&context, hash.data());
        file.Close();
        ++i;
        duplicatedList[hash].push_back(filePath);
        auto evt{ new wxThreadEvent{EVT_SEARCHER_PROCESS} };
        evt->SetPayload(std::make_pair(i, static_cast<int>(files.size())));
        this->QueueEvent(evt);
    }
    auto evt{ new wxThreadEvent{EVT_SEARCHER_SUCCESS} };
    this->QueueEvent(evt);
}
