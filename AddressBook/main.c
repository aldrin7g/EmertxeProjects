#include <stdio.h>
#include <stdlib.h>
#include "contact.h"
#include "modules.h"

int main() 
{
    short choice;
    AddressBook addressBook;
    initialize(&addressBook); // Initialize the address book
    
    //addressBook.contactCount = 0;

    while(1) 
    {
        printf("\n\n" C "***************************************\n" Rst);
        printf(C "|" Y "           ADDRESS BOOK MENU         " C "|\n" Rst);
        printf(C "---------------------------------------\n" Rst);
        printf(C "|   1   | " O "  %-25s" C " |\n" Rst, "CREATE CONTACT");
        printf(C "|   2   | " O "  %-25s" C " |\n" Rst, "SEARCH CONTACT");
        printf(C "|   3   | " O "  %-25s" C " |\n" Rst, "EDIT CONTACT");
        printf(C "|   4   | " O "  %-25s" C " |\n" Rst, "DELETE CONTACT");
        printf(C "|   5   | " O "  %-25s" C " |\n" Rst, "LIST ALL CONTACTS");
        printf(C "|   6   | " O "  %-25s" C " |\n" Rst, "SAVE AND EXIT");
        printf(C "|   7   | " O "  %-25s" C " |\n" Rst, "EXIT");
        printf(C "---------------------------------------\n" Rst);
        printf(B"Enter your choice: "Rst);
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
                printf(G "Saving and Exiting...\n\n" Rst);
                saveAndExit(&addressBook);
                break;
            case 7:
                printf(G "Exiting without Saving!\n\n" Rst);
                exit(0);
            
            default:
                printf(R "Invalid choice. Please try again.\n" Rst);
        }
    }
    
    return 0;
}
