/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      ...
 *   Co-authors:
 *      ...
 *
 */

#include "Client.h"

#include <iostream>
#include <string>

#include "util/Sleep.h"
#include "PacketManager.h"
#include "PacketTypes.h"

using namespace network;

void sender(){

  network::PacketGenerator pck;
  const int PORT = 55556;
  COUT(0) << "Enter address of the server xxx.xxx.xxx.xxx (enter for localhost)" << std::endl;
  std::string str;
  std::getline(std::cin, str);
  COUT(3) << "You entered: " << str << std::endl;
  if(str.compare("")==0)
    str="127.0.0.1";

  Client client( str, PORT );
  if ( client.establishConnection() )
  {
    COUT(3) << "connection established" << std::endl;
  }
  else
  {
    COUT(0) << "problems establishing connection" << std::endl;
  }
  char message[10000];
  char signs[] = "abcdefghijklmnopqrstuvwxy";
  while (true) {
    client.tick(0);

    std::cout << "your message2: ";
    for ( int i=0; i<9999; i++ ) {
      message[i] = signs[0];
    }
    message[9999] = 'z';
    std::string str( message );
    client.sendChat( str );
    std::cout << str << std::endl;
    std::cin.get(); std::cin.get();
  }



}

void listener(){

  const int PORT = 55556;
  std::cout << "Enter address of the server xxx.xxx.xxx.xxx (enter for localhost)" << std::endl;
  std::string str;
  std::getline(std::cin, str);
  std::cout << "You entered: " << str << std::endl;
  if(str.compare("")==0)
    str="127.0.0.1";

  Client client( str, PORT );
  if ( client.establishConnection() )
    std::cout << "connection established" << std::endl;
  else std::cout << "problems establishing connection" << std::endl;

  while (true) {
    client.tick(0);
    usleep(100);
  }



}


int main(int argc, char **argv[]){
  std::string in;
  std::cout << "Please choose: sender(s) oder listener(l)" << std::endl;
  std::getline(std::cin, in);
  if(in.compare("l")==0)
    listener();
  else if(in.compare("s")==0)
    sender();
  else
    std::cout << "wrong answer" << std::endl;
}
