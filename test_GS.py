#import multiprocessing
#import shared_data
import decomposedData_python

def makeDDataMessage(messageType, senderID, intendedReceiverID, *DData):
    retDDataMessage = decomposedData_python.decomposedData()
    retDDataMessage.data.append(messageType)
    retDDataMessage.data.append(str(senderID))
    retDDataMessage.data.append(str(intendedReceiverID))
    for i in DData:
       retDDataMessage.childrenObjects.append(i)
    return retDDataMessage

def formatToSend(*DData):
    retStr = ""
    for i in DData:
        retStr = retStr +  DData.serialize() + ";"
    return retStr

def gameScript(askingForName, messageType, senderID, intendedReceiverID, paramList, IDDictionary, selfID, metaData):
    if senderID == selfID:
        if intendedReceiverID == selfID:
            return ("NULL",0, "asd")
        return ("NULL",0, "")

    gameData = decomposedData_python.decomposedData()
    gameData.createFrom(metaData)


    if messageType == "playerData":
        inDData0 = decomposedData_python.decomposedData()
        inDData0.createFrom(paramList[0])



    #shm = shared_memory.SharedMemory(memoryBlockName)
    if messageType == "onProgramEnd":
        #shm.unlink()
        return("NULL", 0, "")

    if messageType == "onProgramStart":
        #shm.buf[1] = 69
        return ("NULL",0,"")


    #shm.buf[0] = 10
    #print(shm.buf)
    retMessageDData = decomposedData_python.decomposedData()
    retMessageDData.data.append("displayDecal")
    retMessageDData.data.append(str(selfID))
    retMessageDData.data.append(str(0))
    retDData = decomposedData_python.decomposedData()
    retDData.type = "AnimatorSprite"
    retDData.name = "displayedSpriteName"
    retDData.data.append("tooltip.png") #the name of the image
    tempChildren = decomposedData_python.decomposedData()
    tempChildren.data.append("500") #the x and y positions of the image
    tempChildren.data.append("500")
    retDData.childrenObjects.append(tempChildren)
    retDData.data.append("10") #this is the scale of the image
    retDData.data.append("10") #this is how much time the image is displayed for
    retDData.data.append("10") #this is how much time the image is displayed for
    retMessageDData.childrenObjects.append(retDData)

    gameDataRetMessageDData = decomposedData_python.decomposedData()
    gameDataRetMessageDData.data.append("editGameData")
    gameDataRetMessageDData.data.append(str(selfID))
    gameDataRetMessageDData.data.append(str(0))
    gameDataRetMessageDData.childrenObjects.append(gameData)
    #shm.close()

    return("displayDecal", 0, retDData.serialize() + ";")

    return ("messages", 0, makeDDataMessage("displayDecal", selfID, 0, retDData).serialize() + ";" + makeDDataMessage("editGameData", selfID, 0, gameData).serialize()+ ";")

