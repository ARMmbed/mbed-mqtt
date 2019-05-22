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
#include "MQTTSNGWForwarder.h"
#include "SensorNetwork.h"

#include <string.h>

using namespace MQTTSNGW;
using namespace std;

/*=====================================
     Class ForwarderList
 =====================================*/

ForwarderList::ForwarderList()
{
    _head = nullptr;
}

ForwarderList::~ForwarderList()
{
    if ( _head )
    {
        Forwarder* p = _head;
        while ( p )
        {
            Forwarder* next = p->_next;
            delete p;
            p = next;
        }
    }
}


void ForwarderList::initialize(Gateway* gw)
{
    char param[MQTTSNGW_PARAM_MAX];
    string fileName;

    if (gw->getParam("Forwarder", param) == 0 )
    {
        if (!strcasecmp(param, "YES") )
        {
        	gw->getClientList()->setClientList(FORWARDER_TYPE);
        }
    }
}


Forwarder* ForwarderList::getForwarder(SensorNetAddress* addr)
{
    Forwarder* p = _head;
    while ( p )
    {
        if ( p->_sensorNetAddr.isMatch(addr) )
        {
            break;
        }
        p = p->_next;
    }
    return p;
}

Forwarder* ForwarderList::addForwarder(SensorNetAddress* addr,  MQTTSNString* forwarderId)
{
    Forwarder* fdr = new Forwarder(addr, forwarderId);
    if ( _head == nullptr )
    {
        _head = fdr;
    }
    else
    {
        Forwarder* p = _head;
        while ( p )
        {
            if ( p->_next == nullptr )
            {
                p->_next = fdr;
                break;
            }
            else
            {
                p = p->_next;
            }
        }
    }
    return fdr;
}

Forwarder::Forwarder()
{
    _headClient = nullptr;
    _next = nullptr;
}

/*=====================================
     Class ForwarderList
 =====================================*/

Forwarder::Forwarder(SensorNetAddress* addr,  MQTTSNString* forwarderId)
{
    _forwarderName = string(forwarderId->cstring);
    _sensorNetAddr = *addr;
    _headClient = nullptr;
    _next = nullptr;
}

Forwarder::~Forwarder(void)
{
    if ( _headClient )
    {
        ForwarderElement* p = _headClient;
        while ( p )
        {
            ForwarderElement* next = p->_next;
            delete p;
            p = next;
        }
    }
}

const char* Forwarder::getId(void)
{
    return _forwarderName.c_str();
}

void Forwarder::addClient(Client* client, WirelessNodeId* id)
{
    ForwarderElement* p = _headClient;
    ForwarderElement* prev = nullptr;

    client->setForwarder(this);

    if ( p != nullptr )
    {
        while ( p )
        {
            if ( p->_client == client )
            {
                client->setForwarder(this);
                return;
            }
            prev = p;
            p = p->_next;
        }
    }

    ForwarderElement* fclient = new ForwarderElement();

    fclient->setClient(client);
    fclient->setWirelessNodeId(id);

    if ( prev )
    {
        prev->_next = fclient;
    }
    else
    {
        _headClient = fclient;
    }
}

Client* Forwarder::getClient(WirelessNodeId* id)
{
    Client* cl = nullptr;
    _mutex.lock();
    ForwarderElement* p = _headClient;
    while ( p )
    {
        if ( *(p->_wirelessNodeId) == *id )
        {
            cl = p->_client;
            break;
        }
        else
        {
            p = p->_next;
        }
    }
    _mutex.unlock();
    return cl;
}

const char* Forwarder::getName(void)
{
    return _forwarderName.c_str();
}

WirelessNodeId* Forwarder::getWirelessNodeId(Client* client)
{
    WirelessNodeId* nodeId = nullptr;
    _mutex.lock();
    ForwarderElement* p = _headClient;
    while ( p )
    {
        if ( p->_client == client )
        {
            nodeId = p->_wirelessNodeId;
            break;
        }
        else
        {
            p = p->_next;
        }
    }
    _mutex.unlock();
    return nodeId;
}

void Forwarder::eraseClient(Client* client)
{
    ForwarderElement* prev = nullptr;
    _mutex.lock();
    ForwarderElement* p = _headClient;

    while ( p )
    {
        if ( p->_client == client )
        {
            if ( prev )
            {
                prev->_next = p->_next;
            }
            else
            {
                _headClient = p->_next;
            }
            delete p;
            break;
        }
        else
        {
            p = p->_next;
        }
    }
    _mutex.unlock();
}

SensorNetAddress* Forwarder::getSensorNetAddr(void)
{
    return &_sensorNetAddr;
}

/*
 *    Class ForwardedClient
 */

ForwarderElement::ForwarderElement()
    : _client{0}
    , _wirelessNodeId{0}
    , _next{0}
{
}

ForwarderElement::~ForwarderElement()
{
    if (_wirelessNodeId)
    {
        delete _wirelessNodeId;
    }
}

void ForwarderElement::setClient(Client* client)
{
    _client = client;
}

void ForwarderElement::setWirelessNodeId(WirelessNodeId* id)
{
    if ( _wirelessNodeId == nullptr )
    {
        _wirelessNodeId = new WirelessNodeId();
    }
    _wirelessNodeId->setId(id);
}
