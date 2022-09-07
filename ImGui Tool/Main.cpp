#include "Main.h"


static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



void Helpmarker(const char* Text, ImVec4 Color)
{
    ImGui::TextColored(Color, u8"(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(Text);
    }
}


int main(int, char**)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Tool"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("ImGui Tool"), WS_OVERLAPPEDWINDOW, 100, 100, 1, 1, NULL, NULL, wc.hInstance, NULL);

    
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_HIDE);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;


    ImGui::StyleColorsDark();


    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    io.IniFilename = nullptr;
    ImFontConfig Font_cfg;
    Font_cfg.FontDataOwnedByAtlas = false;

    //ImFont* Font = io.Fonts->AddFontFromFileTTF("..\\ImGui Tool\\Font.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    ImFont* Font = io.Fonts->AddFontFromMemoryTTF((void*)Font_data, Font_size, 18.0f, &Font_cfg, io.Fonts->GetGlyphRangesChineseFull());
    ImFont* Font_Big = io.Fonts->AddFontFromMemoryTTF((void*)Font_data, Font_size, 24.0f, &Font_cfg, io.Fonts->GetGlyphRangesChineseFull());

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        {
            ImGuiStyle& Style = ImGui::GetStyle();
            auto Color = Style.Colors;

            static bool WinPos = true;//用于初始化窗口位置
            int Screen_Width{ GetSystemMetrics(SM_CXSCREEN) };//获取显示器的宽
            int Screen_Heigth{ GetSystemMetrics(SM_CYSCREEN) };//获取显示器的高

            static bool CheckBox_1 = false, CheckBox_2 = true;
            static int InputInt = 0;
            static int Comb = 0;
            static float InputFloat = 0;
            static char InputString[80] = { '?' };

            static int Tab = 0;
            enum Tab
            {
                Panel,
                Button,
                Input,
                CheckBox
            };

            static int Color_ = 0;
            enum Color_
            {
                Red,
                Green,
                Blue,
                Orange
            };

            switch (Color_)
            {
            case Color_::Red:
                Style.ChildRounding = 8.0f;
                Style.FrameRounding = 5.0f;

                Color[ImGuiCol_Button] = ImColor(192, 51, 74, 255);
                Color[ImGuiCol_ButtonHovered] = ImColor(212, 71, 94, 255);
                Color[ImGuiCol_ButtonActive] = ImColor(172, 31, 54,255);

                Color[ImGuiCol_FrameBg] = ImColor(54, 54, 54, 150);
                Color[ImGuiCol_FrameBgActive] = ImColor(42, 42, 42, 150);
                Color[ImGuiCol_FrameBgHovered] = ImColor(100, 100, 100, 150);

                Color[ImGuiCol_CheckMark] = ImColor(192, 51, 74, 255);

                Color[ImGuiCol_SliderGrab] = ImColor(192, 51, 74, 255);
                Color[ImGuiCol_SliderGrabActive] = ImColor(172, 31, 54, 255);

                Color[ImGuiCol_Header] = ImColor(192, 51, 74, 255);
                Color[ImGuiCol_HeaderHovered] = ImColor(212, 71, 94, 255);
                Color[ImGuiCol_HeaderActive] = ImColor(172, 31, 54, 255);
                break;
            case Color_::Green:
                Style.ChildRounding = 8.0f;
                Style.FrameRounding = 5.0f;

                Color[ImGuiCol_Button] = ImColor(10, 105, 56, 255);
                Color[ImGuiCol_ButtonHovered] = ImColor(30, 125, 76, 255);
                Color[ImGuiCol_ButtonActive] = ImColor(0, 95, 46, 255);

                Color[ImGuiCol_FrameBg] = ImColor(54, 54, 54, 150);
                Color[ImGuiCol_FrameBgActive] = ImColor(42, 42, 42, 150);
                Color[ImGuiCol_FrameBgHovered] = ImColor(100, 100, 100, 150);

                Color[ImGuiCol_CheckMark] = ImColor(10, 105, 56, 255);

                Color[ImGuiCol_SliderGrab] = ImColor(10, 105, 56, 255);
                Color[ImGuiCol_SliderGrabActive] = ImColor(0, 95, 46, 255);

                Color[ImGuiCol_Header] = ImColor(10, 105, 56, 255);
                Color[ImGuiCol_HeaderHovered] = ImColor(30, 125, 76, 255);
                Color[ImGuiCol_HeaderActive] = ImColor(0, 95, 46, 255);
                
                break;
            case Color_::Blue:
                Style.ChildRounding = 8.0f;
                Style.FrameRounding = 5.0f;

                Color[ImGuiCol_Button] = ImColor(51, 120, 255, 255);
                Color[ImGuiCol_ButtonHovered] = ImColor(71, 140, 255, 255);
                Color[ImGuiCol_ButtonActive] = ImColor(31, 100, 225, 255);

                Color[ImGuiCol_FrameBg] = ImColor(54, 54, 54, 150);
                Color[ImGuiCol_FrameBgActive] = ImColor(42, 42, 42, 150);
                Color[ImGuiCol_FrameBgHovered] = ImColor(100, 100, 100, 150);

                Color[ImGuiCol_CheckMark] = ImColor(51, 120, 255, 255);

                Color[ImGuiCol_SliderGrab] = ImColor(51, 120, 255, 255);
                Color[ImGuiCol_SliderGrabActive] = ImColor(31, 100, 225, 255);

                Color[ImGuiCol_Header] = ImColor(51, 120, 255, 255);
                Color[ImGuiCol_HeaderHovered] = ImColor(71, 140, 255, 255);
                Color[ImGuiCol_HeaderActive] = ImColor(31, 100, 225, 255);

                break;
            case Color_::Orange://233,87,33
                Style.ChildRounding = 8.0f;
                Style.FrameRounding = 5.0f;

                Color[ImGuiCol_Button] = ImColor(233, 87, 33, 255);
                Color[ImGuiCol_ButtonHovered] = ImColor(253, 107, 53, 255);
                Color[ImGuiCol_ButtonActive] = ImColor(213, 67, 13, 255);

                Color[ImGuiCol_FrameBg] = ImColor(54, 54, 54, 150);
                Color[ImGuiCol_FrameBgActive] = ImColor(42, 42, 42, 150);
                Color[ImGuiCol_FrameBgHovered] = ImColor(100, 100, 100, 150);

                Color[ImGuiCol_CheckMark] = ImColor(233, 87, 33, 255);

                Color[ImGuiCol_SliderGrab] = ImColor(233, 87, 33, 255);
                Color[ImGuiCol_SliderGrabActive] = ImColor(213, 67, 13, 255);

                Color[ImGuiCol_Header] = ImColor(233, 87, 33, 255);
                Color[ImGuiCol_HeaderHovered] = ImColor(253, 107, 53, 255);
                Color[ImGuiCol_HeaderActive] = ImColor(213, 67, 13, 255);

                break;
            }


            if (WinPos)//初始化窗口
            {
                ImGui::SetNextWindowPos({ float(Screen_Width - 600) / 2,float(Screen_Heigth - 400) / 2 });
                WinPos = false;//初始化完毕
            }


            ImGui::Begin(u8"ImGui Tool", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);//开始绘制窗口
            ImGui::SetWindowSize({ 600.0f,400.0f });//设置窗口大小


            {
                ImGui::GetWindowDrawList()->AddLine({ ImGui::GetWindowPos().x + 420.0f,ImGui::GetWindowPos().y + 10.0f }, { ImGui::GetWindowPos().x + 420.0f,ImGui::GetWindowPos().y + 390.0f }, ImColor(100, 100, 100, 255));

                ImGui::SetCursorPos({ 430.0f,20.0f });
                ImGui::PushFont(Font_Big);
                ImGui::TextColored(Color[ImGuiCol_Button], u8"ImGui Tool \u9B08");
                ImGui::PopFont();



                ImGui::SetCursorPos({ 430.0f,65.0f });

                ImGui::PushStyleColor(ImGuiCol_Button, Tab == Tab::Panel ? Color[ImGuiCol_Button] : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                if (ImGui::Button(u8"Panel", { 150.0f,40.0f }))
                {
                    Tab = Tab::Panel;
                }
                ImGui::PopStyleColor();

                ImGui::PushStyleColor(ImGuiCol_Button, Tab == Tab::Button ? Color[ImGuiCol_Button] : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::SetCursorPos({ 430.0f,115.0f });
                if (ImGui::Button(u8"Button", { 150.0f,40.0f }))
                {
                    Tab = Tab::Button;
                }
                ImGui::PopStyleColor();

                ImGui::PushStyleColor(ImGuiCol_Button, Tab == Tab::Input ? Color[ImGuiCol_Button] : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::SetCursorPos({ 430.0f,165.0f });
                if (ImGui::Button(u8"Input", { 150.0f,40.0f }))
                {
                    Tab = Tab::Input;
                }
                ImGui::PopStyleColor();

                ImGui::PushStyleColor(ImGuiCol_Button, Tab == Tab::CheckBox ? Color[ImGuiCol_Button] : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::SetCursorPos({ 430.0f,215.0f });
                if (ImGui::Button(u8"CheckBox", { 150.0f,40.0f }))
                {
                    Tab = Tab::CheckBox;
                }
                ImGui::PopStyleColor();

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::SetCursorPos({ 430.0f,265.0f });
                if (ImGui::Button(u8"Exit", { 150.0f,40.0f }))
                {
                    exit(0);
                }
                ImGui::PopStyleColor();

                ImGui::SetCursorPos({ 430.0f,330.0f });
                ImGui::Text(u8"主题颜色");
                ImGui::SameLine();
                ImGui::SetCursorPos({ 505.0f,328.0f });
                ImGui::SetNextItemWidth(80.0f);
                ImGui::Combo(u8" ", &Color_, u8"红色\0绿色\0蓝色\0橙色");

                time_t t = time(0);
                char tmp[32] = { NULL };
                strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M", localtime(&t));

                ImGui::SetCursorPos({ 430.0f,365.0f });
                ImGui::TextColored(Color[ImGuiCol_Button],"%s", tmp);
            }


            ImGui::SetCursorPos({ 10.0f,10.0f });
            ImGui::BeginChild(u8"Fucking", { 400.0f,380.0f }, true);
            switch (Tab)
            {
            case Tab::Panel:
                ImGui::PushFont(Font_Big);
                ImGui::BulletText(u8"Panel");
                ImGui::PopFont();
                ImGui::SameLine();
                Helpmarker(u8"面板示例", Color[ImGuiCol_Button]);
                ImGui::Separator();
                ImGui::TextColored(Color[ImGuiCol_Button], u8"\t这种东西我没法和你解释\n因为我只是个小面板子");

                break;
            case Tab::Button:
                ImGui::PushFont(Font_Big);
                ImGui::BulletText(u8"Button");
                ImGui::PopFont();
                ImGui::SameLine();
                Helpmarker(u8"按钮示例", Color[ImGuiCol_Button]);
                ImGui::Separator();

                ImGui::Button(u8"我是个按钮子", {120.0f,40.0f});
                ImGui::Button(u8"按钮子", {70.0f,35.0f});
                break;
            case Tab::Input:
                ImGui::PushFont(Font_Big);
                ImGui::BulletText(u8"Input");
                ImGui::PopFont();
                ImGui::SameLine();
                Helpmarker(u8"输入示例", Color[ImGuiCol_Button]);
                ImGui::Separator();

                ImGui::InputInt(u8"Int类型输入", &InputInt);
                ImGui::InputFloat(u8"Float类型输入", &InputFloat);
                ImGui::InputText(u8"char[]类型输入", &InputString[80], IM_ARRAYSIZE(InputString));
                ImGui::SliderInt(u8"Int类型滑块", &InputInt, 0, 100);
                ImGui::SliderFloat(u8"Float类型滑块", &InputFloat, 0.0F, 100.0F);
                ImGui::Combo(u8"选择框", &Comb, u8"A11\0A22\0A33\0A44");
                break;
            case Tab::CheckBox:
                ImGui::PushFont(Font_Big);
                ImGui::BulletText(u8"CheckBox");
                ImGui::PopFont();
                ImGui::SameLine();
                Helpmarker(u8"复选框示例", Color[ImGuiCol_Button]);
                ImGui::Separator();

                ImGui::Checkbox(u8"复选框-1", &CheckBox_1);
                ImGui::Checkbox(u8"复选框-2", &CheckBox_2);
                break;
            }
            ImGui::EndChild();


            ImGui::End();
        }

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        g_pSwapChain->Present(1, 0); 
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}


bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
