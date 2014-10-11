#ifndef __MARIO_H_
#define __MARIO_H_

#include "xdebug.h"
#include "env.h"
#include "status.h"
#include "port.h"
#include <deque>
#include "consumer.h"

namespace mario {

class Producer;
class Consumer;

class Mario
{
    public:
        Mario(uint32_t consumer_num, Consumer::Handler *h);
        ~Mario();
        Status Put(const std::string &item);
        Status Get();
        Env *env() { return env_; }
        Logger *info_log() { return info_log_; }
        WritableFile *writefile() { return writefile_; }

        Consumer *consumer() { return consumer_; }

    private:
        
        struct Writer;
        Producer *producer_;
        Consumer *consumer_;
        uint32_t consumer_num_;
        uint64_t item_num_;
        Env* env_;
        WritableFile *writefile_;
        SequentialFile *readfile_;
        Logger *info_log_;
        port::Mutex mutex_;
        port::CondVar bg_cv_;
        uint64_t file_num_;

        static void BGWork(void* m);
        void BackgroundCall();

        std::deque<Writer *> writers_;
        bool exit_all_consume_;
        char* pool_;

        // No copying allowed
        Mario(const Mario&);
        void operator=(const Mario&);

};

} // namespace mario

#endif
