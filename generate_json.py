import json
import random
import string

def random_string(length):
    letters = string.ascii_lowercase
    return ''.join(random.choice(letters) for _ in range(length))

def generate_json():
    data = {
        "machineSettings": {
            "tempUnit": 0,
            "waterInlet": 0,
            "calibratedTemp": 0,
            "filmRotationSpeedSetpoint": 0,
            "rotationIntervalSetpoint": 0,
            "randomSetpoint": 0,
            "isPersistentAlarm": 0,
            "isProcessAutostart": 0,
            "drainFillOverlapSetpoint": 0
        },
        "Filter": {
            "filterName": None,
            "isColorFilter": 0,
            "isBnWFilter": 0,
            "preferredOnly": 0
        },
        "Processes": {}
    }

    # Generating 30 processes
    for i in range(30):
        process_name_string = random_string(random.randint(5, 15))  # Random process name
        data["Processes"][f"Process{i}"] = {
            "processNameString": process_name_string,
            "temp": random.randint(0, 100),
            "tempTolerance": random.randint(0, 10),
            "isTempControlled": random.randint(0, 1),
            "isPreferred": random.randint(0, 1),
            "filmType": random.randint(0, 1),
            "timeMins": random.randint(0, 60),
            "timeSecs": random.randint(0, 59),
            "Steps": {}
        }

        # Generating 20 steps for each process
        for j in range(20):
            step_name_string = random_string(random.randint(5, 15))  # Random step name
            data["Processes"][f"Process{i}"]["Steps"][f"Step{j}"] = {
                "stepNameString": step_name_string,
                "timeMins": random.randint(0, 60),
                "timeSecs": random.randint(0, 59),
                "type": random.randint(0, 2),
                "source": random.randint(0, 3),
                "discardAfterProc": random.randint(0, 1)
            }

    return data

if __name__ == "__main__":
    generated_json = generate_json()
    with open("FilMachine.json", "w") as json_file:
        json.dump(generated_json, json_file, separators=(',', ':'))
