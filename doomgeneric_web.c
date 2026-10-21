// doomgeneric browser port for the Saintscino Doom dApp.
// Video goes straight to a <canvas> through ImageData, and input comes from JS
// (dg_key / dg_mouse), so nothing depends on SDL's window or event handling.
// Sound, when built with FEATURE_SOUND, still uses SDL_mixer (WebAudio).

#include "doomkeys.h"
#include "doomgeneric.h"
#include "d_event.h"

#include <stdio.h>
#include <emscripten.h>

#define KEYQUEUE_SIZE 64

static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int s_KeyQueueWriteIndex = 0;
static unsigned int s_KeyQueueReadIndex = 0;

// Called from JS with a doomkeys.h code (the page does the key mapping).
EMSCRIPTEN_KEEPALIVE void dg_key(int pressed, int doomKey)
{
  s_KeyQueue[s_KeyQueueWriteIndex] = ((pressed ? 1 : 0) << 8) | (doomKey & 0xFF);
  s_KeyQueueWriteIndex = (s_KeyQueueWriteIndex + 1) % KEYQUEUE_SIZE;
}

// Called from JS with pointer-lock movement; buttons is a bitmask (1 = fire, 2 = strafe, 4 = forward).
EMSCRIPTEN_KEEPALIVE void dg_mouse(int buttons, int dx, int dy)
{
  event_t ev;
  ev.type = ev_mouse;
  ev.data1 = buttons;
  ev.data2 = dx;
  ev.data3 = -dy;
  ev.data4 = 0;
  D_PostEvent(&ev);
}

EM_JS(void, js_init, (int w, int h), {
  if (Module.onDoomInit) Module.onDoomInit(w, h);
});

// Copies the 0x00RRGGBB framebuffer into the page's ImageData as opaque RGBA.
EM_JS(void, js_draw, (unsigned int ptr, int w, int h), {
  var out = Module.doomPixels;
  if (!out) return;
  var src = HEAPU32.subarray(ptr >> 2, (ptr >> 2) + w * h);
  for (var i = 0; i < src.length; i++) {
    var p = src[i];
    out[i] = 0xFF000000 | ((p & 0xFF) << 16) | (p & 0xFF00) | ((p >> 16) & 0xFF);
  }
  if (Module.onDoomFrame) Module.onDoomFrame();
});

EM_JS(void, js_title, (const char* title), {
  if (Module.onDoomTitle) Module.onDoomTitle(UTF8ToString(title));
});

void DG_Init()
{
  js_init(DOOMGENERIC_RESX, DOOMGENERIC_RESY);
}

void DG_DrawFrame()
{
  js_draw((unsigned int)DG_ScreenBuffer, DOOMGENERIC_RESX, DOOMGENERIC_RESY);
}

void DG_SleepMs(uint32_t ms)
{
  // The browser drives the loop; sleeping would freeze the page.
}

uint32_t DG_GetTicksMs()
{
  return (uint32_t)emscripten_get_now();
}

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
  if (s_KeyQueueReadIndex == s_KeyQueueWriteIndex)
    return 0;

  unsigned short keyData = s_KeyQueue[s_KeyQueueReadIndex];
  s_KeyQueueReadIndex = (s_KeyQueueReadIndex + 1) % KEYQUEUE_SIZE;
  *pressed = keyData >> 8;
  *doomKey = keyData & 0xFF;
  return 1;
}

void DG_SetWindowTitle(const char* title)
{
  js_title(title);
}

int main(int argc, char** argv)
{
  doomgeneric_Create(argc, argv);
  emscripten_set_main_loop(doomgeneric_Tick, 0, 1);
  return 0;
}
