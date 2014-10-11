#include "auto_roll_logger.h"
#include "mario/include/mutexlock.h"
#include "mario/include/xdebug.h"

using namespace mario;
#define DAYTIME 86400

AutoRollLogger::AutoRollLogger(size_t kMaxLogFileSize, const std::string log_name, const std::string log_path, const int log_type)
    : kMaxLogFileSize_(kMaxLogFileSize),
    log_name_(log_name),
    current_log_name_(""),
    logger_(NULL),
    file_num_(0),
    env_(Env::Default()),
    log_type_(log_type),
    now_records_(0),
    every_N_records_(100)
{
    uint64_t now_t = env_->NowMicros() * (1e-6);

    // calculate the expire_time - 1, add 8 * 3600 to the now_t to make East eight area
    // the blow ResetLogger function will plus one day to the
    expire_time_ = now_t - ((now_t + 8 * 3600) % 86400);
    ResetLogger();
}

Status AutoRollLogger::ResetLogger()
{
    Status s;
    MutexLock l(&mutex_);
    GetNewFileName();
    if (logger_ != NULL) {
        delete(logger_);
    }
    env_->NewLogger(current_log_name_, &logger_);
    expire_time_ += DAYTIME;
    return s;
}

Status AutoRollLogger::GetNewFileName()
{
    mario::Status s;
    char buf[1024];
    if (log_type_ == 0) {
        snprintf(buf, sizeof(buf), "%s.%d", log_name_.c_str(), file_num_);
        file_num_++;
    } else if (log_type_ == 1) {
        time_t timer;
        time(&timer);
        struct tm *t;
        t = localtime(&timer);
        // t->tm_hour += 8;
        // mktime(t);
        snprintf(buf, sizeof(buf), "%s.%d%.2d%.2d", log_name_.c_str(), t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
    }
    current_log_name_ = std::string(buf, strlen(buf));
    return s;
}

Status AutoRollLogger::GetFileSize(const std::string& fname, uint64_t* size) const {
    Status s;
    struct stat sbuf;
    if (stat(fname.c_str(), &sbuf) != 0) { 
        *size = 0; 
        return Status::IOError(fname, errno);
    } else {
        *size = sbuf.st_size;
    }    
    return s;
}

Status AutoRollLogger::RollLogFile() 
{
    Status s;
    std::string old_file_name = current_log_name_;
    MutexLock l(&mutex_);
    GetNewFileName();
    env_->RenameFile(old_file_name, current_log_name_);
    return s;
}

bool AutoRollLogger::CheckExpired()
{
    if (now_records_ >= every_N_records_) {
        now_records_ = 0;
        return static_cast<uint64_t>(env_->NowMicros() * 1e-6) > expire_time_;
    } else {
        now_records_++;
        return false;
    }
}

void AutoRollLogger::Logv(const char* format, va_list ap)
{
    MutexLock l(&mutex_);
    uint64_t size_tmp = 0;
    // GetFileSize(current_log_name_, &size_tmp);
    if ((log_type_ == 0 && kMaxLogFileSize_ > 0 && size_tmp >= kMaxLogFileSize_) ||
            (log_type_ == 1 && CheckExpired())) {
        mutex_.Unlock();
        Status s = ResetLogger();
        mutex_.Lock();
        if (!s.ok()) {
            return;
        }
    }
    logger_->Logv(format, ap);
}

