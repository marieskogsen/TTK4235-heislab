#include "elevator.h"
#include "hardware.h"
#include "elevator_driver.h"
#include "queue_handler.h"
#include <stdlib.h>
#include <stdio.h>

void queue_handler_clear_queue(elevator_t *e){
    for (int floor = 0; floor < QUEUE_FLOOR; floor++) {
        for (int order = 0; order < QUEUE_ORDERS; order++) {
            e->queue[floor][order] = 0;
            hardware_command_order_light(floor,HARDWARE_ORDER_DOWN, 0);
            hardware_command_order_light(floor,HARDWARE_ORDER_UP, 0);
        }
    }
}

void queue_handler_update_queue(elevator_t *e){
        if (hardware_read_order(0, HARDWARE_ORDER_UP)){
            e->queue[0][ORDER_UP] = 1;
            printf("%d", 1);
            hardware_command_order_light(0, HARDWARE_ORDER_UP, 1);
        }
        if (hardware_read_order(1, HARDWARE_ORDER_UP)){
            e->queue[1][ORDER_UP] = 1;
            printf("%d",2);
            hardware_command_order_light(1, HARDWARE_ORDER_UP, 1);
        }
        if (hardware_read_order(1, HARDWARE_ORDER_DOWN)){
            e->queue[1][ORDER_DOWN] = 1;
            printf("%d",2);
            hardware_command_order_light(1, HARDWARE_ORDER_DOWN, 1);
        }
        if (hardware_read_order(2, HARDWARE_ORDER_UP)){
            e->queue[2][ORDER_UP] = 1;
            printf("%d",2);
            hardware_command_order_light(2, HARDWARE_ORDER_UP, 1);
        }
        if (hardware_read_order(2, HARDWARE_ORDER_DOWN)){
            e->queue[2][ORDER_DOWN] = 1;
            printf("%d",2);
            hardware_command_order_light(2, HARDWARE_ORDER_DOWN, 1);
        }
        if (hardware_read_order(3, HARDWARE_ORDER_DOWN)){
            e->queue[3][ORDER_DOWN] = 1;
            printf("%d",3);
            hardware_command_order_light(3, HARDWARE_ORDER_DOWN, 1);
        }
      }


void queue_handler_choose_direction(elevator_t *e){ 
  if (e->current_dir == HARDWARE_MOVEMENT_UP) {
    e->current_dir = HARDWARE_MOVEMENT_STOP;
    for (int floor = e->last_floor; floor < QUEUE_FLOOR; floor++) {
      if (e->queue[floor][ORDER_UP]){
        e->current_dir = HARDWARE_MOVEMENT_UP;
      }
    }
  }
  if (e->current_dir == HARDWARE_MOVEMENT_DOWN) {
    e->current_dir = HARDWARE_MOVEMENT_STOP;
    for (int floor = e->current_floor; floor >= 0; floor--) {
      if (e->queue[floor][ORDER_DOWN]){
        e->current_dir = HARDWARE_MOVEMENT_DOWN;
        e->next_dir = HARDWARE_MOVEMENT_DOWN;
      } 
    }
  }
  if (e->current_dir == HARDWARE_MOVEMENT_STOP){
    if(e->queue[e->last_floor][ORDER_UP] || e->queue[e->last_floor][ORDER_DOWN]){
      e->last_state = e->current_state;
      e->current_state = DOOR_OPEN;
    }
    else {
    for (int floor = 0; floor < 4; floor++) {
      if (e->queue[floor][ORDER_UP]){
        if(floor > e->last_floor){
          e->current_dir = HARDWARE_MOVEMENT_UP;
          e->next_dir = HARDWARE_MOVEMENT_UP;
        }
        else if(floor < e->last_floor){
          e->current_dir = HARDWARE_MOVEMENT_DOWN;
          e->next_dir = HARDWARE_MOVEMENT_UP;
          }
        }
        if (e->queue[floor][ORDER_DOWN]){
          if(floor > e->last_floor){
            e->current_dir = HARDWARE_MOVEMENT_UP;
            e->next_dir = HARDWARE_MOVEMENT_DOWN;
          }
          else if(floor < e->last_floor){
            e->current_dir = HARDWARE_MOVEMENT_DOWN;
            e->next_dir = HARDWARE_MOVEMENT_DOWN;
            }
        }
      }
    }
  }
}

void queue_handler_set_floor(elevator_t *e) {
  if (e->current_dir == HARDWARE_MOVEMENT_UP) {
    for (int floor = 3; floor > e->current_floor; floor--) {
      if (e->queue[floor][ORDER_UP]){
        e->last_floor = e->current_floor;
        e->current_floor = floor;
      }
      else {
        e->current_dir = HARDWARE_MOVEMENT_STOP;
      }
    }
  }
  if (e->current_dir == HARDWARE_MOVEMENT_DOWN) {
    for (int floor = 0; floor > e->current_floor; floor++) {
      if (e->queue[floor][ORDER_UP]){
        e->last_floor = e->current_floor;
        e->current_floor = floor;
      }
      else {
        e->current_dir = HARDWARE_MOVEMENT_STOP;
      }
}
}
  if (e->current_dir == HARDWARE_MOVEMENT_STOP){
    for (int floor = 0; floor < 4; floor++) {
      if (e->queue[floor][ORDER_UP] || e->queue[floor][ORDER_DOWN]){
          e->current_floor = floor;
          }
        }
      }
    }

void queue_handler_order_complete(elevator_t *e){
  if (e->current_dir == HARDWARE_MOVEMENT_UP) {
    e->queue[e->current_floor][ORDER_UP] = 0;
  }
  if (e->current_dir == HARDWARE_MOVEMENT_DOWN) {
    e->queue[e->current_floor][ORDER_DOWN] = 0;
  }
}

void queue_handler_inside_order(elevator_t *e){
        if (hardware_read_order(0, HARDWARE_ORDER_INSIDE)){
            e->queue[0][ORDER_UP] = 1;
            printf("%d", 0);
        }
        if (hardware_read_order(1, HARDWARE_ORDER_INSIDE)){
          hardware_command_order_light(1, HARDWARE_ORDER_INSIDE, 1);
            if (e->current_dir == HARDWARE_MOVEMENT_UP){
              e->queue[1][ORDER_UP] = 1;
              printf("%d",1);
            }
            if (e->current_dir == HARDWARE_MOVEMENT_DOWN){
              e->queue[1][ORDER_DOWN] = 1;
              printf("%d",1);
            }
        }
        if (hardware_read_order(2, HARDWARE_ORDER_INSIDE)){
          hardware_command_order_light(2, HARDWARE_ORDER_INSIDE, 1);
            if (e->current_dir == HARDWARE_MOVEMENT_UP){
              e->queue[2][ORDER_UP] = 1;
              printf("%d",2);
            }
            if (e->current_dir == HARDWARE_MOVEMENT_DOWN){
              e->queue[2][ORDER_DOWN] = 1;
              printf("%d",2);
            }
        }
        if (hardware_read_order(3, HARDWARE_ORDER_INSIDE)){
          hardware_command_order_light(3, HARDWARE_ORDER_INSIDE, 1);
            e->queue[3][ORDER_DOWN] = 1;
            printf("%d",3);
        }
}