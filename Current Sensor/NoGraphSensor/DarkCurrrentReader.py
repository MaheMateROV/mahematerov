from socket import AF_INET,SOCK_DGRAM,socket,gethostname,gethostbyname
from time import sleep as s
import matplotlib.pyplot as plt
from collections import deque
from random import randint
import numpy as np
import serial
import os
import tkinter as tk
from tkinter import ttk
from ttkbootstrap import Style
import threading

"""
TEST CODE
from socket import AF_INET,SOCK_DGRAM,socket,gethostname,gethostbyname
sock = socket(AF_INET, SOCK_DGRAM)
sock.bind((gethostbyname(gethostname()),2222))
sock.setblocking(False)
sock.settimeout(0)  
sock.sendto("1,2,2,1,3,4,1,4".encode(),"CODE IP HERE",1234)#PC whose ip given in ARD1IP
sock.sendto("1,2,2,1".encode(),"CODE IP HERE",1234)#PC whose ip given in ARD2IP
"""

#Testing mode generates random values instead reading from serial port
TESTING=False
ARD1IP=gethostbyname(gethostname())
ARD2IP="to.oi.il.et"
SOCKTIMEOUT = 0
MYIP=gethostbyname(gethostname())
MYPORT=1234
GRAPHPOINTS=20
DELAY=0.000000000000000000000000000001
N = 11#WORKING GRAPHS
CLOSEONERROR=True
GRAPHNAMES = [f"Motor {i+1}: " for i in range(N+1)]
#PORT='COM5'

if not TESTING:
    #ser = serial.Serial(PORT, 9600)
    serverSock = socket(AF_INET, SOCK_DGRAM)
    serverSock.bind((MYIP, MYPORT))
    serverSock.setblocking(False)
    serverSock.settimeout(SOCKTIMEOUT)

def getColor(arr):
    if (max(arr)>=6.5):
        return('r')
    elif(sum(arr)/len(arr) <= 3.5):
        return('g')
    else:
        return('b')

def listen_for_packets():
    while True:
        try:
            data, addr = serverSock.recvfrom(1024)
            strData=data.decode()
            arr=list(map(lambda x:float(x),strData.split(',')))
            if(addr[0]==ARD1IP):updateButton(buttons,arr,8)
            elif(addr[0]==ARD2IP):updateButton(buttons,arr,4)
            else:print(f"\nRecieved {strData} from {addr}\n[{addr}]")
        except:
            pass
        

try:  
    print(f"Running on IP:{MYIP} - PORT:{MYPORT}")

    # Create an instance of Tkinter frame
    root = tk.Tk()

    # Set the geometry of tkinter frame
    root.geometry("1024x720")

    # Create an instance of style
    style = Style(theme="darkly")

    # Function to update button text
    def updateButton(buttons,val,ard=8):
        if ard==8:
            for i in range(8):
                buttons[i].config(text=GRAPHNAMES[i]+str(val[i]))
        elif ard==4:
            for i in range(8, 11):
                buttons[i].config(text=GRAPHNAMES[i]+str(val[i-8]))
            buttons[11].config(text=f"Temp: {val[-1]}")

    # Function to update last 4 button texts
        

    # Create buttons
    buttons = []
    for i in range(3):
        for j in range(4):
            button = ttk.Button(root, text="0", style="success.Outline.TButton")
            button.grid(row=i, column=j, sticky="nsew", padx=5, pady=5)
            buttons.append(button)

    # Configure the grid to expand evenly when the window size changes
    root.grid_columnconfigure(0, weight=1)
    root.grid_columnconfigure(1, weight=1)
    root.grid_columnconfigure(2, weight=1)
    root.grid_columnconfigure(3, weight=1)
    root.grid_rowconfigure(0, weight=1)
    root.grid_rowconfigure(1, weight=1)
    root.grid_rowconfigure(2, weight=1)

    # Start a thread to update button text every second
    threading.Thread(target=listen_for_packets, args=(), daemon=True).start()

    root.mainloop()

    exit("Ended Somehow")         

except Exception as e:
    print(f"LINE [{e.__traceback__.tb_lineno}] - {e.args}")
    if CLOSEONERROR:
        plt.close();exit(" - Error - ")
    else:pass

finally:
    plt.show()
