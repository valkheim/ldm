#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_util.h>
#include "main.h"
#include "events.h"

static void button_press_management(xcb_button_press_event_t * event)
{
  printf("event = %s\n",xcb_event_get_label(event->response_type));
}
static void button_release_management(xcb_button_release_event_t * event)
{
  printf("event = %s\n",xcb_event_get_label(event->response_type));
}
static void configure_request_management(xcb_configure_request_event_t * event)
{
  uint16_t config_win_mask = 0;
  uint32_t config_win_vals[7];
  unsigned short i = 0;

  if(event->value_mask & XCB_CONFIG_WINDOW_X)
  {
    config_win_mask |= XCB_CONFIG_WINDOW_X;
    config_win_vals[i++] = 300;
    printf(" XCB_CONFIG_WINDOW_X\n");
  }
  if(event->value_mask & XCB_CONFIG_WINDOW_Y)
  {
    config_win_mask |= XCB_CONFIG_WINDOW_Y;
    config_win_vals[i++] = 300;
    printf(" XCB_CONFIG_WINDOW_Y\n");
  }
  if(event->value_mask & XCB_CONFIG_WINDOW_WIDTH)
  {
    config_win_mask |= XCB_CONFIG_WINDOW_WIDTH;
    config_win_vals[i++] = event->width;
    printf(" XCB_CONFIG_WINDOW_WIDTH\n");
  }
  if(event->value_mask & XCB_CONFIG_WINDOW_HEIGHT)
  {
    config_win_mask |= XCB_CONFIG_WINDOW_HEIGHT;
    config_win_vals[i++] = event->height;
    printf("XCB_CONFIG_WINDOW_HEIGHT");
  }
  if(event->value_mask & XCB_CONFIG_WINDOW_BORDER_WIDTH)
  {
    config_win_mask |= XCB_CONFIG_WINDOW_BORDER_WIDTH;
    config_win_vals[i++] = event->border_width;
    printf(" XCB_CONFIG_WINDOW_BORDER_WIDTH\n");
  }
  if(event->value_mask & XCB_CONFIG_WINDOW_SIBLING)
  {
    config_win_mask |= XCB_CONFIG_WINDOW_SIBLING;
    config_win_vals[i++] = event->sibling;
    printf(" XCB_CONFIG_WINDOW_SIBLING\n");
  }
  if(event->value_mask & XCB_CONFIG_WINDOW_STACK_MODE)
  {
    config_win_mask |= XCB_CONFIG_WINDOW_STACK_MODE;
    config_win_vals[i++] = event->stack_mode;
    printf(" XCB_CONFIG_WINDOW_STACK_MODE\n");
  }

  xcb_configure_window(c, event->window, config_win_mask, config_win_vals);
  xcb_flush(c);
  printf("event = %s\n",xcb_event_get_label(event->response_type));
}
static void client_message_management(xcb_client_message_event_t * event)
{
  printf("event = %s\n",xcb_event_get_label(event->response_type));
}
static void expose_management(xcb_expose_event_t *event)
{
  printf("event = %s\n",xcb_event_get_label(event->response_type));
}
static void focus_in_management(xcb_focus_in_event_t *event)
{
  printf("event = %s\n",xcb_event_get_label(event->response_type));
}
static void key_press_management(xcb_key_press_event_t *event)
{
  printf("event = %s\n",xcb_event_get_label(event->response_type));
}
static void key_release_management(xcb_key_release_event_t *event)
{
  printf("event = %s\n",xcb_event_get_label(event->response_type));
}
static void motion_notify_management(xcb_motion_notify_event_t * event)
{
  printf("event = %s\n",xcb_event_get_label(event->response_type));
}
static void map_request_management(xcb_map_request_event_t * event)
{
  xcb_map_window(c, event->window);
  xcb_flush(c);
  xcb_grab_button(c,0, event->window,XCB_EVENT_MASK_BUTTON_PRESS, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, XCB_NONE, XCB_NONE, XCB_BUTTON_INDEX_ANY, XCB_MOD_MASK_ANY);
  printf("event = %s\n",xcb_event_get_label(event->response_type));
}
static void mapping_notify_management(xcb_motion_notify_event_t * event)
{
  printf("event = %s\n",xcb_event_get_label(event->response_type));
}
static void reparent_notify_management(xcb_reparent_notify_event_t * event)
{
  printf("event = %s\n",xcb_event_get_label(event->response_type));
}
static void unmap_notify_management(xcb_unmap_notify_event_t * event)
{
  printf("event = %s\n",xcb_event_get_label(event->response_type));
}
static void enter_notify_management(xcb_enter_notify_event_t * event)
{
  printf("event = %s\n",xcb_event_get_label(event->response_type));
}
static void leave_notify_management(xcb_leave_notify_event_t * event)
{
  printf("event = %s\n",xcb_event_get_label(event->response_type));
}

void event_management(xcb_generic_event_t *event)
{
  uint8_t response_type = XCB_EVENT_RESPONSE_TYPE(event);

  if (response_type == 0)
  {
    /* This is an error, not a event */
    perror("response_type = 0");
    return;
  }

  switch(response_type)
  {
    case XCB_BUTTON_PRESS:
      button_press_management((void *) event);
      break;
    case XCB_BUTTON_RELEASE:
      button_release_management((void *)event);
      break;
    case XCB_CONFIGURE_REQUEST:
      configure_request_management((void *)event);
      break;
    case XCB_CLIENT_MESSAGE:
      client_message_management((void *)event);
      break;
    case XCB_EXPOSE:
      expose_management((void *)event);
      break;
    case XCB_FOCUS_IN:
      focus_in_management((void *)event);
      break;
    case XCB_KEY_PRESS:
      key_press_management((void *)event);
      break;
    case XCB_KEY_RELEASE:
      key_release_management((void *)event);
      break;
    case XCB_MAP_REQUEST:
      map_request_management((void *)event);
      break;
    case XCB_MAPPING_NOTIFY:
      mapping_notify_management((void *)event);
      break;case XCB_MOTION_NOTIFY:
        motion_notify_management((void *)event);
      break;
    case XCB_REPARENT_NOTIFY:
      reparent_notify_management((void *)event);
      break;
    case XCB_UNMAP_NOTIFY:
      unmap_notify_management((void *)event);
      break;
    case XCB_ENTER_NOTIFY:
      enter_notify_management((void *)event);
      break;
    case XCB_LEAVE_NOTIFY:
      leave_notify_management((void *)event);
      break;
    default:
      printf("event = %s\n",xcb_event_get_label(event->response_type));
      printf("%d\n",response_type);
      perror("this kind of event is not managed\n");
      break;
  }
}

void dm_event_loop()
{
  xcb_generic_event_t *the_events;
  int done;

  /*enter the main loop*/
  done = 0;
  while (!done && (the_events = xcb_wait_for_event(c)))
  {
    switch(the_events->response_type)
    {
      /*(re)draw the window*/
      case XCB_EXPOSE:
        printf ("EXPOSE\n");
        break;
        /*exit on keypress*/
      //case XCB_KEY_PRESS:
      //  done = 1;
      //  break;
      default:
        event_management(the_events);
        printf("The events = %s\n",xcb_event_get_label(the_events->response_type));
    }
    free(the_events);
  }
}
