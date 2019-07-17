
"""
A smart home function that can push configuration updates to a Google Cloud IoT device
to control individually a few pins

"""

from flask import jsonify
import base64
import json
from googleapiclient import discovery
from googleapiclient.errors import HttpError


DEVICE   = 'd0123156631436CC0FE'
REGISTRY = 'mini'
REGION   = 'us-central1'
PROJECT  = 'az-masters'

def get_client():
    """Returns an authorized API client by discovering the IoT API"""
    # api_scopes = ['https://www.googleapis.com/auth/cloud-platform']
    api_version = 'v1'
    discovery_api = 'https://cloudiot.googleapis.com/$discovery/rest'
    service_name = 'cloudiotcore'
    discovery_url = '{}?version={}'.format(discovery_api, api_version)
    return discovery.build(service_name, api_version, discoveryServiceUrl=discovery_url, cache_discovery=False)

def send_config(data):
    """Push the data to the given device as configuration."""
    string = data.encode('ASCII')
    body = {
        'version_to_update': 0, # -> always update the config

        # The data is passed as raw bytes, so you encode it as base64
        'binary_data': base64.b64encode(string).decode('ascii')
        # Note that the device will receive the decoded string, so you
        # do not need to base64 decode the string on the device
    }

    device_name = 'projects/{}/locations/{}/registries/{}/devices/{}'.format(
                    PROJECT, REGION, REGISTRY, DEVICE)
    devices = get_client().projects().locations().registries().devices()
    request = devices.modifyCloudToDeviceConfig(name=device_name, body=body)
    try:
        request.execute()
    except HttpError as e:
        # If the server responds with a HtppError, log it here, but
        # continue so that the message does not stay NACK'ed on the
        # pubsub channel.
        print('Error executing ModifyCloudToDeviceConfig: {}'.format(e))

def get_state():
    '''Get the latest state published by the device'''    
    device_name = 'projects/{}/locations/{}/registries/{}/devices/{}'.format(
                    PROJECT, REGION, REGISTRY, DEVICE)
    devices = get_client().projects().locations().registries().devices()
    request = devices.states().list(name=device_name, numStates=1)
    try:
        state = request.execute()
    except HttpError as e:
        print('Error requesting State List: {}'.format(e))
        return {}
#    print('State: {}\n'.format(state))
	# extract JSON object returned by device as binary data
    try:
        binary_data = state['deviceStates'][0]['binaryData']
        decoded_data = base64.b64decode(binary_data)
#        print('Decoded:', decoded_data)
        state_js = json.loads(decoded_data)
#        print('JSON:',state)    
    except:
        state_js = {}
    return state_js


def on_sync(input, requestId):
    deviceInfo = { # standard device info for AVR-IOT board
                      "manufacturer": "Microchip Technology, Inc.",
                      "model": "AVR-IOT WG",
                      "hwVersion": "7",
                      "swVersion": "1.1.1"
                 }
    return jsonify({
        'requestId' : requestId,
        'payload'   : {
            'agentUserId' : '123.456',
            'devices'     : [
               {
                 "id": "1",
                 "type": "action.devices.types.SWITCH",
                 "traits": ["action.devices.traits.OnOff"],
                 "name": {"name": "red LED"},
                 "willReportState": True,
                 "deviceInfo": deviceInfo,
                 "customData": {"color": "red"}
               },
                
               {
                 "id": "2",
                 "type": "action.devices.types.SWITCH",
                 "traits": ["action.devices.traits.OnOff"],
                 "name": {"name": "yellow LED"},
                 "willReportState": True,
                 "deviceInfo": deviceInfo,
                 "customData": {"color": "yellow"}
               } 
            ]
        }
    })

def on_execute(commands, requestId):
    print("Command", commands)
    for command in commands:
        results = []
        config = ''
        for device in command['devices']:
            deviceId = device['id']
            for execution in command['execution']:
                execCommand = execution['command']
                if execCommand == 'action.devices.commands.OnOff':
                    onOff = execution['params']['on']
                    onOffString = ['false','true'][onOff]
                    led   = device['customData']['color']
                    config += f'"{led}":{onOffString},'
                    results.append({
                        'ids'    : [deviceId],
                        'status' : 'SUCCESS',
                        'states' : {
                            'on' : onOff,
                            'online': True
                        }
                    })
  
                else: # command unrecognized
                    results.append({
                        'ids'   : [deviceId], 
                        'status': 'ERROR',
                        'errorCode': 'unrecognized'
                    })
        # do send the device config JSON string
        send_config('{' + config + '}')   
                    
        # truly support only 1 command per payload!                    
        return jsonify({
            'requestId': requestId,
            'payload' : {
                'commands': results
            }
        })

def on_query(devices, requestId):
    state = get_state()
    red = state.get('red', False)
    yel = state.get('yellow', False)
    print('Got QUERY:', red, yel)
 
    return jsonify({
        'requestId': requestId,
        'payload'  : {
            'devices'  : {
                device['id']: {
                  'on'    : {"red":red,"yellow":yel}[device['customData']['color']],
                  'online': True,
                } for device in devices}
            }
        })
   

###### function main entry point  ########

def handle_device(request):
    body = request.get_json(force=True)
    #print('body = ', body)
    requestId = body['requestId']
    inputs = body['inputs']
    for input in inputs:
        intent = input['intent']
        if intent == 'action.devices.SYNC':
            return on_sync(input, requestId)
        elif intent == 'action.devices.EXECUTE':
            return on_execute(input['payload']['commands'], requestId)
        elif intent == 'action.devices.QUERY':
            return on_query(input['payload']['devices'], requestId)
#        else: ignore ...
    return jsonify("{ 'requestId':" + requestId + "}")

