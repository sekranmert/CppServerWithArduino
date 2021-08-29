from Sekranmert 2021

Cpp Server With Arduino
 
   For Demo Video https://drive.google.com/drive/folders/1BedYchUlZWaEkna39FEsTRRAQ4z2EOX4?usp=sharing

   This project controls an arduino with a c++ server and client. Client.c++ and Server.c++ are written on ubuntu and compiled with gnu. they both use socket.h and pthread.h libraries.

for socket.h; https://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html

for pthread.h;
https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html

   When compiling for server or client, use $sudo g++ Server.c++ -lpthread -o server and $sudo g++ Client.c++ -lpthread -o client.

   Arduino controls a led, a 16x2 lcd, a buzzer, a servo and gets data from a dht11 temperature sensor. Used an wiznet W5100 ethernet shield. It uses Ethernet.h, LiquidCrystal.h, dht11.h, Servo.h libraries. 

   Gui Client is written with Qt creator. Used QtcpSocket.h library. Each button sends respective messages to the server.
