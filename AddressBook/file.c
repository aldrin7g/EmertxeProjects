#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) {
  FILE *fp;
  fp = fopen("contacts.txt", "w");
  fprintf(fp, "#%d\n", addressBook->contactCount); //save contact count
  for(int i = 0; i<addressBook->contactCount; i++){
   fprintf(fp, "%s,%s,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email); //save each contact in file
  }
  fclose(fp);
}

void loadContactsFromFile(AddressBook *addressBook) {
  FILE *fp;
  if((fp = fopen("contacts.txt", "r")) == NULL){ //error: if can't access file
    fprintf(stderr,"Can't access file contacts.txt!\n");
    return;
  }
  fscanf(fp, "#%d\n", &addressBook->contactCount); //read contact count
  for(int i = 0; i<addressBook->contactCount; i++){
    fscanf(fp, "%[^,],%[^,],%[^\n]\n", addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email); //load each contact
  }
  fclose(fp);
}
