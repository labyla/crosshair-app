#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <glad.h>
#include <glfw3.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <vector>
#include <string>
#include <thread>

using namespace std;

struct CrosshairData {
	int Width = 2;
	int Length = 10;
	int Height = 10;
	bool Centered = true;
	int CoordinateX = 0;
	int CoordinateY = 0;
	float RGBcolor[3] = { 1.0f, 0.0f, 0.0f };
	int CrosshairWindowWidth = 0;
	int CrosshairWindowHeight = 0;
};

struct Windows {
	vector<char*> WindowsTitle;
	vector<HWND> WindowsHWND;
	int WindowsCount = 0;
	vector<CrosshairData> Crosshairs;
};

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	struct Windows& windows = *reinterpret_cast<Windows*>(lParam);

	for (HWND hwnd = GetTopWindow(NULL); hwnd != NULL; hwnd = GetNextWindow(hwnd, GW_HWNDNEXT)) {
		int WindowTitleLength = GetWindowTextLengthW(hwnd);
		if (!IsWindowVisible(hwnd) || WindowTitleLength == 0)
			continue;

		char* WindowTitle = new char[WindowTitleLength + 1];
		GetWindowTextA(hwnd, WindowTitle, WindowTitleLength + 1);
		windows.WindowsTitle.push_back(WindowTitle);
		windows.WindowsHWND.push_back(hwnd);
		windows.WindowsCount++;
		CrosshairData Crosshair;
		windows.Crosshairs.push_back(Crosshair);

		//_tprintf(TEXT("% s\n"), WindowTitle);
		//if (wcsstr(WindowTitle, L"CrosshairsApp - Microsoft Visual Studio")) {
		//	cout << hwnd << endl;
		//}
	}

	return false;
};

void DrawCrosshair(HWND hwnd, thread::id& CurrentThreadId, CrosshairData& Crosshair) {
	tagRECT rect;
	HDC hdc = GetDC(NULL);
	while (this_thread::get_id() == CurrentThreadId && GetWindowRect(hwnd, &rect)) {
		if (rect.left < 0) {
			Crosshair.CrosshairWindowWidth = rect.right + rect.left;
		}
		else {
			Crosshair.CrosshairWindowWidth = rect.right - rect.left;
		}
		if (rect.top < 0) {
			Crosshair.CrosshairWindowHeight = rect.bottom + rect.top;
		}
		else {
			Crosshair.CrosshairWindowHeight = rect.bottom - rect.top;
		}
		if (GetForegroundWindow() == hwnd) {
			int CrosshairWidth = Crosshair.Width;
			int CrosshairLength = Crosshair.Length;
			int CrosshairHeight = Crosshair.Height;
			COLORREF CrosshairRGBcolor = RGB(255 * Crosshair.RGBcolor[0], 255 * Crosshair.RGBcolor[1], 255 * Crosshair.RGBcolor[2]);
			int xCenter, yCenter;
			if (Crosshair.Centered) {
				xCenter = rect.left + (rect.right - rect.left) / 2;
				yCenter = rect.top + (rect.bottom - rect.top) / 2;
				Crosshair.CoordinateX = (rect.right - rect.left) / 2;
				Crosshair.CoordinateY = (rect.bottom - rect.top) / 2;
			}
			else {
				if (Crosshair.CoordinateX > Crosshair.CrosshairWindowWidth)
					Crosshair.CoordinateX = Crosshair.CrosshairWindowWidth;
				if (Crosshair.CoordinateY > Crosshair.CrosshairWindowHeight)
					Crosshair.CoordinateY = Crosshair.CrosshairWindowHeight;
				xCenter = rect.left + Crosshair.CoordinateX;
				yCenter = rect.top + Crosshair.CoordinateY;
			}

			for (int y = yCenter - CrosshairWidth; y < yCenter + CrosshairWidth; y++)
				for (int x = xCenter - CrosshairLength; x <= xCenter + CrosshairLength; x++)
					SetPixel(hdc, x, y, CrosshairRGBcolor);
			for (int x = xCenter - CrosshairWidth; x < xCenter + CrosshairWidth; x++)
				for (int y = yCenter - CrosshairHeight; y < yCenter + CrosshairHeight; y++)
					SetPixel(hdc, x, y, CrosshairRGBcolor);
		}
	}
}

void func() {
	while (true)
		cout << 1 << endl;
}

int main()
{
	//int CrosshairWidth = 2;
	//EnumWindows(EnumWindowsProc, CrosshairWidth);
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Crosshair", NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, 800, 800);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("./fonts/helvetica_bold.ttf", 15, NULL, io.Fonts->GetGlyphRangesCyrillic());
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	
	ImGuiStyle& ImStyle = ImGui::GetStyle();
	//auto& colors = ImStyle.Colors;
	//colors[ImGuiCol_Border] = ImColor(255, 255, 255, 255);

	int IndexOfSelectedItem = -1;
	HWND CurrentHWND = NULL;
	thread::id CurrentThreadId;
	Windows windows;
	EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&windows));

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		int GlfwWindowWidth, GlfwWindowHeight;
		glfwGetFramebufferSize(window, &GlfwWindowWidth, &GlfwWindowHeight);

		ImGui::SetNextWindowSize(ImVec2(GlfwWindowWidth, GlfwWindowHeight));
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

		ImGui::Begin("Crossahair", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		string title = "Set Crosshair";
		
		if (CurrentHWND != NULL) {
			ImGui::BeginChild("##left_side", ImVec2(ImGui::GetContentRegionAvail().x / 2, ImGui::GetContentRegionAvail().y));
		}
		else {
			ImGui::BeginChild("##left_side", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
		}
		int ListOfWindowsWidth = ImGui::GetContentRegionAvail().x * 0.85;
		int ListOfWindowsHeight = ImGui::GetContentRegionAvail().y * 0.7;
		ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x / 2) - (ListOfWindowsWidth / 2));
		ImGui::SetCursorPosY((ImGui::GetContentRegionAvail().y / 2) - (ListOfWindowsHeight / 2));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(255, 255, 255, 255));
		ImGui::BeginChild("ListOfWindows", ImVec2(ListOfWindowsWidth, ListOfWindowsHeight), true, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar()) {
			ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x / 2) - (ImGui::CalcTextSize("Select Window").x / 2));
			ImGui::Text("Select Window");
			ImGui::EndMenuBar();
		}
		ImGui::PopStyleColor();
		if (ImGui::BeginListBox("##listbox", ImVec2(ListOfWindowsWidth - 15, ListOfWindowsHeight - 81))) {
			for (int index = 0; index < windows.WindowsCount; index++)
				if (ImGui::Selectable(windows.WindowsTitle[index], (IndexOfSelectedItem == index)) && windows.WindowsHWND[index] != NULL) {CurrentHWND = windows.WindowsHWND[index];
					if (IndexOfSelectedItem != index) {
						thread CrosshairThread(DrawCrosshair, windows.WindowsHWND[index], ref(CurrentThreadId), ref(windows.Crosshairs[index]));
						CurrentThreadId = CrosshairThread.get_id();
						CrosshairThread.detach();
					}
					CurrentHWND = windows.WindowsHWND[index];
					IndexOfSelectedItem = index;
				}
			ImGui::EndListBox();
		}
		if (ImGui::Button("Refresh", ImVec2(ListOfWindowsWidth - 15, 40)))
		{
			windows = {};
			EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&windows));
		}
		ImGui::EndChild();
		ImGui::EndChild();

		if (CurrentHWND != NULL) {
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Separator, ImColor(255, 255, 255, 255).Value);
			ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
			ImGui::PopStyleColor();

			ImGui::SameLine();
			ImGui::BeginChild("##right_side", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
			{
				int ContentRegion = ImGui::GetContentRegionAvail().x;
				int ElementsWidth = ContentRegion * 0.7;

				ImGui::SetCursorPosX((ContentRegion / 2) - (ImGui::CalcTextSize("Crosshair Length").x / 2));
				ImGui::Text("Crosshair Length");
				ImGui::SetCursorPosX((ContentRegion / 2) - (ElementsWidth / 2));
				ImGui::SliderInt("##CrosshairLength", &windows.Crosshairs[IndexOfSelectedItem].Length, 1, 100);

				ImGui::SetCursorPosX((ContentRegion / 2) - (ImGui::CalcTextSize("Crosshair Height").x / 2));
				ImGui::Text("Crosshair Height");
				ImGui::SetCursorPosX((ContentRegion / 2) - (ElementsWidth / 2));
				ImGui::SliderInt("##CrosshairHeight", &windows.Crosshairs[IndexOfSelectedItem].Height, 1, 100);

				ImGui::SetCursorPosX((ContentRegion / 2) - (ImGui::CalcTextSize("Crosshair Width").x / 2));
				ImGui::Text("Crosshair Width");
				ImGui::SetCursorPosX((ContentRegion / 2) - (ElementsWidth / 2));
				ImGui::SliderInt("##CrosshairWidth", &windows.Crosshairs[IndexOfSelectedItem].Width, 1, 100);

				ImGui::SetCursorPosX((ContentRegion / 2) - (ImGui::CalcTextSize("Crosshair Color").x / 2));
				ImGui::Text("Crosshair Color");
				ImGui::SetCursorPosX((ContentRegion / 2) - (ElementsWidth / 2));
				ImGui::ColorEdit3("##CrosshairColor", windows.Crosshairs[IndexOfSelectedItem].RGBcolor);

				ImGui::SetCursorPosX((ContentRegion / 2) - (ImGui::CalcTextSize("Centered Crosshair").x / 2) - 12);
				ImGui::Checkbox("Centered Crosshair", &windows.Crosshairs[IndexOfSelectedItem].Centered);
				if (!windows.Crosshairs[IndexOfSelectedItem].Centered) {
					ImGui::SetCursorPosX((ContentRegion / 2) - (ImGui::CalcTextSize("Crosshair X Coordinate").x / 2));
					ImGui::Text("Crosshair X Coordinate");
					ImGui::SetCursorPosX((ContentRegion / 2) - (ElementsWidth / 2));
					ImGui::SliderInt("##CrosshairXcoordinate", &windows.Crosshairs[IndexOfSelectedItem].CoordinateX, 1, windows.Crosshairs[IndexOfSelectedItem].CrosshairWindowWidth);

					ImGui::SetCursorPosX((ContentRegion / 2) - (ImGui::CalcTextSize("Crosshair Y Coordinate").x / 2));
					ImGui::Text("Crosshair Y Coordinate");
					ImGui::SetCursorPosX((ContentRegion / 2) - (ElementsWidth / 2));
					ImGui::SliderInt("##CrosshairYcoordinate", &windows.Crosshairs[IndexOfSelectedItem].CoordinateY, 1, windows.Crosshairs[IndexOfSelectedItem].CrosshairWindowHeight);
				}
			}
			ImGui::EndChild();
		}

		ImGui::PopStyleVar();
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}