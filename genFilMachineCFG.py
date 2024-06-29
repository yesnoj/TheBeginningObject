import struct
import random
import os
import json
import string
import time

# Definition of constants
MAX_STEP_MIN = 5 
MAX_PROC_NAME_LEN = 10  
MAX_STEP_ELEMENTS = 3  #MAX 10 PROCESS / DEV.A 30
MAX_PROC_ELEMENTS = 4  #MAX 30 PROCESS / DEV.A 100

# Definition of output file names
OUTPUT_FILE_CFG = 'FilMachine.cfg'
OUTPUT_FILE_JSON = 'FilMachine.JSON'

import struct
import random
import string
import json

# Generate the settingsParams with specified limits
settingsParams = {
    "tempUnit": random.randint(0, 1),
    "waterInlet": random.randint(0, 1),
    "calibratedTemp": random.randint(0, 40),
    "filmRotationSpeedSetpoint": random.randint(20, 90),
    "rotationIntervalSetpoint": random.randint(5, 60),
    "randomSetpoint": random.randint(0, 100),
    "isPersistentAlarm": random.randint(0, 1),
    "isProcessAutostart": random.randint(0, 1),
    "drainFillOverlapSetpoint": random.randrange(0, 100, 50),
    "multiRinseTime": random.randrange(60, 181, 30)
}

# Function to generate random strings for process and step names
def random_string(length=MAX_PROC_NAME_LEN):
    return ''.join(random.choice(string.ascii_letters) for _ in range(length))

# Function to ensure the total time of steps matches the process time
def generate_steps():
    steps = []
    total_mins = 0
    total_secs = 0
    for s in range(MAX_STEP_ELEMENTS):
        step_mins = random.randint(0, MAX_STEP_MIN)
        step_secs = random.randint(0, 59)
        steps.append({
            "stepNameString": random_string(MAX_PROC_NAME_LEN),
            "timeMins": step_mins,
            "timeSecs": step_secs,
            "type": random.randint(0, 2), # No icon for 3 ( CHEMICAL_TYPE_NA ) So limit to 2 for now as will crash
            "source": random.randint(0, 3), # gui can only have 0 to 3 or it will crash
            "discardAfterProc": random.randint(0, 1)
        })
        total_mins += step_mins
        total_secs += step_secs

    total_mins += total_secs // 60
    total_secs = total_secs % 60

    return steps, total_mins, total_secs

# Generate up to 100 processes with 30 steps each
processes = []
for p in range(MAX_PROC_ELEMENTS):
    steps, total_mins, total_secs = generate_steps()
    process = {
        "processNameString": random_string(MAX_PROC_NAME_LEN),
        "temp": random.randint(20, 40),
        "tempTolerance": float(random.randint(0, 5)) / 10,  # Updated as value changed to float in global structure
        "isTempControlled": random.randint(0, 1),
        "isPreferred": random.randint(0, 1),
        "filmType": random.randint(0, 1),
        "timeMins": total_mins,
        "timeSecs": total_secs,
        "steps": steps
    }
    processes.append(process)

    
def write_settings(file, settings):
    file.write(struct.pack('<L', settings["tempUnit"]))
    file.write(struct.pack('<B', settings["waterInlet"]))
    file.write(struct.pack('<B', settings["calibratedTemp"]))  # Updated to a byte value as was 32-bit, has been changed in global structure...
    file.write(struct.pack('<B', settings["filmRotationSpeedSetpoint"]))
    file.write(struct.pack('<B', settings["rotationIntervalSetpoint"]))
    file.write(struct.pack('<B', settings["randomSetpoint"]))
    file.write(struct.pack('<B', settings["isPersistentAlarm"]))
    file.write(struct.pack('<B', settings["isProcessAutostart"]))
    file.write(struct.pack('<B', settings["drainFillOverlapSetpoint"]))
    file.write(struct.pack('<B', settings["multiRinseTime"]))

def write_process(file, process):
    file.write(process["processNameString"].encode('ASCII').ljust(21, b'\x00'))  #updated type to ASCII as UTF-8 can generate characters with more than 1 byte! Crash for us!
    file.write(struct.pack('<L', process["temp"]))
    file.write(struct.pack('<f', process["tempTolerance"])) # Updated for float 
    file.write(struct.pack('<B', process["isTempControlled"]))
    file.write(struct.pack('<B', process["isPreferred"]))
    file.write(struct.pack('<L', process["filmType"]))
    file.write(struct.pack('<L', process["timeMins"]))
    file.write(struct.pack('<B', process["timeSecs"]))
    file.write(struct.pack('<H', len(process["steps"])))
    for step in process["steps"]:
        write_step(file, step)

def write_step(file, step):
    file.write(step["stepNameString"].encode('ASCII').ljust(21, b'\x00')) #
    file.write(struct.pack('<B', step["timeMins"]))
    file.write(struct.pack('<B', step["timeSecs"]))
    file.write(struct.pack('<L', step["type"]))
    file.write(struct.pack('<L', step["source"]))
    file.write(struct.pack('<B', step["discardAfterProc"]))

# Create the configuration file
with open(OUTPUT_FILE_CFG, "wb") as bin_file:
    # Write settingsParams
    write_settings(bin_file, settingsParams)
    
    # Write the number of processes
    bin_file.write(struct.pack('<l', len(processes)))
    # Write each process and its steps
    for process in processes:
        write_process(bin_file, process)

# Create the JSON file
data = {
    "settingsParams": settingsParams,
    "processes": processes
}

with open(OUTPUT_FILE_JSON, "w") as json_file:
    json.dump(data, json_file, indent=4)

