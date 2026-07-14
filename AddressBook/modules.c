#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "contact.h"
#include "modules.h"

char isalphanum(char c){
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
        return 1;

    return 0;
}

void displayContact(AddressBook *addressBook, short index){
    printf("***************************************\n");    
    printf("|           CONTACT DETAILS           |\n");
    printf("---------------------------------------\n");
    //printf("| S.No    | %-25d |\n",index + 1);
    printf("| NAME    | %-25s |\n",addressBook->contacts[index].name);
    printf("| PHONE   | %-25s |\n",addressBook->contacts[index].phone);
    printf("| EMAIL   | %-25s |\n",addressBook->contacts[index].email);
    printf("---------------------------------------\n\n");  
}

char validate_name(char *name){
    // LOOP till name[i] != '\0'
    //     check name[i] is not in the range of A to Z, a to z, ' ', '.'
    //         return 0; // invalid
    // END LOOP

    for(char i = 0; name[i]!='\0'; i++){
        if(!((name[i]>='A' && name[i]<='Z')||(name[i]>='a' && name[i]<='z')||(name[i]==' ')||(name[i]=='.')))
            return 0;
    }

    return 1; // valid.
}

char validate_phone(char *phone, AddressBook *addressBook){
    //check the length
         // != 10  ==> return 0 (invalid)
    if(strlen(phone)!=10) 
            return 1;

    // Loop till str[i] != '\0'
    //     check the str[i] is not in the rage '0' to '9'
    //         return 1; invalid
    for(char i = 0; phone[i]!='\0'; i++){
        if(!(phone[i]>='0' && phone[i]<='9'))
            return 2;
    }

    // check unique or not
    //     not => return 0;
    for(short i=0; i<addressBook->contactCount; i++){
        if(strcmp(phone,addressBook->contacts[i].phone)==0) 
            return 3;
    }

    return 0; // valid;
}

char validate_email(char *str, AddressBook *addressBook){
     int atCount = 0, atPos = -1, dotPos = -1;
     int len = strlen(str);
     //min email: a@b.i
     if(len<5)
        return 0;

     for(char i = 0; i<len; i++){
        if(str[i] == '@'){
            ++atCount;
            atPos = i;
        }
        else if(str[i] == '.'){
            dotPos = i;
            if(i<len-1){
                if(str[i]==str[i+1])
                    return 0;
            }
        }
        else if(!(isalphanum(str[i]) || (str[i]=='-') || (str[i]=='_') || (str[i]=='+')))
            return 0;
     }
     if(atCount==0 || atCount>1)
        return 0;
     else if(atPos==0 || atPos==len-1)
        return 0;
     else if(dotPos<=atPos+1 || dotPos==len-1)
        return 0;

     for(short i = 0; i<addressBook->contactCount; i++){
        if(strcmp(addressBook->contacts[i].email,str) == 0)
            return 2;
     }

     return 1;
}

void inputName(char *name){
    while(1)
    {
        printf("Enter the Name : ");
        scanf(" %[^\n]", name);

        if(validate_name(name))
            break;
        else
            printf("Invalid Name!\n");
    }
}

void inputPhone(char *phone, AddressBook *addressBook){
    while(1)
    {
        printf("Enter the PhoneNo : ");
        scanf(" %[^\n]", phone);
        char res = validate_phone(phone, addressBook);
        if(res == 0)
            break;
        else if(res == 1)
            printf("Invalid Contact Length!\n");
        else if(res == 2) 
            printf("Invalid Phone No!\n");      
        else if(res == 3)
            printf("Duplicate Phone No!\n");  
    }
}

void inputEmail(char *email, AddressBook *addressBook){
    while(1){
        printf("Enter the Email : ");
        scanf(" %[^\n]", email);
        char res = validate_email(email, addressBook);

        if(res==0)
            printf("Invalid Email!\n");
        else if(res==1)
            break; //valid email
        else if(res==2)
            printf("Email already exists!\n");
    }
}

short search_by_name(AddressBook *addressBook, char flag){
    char str[20]; 
    //Enter the name
    //Get the name
    printf("Enter a Name to Search: \n");
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
        printf("No Record's Found!\n\n");
        return -1;
    }

    // count ==> 1 ==> return ind
    else if(count==1){
        printf("Search Found at S.No.: %d!\n", ind + 1);
        displayContact(addressBook, ind);
        return ind;
    }

    // count > 1 ==> return search_by_phone(addressBook);
    else if(count>1){ 
        // printf("Multiple Names Found! Search by PhoneNo.\n\n");
        // return search_by_phone(addressBook);
        if(flag==0){
            printf("Multiple Names Found!\n");
            for(char i = 0; i<count; i++){
                printf("Contact %d:\n",i+1);
                displayContact(addressBook, dup[i]);
            }
            return -1;  
        }
        else{
            printf("Multiple Names Found! Select the contact\n");
            for(char i = 0; i<count; i++){
                printf("Contact %d:\n",i+1);
                displayContact(addressBook, dup[i]);
            }
            while(1){
                printf("\nEnter your choice 1-%d: ",count);
                short choice, val;
                scanf("%hd",&choice);
                val = dup[choice-1];
                free(dup);

                if(choice>=1 && choice<=count)
                    return val;
                printf("Invalid Choice!\n");
            }
        }
    }
}

short search_by_phone(AddressBook *addressBook){
    char str[11];
    //Enter the phone
    //Get the phone
    printf("Enter a PhoneNo to Search: \n");
    scanf("%s",str);
    printf("\n");
    //Loop till contactCount
    for(short ind = 0; ind<addressBook->contactCount; ind++)
    {
        if(strcmp(str, addressBook->contacts[ind].phone) == 0)
        {
            //print the contact
            printf("Search Found at S.No.: %d!\n", ind + 1);
            displayContact(addressBook,ind);
            return ind;
        }
    }

    // Not found
    printf("No Record's Found!\n\n");
    return -1;
}

short search_by_email(AddressBook *addressBook){
    char str[30];
    //Enter the Email
    printf("Enter an Email to Search: \n");
    scanf("%s",str);
    printf("\n");
    //Loop till contactCount
    for(short ind = 0; ind<addressBook->contactCount; ind++)
    {
        if(strcmp(str, addressBook->contacts[ind].email) == 0)
        {
            //print the contact
            printf("Search Found at S.No.: %d!\n", ind + 1);
            displayContact(addressBook,ind);
            return ind;
        }
    }

    // Not found
    printf("No Record's Found!\n\n");
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

