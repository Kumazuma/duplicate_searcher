#include"searcher.h"
#include <wx/dir.h>
#include<lsh/lsh.h>
#include <wx/file.h>
#include <wx/stream.h>
#include <wx/wfstream.h>
#include <wx/filename.h>
#include <unordered_set>
wxDEFINE_EVENT(EVT_SEARCHER_SUCCESS, wxThreadEvent);
wxDEFINE_EVENT(EVT_SEARCHER_PROCESS, wxThreadEvent);

class TravellerGetDir : public wxDirTraverser
{
public:
    TravellerGetDir(std::unordered_set<wxString>& directory) : directory{ directory } { }
    virtual wxDirTraverseResult OnFile(const wxString&)
    {
        return wxDIR_CONTINUE;
    }
    virtual wxDirTraverseResult OnDir(const wxString& dirname)
    {
        directory.emplace(dirname);
        return wxDIR_CONTINUE;
    }
private:
	std::unordered_set<wxString>& directory;
};

class TravellerGetFiles : public wxDirTraverser
{
public:
    TravellerGetFiles(std::unordered_set<wxString>& files, wxRegEx& regex) : files{ files }, regex{ regex }{ }
    virtual wxDirTraverseResult OnFile(const wxString& fileName)
    {
        if (regex.Matches(fileName))
            files.emplace(fileName);
        return wxDIR_CONTINUE;
    }
    virtual wxDirTraverseResult OnDir(const wxString& dirname)
    {
        return wxDIR_CONTINUE;
    }
private:
    std::unordered_set<wxString>& files;
    wxRegEx& regex;
};

wxIMPLEMENT_CLASS(Searcher, wxEvtHandler);

Searcher::Searcher(wxEvtHandler* parent):
    isRunning{ false }
{
    this->SetNextHandler(parent);
    this->regex.Compile(wxT("[^\t]*"));
}

Searcher::~Searcher()
{
    isRunning = false;
    if (thread.joinable())
        thread.join();
}

bool Searcher::SetFileExtFilter(const wxArrayString& arr)
{
    wxString pattern;
    pattern.reserve(1024);
    pattern += wxT("[^.]*.(");
    for (auto& ext: arr)
    {
        pattern += ext;
        pattern += wxT('|');
    }
    pattern.RemoveLast(1);
    pattern += wxT(")$");
    bool ret{ this->regex.Compile(pattern, wxRE_EXTENDED) };
    if (!ret)
        this->regex.Compile(wxT("[^\t]*"));

    return ret;
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
    std::unordered_set<wxString> directories;
    std::unordered_set<wxString> files;
    files.reserve(4096);
    directories.reserve(4096);
    TravellerGetDir travellerDir{ directories };
    TravellerGetFiles travellerFiles{ files, this->regex };
    for (const auto& path : targetPaths)
    {
        if (!isRunning)
            return;
        if (!wxDir::Exists(path))
            continue;
        
        directories.emplace(path);
        wxDir dir{ path };
        dir.Traverse(travellerDir);
    }
    for (const auto& path : directories)
    {
        if (!isRunning)
            return;
        wxDir dir{ path };
        dir.Traverse(travellerFiles, wxEmptyString, wxDIR_FILES | wxDIR_HIDDEN);
    }
    int i{};
    for (auto& filePath : files)
    {
        ++i;
        if (!isRunning)
            return;
        wxFile file;
        if (!wxFileName::IsFileReadable(filePath))
            continue;
        if (!file.Open(filePath))
            continue;
        uint8_t chunk[1024 * 8];
        LSH_Context context;
        lsh_init(&context, LSH_TYPE_256);
        wxFileInputStream fis{ file };
        wxBufferedInputStream bufferInput{ fis, 1024 * 32};
        while (!bufferInput.Eof())
        {
            auto size{ bufferInput.Read(chunk, sizeof(chunk)).LastRead() };
            if (size != 0)
                lsh_update(&context, chunk, size * 8);
            if (!isRunning)
                return;
        }

        std::array<uint8_t, 256 / 8> hash;
        lsh_final(&context, hash.data());
        duplicatedList[hash].push_back(filePath);
        auto evt{ new wxThreadEvent{EVT_SEARCHER_PROCESS} };
        evt->SetPayload(std::make_pair(i, static_cast<int>(files.size())));
        this->QueueEvent(evt);
    }
    auto evt{ new wxThreadEvent{EVT_SEARCHER_SUCCESS} };
    this->QueueEvent(evt);
}
