#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define COLOR_YELLOW "\033[0;33m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\033[0;31m"
#define COLOR_PURPLE "\033[0;35m"
#define COLOR_CYAN "\033[0;36m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_RESET "\033[0m"

#define ORG_CODE "ORG123"  // Example organization code

void register_user(char filename[]);
int user_login(char filename[]);
void password_masking(char pass[20]);
void change_password(char filename[]);
int is_duplicate_username(char filename[], char username[]);
void customize_or_edit_profile(char username[]);
void view_profile(char username[]);
void student_features(char filename[]);
void teacher_features(char filename[]);
void admin_features(char filename[]);
void start_exam(char username[]);
void view_results(char username[]);
void view_schedule();
void create_exam();
void edit_exam();
void delete_exam();
void add_question(char exam_file[]);
void view_scores();
void schedule_exam();
void manage_teacher_accounts();
void view_student_list();
void view_delete_exams();
void center(char *s, int n);

int main() {
    int role, choice;
    char file[20];

    printf(COLOR_GREEN "\n\t\tOnline Exam Management System\n" COLOR_RESET);
    printf(COLOR_YELLOW "\nPress Enter to continue..." COLOR_RESET);
    getchar();
    system("cls");

    while (1) {
        printf(COLOR_PURPLE "\nSelect Role:\n1. Student\n2. Teacher\n3. Admin\n4. Exit\nEnter choice: " COLOR_RESET);
        scanf("%d", &role);

        if (role == 4) {
            printf(COLOR_GREEN "Exiting... Goodbye!\n" COLOR_RESET);
            break;
        }

        if (role == 3) {
            char code[20];
            printf(COLOR_YELLOW "Enter Organization Code: " COLOR_RESET);
            scanf("%s", code);
            if (strcmp(code, ORG_CODE) != 0) {
                printf(COLOR_RED "Invalid code. Access denied.\n" COLOR_RESET);
                continue;
            }
        }

        switch (role) {
            case 1: strcpy(file, "students.txt"); break;
            case 2: strcpy(file, "teachers.txt"); break;
            case 3: strcpy(file, "admins.txt"); break;
            default: printf(COLOR_RED "Invalid role.\n" COLOR_RESET); continue;
        }

        if (role != 3) {
            printf(COLOR_PURPLE "\n1. Register\n2. Login\n3. Go Back\nEnter choice: " COLOR_RESET);
            scanf("%d", &choice);
            if (choice == 1) register_user(file);
            else if (choice == 2 && user_login(file)) {
                if (role == 1) student_features(file);
                else if (role == 2) teacher_features(file);
            } else if (choice == 3) continue;
            else printf(COLOR_RED "Invalid choice.\n" COLOR_RESET);
        } else {
            printf(COLOR_PURPLE "\n1. Login\n2. Go Back\nEnter choice: " COLOR_RESET);
            scanf("%d", &choice);
            if (choice == 1 && user_login(file)) admin_features(file);
            else if (choice == 2) continue;
            else printf(COLOR_RED "Invalid choice.\n" COLOR_RESET);
        }
    }
    return 0;
}
void register_user(char filename[]) {
    FILE *fp = fopen(filename, "a+");
    if (!fp) {
        printf(COLOR_RED "File error.\n" COLOR_RESET);
        return;
    }

    char username[20], password[20];
    printf(COLOR_YELLOW "Enter username: " COLOR_RESET);
    scanf("%s", username);
    if (is_duplicate_username(filename, username)) {
        printf(COLOR_RED "Username already exists.\n" COLOR_RESET);
        fclose(fp);
        return;
    }
    printf(COLOR_YELLOW "Enter password: " COLOR_RESET);
    password_masking(password);

    fprintf(fp, "%s %s\n", username, password);
    fclose(fp);
    printf(COLOR_GREEN "Registered successfully.\n" COLOR_RESET);
}

int user_login(char filename[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf(COLOR_RED "File error.\n" COLOR_RESET);
        return 0;
    }

    char username[20], password[20], file_user[20], file_pass[20];
    printf(COLOR_YELLOW "Username: " COLOR_RESET);
    scanf("%s", username);
    printf(COLOR_YELLOW "Password: " COLOR_RESET);
    password_masking(password);

    while (fscanf(fp, "%s %s", file_user, file_pass) != EOF) {
        if (strcmp(username, file_user) == 0 && strcmp(password, file_pass) == 0) {
            fclose(fp);
            printf(COLOR_GREEN "Login successful.\n" COLOR_RESET);
            return 1;
        }
    }
    fclose(fp);
    printf(COLOR_RED "Login failed.\n" COLOR_RESET);
    return 0;
}

void change_password(char filename[]) {
    char username[20], old_password[20], new_password[20];
    printf(COLOR_YELLOW "\nEnter username: " COLOR_RESET);
    scanf("%s", username);
    printf(COLOR_YELLOW "Enter current password: " COLOR_RESET);
    password_masking(old_password);

    FILE *user = fopen(filename, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!user || !temp) {
        printf(COLOR_RED "File error.\n" COLOR_RESET);
        return;
    }

    int changed = 0;
    char file_user[20], file_pass[20];
    while (fscanf(user, "%s %s", file_user, file_pass) != EOF) {
        if (strcmp(file_user, username) == 0 && strcmp(file_pass, old_password) == 0) {
            printf(COLOR_YELLOW "Enter new password: " COLOR_RESET);
            scanf("%s", new_password);
            fprintf(temp, "%s %s\n", username, new_password);
            changed = 1;
        } else {
            fprintf(temp, "%s %s\n", file_user, file_pass);
        }
    }

    fclose(user);
    fclose(temp);
    remove(filename);
    rename("temp.txt", filename);

    if (changed)
        printf(COLOR_GREEN "Password changed successfully.\n" COLOR_RESET);
    else
        printf(COLOR_RED "Invalid credentials.\n" COLOR_RESET);
}

void password_masking(char pass[20]) {
    int i = 0, ch;
    while ((ch = getch()) != '\r' && i < 19) {
        if (ch == '\b') {
            if (i > 0) {
                printf("\b \b");
                i--;
            }
        } else {
            pass[i++] = ch;
            printf("*");
        }
    }
    pass[i] = '\0';
    printf("\n");
}

int is_duplicate_username(char filename[], char username[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    char file_user[20], file_pass[20];
    while (fscanf(fp, "%s %s", file_user, file_pass) != EOF) {
        if (strcmp(file_user, username) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void student_features(char filename[]) {
    char username[20];
    int choice;

    printf(COLOR_YELLOW "Enter your username: " COLOR_RESET);
    scanf("%s", username);

    while (1) {
        printf(COLOR_CYAN "\n-- Student Menu --\n" COLOR_RESET);
        printf(COLOR_BLUE "1. Change Password\n2. Edit Profile\n3. View Profile\n4. Start Exam\n5. View Results\n6. View Exam Schedule\n7. Go Back\n" COLOR_RESET);
        printf(COLOR_YELLOW "Enter choice: " COLOR_RESET);
        scanf("%d", &choice);

        switch (choice) {
            case 1: change_password(filename); break;
            case 2: customize_or_edit_profile(username); break;
            case 3: view_profile(username); break;
            case 4: start_exam(username); break;
            case 5: view_results(username); break;
            case 6: view_schedule(); break;
            case 7: return;
            default: printf(COLOR_RED "Invalid choice.\n" COLOR_RESET);
        }
    }
}

void customize_or_edit_profile(char username[]) {
    char name[30], semester[10], id[15];
    FILE *fp = fopen("profiles.txt", "r");
    FILE *temp = fopen("temp_profiles.txt", "w");
    int found = 0;
    char file_user[20], file_name[30], file_semester[10], file_id[15];

    getchar();
    printf(COLOR_YELLOW "Full Name: " COLOR_RESET); fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = 0;
    printf(COLOR_YELLOW "Semester: " COLOR_RESET); scanf("%s", semester);
    printf(COLOR_YELLOW "Student ID: " COLOR_RESET); scanf("%s", id);

    if (!fp || !temp) {
        printf(COLOR_RED "File error.\n" COLOR_RESET); return;
    }

    while (fscanf(fp, "%s %s %s %s", file_user, file_name, file_semester, file_id) != EOF) {
        if (strcmp(file_user, username) == 0) {
            fprintf(temp, "%s %s %s %s\n", username, name, semester, id);
            found = 1;
        } else {
            fprintf(temp, "%s %s %s %s\n", file_user, file_name, file_semester, file_id);
        }
    }

    if (!found) {
        fprintf(temp, "%s %s %s %s\n", username, name, semester, id);
    }

    fclose(fp); fclose(temp);
    remove("profiles.txt");
    rename("temp_profiles.txt", "profiles.txt");
    printf(COLOR_GREEN "Profile updated.\n" COLOR_RESET);
}

void view_profile(char username[]) {
    FILE *fp = fopen("profiles.txt", "r");
    if (!fp) {
        printf(COLOR_RED "File error.\n" COLOR_RESET); return;
    }

    char file_user[20], file_name[30], file_semester[10], file_id[15];
    int found = 0;
    while (fscanf(fp, "%s %s %s %s", file_user, file_name, file_semester, file_id) != EOF) {
        if (strcmp(file_user, username) == 0) {
            printf(COLOR_GREEN "\n-- Profile --\n" COLOR_RESET);
            printf("Name     : %s\n", file_name);
            printf("Semester : %s\n", file_semester);
            printf("Student ID: %s\n", file_id);
            found = 1;
            break;
        }
    }
    fclose(fp);
    if (!found) printf(COLOR_RED "Profile not found.\n" COLOR_RESET);
}

void start_exam(char username[]) {
    char exam_file[30], line[256], answer, correct, qtext[200];
    int total = 0, score = 0, qnum = 0;

    printf(COLOR_YELLOW "Enter exam course code (e.g., SE-113): " COLOR_RESET);
    scanf("%s", exam_file);
    sprintf(exam_file, "exam_%s.txt", exam_file);

    FILE *exam = fopen(exam_file, "r");
    FILE *result = fopen("results.txt", "a");
    if (!exam || !result) {
        printf(COLOR_RED "Exam not found.\n" COLOR_RESET);
        return;
    }

    fprintf(result, "\nUsername: %s\nExam: %s\n", username, exam_file);
    while (fgets(line, sizeof(line), exam)) {
        if (strncmp(line, "Q:", 2) == 0) {
            qnum++;
            total++;
            printf(COLOR_CYAN "%s" COLOR_RESET, line);
            for (int i = 0; i < 4; i++) {
                fgets(line, sizeof(line), exam);
                printf("%s", line);
            }
            fgets(line, sizeof(line), exam); // Answer: x
            correct = line[strlen(line) - 2]; // Last char before newline

            printf(COLOR_YELLOW "Your answer (a/b/c/d): " COLOR_RESET);
            scanf(" %c", &answer);
            if (answer == correct) score++;

            fprintf(result, "Q%d: Your Answer: %c - Correct Answer: %c\n", qnum, answer, correct);
        }
    }
    fprintf(result, "Score: %d/%d\n", score, total);
    fclose(exam);
    fclose(result);
    printf(COLOR_GREEN "Exam completed. Score: %d/%d\n" COLOR_RESET, score, total);
}

void view_results(char username[]) {
    FILE *fp = fopen("results.txt", "r");
    if (!fp) {
        printf(COLOR_RED "No result found.\n" COLOR_RESET); return;
    }

    char line[256];
    int show = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, username)) show = 1;
        if (show) printf("%s", line);
        if (line[0] == '\n') show = 0;
    }
    fclose(fp);
}

void view_schedule() {
    FILE *fp = fopen("schedule.txt", "r");
    if (!fp) {
        printf(COLOR_RED "No schedule found.\n" COLOR_RESET);
        return;
    }
    char line[100];
    printf(COLOR_GREEN "\n-- Upcoming Exams --\n" COLOR_RESET);
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}

void teacher_features(char filename[]) {
    int choice;
    while (1) {
        printf(COLOR_CYAN "\n-- Teacher Menu --\n" COLOR_RESET);
        printf(COLOR_BLUE "1. Change Password\n2. Create Exam\n3. Edit Exam\n4. Delete Exam\n5. Add Question to Exam\n6. Schedule Exam\n7. View Scores\n8. Go Back\n" COLOR_RESET);
        printf(COLOR_YELLOW "Enter your choice: " COLOR_RESET);
        scanf("%d", &choice);

        switch (choice) {
            case 1: change_password(filename); break;
            case 2: create_exam(); break;
            case 3: edit_exam(); break;
            case 4: delete_exam(); break;
            case 5: {
                char exam_code[20], filename[30];
                printf(COLOR_YELLOW "Enter course code: " COLOR_RESET);
                scanf("%s", exam_code);
                sprintf(filename, "exam_%s.txt", exam_code);
                add_question(filename);
                break;
            }
            case 6: schedule_exam(); break;
            case 7: view_scores(); break;
            case 8: return;
            default: printf(COLOR_RED "Invalid choice.\n" COLOR_RESET);
        }
    }
}

void create_exam() {
    char code[20], file[40];
    printf(COLOR_YELLOW "Enter course code (e.g. SE-113): " COLOR_RESET);
    scanf("%s", code);
    sprintf(file, "exam_%s.txt", code);
    FILE *fp = fopen(file, "w");
    if (!fp) {
        printf(COLOR_RED "File creation failed.\n" COLOR_RESET); return;
    }
    printf(COLOR_GREEN "Exam created: %s\n" COLOR_RESET, file);
    fclose(fp);
}

void add_question(char exam_file[]) {
    FILE *fp = fopen(exam_file, "a");
    if (!fp) {
        printf(COLOR_RED "File open failed.\n" COLOR_RESET); return;
    }

    char question[100], options[4][50], answer;
    getchar();
    printf(COLOR_YELLOW "Enter question: " COLOR_RESET); fgets(question, sizeof(question), stdin); question[strcspn(question, "\n")] = 0;
    for (int i = 0; i < 4; i++) {
        printf(COLOR_YELLOW "Option %c: " COLOR_RESET, 'a'+i);
        fgets(options[i], sizeof(options[i]), stdin);
        options[i][strcspn(options[i], "\n")] = 0;
    }
    printf(COLOR_YELLOW "Correct answer (a/b/c/d): " COLOR_RESET);
    scanf(" %c", &answer);

    fprintf(fp, "Q: %s\n", question);
    for (int i = 0; i < 4; i++)
        fprintf(fp, "%c) %s\n", 'a'+i, options[i]);
    fprintf(fp, "Answer: %c\n\n", answer);

    fclose(fp);
    printf(COLOR_GREEN "Question added.\n" COLOR_RESET);
}

void edit_exam() {
    printf(COLOR_RED "Feature not implemented yet. (Edit exam manually by opening the file)\n" COLOR_RESET);
}

void delete_exam() {
    char code[20], filename[30];
    printf(COLOR_YELLOW "Enter course code to delete exam (e.g., SE-113): " COLOR_RESET);
    scanf("%s", code);
    sprintf(filename, "exam_%s.txt", code);
    if (remove(filename) == 0)
        printf(COLOR_GREEN "Deleted exam: %s\n" COLOR_RESET, filename);
    else
        printf(COLOR_RED "Exam not found.\n" COLOR_RESET);
}

void schedule_exam() {
    char course[20], date[20];
    FILE *fp = fopen("schedule.txt", "a");
    if (!fp) {
        printf(COLOR_RED "File open error.\n" COLOR_RESET); return;
    }
    printf(COLOR_YELLOW "Enter course code: " COLOR_RESET); scanf("%s", course);
    printf(COLOR_YELLOW "Enter date (YYYY-MM-DD): " COLOR_RESET); scanf("%s", date);
    fprintf(fp, "%s %s\n", course, date);
    fclose(fp);
    printf(COLOR_GREEN "Exam scheduled.\n" COLOR_RESET);
}

void view_scores() {
    FILE *fp = fopen("results.txt", "r");
    if (!fp) {
        printf(COLOR_RED "No results found.\n" COLOR_RESET); return;
    }
    int choice;
    char line[256], key[50];
    printf(COLOR_CYAN "\n-- View Scores --\n" COLOR_RESET);
    printf("1. By Exam\n2. By Student\n3. View All\nChoice: ");
    scanf("%d", &choice);

    printf(COLOR_YELLOW "Enter value: " COLOR_RESET);
    scanf("%s", key);

    rewind(fp);
    int show = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (choice == 1 && strstr(line, "Exam:") && strstr(line, key)) show = 1;
        else if (choice == 2 && strstr(line, "Username:") && strstr(line, key)) show = 1;
        else if (choice == 3) show = 1;

        if (show) printf("%s", line);
        if (line[0] == '\n') show = 0;
    }
    fclose(fp);
}

void admin_features(char filename[]) {
    int choice;
    while (1) {
        printf(COLOR_CYAN "\n-- Admin Menu --\n" COLOR_RESET);
        printf(COLOR_BLUE "1. Change Password\n2. Add/Remove Teachers\n3. View Student List\n4. View/Delete Exams\n5. Go Back\n" COLOR_RESET);
        printf(COLOR_YELLOW "Enter your choice: " COLOR_RESET);
        scanf("%d", &choice);

        switch (choice) {
            case 1: change_password(filename); break;
            case 2: manage_teacher_accounts(); break;
            case 3: view_student_list(); break;
            case 4: view_delete_exams(); break;
            case 5: return;
            default: printf(COLOR_RED "Invalid choice.\n" COLOR_RESET);
        }
    }
}

void manage_teacher_accounts() {
    int ch;
    char username[20];
    FILE *fp;

    printf(COLOR_CYAN "\n1. Add Teacher\n2. Remove Teacher\nChoice: " COLOR_RESET);
    scanf("%d", &ch);

    if (ch == 1) {
        register_user("teachers.txt");
    } else if (ch == 2) {
        fp = fopen("teachers.txt", "r");
        FILE *temp = fopen("temp.txt", "w");
        if (!fp || !temp) {
            printf(COLOR_RED "File error.\n" COLOR_RESET); return;
        }

        printf(COLOR_YELLOW "Enter username to remove: " COLOR_RESET);
        scanf("%s", username);
        char file_user[20], file_pass[20];
        int found = 0;
        while (fscanf(fp, "%s %s", file_user, file_pass) != EOF) {
            if (strcmp(file_user, username) != 0)
                fprintf(temp, "%s %s\n", file_user, file_pass);
            else
                found = 1;
        }

        fclose(fp); fclose(temp);
        remove("teachers.txt");
        rename("temp.txt", "teachers.txt");

        if (found)
            printf(COLOR_GREEN "Teacher removed.\n" COLOR_RESET);
        else
            printf(COLOR_RED "Username not found.\n" COLOR_RESET);
    } else {
        printf(COLOR_RED "Invalid choice.\n" COLOR_RESET);
    }
}

void view_student_list() {
    FILE *fp1 = fopen("students.txt", "r");
    FILE *fp2 = fopen("profiles.txt", "r");

    if (!fp1 || !fp2) {
        printf(COLOR_RED "File error.\n" COLOR_RESET); return;
    }

    char username[20];
    printf(COLOR_GREEN "\n-- Student List --\n" COLOR_RESET);
    while (fscanf(fp1, "%s", username) != EOF) {
        printf("Username: %s\n", username);
        char file_user[20], name[30], semester[10], id[15];
        rewind(fp2);
        while (fscanf(fp2, "%s %s %s %s", file_user, name, semester, id) != EOF) {
            if (strcmp(file_user, username) == 0) {
                printf("  Name: %s, Semester: %s, ID: %s\n", name, semester, id);
                break;
            }
        }
    }

    fclose(fp1);
    fclose(fp2);
}

void view_delete_exams() {
    char course[20], filename[30];
    printf(COLOR_YELLOW "Enter course code to view/delete exam: " COLOR_RESET);
    scanf("%s", course);
    sprintf(filename, "exam_%s.txt", course);

    int choice;
    printf(COLOR_CYAN "\n1. View Exam Content\n2. Delete Exam\nChoice: " COLOR_RESET);
    scanf("%d", &choice);

    if (choice == 1) {
        FILE *fp = fopen(filename, "r");
        if (!fp) {
            printf(COLOR_RED "Exam not found.\n" COLOR_RESET); return;
        }
        char line[200];
        while (fgets(line, sizeof(line), fp)) {
            printf("%s", line);
        }
        fclose(fp);
    } else if (choice == 2) {
        if (remove(filename) == 0)
            printf(COLOR_GREEN "Exam deleted.\n" COLOR_RESET);
        else
            printf(COLOR_RED "Deletion failed.\n" COLOR_RESET);
    } else {
        printf(COLOR_RED "Invalid choice.\n" COLOR_RESET);
    }
}

void center(char *s, int n) {
    printf("%*s\n", (n + (int)strlen(s)) / 2, s);
}



