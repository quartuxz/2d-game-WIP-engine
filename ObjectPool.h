#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H
#include "MessagingComponent.h"

class ObjectPool : public MessagingComponent
{
    public:
        ObjectPool();
        void pv_processMessage(const MessageData&, MessageBus*)override;
        ~ObjectPool();

    protected:

    private:
};

#endif // OBJECTPOOL_H
