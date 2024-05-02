import matplotlib.pyplot as plt
from collections import deque
from random import randint
import numpy as np
import serial
import os

#Testing mode generates random values instead reading from serial port
TESTING=True
PORT='COM5'
DELAY=0.1
N = 4#WORKING GRAPHS
CLOSEONERROR=True
GRAPHNAMES = [f"Motor {i+1}" for i in range(N+1)]

if not TESTING:
    ser = serial.Serial(PORT, 9600)

def update1to8():
    #TO DO - updates only first 8 motors
    pass

def update911temp():
    #TO DO - updates last 3 motors + temp sensor
    pass

def values():
    while True:
        if TESTING:
            data = [randint(100,700)/100 for _ in range(N)]            
        else:
            line = ser.readline().decode('utf-8').rstrip()
            data = [float(i) for i in line.split(',')]

        if not plt.fignum_exists(fig.number):
            exit("Window closed")

        if len(data)==N:
            print(data)
            yield data

try:
    data_gen = values()   
    fig, axs = plt.subplots(1, 4)
    temptext = fig.text(0.6,0,f"Temperature will appear here",ha="center")
    plt.subplots_adjust(left=None, bottom=None, right=None, top=None, wspace=None, hspace=0.42)
    
    lines = [ax.plot([], [])[0] for ax in axs.flatten()] 
    for i in range(len(lines)):axs.flatten()[i].set_title(GRAPHNAMES[i])

    x_data = deque(maxlen=10) 
    y_data = [deque(maxlen=10) for _ in range(N+1)] 

    for x, data in enumerate(data_gen):#datagen has N value array
        x_data.append(x)
        for i, y in enumerate(data):#data is value of ith graph
            y_data[i].append(y)
            lines[i].set_data(list(x_data), list(y_data[i]))
            
            #CONDITIONAL COLOR
            if (max(y_data[i])>=6.5):
                lines[i].set_color('r')
            elif(sum(y_data[i])/len(y_data[i]) <= 3.5):
                lines[i].set_color('g')
            else:
                lines[i].set_color('b')

            axs.flatten()[i].relim()
            axs.flatten()[i].autoscale_view()
        temptext.set_text(f"Temperature = {data[-1]}°C / {data[-1]*9/5+32}°F")
        plt.pause(DELAY)

    exit("Ended Somehow")         

except Exception as e:
    print(f"LINE [{e.__traceback__.tb_lineno}] - {e.args}")
    if CLOSEONERROR:
        plt.close();exit(" - Error - ")
    else:pass

finally:
    plt.show()
