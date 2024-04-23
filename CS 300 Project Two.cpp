//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Ellen Takada
// Version     : 1.0
// Description : Project Two
//============================================================================

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#include "CSVparser.hpp"

using namespace std;

// constructor
struct Course {
    string courseNumber;
    string name;
    vector<string> prereqisite;
    int N = 0; // default number of prerequisite
    
    struct Course* left;
    struct Course* right;
};

class Tree {
public:
    Course* root;

    Tree();
    ~Tree();
    void recursiveDelete(Course* c);
    void recursiveSearchCaller(Tree* t, string courseNumber);
    void recursiveSearch(Course* c, string courseNumber);
    void printInOrder(Course* c);
    void printSampleSchedule(Tree* t);
    void printCourseInformation(Tree* t, string courseNumber);
};

/**
 * Default constructor
 */
Tree::Tree() {
    // FixMe (1): initialize housekeeping variables
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
Tree::~Tree() {
    recursiveDelete(root);
    // recurse from root deleting every node
}

void Tree::recursiveDelete(Course* c) {
    if (c != nullptr) {
        if (c->left == nullptr && c->right == nullptr) {
            delete c;
            return;
        }
        recursiveDelete(c->left);
        recursiveDelete(c->right);
        recursiveDelete(c);
    }
    return;
}

void treeInsert(Tree* t, Course c) {
    if (t->root == nullptr) {
        //cout << "b " << c.courseNumber << endl;
        t->root = new Course;
        t->root->courseNumber = c.courseNumber;
        //cout << "a " << c.courseNumber << endl;
        t->root->name = c.name;
        for (size_t i = 0; i < c.prereqisite.size(); i++){
            t->root->prereqisite.push_back(c.prereqisite[i]);
        }
        t->root->left = nullptr;
        t->root->right = nullptr;
        //cout << t->root->courseNumber << endl;
    }
    else {
        Course* cur = t->root;
        //cout << "Root " << t->root->courseNumber << endl;
        //cout << "before while " << c.courseNumber << " " << cur->courseNumber << "\n";
        while (cur != nullptr) {
            //cout << "while loop " << c.courseNumber << " " << cur->courseNumber << "\n";
            if (c.courseNumber < cur->courseNumber) {
                //cout << "left\n";
                if (cur->left == nullptr) {
                    cur->left = new Course;
                    cur->left->courseNumber = c.courseNumber;
                    cur->left->name = c.name;
                    for (size_t i = 0; i < c.prereqisite.size(); i++) {
                        cur->left->prereqisite.push_back(c.prereqisite[i]);
                    }
                    cur->left->left = nullptr;
                    cur->left->right = nullptr;
                    cur = nullptr;
                }
                else {
                    cur = cur->left;
                    //cout << "second line\n";
                }
            }
            else { // right 
                //cout << "right\n";
                if (cur->right == nullptr) {
                    cur->right = new Course;
                    cur->right->courseNumber = c.courseNumber;
                    cur->right->name = c.name;
                    for (size_t i = 0; i < c.prereqisite.size(); i++) {
                        cur->right->prereqisite.push_back(c.prereqisite[i]);
                    }
                    cur->right->left = nullptr;
                    cur->right->right = nullptr;
                    cur = nullptr;
                }
                else {
                    cur = cur->right;
                }
            }
        }
        c.left = nullptr;
        c.right = nullptr;
    }
}

void loadCourses(string csvPath, Tree* t) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    //csv::Parser file = csv::Parser(csvPath);
    //cout << file.rowCount();
    std::ifstream file(csvPath);

    string line;
    int lineNum = 1;
    while (getline(file, line)) {
        istringstream linestream(line);
        string token;
        Course c;
        int tokenNumber=0;
        while (getline(linestream, token, ',')) {
            if (tokenNumber == 0) {
                c.courseNumber = token;
            }
            else if (tokenNumber == 1) {
                c.name = token;
            }
            else {
                c.prereqisite.push_back(token);
            }
            ++tokenNumber;
            //cout << tokenNumber;
        }
        if (tokenNumber < 1) {
            cout << "Insufficient Information." << endl;
            break;
        }
        // push this bid to the end
        //cout << "\nline read\n";
        treeInsert(t, c);
        //cout << lineNum << " insert\n\n";
        ++lineNum;
    }
}

void Tree::recursiveSearchCaller(Tree* t, string courseNumber) {
    recursiveSearch(t->root, courseNumber);
}

void Tree::recursiveSearch(Course* c, string courseNumber) {
    if (c != nullptr) {
        if (courseNumber == c->courseNumber) {
            cout << c->courseNumber << endl;
            cout << c->name << endl;
            for (int i = 0; i < c->prereqisite.size(); ++i) {
                cout << c->prereqisite[i] << endl;
            }
            return;
        }
        else if (courseNumber < c->courseNumber) {
            //cout << "left\n";
            recursiveSearch(c->left, courseNumber);
        }
        else {
            //cout << "right\n";
            recursiveSearch(c->right, courseNumber);
        }
    }
}

void Tree::printInOrder(Course* c) {
    // print courses in alphanumeric order
    if (c == nullptr) {
        return;
    }
    printInOrder(c->left);
    cout << c->courseNumber << endl;
    printInOrder(c->right);
}

void Tree::printSampleSchedule(Tree* t) {
    Course* head = t->root;
    printInOrder(head);
}

void Tree::printCourseInformation(Tree* t, string courseNumber) {
    // print a single course
    recursiveSearchCaller(t, courseNumber);
    //cout << "Bauwa";
    //cout << targetCourse.name << endl;
    //for (int i = 0; i < targetCourse.prereqisite.size(); ++i) {
    //    cout << targetCourse.prereqisite[i] << endl;
    //}
}



/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

int main(int argc, char* argv[])
{

    // process command line arguments
    string csvPath, CourseKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        CourseKey = argv[2];
        break;
    default:
        csvPath = "ABCU Advising Program Input.csv";
        CourseKey = "CSCI400";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all Courses
    Tree* t;
    t = new Tree();
    Course Course;

    int choice = 0;
    while (choice != 9) {
        cout << endl << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Display Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cout << endl;

        switch (choice) {

        case 1:
            // Initialize a timer variable before loading Courses
            ticks = clock();

            // Complete the method call to load the Courses
            loadCourses(csvPath, t);

            break;

        case 2:
            t->printSampleSchedule(t);
            break;

        case 3: 
            cout << "Enter the courseNumber: ";
            cin >> CourseKey;
            t->printCourseInformation(t, CourseKey);
            break;

        }
    }

    cout << "Good bye." << endl;

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
