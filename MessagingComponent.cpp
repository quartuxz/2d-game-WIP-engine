#include "MessagingComponent.h"


MessageData::MessageData(){

}

MessageData::~MessageData(){

}

MessageData::MessageTypes MessageData::stringToMessageType(std::string str){
	return MessageTypes();

}

MessageData::MessageData(std::string mProps, std::string mData){
    auto props = tokenizeDecomposedDataData(mProps);
    messageType = stringToMessageType(props[0]);
}

MessageData MessageData::processPythonFunc(boost::python::object &pyFunc, size_t selfID, const std::map<std::string, size_t> &nameIDMap)const{
    boost::python::list paramList;
    boost::python::dict IDDictionary;

    for(size_t i = 0; i < messageContents.size(); i++){
        paramList.append(messageContents[i].serialize());
    }

    for (auto const& it : nameIDMap)
    {
        IDDictionary[it.first] = it.second;
    }

    boost::python::object pyFuncRetVal = pyFunc(false, messageType, senderID, intendedReceiverID, paramList, IDDictionary);
    MessageData retVal;
    int tempMessageType = boost::python::extract<int>(pyFuncRetVal[0]);
    retVal.messageType = static_cast<MessageTypes>(tempMessageType);
    retVal.senderID = selfID;
    retVal.intendedReceiverID = boost::python::extract<int>(pyFuncRetVal[1]);
    retVal.messageContents = decomposeString(boost::python::extract<std::string>(pyFuncRetVal[2]));
    return retVal;
}

void MessagingComponent::init(std::size_t ID){
    p_uniqueID = ID;

}

void MessagingComponent::processMessage(const MessageData &tempMessage, MessageBus *bus){
    if(tempMessage.messageType != "NULL"){
        pv_processMessage(tempMessage, bus);
    }

}

MessagingComponent::MessagingComponent()
{
    //ctor
}

MessagingComponent::~MessagingComponent()
{
    //dtor
}
