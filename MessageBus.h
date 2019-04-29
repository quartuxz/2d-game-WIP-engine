#ifndef MESSAGEBUS_H
#define MESSAGEBUS_H
#include<queue>
#include<map>
#include "MessagingComponent.h"

class MessageBus
{
    public:
        MessageBus();
        //add identifiers for MessagingComponents that last the entire lifetime of the MessageBus (can be reffered to at any time)
        void addEntry(std::string, size_t);
        size_t getEntryID(std::string);
        std::string getEntryName(size_t);
        const std::map<std::string, size_t> &getEntryIDs()const;
        //

		void setUpdateInterval(float);

		void startFrame(float);
		bool canMessage();
		void endFrame();
		//bool autoCapFunction = false;

		MessagingComponent *getMessagingComponent(size_t);
        size_t addMessagingComponent(MessagingComponent*, bool dynamicObj = false);
        void addMessage(MessageData*);
        void notify();

        ~MessageBus();

    private:
        //debugging infinite loops
        size_t m_maxChainLength = 10;
        size_t m_chainLength = 0;
        MessageData m_lastMessage;
        //~

		//capping messaging per second
		float m_addedTime = 0;
		float m_updateInterval = 0.5;

		//

        //entry handling(named MessagingComponents)
        std::map<std::string, size_t> m_entryIDs;
        std::map<size_t, std::string> m_entryNames;
        //

		//memory management
		std::vector<MessagingComponent*> m_dynamicMessagingComponents;
		//

        //core messaging
        std::queue<MessageData*> m_messageQueue;
        static size_t m_IDCounter;
        std::map<size_t, MessagingComponent*> m_messagingComponents;
};

#endif // MESSAGEBUS_H
