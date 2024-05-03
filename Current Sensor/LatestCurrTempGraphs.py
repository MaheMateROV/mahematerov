import matplotlib.pyplot as plt
from collections import deque
from random import randint
import numpy as np
import serial
import os

#Testing mode generates random values instead reading from serial port
TESTING=True
PORT='COM5'
DELAY=0.01
N = 11#WORKING GRAPHS
CLOSEONERROR=True
GRAPHNAMES = [f"Motor {i+1}" for i in range(N+1)]

if not TESTING:
    ser = serial.Serial(PORT, 9600)

#READ FROM CLIENT WITH TCP/IP
def get8FromArduino1():
    pass
def get4FromArduino2():
    pass

def getColor(arr):
    if (max(arr)>=6.5):
        return('r')
    elif(sum(arr)/len(arr) <= 3.5):
        return('g')
    else:
        return('b')

def updateGraphs(readings,hasTemp=False):
    #HASTEMP = True when updating last 3 motor otherwise does first 8
    global x1,x2,y1,y2,lines,axs,plt
    if hasTemp:
        temptext.set_text(f"Temperature = {readings[-1]}°C / {readings[-1]*9/5+32}°F")
        for i, y in enumerate(readings[:-1]):#data - array, y - value of graph
            if len(x2[i])!=10:
                x2[i]=[i for i in range(len(y2[i])+1)]
            y2[i].append(y)
            lines[i+8].set_data(list(x2[i]), list(y2[i]))
            lines[i+8].set_color(getColor(y2[i]))
            axs.flatten()[i+8].relim()
            axs.flatten()[i+8].autoscale_view()
    else:
        for i, y in enumerate(readings):#data - array, y - value of graph
            if len(x1[i])!=10:
                x1[i]=[i for i in range(len(y1[i])+1)]
            y1[i].append(y)
            lines[i].set_data(list(x1[i]), list(y1[i]))
            lines[i].set_color(getColor(y1[i]))
            axs.flatten()[i].relim()
            axs.flatten()[i].autoscale_view()

    plt.pause(DELAY)

def values(x=1):
    if TESTING:
        if x==1:
            data = [randint(100,700)/100 for _ in range(8)]  
        else:
            data = [randint(100,700)/7 for _ in range(4)]            
    else:
        #read here
        if x==1:
            data = get8FromArduino1()
        else:
            data = get4FromArduino2()
        # line = ser.readline().decode('utf-8').rstrip()
        # data = [float(i) for i in line.split(',')]
    return data

try:  
    #GRAPHS and SUBPLOT CONFIG
    fig, axs = plt.subplots(3, 4)
    temptext = fig.text(0.6,0,f"Temperature will appear here",ha="center")
    plt.subplots_adjust(left=None, bottom=None, right=None, top=None, wspace=None, hspace=0.42)
    lines = [ax.plot([], [])[0] for ax in axs.flatten()] 
    for i in range(len(lines)):axs.flatten()[i].set_title(GRAPHNAMES[i])
    x1 = [deque(maxlen=10) for _ in range(8)]
    y1 = [deque(maxlen=10) for _ in range(8)] 
    x2 = [deque(maxlen=10) for _ in range(3)]
    y2 = [deque(maxlen=10) for _ in range(3)]  

    while True:
        updateGraphs(values(1),hasTemp=False)#first 8
        updateGraphs(values(2),hasTemp=True)#3+temp

        if not plt.fignum_exists(fig.number):
            exit("Window closed")

    exit("Ended Somehow")         

except Exception as e:
    print(f"LINE [{e.__traceback__.tb_lineno}] - {e.args}")
    if CLOSEONERROR:
        plt.close();exit(" - Error - ")
    else:pass

finally:
    plt.show()
