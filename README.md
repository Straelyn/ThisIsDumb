# SSU DOOM

DOOM running in a Smart Storage Unit dApp panel, compiled to WebAssembly.

## Playing
Open the page (or set it as a storage unit's dApp URL), click the screen, and press **Esc → New Game**.

| Keys | Action |
|---|---|
| W / ↑, S / ↓ | Forward, back |
| A / D | Strafe |
| ← / → or mouse | Turn (click the screen to capture the mouse; Esc releases it) |
| Left click / Ctrl | Fire |
| E / Space / right click | Open doors, press switches |
| Shift | Run |
| 1–7 | Weapons |
| Tab | Automap |

Sound effects play, but there's no music (MIDI isn't supported in this build).

## What's in here
- `index.html`: the page (canvas, controls, input mapping)
- `doom.js`, `doom.wasm`: the engine, [doomgeneric](https://github.com/ozkl/doomgeneric) (GPL-2.0) built with Emscripten
- `doom.data`: DOOM shareware v1.9 `doom1.wad` (Episode 1). © id Software; the shareware version is freely distributable unmodified.
- `doomgeneric_web.c`, `build.sh`: the browser port and build script (source for the GPL)

*The Saintscino | Direct revisions to Strae on Discord*
