#include "filehandler.h"


FileHandler::FileHandler(size_t kMaxLogFileSize, const char* filename, const char* log_path)
{
    trace_log_ = new AutoRollLogger(kMaxLogFileSize, std::string(filename, strlen(filename)), log_path, 0);
}

FileHandler::~FileHandler()
{
    delete(trace_log_);
}

void FileHandler::processMsg(const std::string& item)
{
    Log(trace_log_, "%s", item.c_str());
}
