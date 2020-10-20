// See https://www.inf.usi.ch/carzaniga/edu/sysprog17s/coursesdb.h

#ifndef COURSEDB_H_INCLUDED
#define COURSEDB_H_INCLUDED

#include <stdlib.h>

/** Database Typedefs
 **/

typedef struct {
  int pkey;
  int id;
  char title[60];
  int year;
  char semester;
  int active;
} Course;

typedef struct {
  int pkey;
  int id;
  char name[30];
  int enroll_year;
  int active;
} Student;

typedef struct {
  int pkey;
  int student_id;
  int course_id;
  int active;
} Enrollment;

typedef struct {
  int initialized;
  Course *courses;
  int course_ct, course_max_ct;
  Student *students;
  int student_ct, student_max_ct;
  Enrollment *enrollments;
  int enrollment_ct, enrollment_max_ct;
} Database;

// db-level operations

extern int init_database();
extern int clear_database();

extern int save_tables(const char * prefix);
extern int load_tables(const char * prefix);

// add and delete records

extern int add_course(int id, const char * title, int year, char semester);
extern int delete_course(int id);

extern int add_student(int id, const char * name, int first_year);
extern int delete_student(int id);
extern int enroll_student(int student_id, int course_id);
extern int cancel_enrollment(int student_id, int course_id);

// iterators

struct student_iterator; 
extern struct student_iterator * next_student(struct student_iterator *);
extern void abort_student_iteration(struct student_iterator *);

extern int student_id(const struct student_iterator *);
extern const char * student_name(const struct student_iterator *);
extern int student_enrollment_year(const struct student_iterator *);

struct course_iterator;
extern struct course_iterator * next_course(struct course_iterator *);
extern void abort_course_iteration(struct course_iterator *);
extern int course_id(const struct course_iterator *);
extern const char * course_title(const struct course_iterator *);
extern int course_year(const struct course_iterator *);
extern char course_semester(const struct course_iterator *);

extern struct course_iterator * courses();
extern struct course_iterator * student_courses(int student_id);

extern struct student_iterator * students();
extern struct student_iterator * course_students(int course_id);

// helpers

extern int active_enrollments();
extern void print_stats();
int find_enrollment(int student_id, int course_id);

#endif
