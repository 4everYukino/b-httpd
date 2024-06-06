inline bool Message_Block::initialized() const
{
    return base_;
}

inline char* Message_Block::base() const
{
    return base_;
}

inline size_t Message_Block::capacity() const
{
    return total_size_;
}

inline size_t Message_Block::space() const
{
    return total_size_ - wr_ptr_;
}

inline size_t Message_Block::length() const
{
    return wr_ptr_ - rd_ptr_;
}

inline char* Message_Block::wr_ptr() const
{
    return base_ + wr_ptr_;
}

inline void Message_Block::wr_ptr(char* ptr)
{
    wr_ptr_ = ptr - base_;
}

inline void Message_Block::wr_ptr(size_t offset)
{
    wr_ptr_ += offset;
}

inline char* Message_Block::rd_ptr() const
{
    return base_ + rd_ptr_;
}

inline void Message_Block::rd_ptr(char* ptr)
{
    rd_ptr_ = ptr - base_;
}

inline void Message_Block::rd_ptr(size_t offset)
{
    rd_ptr_ += offset;
}
