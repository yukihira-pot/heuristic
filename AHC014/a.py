import os
import random

path = "C:\\Users\\rmago\\OneDrive\\Documents\\CppScript\\heuristic\\AHC014\\pre_testcases"
files = os.listdir(path)
files = ["pre_testcases/" + elem for elem in files]

# random.shuffle(files)

with open("testcases.txt", "w") as new_file:
    for name in files:
        with open(name) as f:
            for line in f:
                new_file.write(line)
