#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "readfile.h"
#define MAXNAME 64

extern FILE *file; //variable used to read data file
struct person  // Data structure used to store employees into the DB
{
    char fname[MAXNAME];
    char lname[MAXNAME];
    int id;
    int salary;
};
struct person db[1024]; //Create array of person struct for DB
int employee_counter = 0; //Used to keep track of how many employees are in the DB
char term; // used for validation checks

/* The below function is used to print out the DB */        
void PrintDB (struct person arr[], int n)
{
    int i;

    printf("\nName\t\t\t\tSALARY\t   ID\n");
    printf("----------------------------------------------\n");
    for (i = 0; i < n; i++)
    {
        printf("%-15s\t%-15s\t%d\t%d\n", arr[i].fname, arr[i].lname, arr[i].salary, arr[i].id);
    }
    printf("----------------------------------------------\n");
    printf(" Number of Employees (%d)\n", n);
}

/* The below function looks up a user from the DB via their Employee ID using binary search. 
If the user is not found it returns a "User not found" message.
A recursive binary search function. It returns location of x in given array arr[l..r] is present, 
otherwise -1 
*/
int LookupID(struct person arr[], int l, int r, int x) 
{ 
    if (r >= l) { 
        int mid = l + (r - l) / 2; 
  
        // If the element is present at the middle 
        // itself 
        if (arr[mid].id == x) 
            return mid; 
  
        // If element is smaller than mid, then 
        // it can only be present in left subarray 
        if (arr[mid].id > x) 
            return LookupID(arr, l, mid - 1, x); 
  
        // Else the element can only be present 
        // in right subarray 
        return LookupID(arr, mid + 1, r, x); 
    } 
  
    // We reach here when element is not 
    // present in array 
    return -1; 
} 

/* The below function looks up a user from the DB via their last name using a linear search. 
This function will return only one employee and it is the first one listed in the DB via 
Employee ID. If the user is not found it returns a "User not found" message */
int LookupLName (struct person arr[], int m, char* name)
{
    int i;
    for (i = 0; i < m; i++)
    {
        if (strcmp(arr[i].lname, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

/* Adds an employee to the DB. Prompts user for employee details and validates that the info
is in the correct format */
int AddEmployee(struct person arr[])
{
    char newfname[MAXNAME];
    char newlname[MAXNAME];
    int newsalary;
    int confirm;

    printf("\nEnter the first name of the employee: ");
    scanf("%s", newfname);
    if (strlen(newfname) > 64)
    {
        printf("Invalid entry. Please try again.\n");
        return 0;
    }
    printf("Enter the last name of the employee: ");
    scanf("%s", newlname);
    if (strlen(newfname) > 64)
    {
        printf("Invalid entry. Please try again.\n");
        return 0;
    }
    printf("Enter employee's salary (30000 to 150000): ");
    scanf("%d", &newsalary);
    if (newsalary < 30000 || newsalary > 150000)
    {
        printf("Invalid entry. Please try again.\n");
        return 0;
    }
    printf("\nDo you want to add the following employee to the DB?\n");
    printf("\t%s %s, \tSalary: %d\n", newfname, newlname, newsalary);
    printf("\nEnter 1 for yes, 0 for no: ");
    scanf("%d", &confirm);

    if (confirm == 1)
    {
        strcpy(db[employee_counter].fname, newfname);
        strcpy(db[employee_counter].lname, newlname);
        db[employee_counter].salary = newsalary;
        db[employee_counter].id = db[employee_counter -1].id + 1;
        employee_counter++;
        printf("New employee added!\n");
    }
    return 1;
}

/* The below function loads in the data from the data file and creates the DB */
void CreateDB(char* datafile)
{
    if (open_file(datafile) == 0)
    {
        char buffer[135];
        int ret;
        while (fgets(buffer, sizeof(buffer), file)) 
        {
            ret = read_int(buffer, 1, &db[employee_counter].id);
            ret = read_int(buffer, 2, &db[employee_counter].salary);
            ret = read_string(buffer, 1, &db[employee_counter].fname);
            ret = read_string(buffer, 2, &db[employee_counter].lname);
            employee_counter++; 
        }

        if (feof(file)) // hit end of file
        {
            close_file(file);
        } 
    }
}

// Used for sorting
void Swap(struct person* xp, struct person* yp) 
{ 
    struct person temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
  
// Function to perform Selection Sort on Employee ID in ascending order
void SelectionSort(struct person arr[], int n) 
{ 
    int i, j, min_idx; 
  
    // One by one move boundary of unsorted subarray 
    for (i = 0; i < n - 1; i++) { 
  
        // Find the minimum element in unsorted array 
        min_idx = i; 
        for (j = i + 1; j < n; j++) 
            if (arr[j].id < arr[min_idx].id) 
                min_idx = j; 
  
        // Swap the found minimum element 
        // with the first element 
        Swap(&arr[min_idx], &arr[i]); 
    } 
} 

// Function to perform Selection Sort on Employee Salary in descending order
void SelectionSortSalary(struct person arr[], int n) 
{ 
    int i, j, min_idx; 
  
    // One by one move boundary of unsorted subarray 
    for (i = 0; i < n - 1; i++) { 
  
        // Find the minimum element in unsorted array 
        min_idx = i; 
        for (j = i + 1; j < n; j++) 
            if (arr[j].salary > arr[min_idx].salary) 
                min_idx = j; 
  
        // Swap the found minimum element 
        // with the first element 
        Swap(&arr[min_idx], &arr[i]); 
    } 
} 

// Function removes employee from DB and reduces the DB size if employee id is in the DB.
void RemoveEmployee(struct person arr[], int n)
{
    int i;
    int choice;

    printf("\nName\t\t\t\tSALARY\t   ID\n");
    printf("----------------------------------------------\n");
    printf("%-15s\t%-15s\t%d\t%d\n", arr[n].fname, arr[n].lname, arr[n].salary, arr[n].id);
    printf("----------------------------------------------\n");
    printf("Would you like to delete this employee? Enter 1 for Yes, or 0 for No.\n");
    printf("Enter your choice: ");
    scanf("%d", &choice); 
    
    if (choice == 1)
    {
            for (i = n; i < employee_counter; i++)
            {
                arr[i] = arr[i+1];
            }
            employee_counter--;
    }
    else if (choice == 0)
    {
        printf("\nCancelling Action.\n");
    }
    else
    {
        printf("\nInvalid entry. Cancelling action\n");
    }
}

/*This function prompts if they want to change the employee data from an employee id entered.
The user can select a specific field or all of them to update. If the employee ID changes then the 
DB is re-sorted*/
int UpdateEmployee(struct person arr[], int x)
{
    int option;
    char fname[MAXNAME], lname[MAXNAME];
    int salaryNew, idNew;
    printf("\nName\t\t\t\tSALARY\t   ID\n");
    printf("----------------------------------------------\n");
    printf("%-15s\t%-15s\t%d\t%d\n", arr[x].fname, arr[x].lname, arr[x].salary, arr[x].id);
    printf("----------------------------------------------\n");
    printf("Would you like to edit the information of this employee? Enter 1 for Yes, or 0 for No.\n");
    printf("Enter your choice: ");
    scanf("%d", &option); 

    if (option == 0)
    {
        printf("\nCancelling action\n");
        return 0;
    }
    if (option == 1)
    {
        printf("\nWhich information would you like to change?\n");
        printf("\t(1) First Name\n");
        printf("\t(2) Last Name\n");
        printf("\t(3) Salary\n");
        printf("\t(4) Employee ID\n");
        printf("\t(5) All of the Employee's Data\n");
        printf("Enter your choice: ");
        scanf("%d", &option); 

        switch (option) {
            case 1:  
                printf("\n Please enter the updated first name: "); 
                scanf("%s", fname);      

                printf("\nName\t\t\t\tSALARY\t   ID\n");
                printf("----------------------------------------------\n");
                printf("%-15s\t%-15s\t%d\t%d\n", fname, arr[x].lname, arr[x].salary, arr[x].id);
                printf("----------------------------------------------\n");
                printf("Is this information correct? Enter 1 for Yes, or 0 for No.\n");
                printf("Enter your choice: ");
                scanf("%d", &option); 

                if (option == 1)
                {
                    printf("\nUpdate Complete.\n");
                    strcpy(arr[x].fname, fname);
                }
                else if (option == 0)
                {
                    printf("\nUpdate Cancelled.\n");
                }
                else
                {
                    printf("\nInvalid entry. Cancelling action\n");
                }
                break;
            case 2:
                printf("\n Please enter the updated last name: "); 
                scanf("%s", lname);      

                printf("\nName\t\t\t\tSALARY\t   ID\n");
                printf("----------------------------------------------\n");
                printf("%-15s\t%-15s\t%d\t%d\n", arr[x].fname, lname, arr[x].salary, arr[x].id);
                printf("----------------------------------------------\n");
                printf("Is this information correct? Enter 1 for Yes, or 0 for No.\n");
                printf("Enter your choice: ");
                scanf("%d", &option); 

                if (option == 1)
                {
                    printf("\nUpdate Complete.\n");
                    strcpy(arr[x].lname, lname);
                }
                else if (option == 0)
                {
                    printf("\nUpdate Cancelled.\n");
                }
                else
                {
                    printf("\nInvalid entry. Cancelling action\n");
                }
                break;
            case 3:                 
                printf("\n Please enter the updated salary: "); 
                scanf("%d", &salaryNew);      

                printf("\nName\t\t\t\tSALARY\t   ID\n");
                printf("----------------------------------------------\n");
                printf("%-15s\t%-15s\t%d\t%d\n", arr[x].fname, arr[x].lname, salaryNew, arr[x].id);
                printf("----------------------------------------------\n");
                printf("Is this information correct? Enter 1 for Yes, or 0 for No.\n");
                printf("Enter your choice: ");
                scanf("%d", &option); 

                if (option == 1)
                {
                    printf("\nUpdate Complete.\n");
                    arr[x].salary = salaryNew;
                }
                else if (option == 0)
                {
                    printf("\nUpdate Cancelled.\n");
                }
                else
                {
                    printf("\nInvalid entry. Cancelling action\n");
                }
                break;
            case 4:
                printf("\n Please enter the updated employee ID: "); 
                scanf("%d", &idNew);      

                printf("\nName\t\t\t\tSALARY\t   ID\n");
                printf("----------------------------------------------\n");
                printf("%-15s\t%-15s\t%d\t%d\n", arr[x].fname, arr[x].lname, arr[x].salary, idNew);
                printf("----------------------------------------------\n");
                printf("Is this information correct? Enter 1 for Yes, or 0 for No.\n");
                printf("Enter your choice: ");
                scanf("%d", &option); 

                if (option == 1)
                {
                    printf("\nUpdate Complete.\n");
                    arr[x].id = idNew;
                }
                else if (option == 0)
                {
                    printf("\nUpdate Cancelled.\n");
                }
                else
                {
                    printf("\nInvalid entry. Cancelling action\n");
                }             
                SelectionSort(arr, employee_counter); // Resorts the DB by Employee ID      
                break;
            case 5:
                printf("\n Please enter the updated first name: "); 
                scanf("%s", fname);      
                printf("\n Please enter the updated last name: "); 
                scanf("%s", lname);
                printf("\n Please enter the updated salary: "); 
                scanf("%d", &salaryNew);
                printf("\n Please enter the updated employee ID: "); 
                scanf("%d", &idNew); 

                printf("\nName\t\t\t\tSALARY\t   ID\n");
                printf("----------------------------------------------\n");
                printf("%-15s\t%-15s\t%d\t%d\n", fname, lname, salaryNew, idNew);
                printf("----------------------------------------------\n");
                printf("Is this information correct? Enter 1 for Yes, or 0 for No.\n");
                printf("Enter your choice: ");
                scanf("%d", &option); 

                if (option == 1)
                {
                    printf("\nUpdate Complete.\n");
                    strcpy(arr[x].fname, fname);
                    strcpy(arr[x].lname, lname);
                    arr[x].salary = salaryNew;
                    arr[x].id = idNew;
                }
                else if (option == 0)
                {
                    printf("\nUpdate Cancelled.\n");
                }
                else
                {
                    printf("\nInvalid entry. Cancelling action\n");
                }   
                SelectionSort(arr, employee_counter); // Resorts the DB by Employee ID      
                break;
        }
    }
    else
    {
        printf("\nInvalid entry. Cancelling action\n");
        return 0;
    }

}

/* Function that creates a new db of all the employee sorted in descending order of salary
prints off the X amount of employees with the highest salary. Asks user for X value. */
void PrintHighestSalary(struct person arr[], int n)
{
    struct person salaryDB[n];
    int i;
    int choice;
    
    for (i = 0; i < n; i++)
    {
        salaryDB[i] = arr[i];
    }

    SelectionSortSalary(salaryDB, n);
    printf("\nPlease enter how many employees you want to view with the highest salaries\n");
    printf("Enter your choice: ");

    scanf("%d", &choice); //Read user choice for DB menu
    if (choice > n)
    {
        choice = n;
    }
    PrintDB(salaryDB, choice);

}

/* The below function looks up a user from the DB via their last name using a linear search(case insensitive). 
This function will return all employees with the last name used for the search.
If the user is not found it returns a "User not found" message */
int LookupLName2 (struct person arr[], int n, char* name)
{
    int i;
    int check =-1;
    printf("----------------------------------------------\n");
    for (i = 0; i < n; i++)
    {
        if (strcasecmp(arr[i].lname, name) == 0)
        {
            printf("%-15s\t%-15s\t%d\t%d\n", db[i].fname,db[i].lname,db[i].salary, db[i].id);
            check =0;
        }
    }
    printf("----------------------------------------------\n");
    return check;
}

/* Main function that displays the DB actions menu to the user until they choose to close out of the program. */
int main(int argc, char *argv[])
{
    if ( argc != 2 ) /* argc should be 2 for correct execution */
    {
        /* We print argv[0] assuming it is the program name */
        printf( "usage: %s filename\n", argv[0] );
    }
    else
    {   
        CreateDB(argv[1]); // Calls the function above to create the DB from the input data file       
        SelectionSort(db, employee_counter); // Sorts the DB by Employee ID

        int choice; // User's menu selection
        int x = 0; //employee location in DB
        int target_ID; //target employee ID to search
        char target_name[MAXNAME]; //target name to search                 
        
        //Continue to display the DB menu until the user selects option '5' to quit
        while (1) {
            printf("\nEmployee DB Menu:\n");
            printf("----------------------------------\n");
            printf("\t(1) Print the Database\n");
            printf("\t(2) Lookup by ID\n");
            printf("\t(3) Lookup by Last Name\n");
            printf("\t(4) Add an Employee\n");
            printf("\t(5) Quit\n");
            printf("\t(6) Remove an Employee\n");
            printf("\t(7) Update Employee Record\n");
            printf("\t(8) Print Employee(s) with Highest Salary\n");
            printf("\t(9) Find all Employees with Entered Last Name\n");
            printf("----------------------------------\n");
            printf("Enter your choice: ");

            scanf("%d", &choice); //Read user choice for DB menu

            //Select the appropriate function based on user selection.
            switch (choice) {
                case 1:            
                    PrintDB(db, employee_counter);
                    break;
                case 2: ;
                    int target_ID; //target employee ID to search
                    printf("\nPlease enter an employee ID: ");
                    scanf("%d", &target_ID);          
                    x = LookupID(db, 0, employee_counter, target_ID);
                    if (x == -1)
                    {
                        printf("\nUser with employee ID '%d' not found\n", target_ID);
                    }
                    else
                    {
                        printf("\nName\t\t\t\tSALARY\t   ID\n");
                        printf("----------------------------------------------\n");
                        printf("%-15s\t%-15s\t%d\t%d\n", db[x].fname,db[x].lname,db[x].salary, db[x].id);
                        printf("----------------------------------------------\n");
                    }
                    
                    break;
                case 3:
                    printf("\nPlease enter an employee's last name: ");
                    scanf("%s", target_name); 
                    x = LookupLName(db, employee_counter, target_name);
                    if (x == -1)
                    {
                        printf("\nUser with last name '%s' not found\n", target_name);
                    }
                    else
                    {
                        printf("\nName\t\t\t\tSALARY\t   ID\n");
                        printf("----------------------------------------------\n");
                        printf("%-15s\t%-15s\t%d\t%d\n", db[x].fname,db[x].lname,db[x].salary, db[x].id);
                        printf("----------------------------------------------\n");
                    }                    
                    break;
                case 4:        
                    AddEmployee(db);
                    break;
                case 5:
                    printf("\ngoodbye!\n\n");
                    return 0;
                    break;
                case 6:
                    printf("\nPlease enter an employee ID: ");
                    scanf("%d", &target_ID);          
                    x = LookupID(db, 0, employee_counter, target_ID);
                    if (x == -1)
                    {
                        printf("\nUser with employee ID '%d' not found\n", target_ID);
                    }
                    else
                    {
                        RemoveEmployee(db, x);
                    }                   
                    break;
                case 7:
                    printf("\nPlease enter an employee ID: ");
                    scanf("%d", &target_ID);          
                    x = LookupID(db, 0, employee_counter, target_ID);
                    if (x == -1)
                    {
                        printf("\nUser with employee ID '%d' not found\n", target_ID);
                    }
                    else
                    {
                        x = UpdateEmployee(db, x); // this function returns an int that isn't used  
                    }       
                    break;
                case 8:
                    PrintHighestSalary(db, employee_counter);
                    break;
                case 9:        
                    printf("\nPlease enter an employee's last name: ");
                    scanf("%s", target_name); 
                    x = LookupLName2(db, employee_counter, target_name);
                    if (x == -1)
                    {
                        printf("\nUser with last name '%s' not found\n", target_name);
                    }                  
                    break;
                default:            
                    printf( "\nInvalid input. Please try again.\n" );
                    break;
            }
        }
    }
}