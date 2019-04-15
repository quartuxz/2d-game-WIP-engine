#ifndef MESSAGINGCOMPONENT_H
#define MESSAGINGCOMPONENT_H
#include <vector>
#include <string>
#include <map>
#include <boost/python.hpp>
#include "Serializable.h"



struct MessageData{
    enum MessageTypes{
    nullMessage,
    printToConsole
    };
    static MessageTypes stringToMessageType(std::string);
    MessageData();
    MessageData(std::string, std::string);
    ~MessageData();

    MessageTypes messageType;
    std::size_t senderID;
    std::size_t intendedReceiverID;

    //std::string printText;
    std::vector<decomposedData> messageContents;

    MessageData processPythonFunc(boost::python::object&, size_t, const std::map<std::string, size_t>&)const;

    inline MessageData setMessageType(MessageTypes type){
            messageType = type;
            return *this;
    }
    inline MessageData setSenderID(size_t ID){
        senderID = ID;
        return *this;
    }
    inline MessageData setIntendedReceiverID(size_t ID){
        intendedReceiverID = ID;
        return *this;
    }
    inline MessageData addMessageContents(decomposedData dData){
        messageContents.push_back(dData);
        return *this;
    }

};

inline MessageData makeMessageData(std::string messageProperties, std::string messageConts){

}

class MessageBus;

class MessagingComponent
{
    public:
        MessagingComponent();
        void processMessage(const MessageData&, MessageBus*);
        virtual ~MessagingComponent();
        void init(std::size_t);
    protected:
        virtual void pv_processMessage(const MessageData&, MessageBus*) = 0;
        std::size_t p_uniqueID;
};

#endif // MESSAGINGCOMPONENT_H