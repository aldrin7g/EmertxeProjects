#ifndef MODULES_H
#define MODULES_H

typedef enum{
    invalidName,
    validName
}nameStatus;

typedef enum{
   validPhone,
   invalidLength,
   invalidPhone,
   duplicatePhone
}phoneStatus;

typedef enum{
    invalidEmail,
    validEmail,
    duplicateEmail
}emailStatus;

typedef enum{
    invalid,
    valid
}status;

status isalphanum(char c);
void displayContact(AddressBook *addressBook, short index);
nameStatus validate_name(char *name);
phoneStatus validate_phone(char *str, AddressBook *addressBook);
emailStatus validate_email(char *str, AddressBook *addressBook);
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