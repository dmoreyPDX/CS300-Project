/*
Damon Morey, Philip Prater, Samuel Shippey, Son Vu, Yves Wienecke 2018©
ManagerTerminal.cpp
*/

#include "ManagerTerminal.h"

using namespace std;

ManagerTerminal::ManagerTermina(DataCenter &dc) {
  this.dc = dc;
  this.running = false;
}

void ManagerTerminal::quit() {
  running = false;
}

int getOpt(string prompt, int lower, int upper) {
  assert(lower <= upper);

  while(true) {
    cout << prompt << ": ";
    cout.flush();
    cin >> opt;
    cin.ignore(100);
    if(opt >= lower && opt <= upper)
      break;
    else
      cout << "Invalid option, try again." << endl;
  }
  return opt;
}

void ManagerTerminal::run() {
  int opt;
  bool again;

  cout << "Starting Manager Terminal..." << endl;
  running = true;

  //can't do anything if DataCenter if offline
  if(!dc->online()) {
    cout << "Data Center did not respond" << endl;
    quit();
  }

  //main loop for manager terminal operations
  while(running) {
    opt = getOption(
           "Select an option from the list:\n" 
         + "1) Manage Service Directory\n"
         + "2) Manage Memberships\n"
         + "3) Manage Provider Network\n"
         + "4) Request Report\n"
         + "5) Quit\n");
    switch(opt) {
      case 1:
        switch(getOpt(
              "1) Add Service\n2) Remove Service\n3) Modify Service\n"
              1, 3)) {
          case 1:
            addService();
            break;
          case 2:
            removeService();
            break;
          case 3:
            modifyService();
            break;
          default:
            assert(true);
            break;
        }
        break;
      case 2:
        switch(getOpt(
              "1) Add Member\n2) Remove Member\n3) Modify Member\n"
              1, 3)) {
          case 1:
            addMember();
            break;
          case 2:
            removeMember();
            break;
          case 3:
            modifyMember();
            break;
          default:
            assert(true);
            break;
        }
        break;
      case 3:
        switch(getOpt(
              "1) Add Provider\n2) Remove Provider\n3) Modify Provider\n"
              1, 3)) {
          case 1:
            addProvider();
            break;
          case 2:
            removeProvider();
            break;
          case 3:
            modifyProvider();
            break;
          default:
            assert(true);
            break;
        }
        break;
      case 4:
        requestReport();
        break;
      case 5:
        running = false;
        break;
      default:
        break;
    }
  }
}



//TODO read in the rest of the data
//for all of these functions



void ManagerTerminal::addMember(void) {
  string name;
  name = getString("Enter full name of member to be added");
  dc->addMember(name);
}

void ManagerTerminal::removeMember(void) {
  string name;
  name = getString("Enter full name of member to be removed");
  if(dc->hasMember(name)) {
    dc->removeMember(name);
    cout << "Member successfully removed from the system." << endl;
  } else {
    cout << "Member was not found in the system." << endl;
  }
}

void ManagerTerminal::modifyMember(void) {
  string name;
  name = getString("Enter full name of member to be modified");
  if(dc->hasMember(name)) {
    //TODO figure out how to modify
    //I think we should probably be referencing Member objects directly
    //-Damon
  } else {
    cout << "Member was not found in the system." << endl;
  }
}

void ManagerTerminal::addProvider(void) {
  string name;
  name = getString("Enter name of provider to be added");
  dc->addProvider(name);
}

void ManagerTerminal::removeProvider(void) {
  string name;
  name = getString("Enter name of provider to be removed");
  if(dc->hasProvider(name)) {
    dc->removeProvider(name);
    cout << "Provider successfully removed from the system." << endl;
  } else {
    cout << "Provider was not found in the system." << endl;
  }
}

void ManagerTerminal::modifyProvider(void) {
  string name;
  name = getString("Enter name of provider to be modified");
  if(dc->hasProvider(name)) {
    //TODO figure out how to modify
    //I think we should probably be referencing Provider objects directly
    //-Damon
  } else {
    cout << "Provider was not found in the system." << endl;
  }
}

void ManagerTerminal::addService(void) {
  string name;
  name = getString("Enter name of service to be added");
  dc->addService(name);
}

void ManagerTerminal::removeService(void) {
  string name;
  name = getString("Enter name of service to be removed");
  if(dc->hasService(name)) {
    dc->removeService(name);
    cout << "Service successfully removed from the system." << endl;
  } else {
    cout << "Service was not found in the system." << endl;
  }
}

void ManagerTerminal::modifyService(void) {
  string name;
  name = getString("Enter name of service to be modified");
  if(dc->hasService(name)) {
    //TODO figure out how to modify
    //I think we should probably be referencing Service objects directly
    //-Damon
  } else {
    cout << "Service was not found in the system." << endl;
  }
}

void ManagerTerminal::requestReport(void) {
  //TODO
}

