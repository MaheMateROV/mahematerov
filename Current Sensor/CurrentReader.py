import matplotlib.pyplot as plt
from collections import deque
import numpy as np
import serial
import os

ser = serial.Serial('COM5', 9600)

while True:
    try:
        line = ser.readline().decode('utf-8').rstrip()
        data_gen = [float(i) for i in line[:-1].split(',')]
        fig, axs = plt.subplots(2, 4)
        lines = [ax.plot([], [])[0] for ax in axs.flatten()] 

        x_data = deque(maxlen=10) 
        y_data = [deque(maxlen=10) for _ in range(8)] 

        for x, data in enumerate(data_gen):
            x_data.append(x)
            for i, y in enumerate(data):
                y_data[i].append(y)
                lines[i].set_data(list(x_data), list(y_data[i]))  # Update line data
                axs.flatten()[i].relim()
                axs.flatten()[i].autoscale_view()
            plt.pause(0.01)
    except Exception as e:
        print(e)
    finally:
        plt.show()
