#include "stdafx.h"
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <time.h>
#include <Windows.h>
#include "Serial.h"


sf::UdpSocket ssocket;

sf::Packet fdata;
sf::Packet cdata;
#define BUFFSIZE 256
int id = 1; // 1 -> pc, 2 -> dron

sf::IpAddress odbiorca = "192.168.10.1"; //IP laptopa
sf::SocketSelector sel;

unsigned short port = 54323; //OK port komunikacyjny dla serial



int main(){

	//if(socket.bind(port) != sf::Socket::Done){
	//	cout << "problem z polaczeniem na serial\n";
	//}

	sel.add(ssocket);

	tstring commPortName(TEXT("COM5"));
	Serial serial(commPortName);
	//int incoming = serialDataAvail(fd); pisz pod Windowsa xd
	serial.flush();

	while (true){
		///////// pc do malinki >>>>
		

		char bajty[BUFFSIZE];
		int charsRead = serial.read(bajty, BUFFSIZE);
		
		for (int i = 0; i < charsRead; i++){
			fdata << bajty[i];
		}
		//std::cout << incoming << "\n";

		if (charsRead != 0){

			std::cout << charsRead << " ";                 //do odkomentowania!!!
			/*
			for (int i = 0; i < charsRead; i++){
				int bycik;
				fdata >> bycik;
				std::cout << bycik << " ";
			}
			std::cout << "\n";
			*/

			if (ssocket.send(fdata, odbiorca, port) != sf::Socket::Done){
				std::cout << "Blad wysylania\n";
			}
		}

		///////// malinka do pc <<<
		if (sel.wait(sf::seconds(1))){
			if (sel.isReady(ssocket)) {}
			if (ssocket.receive(cdata, odbiorca, port) == sf::Socket::Done){
				char bajty[BUFFSIZE];
				char *it = bajty;
				int dsize = 0;
				//int ind = 0;
				while (!cdata.endOfPacket()){
					cdata >> it;
					it++;
					dsize++;
					//TODO wyslij na com
					//serialPutchar(fd, bajt);
				}
				//std::cout << dsize << " ";                 //do odkomentowania !!!!
				it = bajty;
				serial.write(bajty,dsize);
			}
		}

		Sleep(1);
		fdata.clear();
		cdata.clear();
	}
}
