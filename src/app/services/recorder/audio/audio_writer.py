import wave
import os

from PyQt5.QtCore import QObject

class AudioWriter():

    def __init__(self, outputFolder, nChannels, nChannelFile, byteDepth, sampleRate):
        self.isRecording = False
        self.wavFiles = []
        self.outputFolder = outputFolder
        self.nChannels = nChannels
        self.nChannelsFile = nChannelFile
        self.byteDepth = byteDepth
        self.sampleRate = sampleRate
        self.sourcesBuffer = {'0' :bytearray(), '1': bytearray(), '2': bytearray(), '3': bytearray()}
        
        self.createNewFiles()
        

    
    def createNewFiles(self):
        for i in range(0, self.nChannels):
            outputFile = os.path.join(self.outputFolder, 'outputsrc-{}.wav'.format(i))
            if outputFile and os.path.exists(self.outputFolder):
                self.wavFiles.append(wave.open(outputFile, 'wb'))
                self.wavFiles[i].setnchannels(self.nChannelsFile)
                self.wavFiles[i].setsampwidth(self.byteDepth)
                self.wavFiles[i].setframerate(self.sampleRate)
                self.sourcesBuffer[str(i)] = bytearray()


    def write(self, data):
        offset = 0
        while offset < len(data):
            for key, _ in self.sourcesBuffer.items():
                currentByte = int(offset + int(key))
                self.sourcesBuffer[key] += data[currentByte:currentByte + self.byteDepth]
        
            offset += self.nChannels * self.byteDepth


    def close(self):
        for index, wavFile in enumerate(self.wavFiles):
            audioRaw = self.sourcesBuffer[str(index)]
            if audioRaw and wavFile:
                wavFile.writeframesraw(audioRaw)
             
            if wavFile:
                wavFile.close()
            
            self.sourcesBuffer[str(index)] = bytearray()
        
        self.wavFiles = []

