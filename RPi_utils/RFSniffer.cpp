/*
  RFSniffer

  Usage: ./RFSniffer [<pulseLength>]
  [] = optional

  Hacked from http://code.google.com/p/rc-switch/
  by @justy to provide a handy RF code sniffer
*/

#include "../rc-switch/RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>     
/*
include for JEEDOM Test
*/
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

RCSwitch mySwitch;
 


int main(int argc, char *argv[]) {
  
     // This pin is not the first pin on the RPi GPIO header!
     // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
     // for more information.
     int PIN = 4;
     
     if(wiringPiSetup() == -1) {
       printf("wiringPiSetup failed, exiting...");
       return 0;
     }

     int pulseLength = 0;
     if (argv[1] != NULL) pulseLength = atoi(argv[1]);

     mySwitch = RCSwitch();
     if (pulseLength != 0) mySwitch.setPulseLength(pulseLength);
     mySwitch.enableReceive(PIN);  // Receiver on interrupt 0 => that is pin #2
     
    
     while(1) {
  
      if (mySwitch.available()) {
    
        int value = mySwitch.getReceivedValue();
    
        if (value == 0) {
          printf("Unknown encoding\n");
        } else {    
   
          printf("Received %i\n", mySwitch.getReceivedValue() );
        }
    
        mySwitch.resetAvailable();
    
      }
       /*
       if code reçus modification de la variable dans la base de donnée pour lancer le scenario
       */
      /*
      try {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stmt;
            sql::ResultSet *res;

            /* Create a connection */
/*            driver = get_driver_instance();
            con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
            /* Connect to the MySQL test database */
/*            con->setSchema("test");

            stmt = con->createStatement();
            res = stmt->executeQuery("SELECT 'Hello World!' AS _message");
            while (res->next()) {
              cout << "\t... MySQL replies: ";
              /* Access column data by alias or column name */
/*              cout << res->getString("_message") << endl;
              cout << "\t... MySQL says it again: ";
              /* Access column data by numeric offset, 1 is the first column */
/*              cout << res->getString(1) << endl;
            }
            delete res;
            delete stmt;
            delete con;

        } catch (sql::SQLException &e) {
          cout << "# ERR: SQLException in " << __FILE__;
          cout << "(" << __FUNCTION__ << ") on line " »
               << __LINE__ << endl;
          cout << "# ERR: " << e.what();
          cout << " (MySQL error code: " << e.getErrorCode();
          cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
       */
      sleep(10);
  
  }

  exit(0);


}

