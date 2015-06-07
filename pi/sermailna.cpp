#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <time.h>
#include <wiringSerial.h>

#define BUFFSIZE 512
sf::UdpSocket socket;

sf::Packet fdata;
sf::Packet cdata;
int id = 1; // 1 -> pc, 2 -> dron

sf::IpAddress odbiorca = "192.168.1.7"; //IP laptopa


unsigned short port = 54323;
unsigned short porto = 54324;



int main(){
	
	int fd=serialOpen("/dev/ttyAMA0",115200);
	//struct termios *termios_p;
	//tcgetattr(fd,termios_p);
	//cfgetispeed(fd, &termios_p);
	//std::cout << cfgetispeed(termios_p) << " ";
	socket.bind(port);
	//socket.connect(odbiorca, 54323);
	sf::SocketSelector sel;
	sel.add( socket );
	//std::cout << "tutu ";
	serialFlush(fd); 
	
	while(true){
		fdata.clear();
		cdata.clear();
		
		///////// malinka do pc >>>>
		
		int incoming = serialDataAvail(fd);
		std::cout << incoming << " ";                /////do odkomentowania
		char bajtyo[BUFFSIZE];
		read(fd,bajtyo,incoming);
		char *fdit=bajtyo;
		for(int i=0; i<incoming; i++){
			//std::cout << (sf::Uint8)*fdit << " ";
			fdata << (sf::Uint8)*fdit;
			fdit++;
		}
		/*
		while(incoming--){
			int dane;
			char bajt;
			bajt = (char)serialGetchar(fd);
			//std::cout << bajt << " ";
			char *fdit = &bajt;
			fdata << fdit;
		}
		*/
		int siz = fdata.getDataSize();
		//std::cout << serialDataAvail(fd) << " "; 
		//std::cout << siz << "\n";                  ///do odkomentowania !!!!
		std::cout << "\n";
		
		if(siz != 0)
		if (socket.send(fdata, odbiorca, port) != sf::Socket::Done){
			std::cout<<"Blad wysylania\n";
		}
		
		///////// pc do malinki <<<
		
		if(sel.wait(sf::seconds(1))){
			if( sel.isReady( socket ) ) { }
			if(socket.receive(cdata,odbiorca,port) == sf::Socket::Done){
				//std::cout << "hejo! ";
				//unsigned char bajt;
				char bajty[BUFFSIZE];
				unsigned char out[BUFFSIZE];
				sf::Uint8 bajt;
				//sf::Uint8 *cdit = &bajt;
				char *cdit = bajty;
				int i=0;
				size_t len=0;
				//std::cout << cdata.getDataSize() << " ";
				while(!cdata.endOfPacket()){
					cdata >> bajt;
					*cdit=(char)bajt;
					len++;
					//std::cout << *cdit << " ";
					/*
					if(*cdit=='$' && i!=0){
						*cdit='\0';
						cdit=bajty;
						write(fd, cdit, len);
						len=0;
						//serialPrintf(fd,cdit);
					}
					*/
					out[i]=(unsigned char)bajty[i];
					//write(fd, cdit, 1);
					//unsigned char tmp = (unsigned char)bajty[i];
					//serialPutchar(fd,tmp);
					i++;
					cdit++;
					//serialPutchar(fd,bajt);
				}
				cdit=bajty;
				write(fd, bajty, len);
				//serialPrintf(fd,cdit);
				//serialPrintf(fd,bajty);
				//std::cout << "wyszlem " << i << "\n";
			}
		}
		
		//std::cout << "tutu ";
		//usleep(100000);
		//sf::Time t = sf::seconds(0.5f);
		//sf::sleep(t);
	}
}
