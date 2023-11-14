#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
using namespace std;

int main(int argc, char* argv[]) {
struct sockaddr_in {
 short sin_family;
 unsigned short sin_port;
 struct in_addr sin_addr;
 char sin_zero[8];
};
struct in_addr {
 unsigned long s_addr;
};

int s = socket(AF_INET, SOCK_DGRAM, 0);
if (s == -1) {
    cout << "Socket error\n";
}

sockaddr_in * self_addr = new (sockaddr_in);
self_addr->sin_family = AF_INET;
self_addr->sin_port = htons(44214);
self_addr->sin_addr.s_addr = inet_addr("127.0.0.1");

int b = bind(s,(const sockaddr*) self_addr,sizeof(sockaddr_in));
if(b == -1) {
    cout << "Binding error\n";
    return 1;
}
while(true) {
    char connect[50];
    time_t now = time(0);
    tm *ltm = localtime(&now);
    string day, monday, hour, minute, second;
    string daytime;
    ltm->tm_mday < 10 ? day = '0' + to_string(ltm->tm_mday) : day = to_string(ltm->tm_mday);
    ltm->tm_mon < 10 ? monday = '0' + to_string(ltm->tm_mon + 1) : monday = to_string(ltm->tm_mon + 1);
    ltm->tm_hour < 10 ? hour = '0' + to_string(ltm->tm_hour) : hour = to_string(ltm->tm_hour);
    ltm->tm_min < 10 ? minute = '0' + to_string(ltm->tm_min) : minute = to_string(ltm->tm_min);
    ltm->tm_sec < 10 ? second = '0' + to_string(ltm->tm_sec) : second = to_string(ltm->tm_sec);
    daytime = day + '.' + monday + '.' + to_string(ltm->tm_year + 1900) + ' ' + hour + ':' + minute + ':' + second;
    char daytime_msg[sizeof(daytime)];
    strcpy(daytime_msg, daytime.c_str());
    sockaddr_in * client_addr = new sockaddr_in;
    socklen_t len = sizeof (sockaddr_in);
    if(recvfrom(s, connect, sizeof(connect), 0, (sockaddr*)(client_addr), &len)) {
        cout << "Successfull client connection!\n";
        sendto(s, daytime_msg, sizeof(daytime_msg), 0, (sockaddr*)(client_addr), len);
        cout << "Daytime was sent to client!\n";
    }
}
close(s);
return 0;
}
