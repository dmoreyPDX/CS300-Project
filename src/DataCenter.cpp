/*
   Damon Morey, Philip Prater, Samuel Shippey, Son Vu, Yves Wienecke 2018©
   DataCenter.cpp
   */

#include "DataCenter.h"

//Address
address::address() {}

// Member constructor
Member::Member(string n, string number, string phone, address & ad) {
	name = n;
	phoneNumber = phone;
    memberNumber = number;
	fullAddress.streetAddress = ad.streetAddress;
	fullAddress.city = ad.city;
	fullAddress.state = ad.state;
	fullAddress.zip = ad.zip;
  status = true;
}

Member::Member(){}
Member::~Member(){}

//add new consultation to member
void Member::consultation(Service & service) {
	weeklyConsultations.push_back(service);
}

//clear member's list of consulations
void Member::clear() {
	weeklyConsultations.clear();
}

// Provider constructor
Provider::Provider(string n, string number, address & ad, double f) {
	name = n;
	phoneNumber = number;
	fullAddress.streetAddress = ad.streetAddress;
	fullAddress.city = ad.city;
	fullAddress.state = ad.state;
	fullAddress.zip = ad.zip;
	weeklyConsultationFees = f;
}

Provider::Provider(){}
Provider::~Provider(){}

//add consultation to provider
void Provider::consultation(Service & service) {
	// Went with the running total thing, seemed simpler. 
	// Didn't call parent function cause no reason to go adding more overhead.
	weeklyConsultations.push_back(service);
	weeklyConsultationFees += service.fee;
}

//remove all consultations in provider
void Provider::clear() {
	weeklyConsultations.clear();
	weeklyConsultationFees = 0;
}

// Service constructor for datacenter
Service::Service(string code, string name, double f) {
	serviceCode = code;
    serviceName = name;
	fee = f;
	provider = NULL;
	member = NULL;
	date = NULL;
}

// Service constructor for members and providers
Service::Service(const Service &s, const Member *m, const Provider *p) {
	fee = s.fee;
	serviceCode = s.serviceCode;
    serviceName = s.serviceName;
	provider = p;
	member = m;
	time_t t;
	time(&t);
	date = localtime(&t);
}

Service::Service(){}
Service::~Service(){}

// Data Center
DataCenter::DataCenter() {
	//load data from files
    if (loadServices(PROVIDER_DIRECTORY)) {
        cout << "Successfully loaded services from disk\n";
    } else {
        cout << "Failed to load services from disk\n";
    }

    if (loadMembers(MEMBERS)) {
        cout << "Successfully members from disk\n";
    } else {
        cout << "Failed to load members from disk\n";
    }

    if (loadProviders(PROVIDERS)) {
        cout << "Successfully loaded providers from disk\n";
    } else {
        cout << "Failed to load providers from disk\n";
    }

    // TODO
    if (loadReports("")) {
        cout << "Successfully loaded reports from disk\n";
    } else {
        cout << "Failed to load reports from disk\n";
    }

	//TODO
	activeMemberCount = 0;
	weeklyConsultationCount = 0;
	weeklyConsultationFees = 0.0;
}

DataCenter::~DataCenter() {
	//clean up dynamic data
	//afaik, we don't have any, but then again we may be managing memory incorrectly
	//keep this in mind
	//c++ containers automatically manager their memory
	//adding to the container copies the object
	//because our objects are very small, this is absolutely fine
}


bool DataCenter::confirmConsultation(string memberName, string providerName, string serviceCode) {
  try{
    //get the member, provider, and service
    Member &m = memberMap.at(memberName);
    Provider &p = providerMap.at(providerName);
    Service &s = serviceMap.at(serviceCode);

    //create consultaton for reporting
    Service c(s, &m, &p);

    //give to member and provider
    m.consultation(c);
    p.consultation(c);

    //record fee for manager
    ++weeklyConsultationCount;
    weeklyConsultationFees += c.fee;

    return true;

  } catch(const out_of_range &) {
    return false;
  }
}

//clears all consultations for the week
void DataCenter::newWeek() {
  weeklyConsultationCount = 0;
  weeklyConsultationFees = 0.0;
  for(auto x = memberMap.begin(); x != memberMap.end(); ++x)
    x->second.clear();
  for(auto x = providerMap.begin(); x != providerMap.end(); ++x)
    x->second.clear();
}

//prints a list of services
//XXX should probably be given an output stream
void DataCenter::printServiceList() {
  cout << "Master Service List: (" << serviceMap.size() << ")" << endl;
  if(serviceMap.size() == 0) {
    cout << "No services available" << endl;
    return;
  }
  for(auto x = serviceMap.begin(); x != serviceMap.end(); ++x) {
      cout.precision(6);
      cout.fill('0');
      cout << x->second.serviceCode << " - " << x->second.serviceName << " $" << x->second.fee << endl;
  }
    cout << "\n\n";
}

void DataCenter::addService(Service & service) {
	serviceMap[service.serviceCode] = service;
}

// Loading from disk
bool DataCenter::loadServices(string fileName) {
    ifstream inFile;
    
    // Temporary vars to hold data from file
    Service temp; 
    string name, code, fee;

    // Setting max size for strings

    inFile.open(fileName);
    if (!inFile.is_open())
        return false;
    
    // Ignore the first line in the file
    getline(inFile, name);
    while (!getline(inFile, name, ';').eof()) {
        // service name;cost;code 
        getline(inFile, fee, ';');
        getline(inFile, code); 

        name.resize(20);
        code.resize(6);
        fee.resize(6);
        
        name.shrink_to_fit(); 
        fee.shrink_to_fit();
        code.shrink_to_fit();
        
        temp = Service(code, name, stof(fee));
        addService(temp);    
    }
   
    inFile.close();
    return true;
}

bool DataCenter::loadMembers(string fileName) {
    ifstream inFile;
    
    // Temporary vars to hold data from file
    Member temp; 
    string name, number, phone;
    address ad;

    // Setting max size for strings

    inFile.open(fileName);
    if (!inFile.is_open())
        return false;
    
    while (!getline(inFile, name, ';').eof()) {
        // member name;number;phone;address
        getline(inFile, number, ';');
        getline(inFile, phone, ';');

        // address;city;state;zip 
        getline(inFile, ad.streetAddress); 
        getline(inFile, ad.city); 
        getline(inFile, ad.state); 
        getline(inFile, ad.zip); 

        name.resize(25);
        number.resize(9);
        phone.resize(12);
        ad.streetAddress.resize(25);
        ad.city.resize(14);
        ad.state.resize(2);
        ad.zip.resize(5);
        
        name.shrink_to_fit(); 
        number.shrink_to_fit();
        phone.shrink_to_fit();
        ad.streetAddress.shrink_to_fit();
        ad.city.shrink_to_fit();
        ad.state.shrink_to_fit();
        ad.zip.shrink_to_fit();
        
        temp = Member(name, number, phone, ad);
        addMember(temp);    
    }
   
    inFile.close();
    
    return true;
}

bool DataCenter::loadProviders(string fileName) {
    return true;
}

bool DataCenter::loadReports(string fileName) {
    return true;
}

// Dave data to disk
bool DataCenter::saveServices(string fileName) {
    return true;
}

bool DataCenter::saveMembers(string fileName) {
    return true;
}

bool DataCenter::saveProviders(string fileName) {
    return true;
}

bool DataCenter::saveReports(string fileName) {
    return true;
}

// add new provider to data center
void DataCenter::addProvider(Provider & provider) {
	providerMap[provider.name] = provider;
}

//add new member to provider
void DataCenter::addMember(Member & member) {
	memberMap[member.name] = member;
  if(member.status)
    ++activeMemberCount;
}

//remove service from Data center
void DataCenter::removeService(string serviceCode) {
  serviceMap.erase(serviceCode);
}

//remove provider from Data center
void DataCenter::removeProvider(string providerName) {
  providerMap.erase(providerName);
}

//remove member from Data Center
void DataCenter::removeMember(string memberName) {
  try {
    if(memberMap.at(memberName).status)
      --activeMemberCount;
  } catch(const out_of_range &e) {
    return;
  }
  memberMap.erase(memberName);
}

//check if data center has service 
bool DataCenter::hasService(string serviceCode) {
  try {
    serviceMap.at(serviceCode);
    return true;
  } catch(const out_of_range &e) {
    return false;
  }
}

//check if data center has member in data center
bool DataCenter::hasMember(string memberName) {
  try {
    memberMap.at(memberName);
    return true;
  } catch(const out_of_range &e) {
    return false;
  }
}

//check if data center has provider
bool DataCenter::hasProvider(string providerName) {
  try {
    providerMap.at(providerName);
    return true;
  } catch(const out_of_range &e) {
    return false;
  }
}
