#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100
#define MAX_TITLE 100
#define MAX_AUTHOR 50

typedef struct {
    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int isIssued;
} Book;

Book library[MAX_BOOKS];
int bookCount = 0;

// Function prototypes
void addBook();
void viewBooks();
void searchBook();
void issueBook();
void returnBook();
void deleteBook();
void saveBooksToFile();
void loadBooksFromFile();
void showMenu();
int validateBookID(int bookID);

int main() {
    int choice;

    loadBooksFromFile();

    do {
        showMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                viewBooks();
                break;
            case 3:
                searchBook();
                break;
            case 4:
                issueBook();
                break;
            case 5:
                returnBook();
                break;
            case 6:
                deleteBook();
                break;
            case 7:
                saveBooksToFile();
                printf("Exiting... Data saved.\n");
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 7.\n");
        }
    } while (choice != 7);

    return 0;
}

void showMenu() {
    printf("\nLibrary Management System\n");
    printf("*************************\n");
    printf("1. Add Book\n");
    printf("2. View All Books\n");
    printf("3. Search Book\n");
    printf("4. Issue Book\n");
    printf("5. Return Book\n");
    printf("6. Delete Book\n");
    printf("7. Exit\n");
    printf("*************************\n");
}

void addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("Library is full! Cannot add more books.\n");
        return;
    }

    Book newBook;
    newBook.id = bookCount + 1;

    getchar(); // Consume newline from previous input
    printf("Enter book title: ");
    fgets(newBook.title, MAX_TITLE, stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0; // Remove newline

    if (strlen(newBook.title) == 0) {
        printf("Error: Book title cannot be empty.\n");
        return;
    }

    printf("Enter book author: ");
    fgets(newBook.author, MAX_AUTHOR, stdin);
    newBook.author[strcspn(newBook.author, "\n")] = 0; // Remove newline

    if (strlen(newBook.author) == 0) {
        printf("Error: Book author cannot be empty.\n");
        return;
    }

    newBook.isIssued = 0;

    library[bookCount] = newBook;
    bookCount++;

    printf("Book added successfully! Book ID: %d\n", newBook.id);
}

void viewBooks() {
    if (bookCount == 0) {
        printf("No books available in the library.\n");
        return;
    }

    printf("\nList of Books:\n");
    printf("ID\tTitle\t\tAuthor\t\tStatus\n");
    printf("---------------------------------------------\n");

    for (int i = 0; i < bookCount; i++) {
        printf("%d\t%s\t\t%s\t\t%s\n", library[i].id, library[i].title,
               library[i].author, library[i].isIssued ? "Issued" : "Available");
    }
}

void searchBook() {
    char searchTitle[MAX_TITLE];
    getchar(); // Consume newline from previous input
    printf("Enter book title to search: ");
    fgets(searchTitle, MAX_TITLE, stdin);
    searchTitle[strcspn(searchTitle, "\n")] = 0; // Remove newline

    int found = 0;
    for (int i = 0; i < bookCount; i++) {
        if (strcasecmp(library[i].title, searchTitle) == 0) {
            printf("\nBook found!\n");
            printf("ID: %d\nTitle: %s\nAuthor: %s\nStatus: %s\n",
                   library[i].id, library[i].title, library[i].author,
                   library[i].isIssued ? "Issued" : "Available");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Book not found in the library.\n");
    }
}

void issueBook() {
    int bookID;
    printf("Enter book ID to issue: ");
    scanf("%d", &bookID);

    if (!validateBookID(bookID) || library[bookID - 1].isIssued) {
        printf("Invalid book ID or book already issued.\n");
        return;
    }

    library[bookID - 1].isIssued = 1;
    printf("Book ID %d issued successfully.\n", bookID);
}

void returnBook() {
    int bookID;
    printf("Enter book ID to return: ");
    scanf("%d", &bookID);

    if (!validateBookID(bookID) || !library[bookID - 1].isIssued) {
        printf("Invalid book ID or book is not issued.\n");
        return;
    }

    library[bookID - 1].isIssued = 0;
    printf("Book ID %d returned successfully.\n", bookID);
}

void deleteBook() {
    int bookID;
    printf("Enter book ID to delete: ");
    scanf("%d", &bookID);

    if (!validateBookID(bookID)) {
        printf("Invalid book ID.\n");
        return;
    }

    for (int i = bookID - 1; i < bookCount - 1; i++) {
        library[i] = library[i + 1];
    }

    bookCount--;
    printf("Book ID %d deleted successfully.\n", bookID);
}

void saveBooksToFile() {
    FILE *file = fopen("library.dat", "wb");
    if (file == NULL) {
        printf("Error saving data to file.\n");
        return;
    }

    fwrite(&bookCount, sizeof(int), 1, file);
    fwrite(library, sizeof(Book), bookCount, file);
    fclose(file);
}

void loadBooksFromFile() {
    FILE *file = fopen("library.dat", "rb");
    if (file == NULL) {
        printf("No previous data found. Starting fresh.\n");
        return;
    }

    fread(&bookCount, sizeof(int), 1, file);
    fread(library, sizeof(Book), bookCount, file);
    fclose(file);
}

int validateBookID(int bookID) {
    if (bookID < 1 || bookID > bookCount) {
        return 0;
    }
    return 1;
}
void loadBooksFromFile() {
    FILE *file = fopen("library.dat", "rb");
    if (file == NULL) {
        printf("No previous data found. Starting fresh.\n");
        return;
    }

    fread(&bookCount, sizeof(int), 1, file);
    fread(library, sizeof(Book), bookCount, file);
    fclose(file);
}

int validateBookID(int bookID) {
    if (bookID < 1 || bookID > bookCount) {
        return 0;
    }
    return 1;
}