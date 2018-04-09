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
     string mysql_server = "tcp://127.0.0.1:3306";
     string mysql_user = "root";
     string mysql_password = "root";
     string mysql_database = "test";
  try{
     if(wiringPiSetup() == -1) {
       printf("wiringPiSetup failed, exiting...");
       return 0;
     }

     int pulseLength = 0;
     if (argv[1] != NULL) pulseLength = atoi(argv[1]);

     mySwitch = RCSwitch();
     if (pulseLength != 0) mySwitch.setPulseLength(pulseLength);
     mySwitch.enableReceive(PIN);  // Receiver on interrupt 0 => that is pin #4
     
    
     while(1) {
  
      if (mySwitch.available()) {
    
        int value = mySwitch.getReceivedValue();
    
        if (value == 0) {
          printf("Unknown encoding\n");
        } else {    
   
          printf("Received %i\n", mySwitch.getReceivedValue() );
        }      
      
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;

        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect(mysql_server, mysql_user, mysql_password);
        /* Connect to the MySQL JEEDOM database */
        con->setSchema(mysql_database);

        stmt = con->createStatement();
        
        switch(mySwitch.getReceivedValue()){
          case:1
            res = stmt->executeQuery("UPDATE 'datastore' SET 'value' = '1' WHERE 'datastore'.'id' = 39");
            break();
          case:2
            res = stmt->executeQuery("UPDATE 'datastore' SET 'value' = '1' WHERE 'datastore'.'id' = 40");
            break();
          default:  
            break;
        }   
        
        delete res;
        delete stmt;
        delete con;
        mySwitch.resetAvailable();  
      }  
       
      sleep(10);
  
  }
    
  } catch (sql::SQLException &e) {
          cout << "# ERR: SQLException in " << __FILE__;
          cout << "(" << __FUNCTION__ << ") on line " »
               << __LINE__ << endl;
          cout << "# ERR: " << e.what();
          cout << " (MySQL error code: " << e.getErrorCode();
          cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
  exit(0);


}

