//
// Created by magshimim on 17.12.2019.
//

#include "serverCommand.h"
#include "../data.h"
string OpenServerCommand::oldBuf;
int OpenServerCommand::execute() {
    ++it;
    _port = stoi(*it);
    ++it;
    //bind the socket
    openSocketServer();
    //open thread in background that read from the client
    startTherad();
    return 2;
}
//open thread to read data from client in server
void OpenServerCommand::startTherad() {
    serverThread = thread(&OpenServerCommand::serverRead, this);
}
void OpenServerCommand::serverRead() {
    cout<<"enter to serverRead\n"<<endl;
    string line;
    while(true){
        readLineFromClient(line);
        spliteLineAndUpdate(line);
        line.clear();
    }
}

//read one line from the client
void OpenServerCommand::readLineFromClient(string& line) {
    mutexGeneralSimVariable.lock();
    string bufString = "";
    int indexBsN = 0;
    char buf[1024] = {};
    int n = 0;
    int startLine = -1;
    sleep(3.0);
    while(true)
    {
        //read from socket
        n = read(client_socket, buf, 1024);
        if(n == -1){
            cout<<"error in read line from clinet"<<endl;
        }
        bufString = string() + buf;
        line += bufString;
        for(int i = 0; i < line.length(); i++) {
            if (bufString[i] == '\n' && i > 0) {
                indexBsN++;
            }
        }
        cout<<indexBsN<<endl;
        cout<<"line1  "<<line<<endl;
        cout<<"buff1  "<<bufString<<endl;
        if(indexBsN >= 2) {
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == '\n'&& i > 0) {
                    if (startLine == -1) {
                        startLine = i + 1;
                        cout<<"line  "<<line<<endl;
                        cout<<"buff  "<<bufString<<endl;
                    } else {
                        line = line.substr(startLine, i - startLine);
                        break;
                    }
                }

            }
            break;
        }
    }
    mutexGeneralSimVariable.unlock();
    cout<<"line from sim "<<line<<endl;
}
void OpenServerCommand::spliteLineAndUpdate(string line) {
//    std::mutexGeneralSimVariable mtx;
//    mtx.lock();
    float varSim[NUM_VARIABLE_XML] = {0};
    string word = "";
    char* pend;
    int j = 0;
    for(int i = 0; i < line.length() && j < NUM_VARIABLE_XML ; i++){
        if(line[i] == ',' || line[i] == '\n') {
            varSim[j] = std::stof(word.c_str());
            j++;
            word = "";

        } else{
            word+=line[i];
        }
    }
//    cout<<"end loop to insert to varsim" << endl;
    updateSimTable(varSim, line);
//    mtx.unlock();

}
void OpenServerCommand::updateSimTable(const float* arrayFloat, string line) {
    unordered_map<string, float>::iterator iter;

    int index;
    for(index = 0; index < NUM_VARIABLE_XML; index++) {
        mutexGeneralSimVariable.lock();
        switch (index) {
            case 0:
                iter = command::_generalSimVariable->find(V1);
                break;
            case 1:
                iter = command::_generalSimVariable->find(V2);
                break;
            case 2:
                iter = command::_generalSimVariable->find(V3);
                break;
            case 3:
                iter = command::_generalSimVariable->find(V4);
                break;
            case 4:
                iter = command::_generalSimVariable->find(V5);
                break;
            case 5:
                iter = command::_generalSimVariable->find(V6);
                break;
            case 6:
                iter = command::_generalSimVariable->find(V7);
                break;
            case 7:
                iter = command::_generalSimVariable->find(V8);
                break;
            case 8:
                iter = command::_generalSimVariable->find(V9);
                break;
            case 9:
                iter = command::_generalSimVariable->find(V10);
                break;
            case 10:
                iter = command::_generalSimVariable->find(V11);
                break;
            case 11:
                iter = command::_generalSimVariable->find(V12);
                break;
            case 12:
                iter = command::_generalSimVariable->find(V13);
                break;
            case 13:
                iter = command::_generalSimVariable->find(V14);
                break;
            case 14:
                iter = command::_generalSimVariable->find("/instrumentation/gps/indicated-vertical-speed");
                break;
            case 15:
                iter = command::_generalSimVariable->find("/instrumentation/heading-indicator/indicated-heading-deg");
                break;
            case 16:
                iter = command::_generalSimVariable->find("/instrumentation/magnetic-compass/indicated-heading-deg");
                break;
            case 17:
                iter = command::_generalSimVariable->find("/instrumentation/slip-skid-ball/indicated-slip-skid");
                break;
            case 18:
                iter = command::_generalSimVariable->find("/instrumentation/turn-indicator/indicated-turn-rate");
                break;
            case 19:
                iter = command::_generalSimVariable->find("/instrumentation/vertical-speed-indicator/indicated-speed-fpm");
                break;
            case 20:
                iter = command::_generalSimVariable->find("/controls/flight/aileron");
                break;
            case 21:
                iter = command::_generalSimVariable->find("/controls/flight/elevator");
                break;
            case 22:
                iter = command::_generalSimVariable->find("/controls/flight/rudder");
                break;
            case 23:
                iter = command::_generalSimVariable->find("/controls/flight/flaps");
                break;
            case 24:
                iter = command::_generalSimVariable->find("/controls/engines/engine/throttle");
                break;
            case 25:
                iter = command::_generalSimVariable->find("/controls/engines/current-engine/throttle");
                break;
            case 26:
                iter = command::_generalSimVariable->find("/controls/switches/master-avionics");
                break;
            case 27:
                iter = command::_generalSimVariable->find("/controls/switches/starter");
                break;
            case 28:
                iter = command::_generalSimVariable->find("/engines/active-engine/auto-start");
                break;
            case 29:
                iter = command::_generalSimVariable->find("/controls/flight/speedbrake");
                break;
            case 30:
                iter = command::_generalSimVariable->find("/sim/model/c172p/brake-parking");
                break;
            case 31:
                iter = command::_generalSimVariable->find("/controls/engines/engine/primer");
                break;
            case 32:
                iter = command::_generalSimVariable->find("/controls/engines/current-engine/mixture");
                break;
            case 33:
                iter = command::_generalSimVariable->find("/controls/switches/master-bat");
                break;
            case 34:
                iter = command::_generalSimVariable->find("/controls/switches/master-alt");
                break;
            case 35:
                iter = command::_generalSimVariable->find("/engines/engine/rpm");
                break;
            default:
                break;
        }
        iter->second = arrayFloat[index];
        if(index == 35){
            std::cout << iter->first << " " << iter->second << "\n"<<endl;
        }
        mutexGeneralSimVariable.unlock();
    }
    cout<<"_generalSimVariable after changed\n"<<endl;
//    for(auto it = _generalSimVariable->cbegin(); it != _generalSimVariable->cend(); ++it)
//    {
//        std::cout << it->first << " " << it->second << "\n";
//    }

//    cout<<"update variable from sim"<<endl;
}
void OpenServerCommand::openSocketServer() {
    //create socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        //error
        throw "Could not create a socket";
    }
    this->socketfd = socketfd;
    //bind socket to IP address
    // we first need to create the sockaddr obj.
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //give me any IP allocated for my machine
    address.sin_port = htons(_port);
    //the actual bind command

    if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        // throw "Could not bind the socket to an IP";
        throw "Could not bind the socket to an IP";
    }
    //making socket listen to the port
    if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
        throw "Error during listening command";
    } else {
        std::cout<<"Server is now listening ..."<<std::endl;
    }
    // accepting a client
    int client_socket = accept(socketfd, (struct sockaddr *)&address,
                               (socklen_t*)&address);
    if (client_socket == -1) {
        throw "Error accepting client";
    }

    this->client_socket = client_socket;
}
OpenServerCommand::~OpenServerCommand() {
    if(serverThread.joinable()){
        serverThread.join();
    }
    close(socketfd);

}