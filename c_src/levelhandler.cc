#include "levelhandler.h"

LevelHandler::LevelHandler(size_t kMaxLogFileSize, const char* filename, const char* log_path)
{
    trace_log_ = new AutoRollLogger(kMaxLogFileSize, std::string(filename, strlen(filename)) + "trace", log_path, 1);
    debug_log_ = new AutoRollLogger(kMaxLogFileSize, std::string(filename, strlen(filename)) + "debug", log_path, 1);
    info_log_ = new AutoRollLogger(kMaxLogFileSize, std::string(filename, strlen(filename)) + "info", log_path, 1);
    warn_log_ = new AutoRollLogger(kMaxLogFileSize, std::string(filename, strlen(filename)) + "warn", log_path, 1);
    error_log_ = new AutoRollLogger(kMaxLogFileSize, std::string(filename, strlen(filename)) + "error", log_path, 1);
    fatal_log_ = new AutoRollLogger(kMaxLogFileSize, std::string(filename, strlen(filename)) + "fatal", log_path, 1);
}

LevelHandler::~LevelHandler()
{
    delete(trace_log_);
    delete(debug_log_);
    delete(info_log_);
    delete(warn_log_);
    delete(error_log_);
    delete(fatal_log_);
}

bool LevelHandler::processMsg(const std::string& item)
{
    const char *p = item.c_str();
    int level = 0;
    memcpy(&level, p, sizeof(int));
    switch (level) {
    case kTrace:
        Log(trace_log_, "%s", p + 4);
        break;
    case kDebug:
        Log(debug_log_, "%s", p + 4);
        break;
    case kInfo:
        Log(info_log_, "%s", p + 4);
        break;
    case kWarn:
        Log(warn_log_, "%s", p + 4);
        break;
    case kError:
        Log(error_log_, "%s", p + 4);
        break;
    case kFatal:
        Log(fatal_log_, "%s", p + 4);
        break;
    default:
        break;
    }

    return true;
}
