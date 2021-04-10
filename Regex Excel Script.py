# imports
import re

nodes = input("Enter the number of nodes: ")
with open('/home/paul/CADMIUM/Cadmium-Simulation-Environment/DEVS-Models/COVID-ABM-DAM-Main_Coupled_Model/simulation_results/main_node_test_output_state.txt', "r") as f:
    file = f.read()
    # new_file = open("/home/paul/CADMIUM/Cadmium-Simulation-Environment/DEVS-Models/COVID-ABM-DAM-Main_Coupled_Model/simulation_results/TemporaryFile.txt", "w")
    # with open('/home/paul/CADMIUM/Cadmium-Simulation-Environment/DEVS-Models/COVID-ABM-DAM-Main_Coupled_Model/simulation_results/TemporaryFile.txt', "w+") as temp:
        # Make a temporary file
        # for line in file:
        #     new_file.write(line)
        # new_file.close()
    with open('/home/paul/CADMIUM/Cadmium-Simulation-Environment/DEVS-Models/COVID-ABM-DAM-Main_Coupled_Model/Regex.tsv', 'w') as regex:

        regex.write("Time\tPerson ID\tSick\tMask Wearing\tDistance\n")
        # file = f.replace("State for model input_reader_health is next time: \d\d:\d\d:\d\d:\d\d\d", "#")
        # new_shit = re.compile(r"State for model input_reader_health is next time: \d\d:\d\d:\d\d:\d\d\d")
        # file = new_shit

        # for line in temp:
        #     print(line)

        # remove_time_pattern = re.compile(r"State for model input_reader_health is next time: \d\d:\d\d:\d\d:\d\d\d")
        # remove_time_matches = remove_time_pattern.finditer(file)

        time_pattern = re.compile(r"[^ ]\d\d:\d\d:\d\d:\d\d\d")
        time_matches = time_pattern.finditer(file)

        person_id_pattern = re.compile(r"Person ID: ")
        person_id_matches = person_id_pattern.finditer(file)

        sick_pattern = re.compile(r"Sick\? ")
        sick_matches = sick_pattern.finditer(file)

        mask_pattern = re.compile(r"Mask wearing: ")
        mask_matches = mask_pattern.finditer(file)

        distance_pattern = re.compile(r"Distance from people: ")
        distance_matches = distance_pattern.finditer(file)

        # for match in time_pattern_matches:
        #     index = list(match.span())
        #     print(file[index[0]:index[1]])
        first = True
        for match in time_matches:
            index = list(match.span())
            print(file[index[0]+1:index[1]])
            if first:
                j = 1
                while j <= int(nodes):
                    regex.write(file[0:12] + "\n")
                    j += 1
            i = 1
            while i <= int(nodes):
                regex.write(file[index[0]+1:index[1]] + "\n")
                i += 1
            first = False

        # for match in remove_time_matches:
        #     index = list(match.span())
        #     print(file[index[0]:index[1]])
        #     # temp.replace("State for model input_reader_health is next time: ", " ")
        # # re.sub(r"State for model input_reader_health is next time: \d\d:\d\d:\d\d:\d\d\d", "", temp.read())

        for match in person_id_matches:
            index = list(match.span())
            print(file[index[1]:index[1]+1])
            regex.write("\t" + file[index[1]:index[1]+1] + "\n")

        # regex.seek(f.start())

        for match in sick_matches:
            index = list(match.span())
            print(file[index[1]:index[1]+1])
            regex.write("\t\t" + file[index[1]:index[1]+1] + "\n")

        for match in mask_matches:
            index = list(match.span())
            print(file[index[1]:index[1]+1])
            regex.write("\t\t\t" + file[index[1]:index[1]+1] + "\n")

        for match in distance_matches:
            index = list(match.span())
            print(file[index[1]:index[1]+1])
            regex.write("\t\t\t\t" + file[index[1]:index[1]+1] + "\n")