/**
  * @file    app_trace_emwin.h
  * @brief   Trace emWin
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef APP_TRACE_EMWIN_H
#define APP_TRACE_EMWIN_H


#include <stdio.h>
#include "WM.h"


static
void	app_trace_emwin( char * str, int i )
{
	switch( i )
	{
                #if GUI_WINSUPPORT > 0
		case WM_CREATE:                 printf( "%s WM_CREATE\n",               str ); break;
		case WM_MOVE:                   printf( "%s WM_MOVE\n",                 str ); break;
		case WM_SIZE:                   printf( "%s WM_SIZE\n",                 str ); break;
		case WM_DELETE:                 printf( "%s WM_DELETE\n",               str ); break;
		case WM_TOUCH:                  printf( "%s WM_TOUCH\n",                str ); break;
		case WM_TOUCH_CHILD:            printf( "%s WM_TOUCH_CHILD\n",          str ); break;
		case WM_KEY:                    printf( "%s WM_KEY\n",                  str ); break;
		case WM_PAINT:                  printf( "%s WM_PAINT\n",                str ); break;
                #if GUI_SUPPORT_MOUSE
		case WM_MOUSEOVER:              printf( "%s WM_MOUSEOVER\n",            str ); break;
		case WM_MOUSEOVER_END:          printf( "%s WM_MOUSEOVER_END\n",        str ); break;
                #endif
		case WM_PID_STATE_CHANGED:      printf( "%s WM_PID_STATE_CHANGED\n",    str ); break;
		case WM_GET_INSIDE_RECT:        printf( "%s WM_GET_INSIDE_RECT\n",      str ); break;
		case WM_GET_ID:                 printf( "%s WM_GET_ID\n",               str ); break;
		case WM_SET_ID:                 printf( "%s WM_SET_ID\n",               str ); break;
		case WM_GET_CLIENT_WINDOW:      printf( "%s WM_GET_CLIENT_WINDOW\n",    str ); break;
		case WM_CAPTURE_RELEASED:       printf( "%s WM_CAPTURE_RELEASED\n",     str ); break;
		case WM_INIT_DIALOG:            printf( "%s WM_INIT_DIALOG\n",          str ); break;
		case WM_SET_FOCUS:              printf( "%s WM_SET_FOCUS\n",            str ); break;
		case WM_GET_ACCEPT_FOCUS:       printf( "%s WM_GET_ACCEPT_FOCUS\n",     str ); break;
		case WM_NOTIFY_CHILD_HAS_FOCUS: printf( "%s WM_NOTIFY_CHILD_HAS_FOCUS\n", str ); break;
		case WM_NOTIFY_OWNER_KEY:       printf( "%s WM_NOTIFY_OWNER_KEY\n",     str ); break;
		case WM_GET_BKCOLOR:            printf( "%s WM_GET_BKCOLOR\n",          str ); break;
		case WM_GET_SCROLL_STATE:       printf( "%s WM_GET_SCROLL_STATE\n",     str ); break;
		case WM_SET_SCROLL_STATE:       printf( "%s WM_SET_SCROLL_STATE\n",     str ); break;
		case WM_NOTIFY_CLIENTCHANGE:    printf( "%s WM_NOTIFY_CLIENTCHANGE\n",  str ); break;
		case WM_NOTIFY_PARENT:          printf( "%s WM_NOTIFY_PARENT\n",        str ); break;
		case WM_NOTIFY_PARENT_REFLECTION: printf( "%s WM_NOTIFY_PARENT_REFLECTION\n", str ); break;
		case WM_NOTIFY_ENABLE:          printf( "%s WM_NOTIFY_ENABLE\n",        str ); break;
		case WM_NOTIFY_VIS_CHANGED:     printf( "%s WM_NOTIFY_VIS_CHANGED\n",   str ); break;
		case WM_HANDLE_DIALOG_STATUS:   printf( "%s WM_HANDLE_DIALOG_STATUS\n", str ); break;
		case WM_GET_RADIOGROUP:         printf( "%s WM_GET_RADIOGROUP\n",       str ); break;
		case WM_MENU:                   printf( "%s WM_MENU\n",                 str ); break;
		case WM_SCREENSIZE_CHANGED:     printf( "%s WM_SCREENSIZE_CHANGED\n",   str ); break;
		case WM_PRE_PAINT:              printf( "%s WM_PRE_PAINT\n",            str ); break;
		case WM_POST_PAINT:             printf( "%s WM_POST_PAINT\n",           str ); break;
		case WM_MOTION:                 printf( "%s WM_MOTION\n",               str ); break;
		case WM_GET_WINDOW_ID:          printf( "%s WM_GET_WINDOW_ID\n",        str ); break;
		case WM_PRE_BANDING:            printf( "%s WM_PRE_BANDING\n",          str ); break;
		case WM_POST_BANDING:           printf( "%s WM_POST_BANDING\n",         str ); break;
		case WM_GESTURE:                printf( "%s WM_GESTURE\n",              str ); break;
		case WM_TIMER:                  printf( "%s WM_TIMER\n",                str ); break;
		case WM_WIDGET:                 printf( "%s WM_WIDGET\n",               str ); break;
		case WM_USER:                   printf( "%s WM_USER\n",                 str ); break;
                #endif
		default:                        printf( "%s WM_UNKNOWN %d\n",           str, i ); break;
	}
}

#ifdef  NDEBUG
# define APP_TRACE_EMWIN(...)
#else
# define APP_TRACE_EMWIN( STR, ... )    app_trace_emwin( STR, __VA_ARGS__ )
#endif

#endif	//APP_TRACE_EMWIN_H
