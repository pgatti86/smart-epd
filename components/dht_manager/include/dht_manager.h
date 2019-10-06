/* 

	DHT22 temperature sensor driver

*/

#ifndef DHT22_H_  
#define DHT22_H_

#define DHT_OK 0
#define DHT_CHECKSUM_ERROR -1
#define DHT_TIMEOUT_ERROR -2

// == function prototypes =======================================

void 	dht_manager_setDhtGpio(int gpio);
void 	dht_manager_startReading();
void 	dht_manager_stopReading();
float 	dht_manager_getHumidity();
float 	dht_manager_getTemperature();

#endif
