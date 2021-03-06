#include <iostream>
#include <stdlib.h>
#include <string>

#include "machines.h"

//*****************************************//
// node class functions

node::node(string n, IPAddress a) {
    name = new string;
    *name = n;
    my_address = a;
}

node::~node() {
    delete name;
}

void node::display() {
    
    cout << "   Name: " << *name << "   IP address: ";
    my_address.display();
}

int node::amIThisComputer(IPAddress a) {
    if(my_address.sameAddress(a))
        return 1;
    else
        return 0;
}

int node::myType() {
    return 0;
}

//*****************************************//
// laptop class functions

laptop::laptop(string n, IPAddress a) : node(n,a)  {
    incoming = outgoing = NULL;
    my_server.parse("0.0.0.0");
}

void laptop::display() {
    
    cout << "Laptop computer:  ";
    node::display();
    
    if(my_server.isNULL()) {
        cout << "\n    Not connected to any server.\n";
    }
    else {
        cout << "\nConnected to ";
        my_server.display();
        cout << "\n";
    }
    
    cout << "\n   Incoming datagram:  ";
    if(incoming==NULL) cout << "No datagram.";
    else               {cout << "\n";  incoming->display(); }
    
    cout << "\n   Outgoing datagram:  ";
    if(outgoing==NULL) cout << "No datagram.";
    else               {cout << "\n"; outgoing->display(); }
    
}

int laptop::myType() {
    return LAPTOP;
}

void laptop::initiateDatagram(datagram* d) {
    outgoing = d;
}

void laptop::receiveDatagram(datagram* d) {
    incoming = d;
}

int laptop::canAcceptConnection(int machine_type){
    IPAddress noConnection;
    noConnection.parse("0.0.0.0");
    if (machine_type == SERVER && my_server.sameAddress(noConnection))
        return 1;
    else
        return 0;
}

void laptop::connect(IPAddress x, int machine_type){
    my_server = x;
}


//*****************************************//
// server class functions

server::server(string n, IPAddress a) : node(n,a)  {
    number_of_laptops = number_of_wans = 0;
    dlist = new msg_list; /**********new*********/
}

void server::display() {
    int i;
    
    cout << "Server computer:  ";
    node::display();
    
    cout << "\n   Connections to laptops: ";
    if(number_of_laptops==0) cout << "    List is empty.";
    else for(i=0; i<number_of_laptops; i++) {
        cout << "\n      Laptop " << i+1 << ":   ";
        laptop_list[i].display();
    }
    cout << "\n\n   Connections to WANs:    ";
    if(number_of_wans==0) cout << "    List is empty.";
    else for(i=0; i<number_of_wans; i++) {
        cout << "\n         WAN " << i+1 << ":   ";
        WAN_list[i].display();
    }
    
    cout << "\n\n   Message list:\n";
    dlist->display();
    
    cout << "\n\n";
    
}

int server::myType() {
    return SERVER;
}

int server::canAcceptConnection(int machine_type){
    if (machine_type == LAPTOP && number_of_laptops < sizeof(laptop_list))
        return 1;
    if (machine_type == WAN_MACHINE && number_of_wans < sizeof(WAN_list))
        return 1;
    return 0;
}

void server::connect(IPAddress x, int machine_type){
    if (machine_type == LAPTOP)
        laptop_list[number_of_laptops] = x;
    if (machine_type == WAN_MACHINE)
        WAN_list[number_of_wans] = x;
}

void server::receiveDatagram(datagram *d){
    dlist->append(d);
}


//*****************************************//
// WAN class functions

WAN::WAN(string n, IPAddress a) : node(n,a)  {
    number_of_servers = number_of_wans = 0;
    dlist = new msg_list; /**********new*********/
}

void WAN::display() {
    int i;
    
    cout << "WAN computer:  ";
    node::display();
    
    cout << "\n   Connections to servers: ";
    if(number_of_servers==0) cout << "    List is empty.";
    else for(i=0; i<number_of_servers; i++) {
        cout << "\n      Server " << i+1 << ":   ";
        server_list[i].display();
    }
    cout << "\n\n   Connections to WANs:    ";
    if(number_of_wans==0) cout << "    List is empty.";
    else for(i=0; i<number_of_wans; i++) {
        cout << "\n         WAN " << i+1 << ":   ";
        WAN_list[i].display();
    }
    
    cout << "\n\n   Message list:\n";
    dlist->display();
    
    cout << "\n\n";
    
}

int WAN::myType() {
    return WAN_MACHINE;
}

int WAN::canAcceptConnection(int machine_type){
    if (machine_type == WAN_MACHINE && number_of_wans < sizeof(WAN_list))
        return 1;
    if (machine_type == SERVER && number_of_servers < sizeof(server_list))
        return 1;
    return 0;
}

void WAN::connect(IPAddress x, int machine_type){
    if (machine_type == SERVER)
        server_list[number_of_servers] = x;
    if (machine_type == WAN_MACHINE)
        WAN_list[number_of_wans] = x;
}

void WAN::receiveDatagram(datagram *d){
    dlist->append(d);
}

