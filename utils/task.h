#ifndef TASK_H_
#define TASK_H_

class Task
{
public:
    Task(int id) {
        id_ = id;
    }

    virtual bool run() {
        return true;
    }

    virtual void after_failed() {
        return;
    }

    virtual void after_complated() {
        return;
    }

protected:
    int id_;
};

#endif
