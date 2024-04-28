import tkinter as tk
#import serial

TestingWithoutPort = False #only change for testing 

#ControlArduinoPort = 'COM5'

# Create a serial object
#if not TestingWithoutPort:ser = serial.Serial('COM5', 9600)

def exp(placeholder=0):
    ser.write(b'Expand')

def con(placeholder=0):
    ser.write(b'Contract')

def stop(placeholder=0):
    ser.write(b'Stop')

def tester(event):
    print(f"testy - {event}")

# Create a Tkinter window
root = tk.Tk()

# Create button
Ebutton = tk.Button(root, text="Expand", command=exp, height=3, width=20)
Ebutton.bind("<ButtonPress-1>",exp)
Ebutton.bind("<ButtonRelease-1>",stop)
Ebutton.pack()

Cbutton = tk.Button(root, text="Contract", command=con, height=3, width=20)
Cbutton.bind("<ButtonPress-1>",exp)
Cbutton.bind("<ButtonRelease-1>",stop)
Cbutton.pack()

stopper = tk.Button(root, text="Stop", command=stop, height=3, width=20)
stopper.pack()


# Run the Tkinter event loop
root.mainloop()
