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

size_t MessageBus::addMessagingComponent(MessagingComponent *messagingComponent, bool dynamicObj){
    messagingComponent->init(m_IDCounter);
    m_messagingComponents[m_IDCounter] = messagingComponent;
    return m_IDCounter++;
	if (dynamicObj) {
		m_dynamicMessagingComponents.push_back(messagingComponent);
	}
}

MessagingComponent* MessageBus::getMessagingComponent(size_t ID) {
	return m_messagingComponents[ID];
}

void MessageBus::setUpdateInterval(float interval) {
	m_updateInterval = interval;
}

void MessageBus::startFrame(float delta) {
	m_addedTime += delta;
}
bool MessageBus::canMessage() {
	if (m_addedTime >= m_updateInterval) {
		return true;
	}
	return false;
}
void MessageBus::endFrame() {
	if (canMessage()) {
		m_addedTime = 0;
	}
}

void MessageBus::addMessage(MessageData *message){
	if (message->messageType != "NULL") {
		if (message->messageType == "messages") {
			for (size_t i = 0; i < message->messageContents.size(); i++)
			{
				MessageData *tempMessage = new MessageData();
				tempMessage->createFrom(message->messageContents[i]);
				addMessage(tempMessage);
			}
		}
		else {
			m_messageQueue.push(message);
		}
	}

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
    for (auto const& x : m_dynamicMessagingComponents)
    {
        delete x;
    }

    while(!m_messageQueue.empty()){
        delete m_messageQueue.front();
        m_messageQueue.pop();
    }
    //dtor
}
