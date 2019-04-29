#import multiprocessing
#import shared_memory

def generateMemoryBlock(memorySize):
    shm = shared_memory.SharedMemory(True, memorySize)
    shm.close()
    #while True:
        #print("hello")
    return shm.name