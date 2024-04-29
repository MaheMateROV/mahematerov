import tkinter as tk
import serial
import time
import matplotlib.pyplot as plt

# Create a serial object
ser = serial.Serial('COM6', 115200)

# Initialize lists to store data points
depth_values = []
time_values = []

# Create a function to send 'yes' signal and save data to a text file
def send_yes():
    ser.write(b'Expand')  # Send 'Expand' command
    
    # Record start time
    start_time = time.time()
    
    # Initialize data
    data1 = ''
    
    # Keep checking for data from serial for 45 seconds
    while time.time() - start_time < 45:
        if ser.in_waiting > 0:
            data = ser.readline().decode().strip()  # Read data from serial
            print(data)
            data1 += data + '\n'
    
    # Split the data1 into lines
    lines = data1.split('\n')
    
    # Extract and store desired values
    for line in lines:
        if line.startswith("Received packet"):
            parts = line.split(",")
            if len(parts) >= 4:
                name_parts = parts[0].split(" ")
                name = name_parts[2]  # Skip the first two parts
                timeaa = parts[1]
                pressure_value = parts[2]
                depth_value = parts[3].split()[0]
                print(f"Name: {name}, Time: {timeaa}, Pressure Value: {pressure_value}, Depth Value: {depth_value}")
                
                # Append data to lists
                depth_values.append(float(depth_value))
                time_values.append(timeaa)

    # Plot the graph
    plt.plot(time_values, depth_values)
    plt.xlabel('Time')
    plt.ylabel('Depth Value')
    plt.title('Depth vs Time')
    plt.show()

# Create a Tkinter window
root = tk.Tk()

# Create a 'yes' button
yes_button = tk.Button(root, text="Expand", command=send_yes, height=3, width=20)
yes_button.pack()

# Run the Tkinter event loop
root.mainloop()
