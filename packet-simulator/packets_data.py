import pygame
from packet_info import packet_type
from packet_info import Packet
import constants
# array of [packet_type, session_id, packet_id]
packet_data = [[packet_type["CONTROL"], 0, 0, 1],
               [packet_type["FIRST_IN_SESSION"], 1, 1, 2],
               [packet_type["FIRST_IN_SESSION"], 2, 1, 3],
               [packet_type["CONTROL"], 0, 0, 4],
               [packet_type["NORMAL_SESSION"], 2, 2, 5],
               [packet_type["NORMAL_SESSION"], 1, 3, 6],
               [packet_type["FIRST_IN_SESSION"], 3, 1, 7],
               [packet_type["NORMAL_SESSION"], 1, 2, 8],
               [packet_type["NORMAL_SESSION"], 3, 3, 9],
               ]

packets = []
for p in packet_data:
    new_packet = Packet(constants.NEW_PACKET_X, constants.NEW_PACKET_Y, constants.PACKET_WIDTH,
                        constants.PACKET_HEIGHT, pygame.Color('red'), constants.PACKET_SPEED, p[0], p[1], p[2], p[3])
    packets.append(new_packet)
