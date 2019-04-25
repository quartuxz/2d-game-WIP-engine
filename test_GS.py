import decomposedData_python

def gameScript(askingForName, messageType, senderID, intendedReceiverID, paramList, IDDictionary):
    inDData0 = decomposedData_python.decomposedData()
    inDData0.createFrom(paramList[0])
    print inDData0.data[0]

    counterVal = str(int(inDData0.data[1])+1)
    #print counterVal
    retDData = decomposedData_python.decomposedData()

    if (int(inDData0.data[1]) > 1000):

        retDData.data.append(inDData0.data[0]+".")
        retDData.data.append(str(0))
    else:
        retDData.data.append(inDData0.data[0])
        retDData.data.append(counterVal)
    retDData.typeName = "asd"
    retDData.name = "asd_001"
    return (1, 0, retDData.serialize()+";")

