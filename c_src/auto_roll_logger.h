#ifndef __ELOG_AUTO_ROLL_LOGGER_H__
#define __ELOG_AUTO_ROLL_LOGGER_H__

#include "mario/include/env.h"
#include "mario/include/status.h"
#include "mario/include/port.h"
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace mario;

class AutoRollLogger : public mario::Logger {
    public:
        AutoRollLogger(size_t kMaxLogFileSize, std::string log_name, std::string log_path, const int log_type);
        void Logv(const char* format, va_list ap);
        mario::Status GetNewFileName();
        mario::Status GetFileSize(const std::string& fname, uint64_t* size) const;
        Status ResetLogger();
        Status RollLogFile();
        Logger* logger() const { return logger_; }
        bool CheckExpired();

    private:
        const size_t kMaxLogFileSize_;
        std::string log_name_;
        std::string current_log_name_;
        Logger* logger_;
        mario::port::Mutex mutex_;
        int32_t file_num_;
        Env* env_;
        uint64_t current_time_;
        uint64_t expire_time_;
        // log_type: 0 for by size, 1 for by data
        int log_type_;
        int now_records_;
        int every_N_records_;
};

#endif
