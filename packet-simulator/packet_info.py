import pygame
from pygame.event import set_allowed
import constants
# enum of packet types
packet_type = {
    'CONTROL': 'Control Packet',
    'FIRST_IN_SESSION': 'First Packet in Session',
    'NORMAL_SESSION': 'Normal Session Packet'
}


class Packet:
    def __init__(self, x, y, width, height, color, speed, type, session_id, packet_id, weight):
        self.x = x
        self.y = y
        # self.final_x = x
        # self.final_y = y
        self.width = width
        self.height = height
        self.color = color
        self.speed = speed
        self.type = type
        self.session_id = session_id
        self.packet_id = packet_id
        self.weight = weight
        self.rect = pygame.Rect(self.x, self.y, self.width, self.height)

    def draw(self, screen):
        # self.rect.move_ip(self.x, self.y)
        self.rect = pygame.Rect(self.x, self.y, self.width, self.height)
        pygame.draw.rect(screen, self.color, self.rect, width=1)

    def fill_details(self, screen):

        font = pygame.font.SysFont('Arial', 20)
        packet_type_surface = font.render(
            self.type, True, pygame.Color('white'))
        session_details = font.render(
            f"Session: {self.session_id} Packet: {self.packet_id}", True, pygame.Color('white'))
        weight_details = font.render(
            f"Weight: {self.weight}", True, pygame.Color('white'))
        packet_type_rect = packet_type_surface.get_rect(
            center=(self.x + self.width / 2, self.y + self.height / 4))
        session_details_rect = session_details.get_rect(
            center=(self.x + self.width / 2, self.y + self.height / 4 + 20))
        weight_details_rect = weight_details.get_rect(
            center=(self.x + self.width / 2, self.y + self.height / 4 + 40))
        screen.blit(packet_type_surface, packet_type_rect)
        if self.type == packet_type["CONTROL"]:
            return
        screen.blit(session_details, session_details_rect)
        screen.blit(weight_details, weight_details_rect)

    # def update(self, x, y, screen):
    #     i = 0
    #     self.final_x = x
    #     self.final_y = y
    #     if self.x != self.final_x or self.y != self.final_y:
    #         move_x = (self.final_x - self.x) / abs(self.final_x -
    #                                                self.x) if self.x != self.final_x else 0
    #         self.x = self.x + move_x
    #         move_y = (self.final_y - self.y) / abs(self.final_y -
    #                                                self.y) if self.y != self.final_y else 0
    #         self.y = self.y + move_y
    #         print(i, self.x, self.y)
    #         i += 1
    #         self.rect.move_ip(self.x, self.y)
    #         self.draw(screen)
    #         self.fill_details(screen)
    #     self.draw(screen)
    #     self.fill_details(screen)


class PacketQueue:
    def __init__(self, x, y):
        self.packets = []
        self.x = x
        self.y = y

    def add_packet(self, packet):
        self.packets.append(packet)

    def draw_packets(self, screen):
        x = self.x
        y = self.y
        for packet in self.packets:
            packet.final_x = x
            packet.final_y = y
            print(x, y)
            packet.draw(screen)
            packet.fill_details(screen)
            y = y + packet.height + 10
