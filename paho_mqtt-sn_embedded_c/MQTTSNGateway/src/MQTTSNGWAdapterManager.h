/**************************************************************************************
 * Copyright (c) 2018, Tomoaki Yamaguchi
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

#ifndef MQTTSNGATEWAY_SRC_MQTTSNGWADAPTERMANAGER_H_
#define MQTTSNGATEWAY_SRC_MQTTSNGWADAPTERMANAGER_H_

#include "MQTTSNGWAggregater.h"
#include "MQTTSNGWQoSm1Proxy.h"
namespace MQTTSNGW
{
class Gateway;
class Client;
class QoSm1Proxy;
class Aggregater;
class ForwarderList;
class Forwarder;
class MQTTSNPacket;
class MQTTSNGWPacket;
class ClientRecvTask;
class ClientSendTask;

/*=====================================
     Class AdapterManager
 =====================================*/
class AdapterManager
{
public:
	AdapterManager(Gateway* gw);
    ~AdapterManager(void);
    void initialize(void);
    ForwarderList* getForwarderList(void);
    QoSm1Proxy* getQoSm1Proxy(void);
    Aggregater* getAggregater(void);
    void checkConnection(void);

    bool isAggregatedClient(Client* client);
    Client* getClient(Client& client);
    Client* convertClient(uint16_t msgId, uint16_t* clientMsgId);
    int unicastToClient(Client* client, MQTTSNPacket* packet, ClientSendTask* task);
    bool isAggregaterActive(void);
    AggregateTopicElement* createClientList(Topic* topic);
    int addAggregateTopic(Topic* topic, Client* client);
    void removeAggregateTopic(Topic* topic, Client* client);
    void removeAggregateTopicList(Topics* topics, Client* client);

private:
    Gateway* _gateway {nullptr};
    ForwarderList* _forwarders {nullptr};
    QoSm1Proxy*  _qosm1Proxy {nullptr};
    Aggregater* _aggregater {nullptr};
};




}
#endif /* MQTTSNGATEWAY_SRC_MQTTSNGWADAPTERMANAGER_H_ */
