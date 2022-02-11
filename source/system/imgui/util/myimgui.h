#pragma once
#include	<functional>
#include	"../imgui.h"
#include	"../imgui_impl_dx11.h"
#include	"../imgui_impl_win32.h"

void ImguiInit();
void ImguiDraw(std::function<void(void)>);
void ImguiExit();
