#include "TCPServer.h"

#include <string>
#include <iostream>

#include <sys/socket.h>
#include <netinet/ip.h>
#include <string.h>
#include <stdio.h>

TCPServer::TCPServer(unsigned short port = 0):
m_connect_fd(-1),
m_port(port)
{
    m_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == m_listen_fd){
        perror("Failed to create socket");
    }
}

/* @brief Binds to a socket 
 *
 * @return True if successful, False otherwise */
bool TCPServer::Bind(){
    bool ret = false;
    if(-1 != m_listen_fd){
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(m_port);
        addr.sin_addr.s_addr = INADDR_ANY;
        if( -1 != bind(m_listen_fd, (sockaddr*)&addr, sizeof(addr))){
            ret = true;
        }
        else{
            perror("Failed to bind socket");
        }
    }
    return ret;
}

bool TCPServer::Listen(){
    bool ret = false;
    if(-1 != m_listen_fd){
        if(-1 != listen(m_listen_fd, 1)){
            ret = true;
            struct sockaddr_in addr;
            socklen_t len = sizeof(addr);
            if(-1 != getsockname(m_listen_fd, (struct sockaddr*)&addr, &len)){
                m_port = ntohs(addr.sin_port);
            }
            else{
                perror("Failed to get socket info");
            }
        }
        else{
            perror("Failed to listen for socket connections");
        }
    }
    return ret;
}

bool TCPServer::Accept(){
    bool ret = false;
    if(-1 != m_listen_fd){
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        socklen_t len = sizeof(addr);
        m_connect_fd = accept(m_listen_fd, (sockaddr*)&addr, &len);
        if(-1 != m_connect_fd){
            ret = true;
        }
        else{
            perror("Failed to accept connection on socket");
        }
    }
    return ret;
}

/* @brief sends null-terminated string to socket
 *
 * @return true if succesful, false otherwise */
bool TCPServer::Send(std::string data){
    bool ret = false;
    if(-1 != m_connect_fd){
        if(-1 != send(m_connect_fd, data.c_str(), data.length()+1, 0)){
            ret = true;
        }
        else{
            perror("Failed to send");
        }
    }
    return ret;
}

/* @brief receive a null-terminated string from socket
 *
 * @return true if succesful, false otherwise */
bool TCPServer::Receive(std::string& data){
    static std::string remainder = "";
    bool ret = false;
    char buf[2048];
    //Check if we previously received next message
    size_t loc = remainder.find_first_of('\0');
    if(loc != std::string::npos){
        data = remainder.substr(0, loc);
        if(loc+1 <= remainder.length()){
            remainder = remainder.substr(loc+1);
        }
        ret = true;
    }
    //Nope, need to receive some more
    else if(-1 != m_connect_fd){
        bool messageReceived = false;
        while(!messageReceived){
            int len = recv(m_connect_fd, buf, sizeof(buf), 0);
            if(-1 != len){
                std::string msg = std::string(buf, len);
                size_t loc = msg.find_first_of('\0');
                if(loc != std::string::npos){
                    ret = true;
                    messageReceived=true;
                    data = remainder + msg.substr(0, loc);
                    if(loc+1 <= msg.length()){
                        remainder = msg.substr(loc+1);
                    }
                }
                else{
                    remainder += msg;
                }
            }
            else{
                perror("Failed to receive from socket");
                return false;
            }
        }
    }
    return ret;
}

unsigned short TCPServer::GetPort(){
    return m_port;
}
