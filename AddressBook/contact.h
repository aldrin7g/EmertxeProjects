#ifndef CONTACT_H
#define CONTACT_H

typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;

typedef struct {
    Contact contacts[100];
    int contactCount;
} AddressBook;

void initialize(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);
void createContact(AddressBook *addressBook);
short searchContact(AddressBook *addressBook, char flag);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);

void saveAndExit(AddressBook *addressBook);

#endif
