bl_info = {
    "name": "MQTT Add-on",
    "author": "flowertrump",
    "location": "View3D > Tools > MQTT Panel",
    "version": (1, 0, 0),
    "blender": (2, 8, 3),
    "description": "...",
    "wiki_url": "http://example.com",
    "category": "Development",
}

import bpy
import mathutils
import struct
import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish

FPS = 24
HOSTNAME = "176.37.6.176"
PORT = 1883

global data, msg_mqtt, client_obj


# counter = 0


def on_connect(client, userdata, flags, rc):
    client.subscribe("adam/#")
    # print("Subscribe!")


def _handle_msg(msg):
    data = struct.unpack("ffff", msg.payload)
    msg_mqtt = msg.topic.split("/")
    qw, qx, qy, qz = [float(s) for s in data]
    quaternion = mathutils.Quaternion((qw, qx, qy, qz))
    armature_name, bone_name = [s for s in msg_mqtt]
    return armature_name, bone_name, quaternion


def on_message(client, userdata, msg):
    global data, msg_mqtt
    data = struct.unpack("ffff", msg.payload)
    msg_mqtt = msg.topic.split("/")
    mqtt_data()
    # print(msg_mqtt, data)


def _move_bone(armature_name, bone_name, quaternion):
    armature_obj = bpy.data.objects[armature_name]
    pose_bone = armature_obj.pose.bones[bone_name]
    pose_bone.rotation_quaternion = quaternion
    pose_bone.keyframe_insert(data_path="rotation_quaternion")


def mqtt_data():
    global data, msg_mqtt

    # print("mqtt_data")
    # rate = 1.0 / FPS

    # global counter
    # counter += 1

    # if data and msg_mqtt:
    qw, qx, qy, qz = [float(s) for s in data]
    quaternion = mathutils.Quaternion((qw, qx, qy, qz))
    armature_name, bone_name = [s for s in msg_mqtt]

    print(armature_name, bone_name)
    print(qw, qx, qy, qz)

    _move_bone(armature_name, bone_name, quaternion)

    # if counter == FPS:
    #     return None
    # return rate


def mqtt_connect(context, hostname, port):
    global client_obj

    client_obj = mqtt.Client()

    client_obj.connect(host=hostname, port=port, keepalive=60)
    client_obj.on_connect = on_connect
    client_obj.on_message = on_message
    client_obj.loop_start()

    # bpy.app.timers.register(mqtt_data)


def mqtt_disconnect(context, hostname, port):
    # bpy.app.timers.unregister(mqtt_data)
    client_obj.loop_stop()


def mqtt_start(context, hostname, port):
    msg = "start"
    publish.single(topic="main", payload=msg, hostname=hostname, port=port)


def mqtt_stop(context, hostname, port):
    msg = "stop"
    publish.single(topic="main", payload=msg, hostname=hostname, port=port)


def mqtt_calib(context, hostname, port):
    msg = "calib"
    publish.single(topic="main", payload=msg, hostname=hostname, port=port)


class BaseOperator(bpy.types.Operator):
    bl_idname = "object.base_operator"
    bl_label = "Basic operator"

    hostname = HOSTNAME
    port = PORT

    functions_mapping = {
        "mqtt_disconnect": mqtt_disconnect,
        "mqtt_connect": mqtt_connect,
        "mqtt_start": mqtt_start,
        "mqtt_stop": mqtt_stop,
        "mqtt_calib": mqtt_calib,
    }

    def execute(self, context):
        func = self.functions_mapping.get(self.bl_label)
        if func:
            func(context, self.hostname, self.port)
        else:
            print(f"ERROR: function {self.bl_label} not found")

        return {"FINISHED"}

    @classmethod
    def register(cls):
        print("Registered class: %s " % cls.bl_label)
        # Register properties related to the class here
        # bpy.types.Scene.encouraging_message = bpy.props.StringProperty(
        #     name="", description="Message to print to user", default="Have a nice day!"
        # )

    @classmethod
    def unregister(cls):
        print("Unregistered class: %s " % cls.bl_label)
        # Delete parameters related to the class here
        # del bpy.types.Scene.encouraging_message


class MQTTStartOp(BaseOperator):
    bl_idname = "object.mqtt_start"
    bl_label = "mqtt_start"


class MQTTStopOp(BaseOperator):
    bl_idname = "object.mqtt_stop"
    bl_label = "mqtt_stop"


class MQTTConnectOp(BaseOperator):
    bl_idname = "object.mqtt_connect"
    bl_label = "mqtt_connect"


class MQTTDisconnectOp(BaseOperator):
    bl_idname = "object.mqtt_disconnect"
    bl_label = "mqtt_disconnect"


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
        layout.operator("object.mqtt_disconnect", text="disconnect")

        layout.operator("object.mqtt_calib", text="calibrate")


def register():
    # bpy.types.Scene.mqtt_client = bpy.props.PointerProperty(type=mqtt.Client)

    bpy.utils.register_class(MQTTStartOp)
    bpy.utils.register_class(MQTTStopOp)
    bpy.utils.register_class(MQTTCalibOp)
    bpy.utils.register_class(MQTTConnectOp)
    bpy.utils.register_class(MQTTDisconnectOp)
    bpy.utils.register_class(MQTTPanel)


def unregister():
    bpy.utils.unregister_class(MQTTPanel)
    bpy.utils.unregister_class(MQTTStartOp)
    bpy.utils.unregister_class(MQTTStopOp)
    bpy.utils.unregister_class(MQTTCalibOp)
    bpy.utils.unregister_class(MQTTConnectOp)
    bpy.utils.unregister_class(MQTTDisconnectOp)


if __name__ == "__main__":
    register()
