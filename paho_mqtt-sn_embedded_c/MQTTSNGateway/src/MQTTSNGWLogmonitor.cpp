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
#define RINGBUFFER

#include "MQTTSNGWProcess.h"
#include "MQTTSNGWLogmonitor.h"
#include <stdio.h>

using namespace std;
using namespace MQTTSNGW;

Logmonitor::Logmonitor()
{
	theProcess = this;
}

Logmonitor::~Logmonitor()
{

}

void Logmonitor::run()
{
	while (true)
	{
		const char* data = getLog();
		if ( *data == 0 )
		{
			break;
		}
		else
		{
			printf("%s", data);
		}
	}
}

