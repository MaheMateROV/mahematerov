import tkinter as tk
from tkinter import messagebox
import matplotlib.pyplot as plt
from pandastable import *
import pandas as pd

class Application(tk.Tk):
    def __init__(self):
        tk.Tk.__init__(self)
        self.title("Graph Generator")

        self.dataframe = pd.DataFrame(columns=['x1', 'x2', 'x3', 'x4', 'x5','x6','x7'], index=['x', 'y'])

        # Create a Frame widget
        self.frame = tk.Frame(self)
        self.frame.pack()

        # Use the Frame as the parent for the Table
        self.table = pt = Table(self.frame, dataframe=self.dataframe)
        pt.show()

        self.plot_button = tk.Button(self, text="Generate", command=self.plot_graph)
        self.plot_button.pack()

    def plot_graph(self):
        data = self.table.model.df
        try:
            x_values = data.loc['x'].values.astype(float)
            y_values = data.loc['y'].values.astype(float)
        except ValueError:
            messagebox.showerror("Error", "Please enter valid numbers.")
            return

        if len(x_values) != len(y_values):
            messagebox.showerror("Error", "x and y arrays must have the same length.")
            return

        plt.figure(figsize=(7, 7))
        plt.plot(x_values, y_values)
        plt.show()

if __name__ == "__main__":
    app = Application()
    app.mainloop()
