#ifndef __MARIO_CONSUMER_H_
#define __MARIO_CONSUMER_H_

#include "env.h"
#include "status.h"

namespace mario {


class Consumer 
{
    public:
        class Handler {
            public:
                virtual ~Handler();
                virtual void processMsg(const std::string &item) = 0;
        };

        Consumer(SequentialFile *queue, uint64_t initial_offset, Handler *h, char* pool, uint64_t pool_len);
        ~Consumer();
        Status Consume();

        uint64_t last_record_offset () const { return last_record_offset_; }


    private:
        SequentialFile* const queue_;
        char* const backing_store_;
        Slice buffer_;
        bool eof_;
        Handler *h_;

        uint64_t initial_offset_;
        uint64_t last_record_offset_;
        uint64_t end_of_buffer_offset_;

        unsigned int ReadPhysicalRecord(Slice *fragment);
        unsigned int ReadPhysicalRecord1(Slice *fragment);
        unsigned int ReadMemoryRecord(Slice *fragment);
        inline void CheckEnd(const int distance);
        inline void ConsumeData(const int distance);
        char* pool_;
        char* origin_;
        uint64_t pool_len_;
        // No copying allowed
        Consumer(const Consumer&);
        void operator=(const Consumer&);
};

}
#endif
