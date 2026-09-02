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
    for(short i = 0; i < ad->contactCount-1; i++){
        for(short j = 0; j < ad->contactCount-i-1; j++){
            if(strcmp(ad->contacts[j].name, ad->contacts[j+1].name) > 0){
                //swap contacts
                Contact temp = ad->contacts[j];
                ad->contacts[j] = ad->contacts[j+1];
                ad->contacts[j+1] = temp;
            }
        }
    }
    printf(C "-----------------------------------------------------------------------------\n" Rst);
    printf(C "|" Y "                               ADDRESS BOOK                                " C "|\n" Rst);
    printf(C "*****************************************************************************\n" Rst);
    printf(C "| " M "%-3s" C " | " M "%-25s" C " | " M "%-10s" C " | " M "%-25s" C " |\n" Rst,
        "S.No", "Name", "Phone", "Email");
    printf(C "-----------------------------------------------------------------------------\n" Rst);
    for(short i = 0; i < ad->contactCount; i++)
    {
        printf(C "|  " Rst "%-3d | %-25s | %-10s | %-25s " C "|\n" Rst,
            i+1, ad->contacts[i].name, ad->contacts[i].phone, ad->contacts[i].email);
    }
    printf(C "-----------------------------------------------------------------------------\n" Rst);
}

void createContact(AddressBook *addressBook){
	/* Define the logic to create a Contacts */

    // sizeof(addressBook);
    char name[20];
    inputName(name);

    char phone[11];
    inputPhone(phone, addressBook);

    char email[30];
    inputEmail(email, addressBook);

    strcpy(addressBook->contacts[addressBook->contactCount].name, name);
    strcpy(addressBook->contacts[addressBook->contactCount].phone, phone);
    strcpy(addressBook->contacts[addressBook->contactCount].email, email);
    
    printf(G "\nContact created successfully!\n" Rst);
    displayContact(addressBook, addressBook->contactCount);

    addressBook->contactCount++;
}

short searchContact(AddressBook *addressBook, char flag){
    short sel= 0;
    while(1)
    {
        //promt (printf)  
        printf(C "***************************************\n" Rst);
        printf(C "|" Y "             SEARCH MODE             " C "|\n" Rst);
        printf(C "---------------------------------------\n" Rst);
        printf(C "|   1   | " M "  %-25s" C " |\n" Rst, "SEARCH BY NAME ");
        printf(C "|   2   | " M "  %-25s" C " |\n" Rst, "SEARCH BY PHONE");
        printf(C "|   3   | " M "  %-25s" C " |\n" Rst, "SEARCH BY EMAIL");
        printf(C "---------------------------------------\n" Rst);
        printf(B "Enter your choice: " Rst);

        //get a choice from user (scanf)
        scanf("%hd",&sel);
        printf("\n");
        if(sel>=1 && sel<=3) 
            break;
        while(getc(stdin)!='\n');
        printf(R "Invalid input!\n\n" Rst);
    }
    short index;

    switch(sel)
    {
        case 1:
            index = search_by_name(addressBook, flag);
            break;
        case 2: 
            index = search_by_phone(addressBook);
            break;
        case 3: 
            index = search_by_email(addressBook);
            break;
    }
    return index;
}

void editContact(AddressBook *addressBook){
    short choice;
	/* Define the logic for Editcontact */
    short index = searchContact(addressBook, 1);
    if(index<0) return;

    // Ask the field 1. name  2. phone  3. email
    //choice
    while(1){
        printf(C "***************************************\n" Rst);
        printf(C "|" Y "              EDIT MODE              " C "|\n" Rst);
        printf(C "---------------------------------------\n" Rst);
        printf(C "|   1   | " M "  %-25s" C " |\n" Rst, "EDIT NAME ");
        printf(C "|   2   | " M "  %-25s" C " |\n" Rst, "EDIT PHONE");
        printf(C "|   3   | " M "  %-25s" C " |\n" Rst, "EDIT EMAIL");
        printf(C "---------------------------------------\n" Rst);
        printf(B "Enter your choice: " Rst);
        scanf("%hd",&choice);

        if(choice>=1 && choice<=3)
            break;
        printf(R "Invalid Input\n" Rst);
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

    printf(G "\nContact edited successfully!\n" Rst);
    displayContact(addressBook, index);
}

void deleteContact(AddressBook *addressBook){
	/* Define the logic for deletecontact */
    short index = searchContact(addressBook, 1);
    if(index<0) return;

    for(short i = index; i<addressBook->contactCount-1; i++)
        addressBook->contacts[i] = addressBook->contacts[i+1];

    addressBook->contactCount--;    
    printf(G "Contact deleted successfully!\n" Rst);
}

void saveAndExit(AddressBook *addressBook){
    saveContactsToFile(addressBook); // Save contacts to file
    exit(0);
}
