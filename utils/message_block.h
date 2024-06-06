/// \brief inspired by ACE_Message_Block
/// \author Bruce

#ifndef MESSAGE_BLOCK_H_
#define MESSAGE_BLOCK_H_

#include "stddef.h"

#define DEFAULT_BUFFER_SIZE 2048

class Message_Block
{
public:
    ~Message_Block();

    bool init(size_t size = DEFAULT_BUFFER_SIZE);
    bool initialized() const;
    void free();

    /// Get base pointer.
    char* base() const;

    /// Return total size of buffer.
    size_t capacity() const;

    /// Return remain length of buffer, which is total_size_ - wr_ptr_;
    size_t space() const;

    /// Return valid size of buffer, which is wr_ptr_ - rd_ptr_.
    size_t length() const;

    /// Get write pointer.
    char* wr_ptr() const;

    /// Set write pointer to @a ptr.
    void wr_ptr(char* ptr);

    /// Set write pointer offset.
    void wr_ptr(size_t offset);

    /// Get read pointer.
    char* rd_ptr() const;

    /// Set read pointer to @a ptr.
    void rd_ptr(char* ptr);

    /// Set read pointer offset.
    void rd_ptr(size_t offset);

    /*

    before:
    |____________________|
           ↑      ↑
           r      w

    after:
    |____________________|
    ↑      ↑
    r      w

    */
    bool crunch();

private:
    /// Pointer to beginning of messages payload.
    char* base_ = nullptr;

    /// Total size of buffer.
    size_t total_size_;

    /// Pointer to beginning of next write.
    size_t wr_ptr_;

    /// Pointer to beginning of next read.
    size_t rd_ptr_;
};

#include "message_block.inl"

#endif
