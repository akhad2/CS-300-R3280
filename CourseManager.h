#ifndef COURSE_MANAGER_H
#define COURSE_MANAGER_H

#include <vector>
#include <string>
 //header file

class Course {
public:
    std::string courseCode;
    std::string title;
    std::vector<std::string> prerequisites;

    Course(std::string code, std::string name, std::vector<std::string> prereqs = {});
};

class TreeNode {
public:
    Course course;
    TreeNode* left;
    TreeNode* right;
    TreeNode(Course c);
    ~TreeNode();
};
class CourseManager {
private:
    TreeNode* root;
    TreeNode* insert(TreeNode* node, Course course);
    void inOrderTraversal(TreeNode* node) const;
    TreeNode* find(TreeNode* node, const std::string& courseCode) const;
    void clear(TreeNode* node);

public:
    CourseManager();
    ~CourseManager();
    void loadCoursesFromFile(const std::string& filename);
    void printCourseList() const;
    void printCourse(const std::string& courseCode) const;
};

#endif
