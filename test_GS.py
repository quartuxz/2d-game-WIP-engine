#import multiprocessing
#import shared_data
import decomposedData_python
import random




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
    retDData.data.append("player.png") #the name of the image
    tempChildren = decomposedData_python.decomposedData()
    tempChildren.data.append(str(random.randint(250,500))) #the x and y positions of the image
    tempChildren.data.append(str(random.randint(250,500)))
    retDData.childrenObjects.append(tempChildren)
    retDData.data.append("5") #this is the scale of the image
    retDData.data.append("3") #this is how much time the image is displayed for
    #retDData.data.append("10") #this is how much time the image is displayed for
    retMessageDData.childrenObjects.append(retDData)

    gameDataRetMessageDData = decomposedData_python.decomposedData()
    gameDataRetMessageDData.data.append("editGameData")
    gameDataRetMessageDData.data.append(str(selfID))
    gameDataRetMessageDData.data.append(str(0))
    gameDataRetMessageDData.childrenObjects.append(gameData)
    #shm.close()

    #return("displayDecal", 0, decomposedData_python.formatToSend(retDData))
    #return ("messages", 0, decomposedData_python.makeDDataMessage("displayDecal", selfID, 0, retDData).serialize()+";"+ gameDataRetMessageDData.serialize()+";")
    return ("messages", 0, decomposedData_python.formatToSend(decomposedData_python.makeDDataMessage("displayDecal", selfID, 0, retDData), decomposedData_python.makeDDataMessage("editGameData", selfID, 0, gameData)))

