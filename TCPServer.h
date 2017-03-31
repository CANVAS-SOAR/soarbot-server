#include <string>
class TCPServer{
    public:
        TCPServer(unsigned short port);

        /* @brief Binds to a socket 
         *
         * @return True if successful, False otherwise */
        bool Bind();
        bool Listen();
        bool Accept();
        bool Send(std::string data);
        bool Receive(std::string& data);
        unsigned short GetPort();

    private:
        int m_listen_fd;
        int m_connect_fd;
        unsigned short m_port;
};
