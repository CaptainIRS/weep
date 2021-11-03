import pygame
from packet_info import Packet, PacketQueue, packet_type
import constants
from packets_data import packets


def set_pos(queue1, FIRST_LINE_X, GAP, moving):
    queue1.sort(key=lambda x: x.weight, reverse=True)
    x = FIRST_LINE_X + GAP/2
    y = 100
    h = constants.PACKET_HEIGHT + GAP
    queue1_updated = []
    for p in queue1:
        dest = (x, y)
        for mp, d in moving:
            if mp == p:
                moving.remove([mp, d])
        moving.append([p, dest])
        y += h
    return moving


def print_queue_title(screen, FIRST_LINE_X, SECOND_LINE_X, THIRD_LINE_X, GAP):
    font = pygame.font.SysFont('Arial', 20)
    queue1_text = font.render('Queue 1', True, pygame.Color('white'))
    queue2_text = font.render('Queue 2', True, pygame.Color('white'))
    queue3_text = font.render('Queue 3', True, pygame.Color('white'))
    queue1_rect = queue1_text.get_rect(
        center=(FIRST_LINE_X+constants.PACKET_WIDTH/2+GAP/2, 60))
    queue2_rect = queue2_text.get_rect(
        center=(SECOND_LINE_X+constants.PACKET_WIDTH/2, 60))
    queue3_rect = queue3_text.get_rect(
        center=(THIRD_LINE_X+constants.PACKET_WIDTH/2, 60))
    screen.blit(queue1_text, queue1_rect)
    screen.blit(queue2_text, queue2_rect)
    screen.blit(queue3_text, queue3_rect)


def print_names(screen):
    font = pygame.font.SysFont('Arial', 25)
    names_text = font.render(
        'By Anirudh (12) and Rinish (102)', True, pygame.Color('white'))
    names_rect = names_text.get_rect(
        center=(constants.WIDTH/2, constants.HEIGHT-30))
    screen.blit(names_text, names_rect)


def print_speed(screen, packet_speed):
    font = pygame.font.SysFont('Arial', 20)
    speed_text = font.render(
        'Speed: ' + str(packet_speed), True, pygame.Color('white'))
    screen.blit(speed_text, (10, constants.HEIGHT-30))


if __name__ == '__main__':
    pygame.init()

    screen = pygame.display.set_mode((constants.WIDTH, constants.HEIGHT))
    pygame.display.set_caption('Packet Organization Simulation')
    fps_clock = pygame.time.Clock()

    moving = list()
    run = True
    pause = False
    cur_packet_index = -1
    uncategorized_packet = []
    new_packet = []
    queue1 = []
    queue2 = []
    queue3 = []
    while run:

        # constants for queue display
        FIRST_LINE_X = 60
        GAP = 20
        SECOND_LINE_X = FIRST_LINE_X + packets[0].width + 5*GAP
        THIRD_LINE_X = SECOND_LINE_X + packets[0].width + 5*GAP

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    pause = not pause
                if event.key == pygame.K_n:
                    cur_packet_index += 1
                    if cur_packet_index < len(packets):
                        uncategorized_packet.append(packets[cur_packet_index])
                        new_packet.append(packets[cur_packet_index])
                if event.key == pygame.K_m:
                    # print(moving, packets[cur_packet_index],
                    #       (FIRST_LINE_X+GAP/2, 100))
                    if len(new_packet) <= 0:
                        continue
                    for p, dest in moving:
                        if p.session_id == new_packet[0].session_id and p.packet_id == new_packet[0].packet_id:
                            moving.remove([p, dest])
                            break
                    dest = ()
                    p = new_packet[0]
                    if p.type == packet_type["CONTROL"]:
                        print("CONTROL")
                        dest = (FIRST_LINE_X+GAP/2, 100)
                    elif p.type == packet_type["FIRST_IN_SESSION"]:
                        print("FIRST_IN_SESSION")
                        dest = (SECOND_LINE_X+GAP/2, 100)
                    elif p.type == packet_type["NORMAL_SESSION"]:
                        print("NORMAL_SESSION")
                        dest = (THIRD_LINE_X+GAP/2, 100)
                    moving.append(
                        [new_packet[0], (dest[0], dest[1])])
                    new_packet.pop(0)
                if event.key == pygame.K_e:
                    constants.PACKET_SPEED += 1
                    for p, dest in moving:
                        p.speed = constants.PACKET_SPEED
                if event.key == pygame.K_q:
                    constants.PACKET_SPEED -= 1
                    for p, dest in moving:
                        p.speed = constants.PACKET_SPEED
        # print("Packet speed", constants.PACKET_SPEED)
        if pause:
            continue
        screen.fill(0)
        updated_moving = list()
        # update packet's coordinates
        for p, dest in moving:
            if(p.x == dest[0] and p.y == dest[1]):
                if p.type == packet_type["CONTROL"] and p not in queue1:
                    print("Added", p.session_id, p.packet_id, "to queue1")
                    queue1.append(p)
                    p.color = pygame.Color('yellow')
                    updated_moving = set_pos(
                        queue1, FIRST_LINE_X, GAP, updated_moving)
                elif p.type == packet_type["FIRST_IN_SESSION"] and p not in queue2:
                    print("Added", p.session_id, p.packet_id, "to queue2")
                    queue2.append(p)
                    p.color = pygame.Color('blue')
                    updated_moving = set_pos(
                        queue2, SECOND_LINE_X, GAP, updated_moving)

                elif p.type == packet_type["NORMAL_SESSION"] and p not in queue3:
                    print("Added", p.session_id, p.packet_id, "to queue3")
                    queue3.append(p)
                    p.color = pygame.Color('green')
                    updated_moving = set_pos(
                        queue3, THIRD_LINE_X, GAP, updated_moving)
                if p in uncategorized_packet:
                    uncategorized_packet.remove(p)
                continue
            move_x = (dest[0] - p.x) / abs(dest[0] -
                                           p.x) if p.x != dest[0] else 0
            p.x = p.x + move_x*p.speed
            move_y = (dest[1] - p.y) / abs(dest[1] -
                                           p.y) if p.y != dest[1] else 0
            p.y = p.y + move_y*p.speed
            updated_moving.append([p, dest])
        moving = updated_moving

        pygame.draw.line(screen, pygame.Color('yellow'),
                         (FIRST_LINE_X, 80), (FIRST_LINE_X, 700), 3)
        pygame.draw.line(screen, pygame.Color('yellow'),
                         (FIRST_LINE_X + packets[0].width + GAP, 80), (FIRST_LINE_X + packets[0].width+GAP, 700), 3)

        pygame.draw.line(screen, pygame.Color('blue'),
                         (SECOND_LINE_X, 80), (SECOND_LINE_X, 700), 3)
        pygame.draw.line(screen, pygame.Color('blue'),
                         (SECOND_LINE_X+packets[0].width+GAP, 80), (SECOND_LINE_X+packets[0].width+GAP, 700), 3)

        pygame.draw.line(screen, pygame.Color('green'),
                         (THIRD_LINE_X, 80), (THIRD_LINE_X, 700), 3)
        pygame.draw.line(screen, pygame.Color('green'),
                         (THIRD_LINE_X+packets[0].width+GAP, 80), (THIRD_LINE_X+packets[0].width+GAP, 700), 3)

        # draw all packets
        # print(queue1, queue2, queue3)
        # print(uncategorized_packet)
        for p in uncategorized_packet:
            p.draw(screen)
            p.fill_details(screen)
        for p in queue1:
            p.draw(screen)
            p.fill_details(screen)
        for p in queue2:
            p.draw(screen)
            p.fill_details(screen)
        for p in queue3:
            p.draw(screen)
            p.fill_details(screen)
        print_queue_title(screen, FIRST_LINE_X,
                          SECOND_LINE_X, THIRD_LINE_X, GAP)
        print_names(screen)
        print_speed(screen, constants.PACKET_SPEED)
        fps_clock.tick(constants.FPS)
        pygame.display.flip()
