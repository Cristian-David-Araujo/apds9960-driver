import serial
import re
import matplotlib.pyplot as plt
import matplotlib.patches as patches

# Serial configuration
PORT = 'COM12'        # ⚠️ Change this to your serial port
BAUDRATE = 115200  # ⚠️ Change this to your baud rate
TIMEOUT = 1

# Regex to match the incoming serial data
regex = r"Color Data - R: (\d+), G: (\d+), B: (\d+), C: (\d+)"

# Enable interactive mode and create dark-themed figure
plt.ion()
fig, (ax_bars, ax_color) = plt.subplots(1, 2, figsize=(9, 4))
fig.patch.set_facecolor('#1e1e1e')  # Dark background

# Apply dark style to both subplots
for ax in (ax_bars, ax_color):
    ax.set_facecolor('#1e1e1e')
    ax.tick_params(colors='white')
    ax.title.set_color('white')
    ax.spines['bottom'].set_color('white')
    ax.spines['left'].set_color('white')

def update_plot(r, g, b, c):
    # Clear previous plots
    ax_bars.clear()
    ax_color.clear()

    # Bar chart
    labels = ['Red', 'Green', 'Blue', 'Clear']
    values = [r, g, b, c]
    colors = ['red', 'green', 'blue', 'gray']
    ax_bars.bar(labels, values, color=colors)
    ax_bars.set_ylim(0, 255)
    ax_bars.set_title("Color Intensity (0–255)", color='white')
    ax_bars.set_facecolor('#1e1e1e')
    ax_bars.tick_params(colors='white')
    ax_bars.spines['bottom'].set_color('white')
    ax_bars.spines['left'].set_color('white')

    # Combined color square
    rgb_normalized = (r / 255, g / 255, b / 255)
    ax_color.add_patch(patches.Rectangle((0, 0), 1, 1, color=rgb_normalized))
    ax_color.set_xlim(0, 1)
    ax_color.set_ylim(0, 1)
    ax_color.axis('off')
    ax_color.set_title("Combined Color", color='white')
    ax_color.set_facecolor('#1e1e1e')

    plt.tight_layout()
    plt.pause(0.01)

# Start reading from serial port
try:
    with serial.Serial(PORT, BAUDRATE, timeout=TIMEOUT) as ser:
        print("Reading serial data... Press Ctrl+C to stop.")
        while True:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            match = re.match(regex, line)
            if match:
                r, g, b, c = map(int, match.groups())
                print(f"R: {r}, G: {g}, B: {b}, C: {c}")
                update_plot(r, g, b, c)

except serial.SerialException as e:
    print(f"Serial connection error: {e}")
except KeyboardInterrupt:
    print("\nSerial reading stopped by user.")
