#!/bin/sh
# Builds doomgeneric for the browser into ./out (doom.js, doom.wasm, doom.data).
# Usage: sh build.sh [sound]
set -e
ROOT="$(cd "$(dirname "$0")" && pwd)"
export EMSDK_PYTHON="/c/Users/impac/AppData/Local/Python/bin/python.exe"
EMCC="$ROOT/emsdk/upstream/emscripten/emcc.exe"
SRC="$ROOT/doomgeneric/doomgeneric"
OUT="$ROOT/out"
OBJ="$ROOT/obj"
mkdir -p "$OUT" "$OBJ"

FILES="dummy am_map doomdef doomstat dstrings d_event d_items d_iwad d_loop d_main d_mode d_net f_finale f_wipe g_game hu_lib hu_stuff info i_cdmus i_endoom i_joystick i_scale i_sound i_system i_timer memio m_argv m_bbox m_cheat m_config m_controls m_fixed m_menu m_misc m_random p_ceilng p_doors p_enemy p_floor p_inter p_lights p_map p_maputl p_mobj p_plats p_pspr p_saveg p_setup p_sight p_spec p_switch p_telept p_tick p_user r_bsp r_data r_draw r_main r_plane r_segs r_sky r_things sha1 sounds statdump st_lib st_stuff s_sound tables v_video wi_stuff w_checksum w_file w_main w_wad z_zone w_file_stdc i_input i_video doomgeneric doomgeneric_web"
CFLAGS="-O2 -DDOOMGENERIC_RESX=320 -DDOOMGENERIC_RESY=200 -Wno-everything"
LINK=""
if [ "$1" = "sound" ]; then
  FILES="$FILES mus2mid i_sdlmusic i_sdlsound"
  CFLAGS="$CFLAGS -DFEATURE_SOUND -sUSE_SDL=2 -sUSE_SDL_MIXER=2"
  LINK="-sUSE_SDL=2 -sUSE_SDL_MIXER=2"
fi

OBJS=""
for f in $FILES; do
  echo "cc $f"
  "$EMCC" $CFLAGS -c "$SRC/$f.c" -o "$OBJ/$f.o"
  OBJS="$OBJS $OBJ/$f.o"
done

echo "link"
cd "$ROOT/wad"
"$EMCC" -O2 $LINK $OBJS -o "$OUT/doom.js" \
  --preload-file doom1.wad \
  -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=64MB \
  -sEXPORTED_FUNCTIONS=_main,_dg_key,_dg_mouse,_dg_menu_active \
  -sEXPORTED_RUNTIME_METHODS=callMain \
  -sENVIRONMENT=web -lm
ls -l "$OUT"
