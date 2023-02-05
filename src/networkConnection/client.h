#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <SFML/Network.hpp>

#include "../converters/VideoConverter.h"

using namespace std;

//tentando com threads
sf::IpAddress server_sender;

void sendToServer(const char *server_IP_address){
    sf::UdpSocket socket;
    // UDP socket:
    sf::IpAddress recipient("147.250.226.201");
    std::cout<<"Trying to connect to " << recipient.toString()<<"\n";;
    unsigned short server_receive_port = 54000;
    
    //camera capture
    cv::VideoCapture cap(0);
    cv::Mat img;

     while(1){
        //captando a imagem
        Converter c;
        cap.read(img);
        c.setImage(img);
        c.convertGrayScale();
        c.resize(50, 50);

        //enviando para server
        if (socket.send((char*)c.parallelConvert(c.getImage(), 1, 3).get(), 2500, recipient, server_receive_port) != sf::Socket::Done)
        {
        std::cout<<"Error in sending to " << recipient.toString() <<"\n";;        
        }        
    }

}

void receiveFromServer(const char* server_IP_address){
    char buffer[2500];
    std::size_t received;
    unsigned short server_sender_port;

    sf::UdpSocket socket;
    if (socket.bind(54001) != sf::Socket::Done)
    {
        printf("Error bro\n");
        return;
    }
    std::cout<<"Binded to " << socket.getLocalPort()<<"\n";
    while(1){
        // UDP socket:
        if (socket.receive(buffer, sizeof(buffer), received, server_sender, server_sender_port) != sf::Socket::Done)
        {
            std::cout<<"Error in rcv" << std::endl;
     
        }
        std::cout<<received;
        if(received == sizeof(buffer)){
            system("clear");
            std::cout<<buffer;
        }
    
    }
}


void be_client(const char* server_IP_address){

    std::thread t1 = std::thread(sendToServer, server_IP_address);

    std::thread t2 = std::thread(receiveFromServer, server_IP_address);

    t1.join();
    t2.join();
}