#include <stdio.h>
#include <stdlib.h>
#include "contact.h"

int main() 
{
    short choice;
    AddressBook addressBook;
    initialize(&addressBook); // Initialize the address book
    
    //addressBook.contactCount = 0;

    while(1) 
    {
        printf("\n\n***************************************\n");    
        printf("|           ADDRESS BOOK MENU         |\n");
        printf("---------------------------------------\n");
        printf("|   1   |   %-25s |\n","CREATE CONTACT");
        printf("|   2   |   %-25s |\n","SEARCH CONTACT");
        printf("|   3   |   %-25s |\n","EDIT CONTACT");
        printf("|   4   |   %-25s |\n","DELETE CONTACT");
        printf("|   5   |   %-25s |\n","LIST ALL CONTACTS");
        printf("|   6   |   %-25s |\n","SAVE AND EXIT");
        printf("|   7   |   %-25s |\n","EXIT");
        printf("---------------------------------------\n"); 
        printf("Enter your choice: ");
        scanf("%hd", &choice);
        printf("\n");
        
        switch (choice) 
        {
            case 1:
                createContact(&addressBook);
                break;
            case 2:
                searchContact(&addressBook, 0);
                break;
            case 3:
                editContact(&addressBook);
                break;
            case 4:
                deleteContact(&addressBook);
                break;
            case 5:          
                listContacts(&addressBook);
                break;
            case 6:
                printf("Saving and Exiting...\n\n");
                saveAndExit(&addressBook);
                break;
            case 7:
                printf("Exiting without Saving!\n\n");
                exit(0);
            
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}
