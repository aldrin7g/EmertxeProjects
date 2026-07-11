#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "modules.h"
#include "file.h"

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}

void listContacts(AddressBook *addressBook){
    AddressBook *ad = addressBook;
    // Sort contacts based on the chosen criteria
    //buble sort
    for(int i = 0; i < ad->contactCount-1; i++){
        for(int j = 0; j < ad->contactCount-i-1; j++){
            if(strcmp(ad->contacts[j].name, ad->contacts[j+1].name) > 0){
                //swap contacts
                Contact temp = ad->contacts[j];
                ad->contacts[j] = ad->contacts[j+1];
                ad->contacts[j+1] = temp;
            }
        }
    }
    printf("----------------------------------------------------------------------------\n");
    printf("|                               ADDRESS BOOK                               |\n");
    printf("****************************************************************************\n");
    printf("| %-3s | %-25s | %-10s | %-25s |\n","No", "Name", "Phone", "Email");
    printf("----------------------------------------------------------------------------\n");

    for(int i = 0; i < ad->contactCount; i++)
    {
        printf("| %-3d | %-25s | %-10s | %-25s |\n", 
            i+1, 
            ad->contacts[i].name, 
            ad->contacts[i].phone, 
            ad->contacts[i].email);
    }
    printf("----------------------------------------------------------------------------\n");
}

void createContact(AddressBook *addressBook){
	/* Define the logic to create a Contacts */

    // sizeof(addressBook);
    char name[20];
    inputName(name);

    char mobile[11];
    inputPhone(mobile, addressBook);

    char email[30];
    inputEmail(email);

    strcpy(addressBook->contacts[addressBook->contactCount].name, name);
    strcpy(addressBook->contacts[addressBook->contactCount].phone, mobile);
    strcpy(addressBook->contacts[addressBook->contactCount].email, email);

    addressBook->contactCount++;
}

int searchContact(AddressBook *addressBook){
    int sel= 0;
    while(1)
    {
        //promt (printf)  
        printf("***************************************\n");    
        printf("|             SEARCH MODE             |\n");
        printf("---------------------------------------\n");
        printf("|   1   |   %-25s |\n","SEARCH BY NAME ");
        printf("|   2   |   %-25s |\n","SEARCH BY PHONE");
        printf("|   3   |   %-25s |\n","SEARCH BY EMAIL");
        printf("---------------------------------------\n"); 
        printf("Enter your choice: ");

        //get a choice from user (scanf)
        scanf("%d",&sel);
        printf("\n");
        if(sel>=1 && sel<=3) 
            break;
        printf("Invalid input\n");
    }
    int index;

    switch(sel)
    {
        case 1:
            index = search_by_name(addressBook);
            break;
        case 2: 
            index = search_by_phone(addressBook);
            break;
        case 3: 
            index = search_by_email(addressBook);
            break;
    }
}

void editContact(AddressBook *addressBook){
    int choice;
	/* Define the logic for Editcontact */
    int index = searchContact(addressBook);
    if(index<0) return;

    // Ask the field 1. name  2. phone  3. email
    //choice
    while(1){
        printf("***************************************\n");    
        printf("|              EDIT MODE              |\n");
        printf("---------------------------------------\n");
        printf("|   1   |   %-25s |\n","EDIT NAME ");
        printf("|   2   |   %-25s |\n","EDIT PHONE");
        printf("|   3   |   %-25s |\n","EDIT EMAIL");
        printf("---------------------------------------\n"); 
        printf("Enter your choice: ");
        scanf("%d",&choice);

        if(choice>=1 && choice<=3)
            break;
        printf("Invalid Input\n");
    }
    printf("\n");

    switch(choice)
    {
        case 1:
            edit_name(addressBook, index);
            break;
        case 2: 
            edit_phone(addressBook, index);
            break;
        case 3: 
            edit_mail(addressBook, index);
            break;
    }
    printf("Contact edited successfully!\n");
}

void deleteContact(AddressBook *addressBook){
	/* Define the logic for deletecontact */
    int index = searchContact(addressBook);
    if(index<0) return;

    for(int i = index; i<addressBook->contactCount-1; i++){
        addressBook->contacts[i] = addressBook->contacts[i+1];
        addressBook->contactCount--;
    }
    printf("Contact deleted successfully!\n");
}

void saveAndExit(AddressBook *addressBook){
    saveContactsToFile(addressBook); // Save contacts to file
    exit(0);
}
