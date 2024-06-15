import tkinter as tk
from tkinterdnd2 import DND_FILES, TkinterDnD
import os
import pandas as pd
import matplotlib.pyplot as plt

def plot_data(filepath):
    if os.path.splitext(filepath)[1] in ['.xlsx', '.xls']:
        df = pd.read_excel(filepath, sheet_name='Sheet1', skiprows=2, index_col=1)
    else: 
        df = pd.read_csv(filepath, skiprows=2, index_col=1)
    df = df.drop(df.columns[0], axis=1)
    plt.style.use('ggplot')
    plt.figure(figsize=(10, 6))
    for receiver in df.index:
        plt.plot(df.columns, df.loc[receiver], label=receiver, linewidth=2)
    plt.xlabel('Day', fontsize=14)
    plt.ylabel('# of sturgeon', fontsize=14)
    plt.title('Data Table F', fontsize=16)
    plt.legend()
    root.withdraw()
    plt.show(block=True)
    root.deiconify()

def drop(event):
    try:
        filepath = event.data.replace('{','').replace("}",'')
        print(f'Filepath: {filepath}')
        plot_data(filepath)
        label.config(text=f'Drag and Drop a File Here\n{filepath}')

    except Exception as e:
        print(f'Error: {e}')
        current_text = label.cget("text")
        label.config(text=current_text + f"\n{e}")
        

root = TkinterDnD.Tk()
root.title('Drag and Drop Interface')

root.configure(bg='blue')

label = tk.Label(root, text='Drag and Drop a File Here', font=('Helvetica', 26, 'bold'), width=50, height=20, bg='blue', fg='white')
label.pack()

root.drop_target_register(DND_FILES)
root.dnd_bind('<<Drop>>', drop)

root.mainloop()
