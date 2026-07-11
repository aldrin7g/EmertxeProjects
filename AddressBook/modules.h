#ifndef MODULES_H
#define MODULES_H

int validate_name(char *name);
char validate_phone(char *str, AddressBook *addressBook);
int validate_email(char *str);
void inputName(char *name);
void inputPhone(char *mobile, AddressBook *addressBook);
void inputEmail(char *email);
int search_by_name(AddressBook *addressBook);
int search_by_phone(AddressBook *addressBook);
int search_by_email(AddressBook *AddressBook);
void edit_name(AddressBook *addressBook, int index);
void edit_phone(AddressBook *addressBook, int index);
void edit_mail(AddressBook *addressBook, int index);

#endif