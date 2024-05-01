import tkinter as tk
import serial
import time
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

# Create a serial object
ser = serial.Serial('COM6', 115200)

# Create a Tkinter window
root = tk.Tk()
root.title("Data Collection")

# Set window size to 1920x1080
root.geometry("1920x1080")

# Initialize lists to store data points
depth_values = []
time_values = []
team_name = tk.StringVar()
team_name_label = tk.Label(root, textvariable=team_name, font=("Arial", 20))  # Create label with text variable

# Function to add column headers
def add_headers():
    table.insert(tk.END, "Time\tDepth\tPressure\n")

# Create a table with a text box displaying team name and columns for time, depth, and pressure
team_name_label = tk.Label(root, text="Team Name:", font=("Arial", 20))
team_name_label.grid(row=0, column=0, sticky='w', padx=20, pady=20)

team_name_entry = tk.Entry(root, textvariable=team_name, font=("Arial", 20))
team_name_entry.grid(row=0, column=1, sticky='w', padx=20, pady=20)

table = tk.Text(root, height=10, width=40, font=("Arial", 20))
table.grid(row=1, column=0, columnspan=2, padx=20, pady=20)

# Add headers initially
add_headers()

# Create a function to clear the table
def clear_table():
    table.delete('1.0', tk.END)
    add_headers()  # Add headers after clearing

# Create a function to send 'yes' signal and save data to a text file
def send_yes():
    clear_table()  # Clear the table
    
    # Initialize lists to store data points
    depth_values.clear()
    time_values.clear()
    
    ser.write(b'Expand')  # Send 'Expand' command
    
    # Record start time
    start_time = time.time()
    
    # Initialize data
    data1 = ''
    
    # Keep checking for data from serial for 30 seconds
    while time.time() - start_time < 30:
        if ser.in_waiting > 0:
            data = ser.readline().decode().strip()  # Read data from serial
            print(data)
            data1 += data + '\n'
            if "Team Name" in data:  # Extract team name from data
                team_name.set(data.split(":")[1].strip())  # Update team name entry
    
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
                
                # Update table
                table.insert(tk.END, f"{timeaa}\t{depth_value}\t{pressure_value}\n")
    
    # Plot the graph
    plot_graph()

def plot_graph():
    # Plot the graph
    plt.figure(figsize=(10, 6))
    plt.plot(time_values, depth_values)
    plt.xlabel('Time', fontsize=20)
    plt.ylabel('Depth Value', fontsize=20)
    plt.title('Depth vs Time', fontsize=24)
    plt.xticks(fontsize=16)
    plt.yticks(fontsize=16)
    plt.grid(True)
    plt.show()

def start():
    clear_table()  # Clear the table
    
    # Initialize lists to store data points
    depth_values.clear()
    time_values.clear()
    
    ser.write(b'Start')  # Send 'Expand' command
    
    # Record start time
    start_time = time.time()
    
    # Initialize data
    data1 = ''
    
    # Keep checking for data from serial for 30 seconds
    while time.time() - start_time < 30:
        if ser.in_waiting > 0:
            data = ser.readline().decode().strip()  # Read data from serial
            print(data)
            data1 += data + '\n'
            if "Team Name" in data:  # Extract team name from data
                team_name.set(data.split(":")[1].strip())  # Update team name entry
    
    # Split the data1 into lines
    lines = data1.split('\n')
    
    # Extract and display data
    for line in lines:
        if line.startswith("Received packet"):
            parts = line.split(",")
            if len(parts) >= 4:
                timeaa = parts[1]
                pressure_value = parts[2]
                depth_value = parts[3].split()[0]
                table.insert(tk.END, f"{timeaa}\t{depth_value}\t{pressure_value}\n")

# Create a 'Start' button
start_button = tk.Button(root, text="Start", command=start, height=2, width=20, font=("Arial", 20))
start_button.grid(row=2, column=0, pady=20, padx=20)

# Create a 'Expand' button
expand_button = tk.Button(root, text="Expand", command=send_yes, height=2, width=20, font=("Arial", 20))
expand_button.grid(row=2, column=1, pady=20, padx=20)

# Run the Tkinter event loop
root.mainloop()
