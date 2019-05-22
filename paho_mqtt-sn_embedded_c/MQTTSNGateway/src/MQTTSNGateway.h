/**************************************************************************************
 * Copyright (c) 2016, Tomoaki Yamaguchi
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Tomoaki Yamaguchi - initial API and implementation and/or initial documentation
 **************************************************************************************/
#ifndef MQTTSNGATEWAY_H_
#define MQTTSNGATEWAY_H_

#include <MQTTSNGWAdapterManager.h>
#include "MQTTSNGWProcess.h"
#include "MQTTSNPacket.h"
#include "MQTTSNGWClient.h"

namespace MQTTSNGW
{
/*=================================
 *    Starting prompt
 ==================================*/
#define PAHO_COPYRIGHT0 " * MQTT-SN Transparent Gateway"
#define PAHO_COPYRIGHT1 " * Part of Project Paho in Eclipse"
#define PAHO_COPYRIGHT2 " * (http://git.eclipse.org/c/paho/org.eclipse.paho.mqtt-sn.embedded-c.git/)"
#define PAHO_COPYRIGHT3 " * Author : Tomoaki YAMAGUCHI"
#define PAHO_COPYRIGHT4 " ***************************************************************************"
/*==========================================================
 *           Log Formats
 *
 *           RED    : \033[0m\033[1;31m
 *           green  : \033[0m\033[0;32m
 *           yellow : \033[0m\033[0;33m
 *           blue   : \033[0m\033[0;34m
 *           white  : \033[0m\033[0;37m
 ===========================================================*/
#define CLIENT      "Client"
#define CLIENTS     "Clients"
#define UNKNOWNCL   "Unknown Client !"

#define LEFTARROW   "<---"
#define RIGHTARROW  "--->"
#define LEFTARROWB   "<==="
#define RIGHTARROWB  "===>"

#define FORMAT_Y_G_G_NL        "\n%s   \033[0m\033[0;33m%-18s\033[0m\033[0;32m%-6s%-34.32s \033[0m\033[0;34m%s\033[0m\033[0;37m\n"
#define FORMAT_Y_G_G             "%s   \033[0m\033[0;33m%-18s\033[0m\033[0;32m%-6s%-34.32s \033[0m\033[0;34m%s\033[0m\033[0;37m\n"
#define FORMAT_Y_Y_G             "%s   \033[0m\033[0;33m%-18s%-6s\033[0m\033[0;32m%-34.32s \033[0m\033[0;34m%s\033[0m\033[0;37m\n"
#define FORMAT_Y_W_G             "%s   \033[0m\033[0;33m%-18s\033[0m\033[0;37m%-6s\033[0m\033[0;32m%-34.32s \033[0m\033[0;34m%s\033[0m\033[0;37m\n"
#define FORMAT_Y_Y_W             "%s   \033[0m\033[0;33m%-18s%-6s\033[0m\033[0;37m%-34.32s \033[0m\033[0;34m%s\033[0m\033[0;37m\n"

#define FORMAT_G_MSGID_G_G_NL  "\n%s   \033[0m\033[0;32m%-11s%-5s  %-6s%-34.32s \033[0m\033[0;34m%s\033[0m\033[0;37m\n"
#define FORMAT_G_MSGID_G_G       "%s   \033[0m\033[0;32m%-11s%-5s  %-6s%-34.32s \033[0m\033[0;34m%s\033[0m\033[0;37m\n"
#define FORMAT_G_MSGID_W_G       "%s   \033[0m\033[0;32m%-11s%-5s  \033[0m\033[0;37m%-6s\033[0m\033[0;32m%-34.32 s\033[0m\033[0;34m%s\033[0m\033[0;37m\n"
#define FORMAT_G_MSGID_Y_W       "%s   \033[0m\033[0;32m%-11s%-5s  \033[0m\033[0;33m%-6s\033[0m\033[0;37m%-34.32s \033[0m\033[0;34m%s\033[0m\033[0;37m\n"

#define FORMAT_W_MSGID_Y_W_NL  "\n%s   %-11s%-5s  \033[0m\033[0;33m%-6s\033[0m\033[0;37m%-34.32s \033[0m\033[0;34m%s\033[0m\033[0;37m\n"
#define FORMAT_W_MSGID_Y_W       "%s   %-11s%-5s  \033[0m\033[0;33m%-6s\033[0m\033[0;37m%-34.32s \033[0m\033[0;34m%s\033[0m\033[0;37m\n"
#define FORMAT_W_MSGID_W_G       "%s   %-11s%-5s  %-6s\033[0m\033[0;32m%-34.32s \033[0m\033[0;34m%s\033[0m\033[0;37m\n"
#define FORMAT_W_MSGID_G_G       "%s   %-11s%-5s  \033[0m\033[0;32m%-6s%-34.32s \033[0m\033[0;34m%s\033[0m\033[0;37m\n"

#define FORMAT_BL_NL           "\n%s   \033[0m\033[0;34m%-18s%-6s%-34.32s %s\033[0m\033[0;37m\n"
#define FORMAT_W_NL            "\n%s   %-18s%-6s%-34.32s %s\n"

#define ERRMSG_HEADER            "\033[0m\033[0;31mError:"
#define ERRMSG_FOOTER            "\033[0m\033[0;37m"

/*=====================================
         Class Event
  ====================================*/
class Client;

enum EventType{
	Et_NA = 0,
	EtStop,
	EtTimeout,
	EtBrokerRecv,
	EtBrokerSend,
	EtClientRecv,
	EtClientSend,
	EtBroadcast,
	EtSensornetSend
};


class Event{
public:
	Event();
	~Event();
	EventType getEventType(void);
	void setClientRecvEvent(Client*, MQTTSNPacket*);
	void setClientSendEvent(Client*, MQTTSNPacket*);
	void setBrokerRecvEvent(Client*, MQTTGWPacket*);
	void setBrokerSendEvent(Client*, MQTTGWPacket*);
	void setBrodcastEvent(MQTTSNPacket*);  // ADVERTISE and GWINFO
	void setTimeout(void);                 // Required by EventQue<Event>.timedwait()
	void setStop(void);
	void setClientSendEvent(SensorNetAddress*, MQTTSNPacket*);
	Client* getClient(void);
	SensorNetAddress* getSensorNetAddress(void);
	MQTTSNPacket* getMQTTSNPacket(void);
	MQTTGWPacket* getMQTTGWPacket(void);

private:
	EventType   _eventType {Et_NA};
	Client*     _client {nullptr};
	SensorNetAddress* _sensorNetAddr {nullptr};
	MQTTSNPacket* _mqttSNPacket {nullptr};
	MQTTGWPacket* _mqttGWPacket {nullptr};
};


/*=====================================
 Class EventQue
 ====================================*/
class EventQue
{
public:
	EventQue();
	~EventQue();
	Event* wait(void);
	Event* timedwait(uint16_t millsec);
	void setMaxSize(uint16_t maxSize);
	void post(Event*);
	int  size();

private:
	Que<Event> _que;
	Mutex      _mutex;
	Semaphore  _sem;
};



/*=====================================
 Class GatewayParams
 ====================================*/
class GatewayParams
{
public:
     string configDir;
	char* configName {nullptr};
	char* clientListName {nullptr};
	char* loginId {nullptr};
	char* password {nullptr};
	uint16_t keepAlive {0};
	uint8_t  gatewayId {0};
	uint8_t  mqttVersion {0};
	uint16_t maxInflightMsgs {0};
	char* gatewayName {nullptr};
	char* brokerName {nullptr};
	char* port {nullptr};
	char* portSecure {nullptr};
	char* rootCApath {nullptr};
	char* rootCAfile {nullptr};
	char* certKey {nullptr};
	char* privateKey {nullptr};
	char* predefinedTopicFileName {nullptr};
	char* qosMinusClientListName {nullptr};
	bool  clientAuthentication {false};
};



/*=====================================
     Class Gateway
 =====================================*/
class AdapterManager;
class ClientList;

class Gateway: public MultiTaskProcess{
public:
    Gateway(void);
	~Gateway();
	virtual void initialize(int argc, char** argv);
	void run(void);

	EventQue* getPacketEventQue(void);
	EventQue* getClientSendQue(void);
	EventQue* getBrokerSendQue(void);
	ClientList* getClientList(void);
	SensorNetwork* getSensorNetwork(void);
	LightIndicator* getLightIndicator(void);
	GatewayParams* getGWParams(void);
	AdapterManager* getAdapterManager(void);
	int getParam(const char* parameter, char* value);
	bool hasSecureConnection(void);
	Topics* getTopics(void);

private:
	GatewayParams  _params;
	ClientList* _clientList {nullptr};
	EventQue   _packetEventQue;
	EventQue   _brokerSendQue;
	EventQue   _clientSendQue;
	LightIndicator _lightIndicator;
	SensorNetwork  _sensorNetwork;
	AdapterManager* _adapterManager {nullptr};
	Topics* _topics;
};

}

#endif /* MQTTSNGATEWAY_H_ */
