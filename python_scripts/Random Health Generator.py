"""
This script will randomly generate the Health Generator file.
The script will either:
    1. Randomly generate the Health Generator input file with a specified number of sick people, or
    2. Randomly generate the Health Generator input file with a random number of sick people
Make sure to change thy directory to match thy computer.
"""

# imports
import random

# Change directory if needed
with open("/Volumes/[C] Windows 10.hidden/Cadmium-Simulation-Environment/DEVS-Models/Node_Coupled_Model/input_data/health_generator.txt", "w") as f:

    nodes = input("Enter the number of nodes: ")
    while not(int(nodes) > 0):
        print("\nEnter a valid number of nodes (greater than 0)")
        nodes = input("Enter the number of nodes: ")

    sick_people = input("Enter the number of sick people initially (or type \"random\"): ")
    if sick_people != "random":
        while not(0 <= int(sick_people) <= int(nodes)):
            print("\nEnter a valid number of sick people (>= 0 and <= number of nodes)")
            sick_people = input("Enter the number of sick people initially: ")

    # sick_bool = [0, 1]
    # sick_count = 0
    # for i in range(1, int(nodes)+1):
    #     f.write("00:00:00:00 " + str(i) + " ")
    #     temp = random.choice(sick_bool)
    #     if temp == 1 and sick_count <= int(sick_people):
    #         sick_count = sick_count + 1
    #         f.write(str(temp) + "\n")
    #     else:
    #         f.write("0\n")

    node_list = []
    for i in range(1, int(nodes)+1):
        node_list.append(i)

    if sick_people == "random":
        x = random.randint(1, int(nodes))
        temp = random.sample(node_list, k=x)
        for i in range(1, int(nodes)+1):
            if i in temp:
                f.write("00:00:00:00 " + str(i) + " 1\n")
            else:
                f.write("00:00:00:00 " + str(i) + " 0\n")
    else:
        temp = random.sample(node_list, k=int(sick_people))
        for i in range(1, int(nodes)+1):
            if i in temp:
                f.write("00:00:00:00 " + str(i) + " 1\n")
            else:
                f.write("00:00:00:00 " + str(i) + " 0\n")