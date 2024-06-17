import json
import pickle

def convert_binary_to_json():
    # Leggi il file binario
    with open("FilMachine.cfg", "rb") as binary_file:
        data = pickle.load(binary_file)

    # Scrivi i dati in un file JSON
    with open("FilMachine.json", "w") as json_file:
        json.dump(data, json_file, indent=4)

if __name__ == "__main__":
    convert_binary_to_json()