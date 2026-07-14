#ifndef MODULES_H
#define MODULES_H

char isalphanum(char c);
void displayContact(AddressBook *addressBook, short index);
char validate_name(char *name);
char validate_phone(char *str, AddressBook *addressBook);
char validate_email(char *str, AddressBook *addressBook);
void inputName(char *name);
void inputPhone(char *mobile, AddressBook *addressBook);
void inputEmail(char *email, AddressBook *addressBook);
short search_by_name(AddressBook *addressBook, char flag);
short search_by_phone(AddressBook *addressBook);
short search_by_email(AddressBook *AddressBook);
void edit_name(AddressBook *addressBook, short index);
void edit_phone(AddressBook *addressBook, short index);
void edit_mail(AddressBook *addressBook, short index);

#endif