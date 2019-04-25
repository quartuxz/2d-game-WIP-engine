#include "ObjectPool.h"

ObjectPool::ObjectPool()
{
    //ctor
}

void ObjectPool::pv_processMessage(const MessageData &message, MessageBus *bus){
    switch(message.messageType){
    case MessageData::printToConsole:
        break;
    }
}

ObjectPool::~ObjectPool()
{
    //dtor
}
