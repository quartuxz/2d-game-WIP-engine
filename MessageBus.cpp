#include "MessageBus.h"


size_t MessageBus::m_IDCounter = 0;

MessageBus::MessageBus(){

}

void MessageBus::addEntry(std::string MCName, size_t uniqueID){

    m_entryIDs[MCName] = uniqueID;
    m_entryNames[uniqueID] = MCName;
}

size_t MessageBus::getEntryID(std::string MCName){
    return m_entryIDs[MCName];

}

std::string MessageBus::getEntryName(size_t uniqueID){
    return m_entryNames[uniqueID];
}

const std::map<std::string, size_t> &MessageBus::getEntryIDs()const{
    return m_entryIDs;
}

size_t MessageBus::addMessagingComponent(MessagingComponent *messagingComponent){
    messagingComponent->init(m_IDCounter);
    m_messagingComponents[m_IDCounter] = messagingComponent;
    return m_IDCounter++;
}

void MessageBus::addMessage(MessageData *message){
    m_messageQueue.push(message);
}

void MessageBus::notify(){
    if(!m_messageQueue.empty()){
        MessageData *currentMessage = m_messageQueue.front();
        m_messageQueue.pop();
        for (auto const& x : m_messagingComponents)
        {
            x.second->processMessage(*currentMessage, this);
        }
        delete currentMessage;
    }


}

MessageBus::~MessageBus()
{
    for (auto const& x : m_messagingComponents)
    {
        delete x.second;
    }

    while(!m_messageQueue.empty()){
        delete m_messageQueue.front();
        m_messageQueue.pop();
    }
    //dtor
}
