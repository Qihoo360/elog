#ifndef __ELOG_LEVEL_HANDLER_H__
#define __ELOG_LEVEL_HANDLER_H__

#include "mario/include/mario.h"
#include "mario/include/consumer.h"
#include "mario/include/mutexlock.h"
#include "mario/include/port.h"
#include "auto_roll_logger.h"

class LevelHandler : public mario::Consumer::Handler {
    public:
        LevelHandler(size_t kMaxLogFileSize, const char* filename, const char* log_path); 
        ~LevelHandler();
        virtual void processMsg(const std::string& item);

    private:
        enum Level {
            kTrace = 0,
            kDebug = 2,
            kInfo = 4,
            kWarn = 8,
            kError = 16,
            kFatal = 32
        };
        AutoRollLogger *trace_log_;
        AutoRollLogger *debug_log_;
        AutoRollLogger *info_log_;
        AutoRollLogger *warn_log_;
        AutoRollLogger *error_log_;
        AutoRollLogger *fatal_log_;
};

#endif
