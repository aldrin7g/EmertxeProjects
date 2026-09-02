#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "contact.h"
#include "modules.h"

status isalphanum(char c){ //check if alpha-numeric character or not
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
        return valid;

    return invalid;
}

void displayContact(AddressBook *addressBook, short index){
    printf(C "***************************************\n" Rst);
    printf(C "|" Y "           CONTACT DETAILS           " C "|\n" Rst);
    printf(C "---------------------------------------\n" Rst);
    //printf("| S.No    | %-25d |\n",index + 1);
    printf(C "| " M "NAME    " C "| " W "%-25s" C " |\n" Rst, addressBook->contacts[index].name);
    printf(C "| " M "PHONE   " C "| " W "%-25s" C " |\n" Rst, addressBook->contacts[index].phone);
    printf(C "| " M "EMAIL   " C "| " W "%-25s" C " |\n" Rst, addressBook->contacts[index].email);
    printf(C "---------------------------------------\n\n" Rst); 
}

nameStatus validate_name(char *name){
    // LOOP till name[i] != '\0'
    //     check name[i] is not in the range of A to Z, a to z, ' ', '.'
    //         return 0; // invalid
    // END LOOP

    for(char i = 0; name[i]!='\0'; i++){
        if(!((name[i]>='A' && name[i]<='Z')||(name[i]>='a' && name[i]<='z')||(name[i]==' ')||(name[i]=='.')))
            return invalidName;
    }

    return validName; // valid.
}

phoneStatus validate_phone(char *phone, AddressBook *addressBook){
    //check the length
         // != 10  ==> return 0 (invalid)
    if(strlen(phone)!=10) 
            return invalidLength;

    // Loop till str[i] != '\0'
    //     check the str[i] is not in the rage '0' to '9'
    //         return 1; invalid
    for(char i = 0; phone[i]!='\0'; i++){
        if(!(phone[i]>='0' && phone[i]<='9'))
            return invalidPhone;
    }

    // check unique or not
    //     not => return 0;
    for(short i=0; i<addressBook->contactCount; i++){
        if(strcmp(phone,addressBook->contacts[i].phone)==0) 
            return duplicatePhone;
    }

    return validPhone; // valid;
}

emailStatus validate_email(char *str, AddressBook *addressBook){
     int atCount = 0, atPos = -1, dotPos = -1;
     int len = strlen(str);
     //min email: a@b.i
     if(len<5)
        return invalidEmail;

     for(char i = 0; i<len; i++){
        if(str[i] == '@'){
            ++atCount;
            atPos = i;
        }
        else if(str[i] == '.'){
            dotPos = i;
            if(i<len-1){
                if(str[i]==str[i+1])
                    return invalidEmail;
            }
        }
        else if(!(isalphanum(str[i]) || (str[i]=='-') || (str[i]=='_') || (str[i]=='+')))
            return invalidEmail;
     }
     if(atCount==0 || atCount>1)
        return invalidEmail;
     else if(atPos==0 || atPos==len-1)
        return invalidEmail;
     else if(dotPos<=atPos+1 || dotPos==len-1)
        return invalidEmail;

     for(short i = 0; i<addressBook->contactCount; i++){
        if(strcmp(addressBook->contacts[i].email,str) == 0)
            return duplicateEmail;
     }

     return validEmail;
}

void inputName(char *name){
    while(1)
    {
        printf(B "Enter the Name : " Rst);
        scanf(" %[^\n]", name);

        nameStatus status = validate_name(name);
        if(status == validName)
            break;
        else
            printf(R "Invalid Name!\n" Rst);
    }
}

void inputPhone(char *phone, AddressBook *addressBook){
    while(1)
    {
        printf(B "Enter the PhoneNo : " Rst);
        scanf(" %[^\n]", phone);
        phoneStatus status = validate_phone(phone, addressBook);

        if(status == validPhone)
            break;
        else if(status == invalidLength)
            printf(R "Invalid Contact Length!\n" Rst);
        else if(status == invalidPhone) 
            printf(R "Invalid Phone No!\n" Rst);      
        else if(status == duplicatePhone)
            printf(R "Duplicate Phone No!\n" Rst);  
    }
}

void inputEmail(char *email, AddressBook *addressBook){
    while(1){
        printf(B "Enter the Email : " Rst);
        scanf(" %[^\n]", email);
        emailStatus status = validate_email(email, addressBook);

        if(status == invalidEmail)
            printf(R "Invalid Email!\n" Rst);
        else if(status == validEmail)
            break; //valid email
        else if(status == duplicateEmail)
            printf(R "Email already exists!\n" Rst);
    }
}

short search_by_name(AddressBook *addressBook, char flag){
    char str[20]; 
    //Enter the name
    //Get the name
    printf(B "Enter a Name to Search: \n" Rst);
    scanf(" %[^\n]",str);
    char count = 0;
    short ind;
    short* dup= malloc(sizeof(short) * 100);

    //Loop till contactCount
    for(short i = 0; i<addressBook->contactCount; i++)
    {
        if(strcmp(str, addressBook->contacts[i].name) == 0)
        {
            ind = i;
            dup[count] = ind;
            count++;
        }
    }
    printf("\n");
    // count ==> 0 ==> No match ==> return -1;
    if(count==0){
        printf(R "No Record's Found!\n\n" Rst);
        return -1;
    }

    // count ==> 1 ==> return ind
    else if(count==1){
        printf(G "Search Found at S.No.: %d!\n" Rst, ind + 1);
        displayContact(addressBook, ind);
        return ind;
    }

    // count > 1 ==> return search_by_phone(addressBook);
    else if(count>1){ 
        // printf("Multiple Names Found! Search by PhoneNo.\n\n");
        // return search_by_phone(addressBook);
        if(flag==0){
            printf(R "Multiple Names Found!\n" Rst);
            for(char i = 0; i<count; i++){
                printf(M"Contact %d:\n"Rst,i+1);
                displayContact(addressBook, dup[i]);
            }
            return -1;  
        }
        else{
            printf(G "Multiple Names Found! Select the contact\n" Rst);
            for(char i = 0; i<count; i++){
                printf(M "Contact %d:\n" Rst,i+1);
                displayContact(addressBook, dup[i]);
            }
            while(1){
                printf(B "\nEnter your choice 1-%d: " Rst,count);
                short choice, val;
                scanf("%hd",&choice);
                val = dup[choice-1];
                free(dup);

                if(choice>=1 && choice<=count)
                    return val;
                printf(R "Invalid Choice!\n" Rst);
            }
        }
    }
}

short search_by_phone(AddressBook *addressBook){
    char str[11];
    //Enter the phone
    //Get the phone
    printf(B "Enter a PhoneNo to Search: \n" Rst);
    scanf("%s",str);
    printf("\n");
    //Loop till contactCount
    for(short ind = 0; ind<addressBook->contactCount; ind++)
    {
        if(strcmp(str, addressBook->contacts[ind].phone) == 0)
        {
            //print the contact
            printf(G "Search Found at S.No.: %d!\n" Rst, ind + 1);
            displayContact(addressBook,ind);
            return ind;
        }
    }

    // Not found
    printf(R "No Record's Found!\n\n" Rst);
    return -1;
}

short search_by_email(AddressBook *addressBook){
    char str[30];
    //Enter the Email
    printf(B "Enter an Email to Search: \n" Rst);
    scanf("%s",str);
    printf("\n");
    //Loop till contactCount
    for(short ind = 0; ind<addressBook->contactCount; ind++)
    {
        if(strcmp(str, addressBook->contacts[ind].email) == 0)
        {
            //print the contact
            printf(G "Search Found at S.No.: %d!\n" Rst, ind + 1);
            displayContact(addressBook,ind);
            return ind;
        }
    }

    // Not found
    printf(R "No Record's Found!\n\n" Rst);
    return -1;
}

void edit_name(AddressBook *addressBook, short index){
    // get the name
    char name[20];

    inputName(name);

    strcpy(addressBook->contacts[index].name, name);
}

void edit_phone(AddressBook *addressBook, short index){
    char mobile[11];

    inputPhone(mobile, addressBook);

    strcpy(addressBook->contacts[index].phone, mobile);
}

void edit_mail(AddressBook *addressBook, short index){
    char email[30];

    inputEmail(email, addressBook);

    strcpy(addressBook->contacts[index].email, email);
}