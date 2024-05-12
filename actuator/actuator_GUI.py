import tkinter as tk
from tkinter import messagebox
import threading
import socket
import time
from pynput import keyboard

client_ip = "192.168.2.100"
server_ip = "192.168.2.20"
PORT = 12345

# Global list to hold the status of each button set
button_status = [0, 0, 0, 0]
server_running = False
server_socket = None

def update_status(index, value):
    button_status[index] = value

def print_button_status():
    print("Button Status:", button_status)
    #root.after(10, print_button_status)  # Print every 1000 milliseconds (1 second)

def update_button_colors():
    for i, (front_button, back_button) in enumerate(zip(front_buttons, back_buttons), start=1):
        if button_status[i - 1] == 1:
            front_button.config(bg="green")
            back_button.config(bg="SystemButtonFace")
        elif button_status[i - 1] == 2:
            front_button.config(bg="SystemButtonFace")
            back_button.config(bg="blue")
        else:
            front_button.config(bg="SystemButtonFace")
            back_button.config(bg="SystemButtonFace")

def on_press(key):
    try:
        key_index = int(key.char)
        if 1 <= key_index <= 8:
            # Map number keys to alternating between front and back buttons
            set_index = (key_index - 1) // 2
            if key_index % 2 == 1:
                update_status(set_index, 1)
            else:
                update_status(set_index, 2)
            update_button_colors()
        elif key_index == 0:
            # Set the status of the last back button set to 2
            update_status(len(button_status) - 1, 2)
            update_button_colors()
    except (ValueError, AttributeError):
        pass

def on_release(key):
    try:
        key_index = int(key.char)
        if 1 <= key_index <= 8:
            # Map number keys to alternating between front and back buttons
            set_index = (key_index - 1) // 2
            update_status(set_index, 0)
            update_button_colors()
        elif key_index == 0:
            # Reset the status of the last back button set to 0
            update_status(len(button_status) - 1, 0)
            update_button_colors()
    except (ValueError, AttributeError):
        pass

def start_server():
    global server_running
    try:
        global server_socket
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        server_socket.bind((server_ip,PORT))
        server_running = True
        start_button.config(state=tk.DISABLED)
        stop_button.config(state=tk.NORMAL)
        print("Server started.")
        send_data_thread = threading.Thread(target=send_data)
        send_data_thread.start()
    except Exception as e:
        messagebox.showerror("Error", f"Failed to start server: {str(e)}")

def stop_server():
    global server_running
    try:
        server_running = False
        server_socket.close()
        print("Server stopped.")
        start_button.config(state=tk.NORMAL)
        stop_button.config(state=tk.DISABLED)
    except Exception as e:
        messagebox.showerror("Error", f"Failed to stop server: {str(e)}")

def send_data():
    global button_status
    destination = (client_ip, PORT)  # Specify the IP address and port of the client
    while server_running:
        try:
            packet = ""
            for val in button_status:
                packet += str(val)
            server_socket.sendto(packet.encode(), destination)
            print(button_status)
            #print("Data sent to client")
            # Sleep for a short duration to avoid excessive CPU usage
            # Adjust the duration as needed
            time.sleep(0.035)
        except Exception as e:
            print(f"Error sending data: {str(e)}")
            break

def main():
    global root, front_buttons, back_buttons, start_button, stop_button
    # Create the main window
    root = tk.Tk()
    root.title("Front and Back Buttons")

    # Create a frame to contain all button sets
    all_frames = tk.Frame(root)
    all_frames.pack(padx=10, pady=5)

    front_buttons = []
    back_buttons = []

    # Create 5 sets of front and back buttons
    for i in range(4):
        # Create a frame for each set of buttons
        frame = tk.Frame(all_frames)
        frame.pack(side=tk.LEFT, padx=(0, 20), pady=5)  # Adjust padx for spacing between sets

        # Create front button
        front_button = tk.Button(frame, text=f"Front {i+1}")
        front_button.pack(side=tk.LEFT, padx=(0, 5), pady=5)  # Adjust padx for spacing between buttons
        front_buttons.append(front_button)

        # Create back button
        back_button = tk.Button(frame, text=f"Back {i+1}")
        back_button.pack(side=tk.LEFT, padx=(0, 5), pady=5)  # Adjust padx for spacing between buttons
        back_buttons.append(back_button)

    # Create start server button
    start_button = tk.Button(root, text="Start Server", command=start_server)
    start_button.pack(side=tk.TOP, pady=5)

    # Create stop server button
    stop_button = tk.Button(root, text="Stop Server", command=stop_server, state=tk.DISABLED)
    stop_button.pack(side=tk.TOP, pady=5)

    # Print the button status continuously
    print_button_status()

    # Collect events until released
    with keyboard.Listener(
            on_press=on_press,
            on_release=on_release) as listener:
        root.mainloop()

    print("Exiting...")

if __name__ == "__main__":
    main()
