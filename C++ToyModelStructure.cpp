#include <iostream>

int main() {
    struct UniversityStatus {
        string student_type; // Undergrad or Grad
        bool OnCampus; // We are only interested in students physically on-campus
    };

    struct Relationship {
        long PersonID; // BannerID of another individual
        vector<string> relationship_type; // A person might be a friend and also a classmate with another student
    };
    
    struct BehaviourRulesPerson {
        string person_status; // Different relationship types means different behaviour rules
        string safe_distance_probability;
        string mask_probability;
    };
    
    struct BehaviourRulesRoom {
        string room_size; // Different room sizes means different behaviour rules
        
        vector<NumberOfPeople> group; // The size of the group is either small or large
        vector<RoomSize> room_size; // The room is either small or large
    };

    struct student {
        long BannerID; // Unique number that will be used to differentiate between individuals
        
        bool IsSick; // Is the individual sick or healthy?
        
        string location; // Location of the model and where the simulation will occur
        string probability_of_safe_distance;
        string probability_wearing_mask;
        
        UniversityStatus status; // Undergrad or Grad
        
        vector<Relationship> relations; // There are different relation types (friend, familly, etc.)     
        vector<BehaviourRulesPerson> behaviour_person;
        vector<BehaviourRulesRoom> behaviour_room;
    };

    return 0;
}