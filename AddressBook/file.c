#include <stdio.h>
#include "file.h"

// Function to save contacts to file
void saveContactsToFile(AddressBook *addressBook) {
  FILE *fp;
  fp = fopen("contacts.txt", "w");
  fprintf(fp, "#%d\n", addressBook->contactCount); //save contact count
  for(int i = 0; i<addressBook->contactCount; i++){
   fprintf(fp, "%s,%s,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email); //save each contact in file
  }
  fclose(fp);
}

// Function to load contacts from file
void loadContactsFromFile(AddressBook *addressBook) {
  FILE *fp;
  if((fp = fopen("contacts.csv", "r")) == NULL){ //error: if can't access file
    fprintf(stderr,"Can't access file contacts.csv!\n");
    return;
  }
  fscanf(fp, "#%d\n", &addressBook->contactCount); //read contact count
  for(int i = 0; i<addressBook->contactCount; i++){
    fscanf(fp, "%[^,],%[^,],%[^\n]\n", addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email); //load each contact
  }
  fclose(fp);
}
