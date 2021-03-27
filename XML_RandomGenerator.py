# imports
import random
import xml.etree.ElementTree as Student_XML
from xml.etree.ElementTree import ElementTree
from xml.etree.ElementTree import Element
import os
import re

number_of_students = input("\nEnter the number of students in the room: ")

while int(number_of_students) <= 0:
    print("\nEnter a valid number greater than 0")
    number_of_students = input("\nEnter the number of students in the room: ")

for student in range(1, int(number_of_students)+1):
    relationship_types = ["Family", "Friend", "Classmate", "Professor"]
    probabilities = ["Low", "High"]
    sick = ["True", "False"]
    room = ["1st_Mackenzie", "2nd_Mackenzie", "3rd_Mackenzie", "4th_Mackenzie"]
    time_in_room = ["30", "60", "90", "120", "180"]
    # size = ["Large", "Small"]
    # file_path = "/home/paul/Desktop/XML" + str(student) + ".xml"

    with open("/home/paul/CADMIUM/Cadmium-Simulation-Environment/DEVS-Models/COVID-ABM-DAM-Main_Coupled_Model/data/XML" + str(student) + ".xml", 'w') as f:
        f.write("<?xml version=\"1.0\" ?>\n")
        f.write("<DecisionMakerBehaviour>\n")
        f.write("<ID>" + str(student) + "</ID>\n")
        f.write("<Location>4th_Mackenzie</Location>\n")
        f.write("<IsSick>" + random.choice(sick) + "</IsSick>\n")
        f.write("<ProbabilityOfWearingMask>" + random.choice(probabilities) + "</ProbabilityOfWearingMask>\n")
        f.write("<Relationship>\n")

        for student_id in range(1, int(number_of_students)+1):
            if student_id != student:
                f.write("\t<Relationship id = \"" + str(student_id) + "\"\n")
                f.write("\t\ttype = \"" + random.choice(relationship_types) + "\"/>\n\n")

        f.write("</Relationship>\n\n")
        f.write("<BehaviourRulesPerson>\n")

        f.write("\t<PersonRelations status=\"Family\"\n")
        f.write("\t\tprobability=\"" + random.choice(probabilities) + "\"\n")
        f.write("\t\tchance=\"" + random.choice(probabilities) + "\"/>\n\n")

        f.write("\t<PersonRelations status=\"Friend\"\n")
        f.write("\t\tprobability=\"" + random.choice(probabilities) + "\"\n")
        f.write("\t\tchance=\"" + random.choice(probabilities) + "\"/>\n\n")

        f.write("\t<PersonRelations status= \"Professor\"\n")
        f.write("\t\tprobability=\"" + random.choice(probabilities) + "\"\n")
        f.write("\t\tchance=\"" + random.choice(probabilities) + "\"/>\n\n")

        f.write("\t<PersonRelations status= \"Classmate\"\n")
        f.write("\t\tprobability=\"" + random.choice(probabilities) + "\"\n")
        f.write("\t\tchance=\"" + random.choice(probabilities) + "\"/>\n\n")

        f.write("</BehaviourRulesPerson>\n\n")
        f.write("<BehaviourRulesRoom>\n")

        f.write("\t<RoomSize size=\"Small\">\n")
        f.write("\t\t<NumberOfPeople group=\"Large\"\n")
        f.write("\t\tprobability=\"" + random.choice(probabilities) + "\"\n")
        f.write("\t\tchance=\"" + random.choice(probabilities) + "\"/>\n\n")

        f.write("\t\t<NumberOfPeople group=\"Small\"\n")
        f.write("\t\tprobability=\"" + random.choice(probabilities) + "\"\n")
        f.write("\t\tchance=\"" + random.choice(probabilities) + "\"/>\n\n")
        f.write("\t</RoomSize>\n")

        f.write("\t<RoomSize size=\"Large\">\n")
        f.write("\t\t<NumberOfPeople group=\"Large\"\n")
        f.write("\t\tprobability=\"" + random.choice(probabilities) + "\"\n")
        f.write("\t\tchance=\"" + random.choice(probabilities) + "\"/>\n\n")

        f.write("\t\t<NumberOfPeople group=\"Small\"\n")
        f.write("\t\tprobability=\"" + random.choice(probabilities) + "\"\n")
        f.write("\t\tchance=\"" + random.choice(probabilities) + "\"/>\n\n")
        f.write("\t</RoomSize>\n")

        f.write("</BehaviourRulesRoom>\n")

        f.write("<LocationPlan>\n")
        f.write("\t<LocationPlan room = \"" + random.choice(room) + "\"\n")
        f.write("\t\ttimeinroom = \"" + random.choice(time_in_room) + "\"/>\n\n")

        f.write("\t<LocationPlan room = \"" + random.choice(room) + "\"\n")
        f.write("\t\ttimeinroom = \"" + random.choice(time_in_room) + "\"/>\n\n")

        f.write("\t<LocationPlan room = \"" + random.choice(room) + "\"\n")
        f.write("\t\ttimeinroom = \"" + random.choice(time_in_room) + "\"/>\n\n")

        f.write("\t<LocationPlan room = \"" + random.choice(room) + "\"\n")
        f.write("\t\ttimeinroom = \"" + random.choice(time_in_room) + "\"/>\n\n")

        f.write("</LocationPlan>\n")
        f.write("</DecisionMakerBehaviour>\n")

