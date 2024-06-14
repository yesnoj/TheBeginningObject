import json
import random
import string

# Definizione delle costanti
MAX_PROC_NAME_LEN = 20  
MAX_STEP_ELEMENTS = 10  #MAX 10 PROCESS
MAX_PROC_ELEMENTS = 30  #MAX 30 PROCESS

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
        "Processes": {}
    }

    # Generazione dei processi
    for i in range(MAX_PROC_ELEMENTS):
        process_name_string = random_string(random.randint(5, MAX_PROC_NAME_LEN))  # Nome casuale del processo
        data["Processes"][f"Process{i}"] = {
            "processNameString": process_name_string,
            "temp": random.randint(0, 100),
            "tempTolerance": random.randint(0, 10),
            "isTempControlled": random.randint(0, 1),
            "isPreferred": random.randint(0, 1),
            "filmType": random.randint(0, 1),
            "timeMins": 0,  # Placeholder, verrà calcolato più avanti
            "timeSecs": 0,  # Placeholder, verrà calcolato più avanti
            "Steps": {}
        }

        total_mins = 0
        total_secs = 0

        # Generazione degli step per ciascun processo
        for j in range(MAX_STEP_ELEMENTS):
            step_name_string = random_string(random.randint(5, MAX_PROC_NAME_LEN))  # Nome casuale dello step
            step_mins = random.randint(0, 5)
            step_secs = random.randint(0, 59)
            total_mins += step_mins
            total_secs += step_secs

            # Conversione dei secondi totali in minuti e secondi
            if total_secs >= 60:
                total_mins += total_secs // 60
                total_secs = total_secs % 60

            data["Processes"][f"Process{i}"]["Steps"][f"Step{j}"] = {
                "stepNameString": step_name_string,
                "timeMins": step_mins,
                "timeSecs": step_secs,
                "type": random.randint(0, 2),
                "source": random.randint(0, 3),
                "discardAfterProc": random.randint(0, 1)
            }

        # Impostazione dei valori totali di timeMins e timeSecs per il processo
        data["Processes"][f"Process{i}"]["timeMins"] = total_mins
        data["Processes"][f"Process{i}"]["timeSecs"] = total_secs

    return data

if __name__ == "__main__":
    generated_json = generate_json()
    with open("FilMachine.json", "w") as json_file:
        json.dump(generated_json, json_file, separators=(',', ':'))  # Formato JSON in linea
        #json.dump(generated_json, json_file, indent=4)  # Formato JSON standard, ESP32 doesn't like it!
