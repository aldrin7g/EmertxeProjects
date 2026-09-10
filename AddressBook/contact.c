#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "modules.h"
#include "file.h"

// Function to initialize the address book
void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    
    // Load contacts from file during initialization
    loadContactsFromFile(addressBook);
}

// Function to display the main menu
void listContacts(AddressBook *addressBook){
    AddressBook *ad = addressBook;
    // Sort contacts using bubble sort based on name before displaying
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
    // Display the contacts in a formatted table
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

// Function to create a new contact
void createContact(AddressBook *addressBook){
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

// Function to search for a contact based on the chosen criteria
short searchContact(AddressBook *addressBook, char flag){
    short sel= 0;
    while(1)
    {
        // Display the search menu 
        printf(C "***************************************\n" Rst);
        printf(C "|" Y "             SEARCH MODE             " C "|\n" Rst);
        printf(C "---------------------------------------\n" Rst);
        printf(C "|   1   | " M "  %-25s" C " |\n" Rst, "SEARCH BY NAME ");
        printf(C "|   2   | " M "  %-25s" C " |\n" Rst, "SEARCH BY PHONE");
        printf(C "|   3   | " M "  %-25s" C " |\n" Rst, "SEARCH BY EMAIL");
        printf(C "---------------------------------------\n" Rst);
        printf(B "Enter your choice: " Rst);

        // Get user input for the search criteria
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

// Function to edit an existing contact
void editContact(AddressBook *addressBook){
    short choice;
    short index = searchContact(addressBook, 1);
    if(index<0) return; // If the contact is not found, return

    // Display the edit menu and get user input for the field to edit
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

// Function to delete an existing contact
void deleteContact(AddressBook *addressBook){
    short index = searchContact(addressBook, 1);
    if(index<0) return; // If the contact is not found, return

    // Shift all contacts after the deleted contact to the left
    for(short i = index; i<addressBook->contactCount-1; i++)
        addressBook->contacts[i] = addressBook->contacts[i+1];

    addressBook->contactCount--;    
    printf(G "Contact deleted successfully!\n" Rst);
}

// Function to save contacts to file and exit the program
void saveAndExit(AddressBook *addressBook){
    saveContactsToFile(addressBook);
    exit(0);
}