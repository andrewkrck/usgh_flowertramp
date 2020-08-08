import bge
import mathutils
import struct
import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish

client = mqtt.Client()
# quat = 1.0, 0.0, 0.0, 0.0
global data, msg_mqtt


def on_connect(client, userdata, flags, rc):
    client.subscribe("adam/#")
    # print("Subscribe!")


def on_message(client, userdata, msg):
    global data, msg_mqtt
    data = struct.unpack('ffff', msg.payload)
    msg_mqtt = msg.topic.split('/')


def mqtt_data():
    global data, msg_mqtt
    qw, qx, qy, qz = [float(s) for s in data]
    arma_mqtt, bone_mqtt = [s for s in msg_mqtt]
    print(qw, qx, qy, qz)
    quat = mathutils.Quaternion((qw, qx, qy, qz))
    scene = bge.logic.getCurrentScene()
    cont = bge.logic.getCurrentController()
    arma = scene.objects[arma_mqtt]
    bone = arma.channels[bone_mqtt]
    bone.rotation_quaternion = quat
    arma.update()


def mqtt_connect():
    client.connect("127.0.0.1", 1883, 60)
    client.on_connect = on_connect
    client.on_message = on_message
    client.loop_start()


def mqtt_start():
    msg = "start"
    publish.single("main", msg, hostname="127.0.0.1")


def mqtt_stop():
    msg = "stop"
    publish.single("main", msg, hostname="127.0.0.1")


def mqtt_calib():
    msg = "calib"
    publish.single("main", msg, hostname="127.0.0.1")