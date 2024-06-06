#include "message_block.h"

#include <cstdlib>

#include <spdlog/spdlog.h>

Message_Block::~Message_Block()
{
    free();
}

bool Message_Block::init(size_t size)
{
    if (initialized())
        return true;

    base_ = (char *)malloc(size);
    if (!base_) {
        spdlog::error("Not enough buffer space {} to init message block.", size);
        return false;
    }

    total_size_ = size;
    wr_ptr_ = 0;
    rd_ptr_ = 0;

    return true;
}

void Message_Block::free()
{
    if (base_) {
        ::free(base_);
        base_ = nullptr;
    }
}

bool Message_Block::crunch()
{
    if (rd_ptr_ != 0) {
        if (rd_ptr_ > wr_ptr_) {
            spdlog::error("Unexpected situation: rd_ptr_ > wr_ptr_, cannot crunch.");
            return false;
        }

        auto len = length();
        memmove(base_, rd_ptr(), len);
        rd_ptr(base_);
        wr_ptr(base_ + len);
    }

    return true;
}
