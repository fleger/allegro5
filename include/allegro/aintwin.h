/*         ______   ___    ___ 
 *        /\  _  \ /\_ \  /\_ \ 
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___ 
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      Some definitions for internal use by the Windows library code.
 *
 *      By Stefan Schimanski.
 *
 *      See readme.txt for copyright information.
 */


#ifndef AINTWIN_H
#define AINTWIN_H

#ifndef ALLEGRO_H
   #error must include allegro.h first
#endif

#ifndef ALLEGRO_WINDOWS
   #error bad include
#endif

#ifdef __cplusplus
   extern "C" {
#endif


#include "winalleg.h"

#if (!defined SCAN_EXPORT) && (!defined SCAN_DEPEND)
   /* workaround for buggy mingw headers */
   #ifdef ALLEGRO_MINGW32
      #ifndef HMONITOR_DECLARED
         #define HMONITOR_DECLARED
      #endif
      #if (defined _HRESULT_DEFINED) && (defined WINNT)
         #undef WINNT
      #endif
   #endif

   #include <ddraw.h>
#endif

/*******************************************/
/***************** general *****************/
/*******************************************/
AL_VAR(HINSTANCE, allegro_inst);
AL_VAR(HWND, allegro_wnd);
AL_VAR(HANDLE, allegro_thread);
AL_VAR(int, _dx_ver);

AL_FUNC(int, init_directx_window, (void));
AL_FUNC(void, exit_directx_window, (void));
AL_FUNC(int, wnd_call_proc, (int (*proc)(void)));
AL_FUNC(int, get_dx_ver, (void));
AL_FUNC(void, set_sync_timer_freq, (int freq));
AL_FUNC(void, restore_window_style, (void));

/* main window */
AL_ARRAY(char, wnd_title);
AL_VAR(int, wnd_x);
AL_VAR(int, wnd_y);
AL_VAR(int, wnd_width);
AL_VAR(int, wnd_height);
AL_VAR(int, wnd_windowed);
AL_VAR(int, wnd_sysmenu);

AL_FUNCPTR(void, user_close_proc, (void));

/* gfx synchronization */
AL_VAR(CRITICAL_SECTION, gfx_crit_sect);
AL_VAR(int, gfx_crit_sect_nesting);

#define _enter_gfx_critical()  EnterCriticalSection(&gfx_crit_sect); \
                               gfx_crit_sect_nesting++
#define _exit_gfx_critical()   LeaveCriticalSection(&gfx_crit_sect); \
                               gfx_crit_sect_nesting--
#define GFX_CRITICAL_RELEASED  (!gfx_crit_sect_nesting)

struct WIN_GFX_DRIVER {
   AL_METHOD(void, switch_in, (void));
   AL_METHOD(void, switch_out, (void));
   AL_METHOD(void, enter_sysmode, (void));
   AL_METHOD(void, exit_sysmode, (void));
   AL_METHOD(void, move, (int x, int y, int w, int h));
   AL_METHOD(void, iconify, (void));
   AL_METHOD(void, paint, (RECT *rect));
};   

AL_VAR(struct WIN_GFX_DRIVER *, win_gfx_driver);

/* stuff moved over from wddraw.h */
typedef struct BMP_EXTRA_INFO {
   LPDIRECTDRAWSURFACE2 surf;
   struct BMP_EXTRA_INFO *next;
   struct BMP_EXTRA_INFO *prev;
   int flags;
   int lock_nesting;
} BMP_EXTRA_INFO;

#define BMP_EXTRA(bmp) ((BMP_EXTRA_INFO *)(bmp->extra))

#define BMP_FLAG_LOST      1

AL_VAR(LPDIRECTDRAW2, directdraw);
AL_VAR(LPDIRECTDRAWSURFACE2, dd_prim_surface);
AL_VAR(LPDIRECTDRAWPALETTE, dd_palette);
AL_VAR(LPDIRECTDRAWCLIPPER, dd_clipper);
AL_VAR(DDCAPS, dd_caps);
AL_VAR(LPDDPIXELFORMAT, dd_pixelformat);
AL_VAR(BITMAP *, dd_frontbuffer);
AL_VAR(char *, pseudo_surf_mem);

/* focus switch routines */
AL_VAR(BOOL, app_foreground);
AL_VAR(HANDLE, _foreground_event);

AL_FUNC(void, sys_directx_display_switch_init, (void));
AL_FUNC(void, sys_directx_display_switch_exit, (void));
AL_FUNC(int, sys_directx_set_display_switch_mode, (int mode));
AL_FUNC(int, sys_directx_set_display_switch_callback, (int dir, void (*cb)(void)));
AL_FUNC(void, sys_directx_remove_display_switch_callback, (void (*cb)(void)));

AL_FUNC(void, sys_switch_in, (void));

AL_FUNC(void, sys_switch_out, (void));
AL_FUNC(void, thread_switch_out, (void));
AL_FUNC(void, midi_switch_out, (void));

AL_FUNC(void, sys_directx_switch_out_callback, (void));
AL_FUNC(void, sys_directx_switch_in_callback, (void));



/*******************************************/
/************** keyboard routines **********/
/*******************************************/
AL_FUNC(int, key_dinput_acquire, (void));
AL_FUNC(void, key_dinput_unacquire, (void));
AL_FUNC(void, wnd_acquire_keyboard, (void));



/*******************************************/
/************** mouse routines *************/
/*******************************************/
AL_FUNC(int, mouse_dinput_acquire, (void));
AL_FUNC(void, mouse_dinput_unacquire, (void));
AL_FUNC(int, mouse_set_cursor, (void));
AL_FUNC(void, mouse_sysmenu_changed, (void));
AL_FUNC(void, wnd_acquire_mouse, (void));
AL_FUNC(void, wnd_set_cursor, (void));



/*******************************************/
/************* thread routines *************/
/*******************************************/
AL_FUNC(void, _enter_critical, (void));
AL_FUNC(void, _exit_critical, (void));
AL_FUNC(void, win_init_thread, (void));
AL_FUNC(void, win_exit_thread, (void));



/******************************************/
/************* sound routines *************/
/******************************************/
AL_FUNC(_DRIVER_INFO *, _get_digi_driver_list, (void));
AL_FUNC(_DRIVER_INFO *, _get_midi_driver_list, (void));

AL_FUNC(DIGI_DRIVER *, _get_dsalmix_driver, (char *name, LPGUID guid, int num));
AL_FUNC(DIGI_DRIVER *, _get_woalmix_driver, (int num));

AL_FUNC(int, digi_directsound_capture_init, (LPGUID guid));
AL_FUNC(void, digi_directsound_capture_exit, (void));
AL_FUNC(int, digi_directsound_capture_detect, (LPGUID guid));
AL_FUNC(int, digi_directsound_rec_cap_rate, (int bits, int stereo));
AL_FUNC(int, digi_directsound_rec_cap_param, (int rate, int bits, int stereo));
AL_FUNC(int, digi_directsound_rec_source, (int source));
AL_FUNC(int, digi_directsound_rec_start, (int rate, int bits, int stereo));
AL_FUNC(void, digi_directsound_rec_stop, (void));
AL_FUNC(int, digi_directsound_rec_read, (void *buf));



/*******************************************/
/************* error handling **************/
/*******************************************/
AL_FUNC(char* , win_err_str, (long err));
AL_FUNC(void, thread_safe_trace, (char *msg, ...));

#ifdef DEBUGMODE
   #define _TRACE                 thread_safe_trace
#else
   #define _TRACE                 1 ? (void) 0 : thread_safe_trace
#endif



#ifdef __cplusplus
   }
#endif

#endif          /* ifndef AINTWIN_H */

