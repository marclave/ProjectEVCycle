from PyQt5.QtCore import QUrl, QObject, QVariant, QRunnable, QCoreApplication, QThread, pyqtSignal, QThreadPool
from PyQt5.QtGui import QGuiApplication 
from PyQt5.QtQuick import QQuickView
import os
import sys
import time
import threading
import CAN_Main
  
#inherit from Qthread and setup our own thread class  
class updateThread(QThread):
    speedSignal = pyqtSignal(str) #create a custom sygnal we can subscribe to to emit update commands 
    timeSignal = pyqtSignal(str) #create a custom sygnal we can subscribe to to emit update commands
    voltageSignal = pyqtSignal(str) #create a custom sygnal we can subscribe to to emit update commands 
      
    def __init__(self,parent=None):  
        super(updateThread,self).__init__(parent)  
        self.exiting = False
        self.canMain = CAN_Main.CAN_Main()  
  
    def run(self):
        self.speed = 0
        while True:  
            self.msleep(1000)  
            self.speedSignal.emit(str(self.speed))
            self.speed = self.speed + 1  
  
class myDialog(QQuickView):  
    def __init__(self,parent=None):  
        super(myDialog,self).__init__(parent)  
        self.resize(1000,1600)  
        
        self.setSource(QUrl.fromLocalFile(os.path.join(os.path.dirname(__file__),'TestPS2QT.qml')))
        
        self.setupUpdateThread()  
        self.qml = self.rootObject()
  
    def updateSpeed(self,speed):  
        self.qml.setProperty("speedText", QVariant(str(speed)))  
  
    def setupUpdateThread(self):  
        self.updateThread = updateThread()  
        #connect our update functoin to the progress signal of the update thread  
        self.updateThread.progress.connect(self.updateSpeed)  
        if not self.updateThread.isRunning():#if the thread has not been started let's kick it off  
            self.updateThread.start()  
  
if __name__ == '__main__':
	os.system("sudo ip link set can0 up type can bitrate 1000000")  
    app = QGuiApplication(sys.argv)  
    win = myDialog()  
    win.show()  
    sys.exit(app.exec_())  
	
