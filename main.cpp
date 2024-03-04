//had few issues with seperating into seperate class files. chose to keep all in one cpp file
#include "CourseManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>


Course::Course(std::string code, std::string name, std::vector<std::string> prereqs)
    : courseCode(std::move(code)), title(std::move(name)), prerequisites(std::move(prereqs)) {}
//constructs for course code, name, preq.

TreeNode::TreeNode(Course c) : course(std::move(c)), left(nullptr), right(nullptr) {}
//treenode intializing left pointer and right pointer
TreeNode::~TreeNode() { //to delete data ; destructor for tree node above
    delete left;
    delete right;
}


CourseManager::CourseManager() : root(nullptr) {} //starting with empty tree

CourseManager::~CourseManager() {
    clear(root); //deletes all of the nodes
}

void CourseManager::clear(TreeNode* node) { //recursive
    if (node) {
        clear(node->left); // will delete left or right tree
        clear(node->right);
        delete node; // specific node to be deleted
    }
}

TreeNode* CourseManager::insert(TreeNode* node, Course course) { //to insert new course
    if (!node) return new TreeNode(std::move(course)); //will make new node if its empty
    if (course.courseCode < node->course.courseCode) { //check if it needs to be inserted lifted left or right
        node->left = insert(node->left, std::move(course)); // to keep it alphabetical order
    } else if (course.courseCode > node->course.courseCode) {
        node->right = insert(node->right, std::move(course));
    }
    return node; // outputs the updated the tree
}

void CourseManager::inOrderTraversal(TreeNode* node) const {
    if (node) {
        inOrderTraversal(node->left);  //traverse the tree. in this case left side
        std::cout << node->course.courseCode << ", " << node->course.title << "\n"; // outpount course code and name
        inOrderTraversal(node->right); // right side
    }
}

TreeNode* CourseManager::find(TreeNode* node, const std::string& courseCode) const { //searches by its code such as MATH201
    if (!node) return nullptr; //error check in case if node is empty
    if (courseCode == node->course.courseCode) return node; //if found will return node
    if (courseCode < node->course.courseCode) return find(node->left, courseCode); // searches to the left if alphabetically requires
    return find(node->right, courseCode); // otherwise search after the current node
}

void CourseManager::loadCoursesFromFile(const std::string& filename) { // open file
    std::ifstream file(filename); // will open the given file name
    if (!file) {
        std::cout << "Failed to open file: " << filename << std::endl;
        return; //if boolean check in case if file name could not be opened and will output error
    }

    std::string line;
    bool loadSuccess = false; // if loaded successfully
    while (getline(file, line)) { // will read line by line
        std::istringstream iss(line);
        std::string code, title, part; //will store the seperated text by comma into respective variable
        std::vector<std::string> prereqs;

        // Attempt to parse the course code and title
        if (!getline(iss, code, ',') || !getline(iss, title, ',')) {
            std::cout << "Error parsing line: " << line << std::endl; //fomat check. if no correct course code, course name then error
            continue; // will continue reading the next line
        }

      
        while (getline(iss, part, ',')) { //adding to prerequisites to
            prereqs.push_back(part);
        }

        Course course(code, title, prereqs); // creates the object with details read previously
        root = insert(root, std::move(course)); // adds new course from root with std::move feature
        loadSuccess = true; //so that program can output something instead of failure to read file. will indicate 1 or more line read
    }

    if (loadSuccess) {
        std::cout << "Data loaded successfully.\n"; // ui output to tell user loadSucess is valid
    } else {
        std::cout << "No valid course data found in the file.\n"; // will indicate user to fix input text file
    }
}

void CourseManager::printCourseList() const {
    inOrderTraversal(root); //outputs the entire tree
}

void CourseManager::printCourse(const std::string& courseCode) const { //essentially looks for course code and ouputs info
    TreeNode* node = find(root, courseCode); //to find from root to specified course code
    if (node) {  // if found
        std::cout << node->course.courseCode << ", " << node->course.title << "\nPrerequisites: "; // course output (code, name)
        for (const auto& prereq : node->course.prerequisites) {
            std::cout << prereq << (prereq != node->course.prerequisites.back() ? ", " : ""); //will keep outputting prerequisitites until end
        }
        std::cout << "\n";
    } else {
        std::cout << "Course not found.\n"; //tell user their specified couse code doesnt exist in given file
    }
}
int main() {
    CourseManager manager;
    int choice; // for menu selection
    std::string filename, courseCode; //string for courses

    std::cout << "Welcome to the course planner.\n";

    do { // do while loop as long as user doesnt exit/ type 4
        std::cout << "1. Load Data Structure.\n";
        std::cout << "2. Print Course List.\n";
        std::cout << "3. Print Course.\n";
        std::cout << "4. Exit\n";
        std::cout << "What would you like to do? ";
        if (!(std::cin >> choice)) {
            std::cin.clear(); // cleans error
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // input thrown away
            choice = -1; // //choice becomes invalid and user will be requested to try again to set it to 1,2,3,4
        }

        switch (choice) {
            case 1: // to open and parse file
                std::cout << "Enter filename: ";
                std::cin >> filename; //will look and open file. type test1.txt to test
                manager.loadCoursesFromFile(filename); //parse file, will output success statement if valid
                break;
            case 2:
                manager.printCourseList(); // prints all data in the given file
                break;
            case 3: // to search for specific file
                std::cout << "What course do you want to know about? ";
                std::cin >> courseCode;
                manager.printCourse(courseCode); //prints the specified course details and prerequisites if necessary
                break;
            case 4:
                std::cout << "Thank you for using the course planner!\n"; // exit
                break;
            default:
                std::cout << "Invalid option. Please try again.\n"; //when starting or choice=-1 will be prompted this
                
        }
    } while (choice != 4); //if user types 4, program quits

    return 0;
}
