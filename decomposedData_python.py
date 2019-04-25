import gameSerializationUtils

class decomposedData:
    def __init__(self):
        self.typeName = ""
        self.name = ""
        self.data = []
        self.childrenObjects = []

    def __m_parseData(self, passedData):
        tempData = gameSerializationUtils.tokenizeDecomposedDataData(passedData, ",")
        self.typeName = tempData[0]
        self.name = tempData[1]
        self.data = tempData[2:]

    def serialize(self):
        retVal = []
        retVal = ["{", self.typeName, ",", self.name, ","]
        for i in self.data:
            retVal.append(i)
            retVal.append(",")
        for o in self.childrenObjects:
            retVal.append(o.serialize())
        retVal.append("}")
        return "".join(retVal)

    def createFrom(self, passedData):
        allData = gameSerializationUtils.processBlock(passedData)
        self.__m_parseData(allData[0])
        self.childrenObjects = []
        allData = allData[1:]
        for i in allData:
            tempObj = decomposedData()
            tempObj.createFrom(i)
            self.childrenObjects.append(tempObj)
        return self


