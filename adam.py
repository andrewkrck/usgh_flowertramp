bl_info = {
    "name": "MQTT Add-on",
    "author": "flowertrump",
    "location": "View3D > Tools > MQTT Panel",
    "version": (1, 0, 0),
    "blender": (2, 8, 3),
    "description": "...",
    "wiki_url": "http://example.com",
    "category": "Development"
}

import bpy
import freestyle

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
    # print(msg_mqtt, data)
    mqtt_data() # <<<<????


def mqtt_data():
    global data, msg_mqtt

    qw, qx, qy, qz = [float(s) for s in data]
    arma_mqtt, bone_mqtt = [s for s in msg_mqtt]

    # print(arma_mqtt, bone_mqtt)
    # print(qw, qx, qy, qz)

    quat = mathutils.Quaternion((qw, qx, qy, qz))

    arma = bpy.data.objects[arma_mqtt]

    pose_bone = arma.pose.bones[bone_mqtt]

    pose_bone.rotation_quaternion = quat

    pose_bone.keyframe_insert('rotation_quaternion', frame=23)


def mqtt_connect(hostname, port):
    client.connect(host=hostname, port=port, keepalive=60)
    client.on_connect = on_connect
    client.on_message = on_message
    client.loop_start()


def mqtt_start(hostname, port):
    msg = "start"
    publish.single(topic="main", payload=msg, hostname=hostname, port=port)


def mqtt_stop(hostname, port):
    msg = "stop"
    publish.single(topic="main", payload=msg, hostname=hostname, port=port)


def mqtt_calib(hostname, port):
    msg = "calib"
    publish.single(topic="main", payload=msg, hostname=hostname, port=port)


class BaseOperator(bpy.types.Operator):
    bl_idname = "object.base_operator"
    bl_label = "Basic operator"

    hostname = "176.37.6.176"
    port = 1883

    functions_mapping = {
        "mqtt_connect": mqtt_connect,
        "mqtt_start": mqtt_start,
        "mqtt_stop": mqtt_stop,
        "mqtt_calib": mqtt_calib
    }

    def execute(self, context):
        func = self.functions_mapping.get(self.bl_label)
        if func:
            func(self.hostname, self.port)

        return {'FINISHED'}

    @classmethod
    def register(cls):
        print("Registered class: %s " % cls.bl_label)
        # Register properties related to the class here
        bpy.types.Scene.encouraging_message = bpy.props.StringProperty(
            name="",
            description="Message to print to user",
            default="Have a nice day!")

    @classmethod
    def unregister(cls):
        print("Unregistered class: %s " % cls.bl_label)
        # Delete parameters related to the class here
        del bpy.types.Scene.encouraging_message


class MQTTStartOp(BaseOperator):
    bl_idname = "object.mqtt_start"
    bl_label = "mqtt_start"


class MQTTStopOp(BaseOperator):
    bl_idname = "object.mqtt_stop"
    bl_label = "mqtt_stop"


class MQTTConnectOp(BaseOperator):
    bl_idname = "object.mqtt_connect"
    bl_label = "mqtt_connect"


class MQTTCalibOp(BaseOperator):
    bl_idname = "object.mqtt_calib"
    bl_label = "mqtt_calib"


class MQTTPanel(bpy.types.Panel):
    bl_label = "MQTT"
    bl_space_type = "VIEW_3D"
    bl_region_type = "TOOLS"

    def draw(self, context):
        layout = self.layout

        layout.label(text="MQTT Panel: ")

        layout.operator("object.mqtt_start", text="start")
        layout.operator("object.mqtt_stop", text="stop")
        layout.operator("object.mqtt_connect", text="connect")
        layout.operator("object.mqtt_calib", text="calibrate")


def register():
    bpy.utils.register_class(MQTTStartOp)
    bpy.utils.register_class(MQTTStopOp)
    bpy.utils.register_class(MQTTCalibOp)
    bpy.utils.register_class(MQTTConnectOp)
    bpy.utils.register_class(MQTTPanel)


def unregister():
    bpy.utils.unregister_class(MQTTStartOp)
    bpy.utils.unregister_class(MQTTStopOp)
    bpy.utils.unregister_class(MQTTCalibOp)
    bpy.utils.unregister_class(MQTTConnectOp)
    bpy.utils.unregister_class(MQTTPanel)


if __name__ == "__main__":
    register()
