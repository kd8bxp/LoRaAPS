#works with python3
# based on code from https://www.digi.com/resources/documentation/Digidocs/90001541/reference/r_example_subscribe_mqtt.htm
# and https://www.w3schools.com/python/python_file_write.asp

import paho.mqtt.client as mqtt


def on_connect(client, userdata, flags, rc):  # The callback for when the client connects to the broker
    print("Connected with result code {0}".format(str(rc)))  # Print result of connection attempt
    client.subscribe("/hamradio/loraradio/message/pager/loraham/v2/")  # Subscribe to the topic “/hamradio/loraradio/message/pager/loraham/v2/”, receive any messages published on it


def on_message(client, userdata, msg):  # The callback for when a PUBLISH message is received from the server.
    #print("Message received-> " + msg.topic + " " + str(msg.payload))  # Print a received msg
    print(str(msg.payload))
    f = open("LoRaAPS_History.txt","a")
    f.write(str(msg.payload))
    f.write("\n")
    f.close()

client = mqtt.Client("LoRaAPSMQTT328094")  # Create instance of client with client ID “digi_mqtt_test”
client.on_connect = on_connect  # Define callback function for successful connection
client.on_message = on_message  # Define callback function for receipt of a message
# client.connect("m2m.eclipse.org", 1883, 60)  # Connect to (broker, port, keepalive-time)
client.connect('broker.mqtt-dashboard.com', 1883)
client.loop_forever()  # Start network
