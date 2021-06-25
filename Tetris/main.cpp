#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include "GLWindow.h"
#include "Graphics.h"
#include "Game.h"
#include <windows.h>

void show_console() {
  AllocConsole();
  freopen("conin$", "r", stdin);
  freopen("conout$", "w", stdout);
  freopen("conout$", "w", stderr);
}

#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
  show_console();

  GLWindow glWindow(hInstance);

  glWindow.create(800, 600, 32, false, true);
  {
    Game game;
    Graphics graphics(&game);
    Input input(&game);
    glWindow.setInput(&input);
    graphics.init();

    while (glWindow.running()) {
      glWindow.processEvents();
      float dt = glWindow.getElapsedTime();
      game.update(dt);

      graphics.prepare(dt);
      graphics.render();
      glWindow.swapBuffers();

      if (game.isGameOver()) {
        MessageBox(NULL, "GAME OVER", "GAME OVER", MB_OK);
        break;
      }

    }
    graphics.shutdown();
  }
  return 0;
}
