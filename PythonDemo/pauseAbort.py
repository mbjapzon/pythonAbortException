import time

class pauseAbortDemo:
    def __init__(self):
        self.counter = 1

    def incrementcounter(self):
        try:
            self.counter += 1
        except:
            print("increment counter caught exception")

    def getCounter(self):
        return self.counter

    def runLoop(self):
        try:
            while True:
                print("working")
                time.sleep(1)  # Delay for 1 second.
                self.incrementcounter()  
        except:
            print("caught exception")
            
        finally:
            print("finally")

        
if __name__ == '__main__':
    my_demo = pauseAbortDemo()
    my_demo.runLoop()

