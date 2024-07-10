/**
 * @file schedule.cpp
 * Exam scheduling using graph coloring
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <unordered_map>

#include "schedule.h"
#include "utils.h"
#include <algorithm>

/**
 * Given a filename to a CSV-formatted text file, create a 2D vector of strings where each row
 * in the text file is a row in the V2D and each comma-separated value is stripped of whitespace
 * and stored as its own string. 
 * 
 * Your V2D should match the exact structure of the input file -- so the first row, first column
 * in the original file should be the first row, first column of the V2D.
 *  
 * @param filename The filename of a CSV-formatted text file. 
 */
V2D file_to_V2D(const std::string & filename){
    // Your code here!
    V2D vec;
    std::string file = file_to_string(filename);
    std::vector<std::string> curvec;
    std::string curstr = "";
    for (unsigned i = 0; i < file.size(); i++) {
        if (file[i] != '\n' && file[i] != ',') {
            curstr += file.at(i);
        } else if (file[i] == '\n') {
            curvec.push_back(trim(curstr));
            vec.push_back(curvec);
            curvec.clear();
            curstr = "";
        } else {
            curvec.push_back(trim(curstr));
            curstr = "";
        }
    }
    if (!curvec.empty()) {
        if (curstr.size() > 0) {
            curvec.push_back(trim(curstr));
        }
        vec.push_back(curvec);
    }
    return vec;
}

/**
 * Given a course roster and a list of students and their courses, 
 * perform data correction and return a course roster of valid students (and only non-empty courses).
 * 
 * A 'valid student' is a student who is both in the course roster and the student's own listing contains the course
 * A course which has no students (or all students have been removed for not being valid) should be removed
 * 
 * @param cv A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param student A 2D vector of strings where each row is a student ID followed by the courses they are taking
 */
V2D clean(const V2D & cv, const V2D & student){
    // YOUR CODE HERE
    V2D vec = cv;
    for (unsigned i = 0; i < vec.size(); i++) {
        std::string curcourse = vec[i][0];
        for (unsigned k = vec[i].size()-1; k > 0; k--) {
            std::string curstudent = vec[i][k];
            if (!inCourse(vec, student, curcourse, curstudent)) {
                vec[i].erase(vec[i].begin()+k);
            }
        }
    }
    for (unsigned i = vec.size()-1; i > 0; i--) {
        if (vec[i].size() <= 1) {
            vec.erase(vec.begin()+i);
        }
    }
    return vec;
}

bool inCourse(const V2D & cv, const V2D & student, std::string curcourse, std::string curstudent) {
    for (unsigned i = 0; i < student.size(); i++) {
        if (curstudent == student[i][0]) {
            for (unsigned k = 1; k < student[i].size(); k++) {
                if (student[i][k] == curcourse) {
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

void buildMatrix(const V2D &courses) {
    unsigned length = courses.size();
    for (unsigned i = 0; i < length; i++) {
        for (unsigned k = 0; k < length; k++) {
            
        }
    }
}

/**
 * Given a collection of courses and a list of available times, create a valid scheduling (if possible).
 * 
 * A 'valid schedule' should assign each course to a timeslot in such a way that there are no conflicts for exams
 * In other words, two courses who share a student should not share an exam time.
 * Your solution should try to minimize the total number of timeslots but should not exceed the timeslots given.
 * 
 * The output V2D should have one row for each timeslot, even if that timeslot is not used.
 * 
 * As the problem is NP-complete, your first scheduling might not result in a valid match. Your solution should 
 * continue to attempt different schedulings until 1) a valid scheduling is found or 2) you have exhausted all possible
 * starting positions. If no match is possible, return a V2D with one row with the string '-1' as the only value. 
 * 
 * @param courses A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param timeslots A vector of strings giving the total number of unique timeslots
 */
V2D schedule(const V2D &courses, const std::vector<std::string> &timeslots){
    // Your code here!
    return courses;
}