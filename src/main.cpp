/*
I2Cdev library collection - ADXL345 RPi example
Based on the example in Arduino/ADXL345/

==============================================
I2Cdev device library code is placed under the MIT license

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software isa
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================

To compile on a Raspberry Pi (1 or 2)
  1. install the bcm2835 library, see
http://www.airspayce.com/mikem/bcm2835/index.html
  2. enable i2c on your RPi , see
https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-i2c
  3. connect your i2c devices
  4. then from bash
      $ PATH_I2CDEVLIB=~/i2cdevlib/
      $ gcc -o ADXL345_example_1
"ADXL345_example_1.cpp" 163L, 5859C 2,1           Top
      $ PATH_I2CDEVLIB=~/i2cdevlib/
      $ gcc -o ADXL345_example_1
${PATH_I2CDEVLIB}RaspberryPi_bcm2835/ADXL345/examples/ADXL345_example_1.cpp \
         -I ${PATH_I2CDEVLIB}RaspberryPi_bcm2835/I2Cdev
${PATH_I2CDEVLIB}RaspberryPi_bcm2835/I2Cdev/I2Cdev.cpp \
         -I ${PATH_I2CDEVLIB}/Arduino/ADXL345/
${PATH_I2CDEVLIB}/Arduino/ADXL345/ADXL345.cpp -l bcm2835 -l m
      $ sudo ./ADXL345_example_1

*/
#include <stdint.h>
#include "ADXL345.h"
#include "Communicator.h"
#include "I2Cdev.h"
#include <bcm2835.h>
#include <iostream>
#include <fstream>
#include <json/json.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;
void *sensor_1(void *arg);
void *sensor_2(void *arg);

pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;
struct timeval start_t;


int data_rate = 10;
int data_range = 0;
int s, j;
class Communicator *comm = NULL;
int main(int argc, char **argv) {
 I2Cdev::initialize();
   
//    a.initialize();
  //  b.initialize();
//if (a.testConnection()&& b.testConnection())
  //   printf("Both sensors' connection test successful\n");
  // else {
   //  fprintf(stderr, "ADXL345 connection test failed! exiting ...\n");
    // return 1;
  // }

//    cout << "current data rate of sensor_1 is " << int(a.getRate())<< endl;
//    cout << "current data rate of sensor_2 is " << int(b.getRate()) << endl;
//    a.setRate(data_rate);
//    b.setRate(data_rate);
//    cout << "data rate of sensor_1 after change " <<int(a.getRate()) << endl;
//    cout << "data rate of sensor_2 after change " << int(b.getRate()) << endl;

//    cout << "current data range of sensor_1 is " << int(a.getRange()) << endl;
//    cout << "current data range of sensor_2 is " << int(b.getRange()) << endl;
//    a.setRange(data_range);
//    b.setRange(data_range);
//    cout << "data range of sensor_1 after change" << int(a.getRange()) << endl;
//    cout << "data range of sensor_2 after change" << int(b.getRange()) << endl;
 comm = new Communicator("1", "131.204.27.59", 1883);
   gettimeofday(&start_t, NULL);
    printf("start time : %lld\n", start_t.tv_sec * (uint64_t)1000000+ start_t.tv_usec);

    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, sensor_1, NULL);
    pthread_create(&thread2, NULL, sensor_2, NULL);
    //pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
	return 0;
}

void *sensor_1(void *arg)

{
Json::FastWriter fw;
Json::Value root;
    int16_t x, y, z;
    long long diff;
    cpu_set_t mask ;
    cpu_set_t get;
    int i, cpus=0;
    CPU_ZERO(&mask);
    CPU_SET(2, &mask);
int msg_index = 1;

ADXL345 a;
a.initialize();
struct timeval end_t;    
	if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0) {
        	fprintf(stderr, "set thread affinity failed\n");
    	}

//     CPU_ZERO(&get);
//     if (pthread_getaffinity_np(pthread_self(), sizeof(get), &get) < 0) {
 //        fprintf(stderr, "get thread affinity failed\n");
 //    }   
	 while(true){
    pthread_mutex_lock(&qlock);
		a.getAcceleration(&x, &y, &z);
   pthread_mutex_unlock(&qlock);    		
gettimeofday(&end_t, NULL);
    		diff = (end_t.tv_sec - start_t.tv_sec) * (uint64_t)1000000 +
           (end_t.tv_usec - start_t.tv_usec);
         root["rpi_id"] = 1;
   	root["sensor_id"] = 1;
   	root["x"] =x;
    	root["y"] = y;
    	root["z"] = z;
    	root["elapsed_time"] = diff;
    	root["msg_index"] = msg_index;
 //	cout << fw.write(root);
    string json = fw.write(root);
    const char *j = json.c_str();
//	publish to broker
    comm->send_message(j);
  //  pthread_mutex_lock(&qlock);
    msg_index++;
    //pthread_mutex_unlock(&qlock);
}
    return NULL;

}


void *sensor_2(void *arg)
{Json::FastWriter fw;
Json::Value root;
    long long diff;
    struct timeval end_t;
    int16_t x, y, z;
    cpu_set_t mask ;
    cpu_set_t get;
    int i, cpus=0;
    CPU_ZERO(&mask);
    CPU_SET(1, &mask);
int msg_index = 1;

ADXL345 b(ADXL345_ADDRESS_ALT_HIGH);
b.initialize();
    if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0) {
        fprintf(stderr, "set thread affinity failed\n");
    }

     //CPU_ZERO(&get);
     //if (pthread_getaffinity_np(pthread_self(), sizeof(get), &get) < 0) {
       //  fprintf(stderr, "get thread affinity failed\n");
    // }
    while(true){
     
   gettimeofday(&end_t, NULL);
pthread_mutex_lock(&qlock);
                b.getAcceleration(&x, &y, &z);
  pthread_mutex_unlock(&qlock);
        diff = (end_t.tv_sec - start_t.tv_sec) * (uint64_t)1000000 +
           (end_t.tv_usec - start_t.tv_usec);
        root["rpi_id"] = 1;
        root["sensor_id"] = 2;
        root["x"] = x;
        root["y"] = y;
        root["z"] = z;
        root["elapsed_time"] = diff;
        root["msg_index"] = msg_index;
// 	cout << fw.write(root);
        string json = fw.write(root);
        const char *j = json.c_str();
//	publish to broker
        comm->send_message(j);
   // pthread_mutex_lock(&qlock);
        msg_index++;
     //   pthread_mutex_unlock(&qlock);
}
    return NULL;
}






