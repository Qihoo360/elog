#ifndef __ELOG_FILE_HANDLER_H__
#define __ELOG_FILE_HANDLER_H__

#include "mario/include/mario.h"
#include "mario/include/consumer.h"
#include "mario/include/mutexlock.h"
#include "mario/include/port.h"
#include "auto_roll_logger.h"

using namespace mario;

/**
 * @name The consumer handler
 * @{ */
/**  @} */
class FileHandler : public mario::Consumer::Handler {
    public:
        FileHandler(size_t kMaxLogFileSize, const char* filename, const char* log_path); 
        ~FileHandler();
        virtual bool processMsg(const std::string& item);

    private:
        AutoRollLogger *trace_log_;
};


#endif
