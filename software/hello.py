import threading 

class thread(threading.Thread):
    def __init__(self, id):
        threading.Thread.__init__(self)
        self.id = id
    
    def run(self):
        print("Starting " + str(self.id))
        print_hello(self.id)
        print("Exiting "  + str(self.id))

def print_hello(id):
    print("Hello " + str(id))

thread1 = thread(1)
thread2 = thread(2)

thread1.start()
thread2.start()