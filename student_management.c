#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define FILE_NAME "students.dat"

// Define the structure for a student
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    int age;
    float grade;
} Student;

// Function prototypes
void addStudent();
void viewStudents();
void searchStudent();
void deleteStudent();
void updateStudent();
void menu();

// Main function
int main() {
    menu();
    return 0;
}

// Display the menu
void menu() {
    int choice;
    while (1) {
        printf("\n--- Student Management System ---\n");
        printf("1. Add Student\n");
        printf("2. View All Students\n");
        printf("3. Search Student by ID\n");
        printf("4. Delete Student\n");
        printf("5. Update Student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: deleteStudent(); break;
            case 5: updateStudent(); break;
            case 6: exit(0); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    }
}

// Add a new student record
void addStudent() {
    FILE *file = fopen(FILE_NAME, "ab");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    Student s;
    printf("Enter student ID: ");
    scanf("%d", &s.id);
    printf("Enter student name: ");
    getchar(); // Clear the newline character from input buffer
    fgets(s.name, MAX_NAME_LENGTH, stdin);
    s.name[strcspn(s.name, "\n")] = '\0'; // Remove trailing newline
    printf("Enter student age: ");
    scanf("%d", &s.age);
    printf("Enter student grade: ");
    scanf("%f", &s.grade);

    fwrite(&s, sizeof(Student), 1, file);
    fclose(file);
    printf("Student record added successfully!\n");
}

// View all student records
void viewStudents() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (!file) {
        printf("Error opening file or no records found!\n");
        return;
    }

    Student s;
    printf("\n--- Student Records ---\n");
    while (fread(&s, sizeof(Student), 1, file)) {
        printf("ID: %d, Name: %s, Age: %d, Grade: %.2f\n", s.id, s.name, s.age, s.grade);
    }
    fclose(file);
}

// Search for a student by ID
void searchStudent() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (!file) {
        printf("Error opening file or no records found!\n");
        return;
    }

    int id;
    printf("Enter student ID to search: ");
    scanf("%d", &id);

    Student s;
    int found = 0;
    while (fread(&s, sizeof(Student), 1, file)) {
        if (s.id == id) {
            printf("Student found: ID: %d, Name: %s, Age: %d, Grade: %.2f\n", s.id, s.name, s.age, s.grade);
            found = 1;
            break;
        }
    }
    if (!found) printf("Student with ID %d not found!\n", id);
    fclose(file);
}

// Delete a student record
void deleteStudent() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (!file) {
        printf("Error opening file or no records found!\n");
        return;
    }

    int id;
    printf("Enter student ID to delete: ");
    scanf("%d", &id);

    FILE *tempFile = fopen("temp.dat", "wb");
    if (!tempFile) {
        printf("Error creating temporary file!\n");
        fclose(file);
        return;
    }

    Student s;
    int found = 0;
    while (fread(&s, sizeof(Student), 1, file)) {
        if (s.id == id) {
            found = 1;
            continue;
        }
        fwrite(&s, sizeof(Student), 1, tempFile);
    }

    fclose(file);
    fclose(tempFile);

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (found)
        printf("Student record deleted successfully!\n");
    else
        printf("Student with ID %d not found!\n", id);
}

// Update a student record
void updateStudent() {
    FILE *file = fopen(FILE_NAME, "rb+");
    if (!file) {
        printf("Error opening file or no records found!\n");
        return;
    }

    int id;
    printf("Enter student ID to update: ");
    scanf("%d", &id);

    Student s;
    int found = 0;
    while (fread(&s, sizeof(Student), 1, file)) {
        if (s.id == id) {
            found = 1;
            printf("Enter new name: ");
            getchar(); // Clear the newline character
            fgets(s.name, MAX_NAME_LENGTH, stdin);
            s.name[strcspn(s.name, "\n")] = '\0';
            printf("Enter new age: ");
            scanf("%d", &s.age);
            printf("Enter new grade: ");
            scanf("%f", &s.grade);

            fseek(file, -sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, file);
            printf("Student record updated successfully!\n");
            break;
        }
    }
    if (!found) printf("Student with ID %d not found!\n", id);
    fclose(file);
}
