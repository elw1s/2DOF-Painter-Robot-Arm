import json

filename = "/home/ardakilic/Desktop/CSE396/simulate_embedded/tmp/received.json"

with open(filename, "r") as line_file:
            lines = json.load(line_file)

print(lines)
print("aaaaaa")
print(lines[0])
print("aaaaaaa")
line = lines[0]
x,y = line[0]
print(x)
