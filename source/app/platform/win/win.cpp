#include "StdInc.h"

#include <Windows.h>

#include "win.h"

#include "VideoPlayer.h"
#include "Gamma.h"

// #include "InputEvents.h"
#include "platform.h"

#include "PostEffects.h"
#include "Clouds.h"
#include "Skidmarks.h"
#include "LoadingScreen.h"
#include "VideoMode.h"
#include "ControllerConfigManager.h"
#include "Input.h"

extern void WinPsInjectHooks();

static LPSTR AppClassName = LPSTR(APP_CLASS);

// forward declarations
bool IsAlreadyRunning();
char** CommandLineToArgv(char* cmdLine, int* argCount);
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT WINAPI __WinMain(HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR cmdLine, INT nCmdShow);

void Win32InjectHooks() {
    RH_ScopedCategory("Win");
    RH_ScopedNamespaceName("Win");

    RH_ScopedGlobalInstall(IsForegroundApp, 0x746060);
    RH_ScopedGlobalInstall(IsAlreadyRunning, 0x7468E0);
    RH_ScopedGlobalInstall(CommandLineToArgv, 0x746480, {.reversed = false});

    RH_ScopedGlobalInstall(MainWndProc, 0x747EB0, {.reversed = false});
    RH_ScopedNamedGlobalInstall(__WinMain, "WinMain", 0x745560, {.reversed = false});

    WinPsInjectHooks();
    WinInput::InjectHooks();
}

// @notsa
// @brief Resets the screen gamma if ever changed.
void ResetGammaWhenExiting() {
    if (gammaChanged) {
        if (auto d3dDevice = (IDirect3DDevice9*)RwD3D9GetCurrentD3DDevice()) {
            d3dDevice->SetGammaRamp(0u, D3DSGR_CALIBRATE, savedGamma);
        }
        gammaChanged = false;
    }
}

// 0x746870
void MessageLoop() {
    tagMSG msg;
    while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE | PM_NOYIELD)) {
        if (msg.message == WM_QUIT) {
            RsGlobal.quit = true;
        } else {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }
}

// 0x7486A0
bool InitApplication(HINSTANCE hInstance) {
    WNDCLASS windowClass      = { 0 };
    windowClass.style         = CS_BYTEALIGNWINDOW;
    windowClass.lpfnWndProc   = MainWndProc;
    windowClass.hInstance     = hInstance;
    // windowClass.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    windowClass.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    windowClass.lpszClassName = AppClassName;
    return RegisterClass(&windowClass);
}

// 0x745560
HWND InitInstance(HINSTANCE hInstance) {
    RECT rect = { 0, 0, RsGlobal.maximumWidth, RsGlobal.maximumHeight };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    return CreateWindowEx(
        0,
        APP_CLASS,
        RsGlobal.appName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );
}

// 0x7468E0
bool IsAlreadyRunning() {
    CreateEventA(nullptr, false, true, AppClassName);
    if (GetLastError() != ERROR_ALREADY_EXISTS) {
        return false;
    }

    HWND window = FindWindowA(AppClassName, RsGlobal.appName);
    if (window)
        SetForegroundWindow(window);
    else
        SetForegroundWindow(PSGLOBAL(window));

    return true;
};

// 0x746060
bool IsForegroundApp() {
    return ForegroundApp;
}

// 0x746480
char** CommandLineToArgv(char* cmdLine, int* argCount) {
    return plugin::CallAndReturn<char**, 0x746480, char*, int*>(cmdLine, argCount);
}

// 0x747EB0
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return plugin::CallAndReturn<LRESULT, 0x747EB0, HWND, UINT, WPARAM, LPARAM>(hwnd, uMsg, wParam, lParam);
}

// 0x748710
INT WINAPI __WinMain(HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR cmdLine, INT nCmdShow) {
    SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0u, nullptr, 2);
    if (IsAlreadyRunning()) {
        return false;
    }

    auto initializeEvent = RsEventHandler(rsINITIALIZE, nullptr);
    if (rsEVENTERROR == initializeEvent) {
        return false;
    }

    if (!InitApplication(instance)) {
        return false;
    }

    cmdLine = GetCommandLine();
    int argc;
    char** argv = CommandLineToArgv(cmdLine, &argc);
    for (int i = 0; i < argc; i++) {
        RsEventHandler(rsPREINITCOMMANDLINE, argv[i]);
    }

    PSGLOBAL(window) = InitInstance(instance);
    if (!PSGLOBAL(window)) {
        return false;
    }
    PSGLOBAL(instance) = instance;

    // 0x7487CF
    WinInput::Initialise();

    // todo: CInputEvents::MapMouseButtons && See ASM
    // const auto state = GetMouseButtonMask();
    // ControlsManager.InitDefaultControlConfigMouse(state, FrontEndMenuManager.m_nController == 0);
    ControlsManager.InitDefaultControlConfigMouse(WinInput::GetMouseButtonMask(), !FrontEndMenuManager.m_nController);

    // 0x748847
    if (RsEventHandler(rsRWINITIALIZE, PSGLOBAL(window)) == rsEVENTERROR) {
        DestroyWindow(PSGLOBAL(window));
        RsEventHandler(rsTERMINATE, nullptr);
        return false;
    }

    // 0x7488EE
    for (auto i = 0; i < argc; i++) {
        RsEventHandler(rsCOMMANDLINE, argv[i]);
    }

    if (MultipleVideoModes || PSGLOBAL(fullScreen)) {
        SetWindowLongPtr(PSGLOBAL(window), -16, (LONG_PTR) 0x80000000);
        SetWindowPos(PSGLOBAL(window), nullptr, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }

    RwRect rect{ 0, 0, RsGlobal.maximumWidth, RsGlobal.maximumHeight };
    RsEventHandler(rsCAMERASIZE, &rect);

    SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, 0u, nullptr, 2u);
    SystemParametersInfo(SPI_SETPOWEROFFACTIVE, 0u, nullptr, 2u);
    SystemParametersInfo(SPI_SETLOWPOWERACTIVE, 0u, nullptr, 2u);
    STICKYKEYS pvParam { .cbSize = sizeof(STICKYKEYS) };
    SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &pvParam, 2u);
    STICKYKEYS pvParam1 = { .cbSize = sizeof(STICKYKEYS), .dwFlags = SKF_TWOKEYSOFF };
    SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &pvParam1, 2u);

    ShowWindow(PSGLOBAL(window), nCmdShow);
    UpdateWindow(PSGLOBAL(window));

    // 0x748995
    CFileMgr::SetDirMyDocuments();
    if (auto* file = CFileMgr::OpenFile("gta_sa.set", "rb")) {
        if (!ControlsManager.LoadSettings(file)) {
            ControlsManager.ReinitControls();
        }
        CFileMgr::CloseFile(file);
    }
    CFileMgr::SetDir("");

    SetErrorMode(SEM_FAILCRITICALERRORS);

    bool bNewGameFirstTime = true;
    MSG Msg{};

    // 0x7489FB
    while (true) {
        RwInitialized = true;

        RwV2d pos{ SCREEN_WIDTH * 2.0f, SCREEN_HEIGHT * 2.0f };
        RsMouseSetPos(&pos);

        gamma.Init();

        while (!RsGlobal.quit) {
            if (FrontEndMenuManager.m_bStartGameLoading) {
                break;
            }

            if (PeekMessage(&Msg, nullptr, 0u, 0u, PM_REMOVE | PM_NOYIELD)) {
                if (Msg.message == WM_QUIT) {
                    break;
                }
                TranslateMessage(&Msg);
                DispatchMessageA(&Msg);
            } else if (ForegroundApp) {
                switch (gGameState) {
                case GAME_STATE_INITIAL:
                    CLoadingScreen::LoadSplashes(true, false);
                    CLoadingScreen::Init(true, true);
                    CLoadingScreen::DoPCTitleFadeOut();
                    CLoadingScreen::DoPCTitleFadeIn();
                    CLoadingScreen::Shutdown();

                    CLoadingScreen::LoadSplashes(true, true);
                    CLoadingScreen::Init(true, true);
                    CLoadingScreen::DoPCTitleFadeOut();
                    CLoadingScreen::DoPCTitleFadeIn();
                    CLoadingScreen::Shutdown();
                    gGameState = GAME_STATE_LOGO;
                    break;
                case GAME_STATE_LOGO:
                    if (!Windowed) {
                        VideoPlayer::Play(nCmdShow, "movies\\Logo.mpg");
                    }
                    gGameState = GAME_STATE_PLAYING_LOGO;
                    break;
                case GAME_STATE_PLAYING_LOGO:
                case GAME_STATE_PLAYING_INTRO: // 0x748B17
                {
                    CPad::UpdatePads();
                    auto* pad = CPad::GetPad();
                    if (Windowed || ControlsManager.GetJoyButtonJustDown() || pad->NewState.CheckForInput() || CPad::IsMouseLButtonPressed() || CPad::IsEnterJustPressed() ||
                        pad->IsStandardKeyJustPressed(VK_SPACE) || CPad::IsMenuKeyJustPressed() || CPad::IsTabJustPressed()) {
                        gGameState = eGameState(gGameState + 1);
                    }
                    break;
                }
                case GAME_STATE_TITLE:
                    VideoPlayer::Shutdown();
                    VideoPlayer::Play(nCmdShow, FrontEndMenuManager.GetMovieFileName());
                    gGameState = GAME_STATE_PLAYING_INTRO;
                    break;
                case GAME_STATE_FRONTEND_LOADING:
                    VideoPlayer::Shutdown();
                    CLoadingScreen::Init(true, false);
                    CLoadingScreen::DoPCTitleFadeOut();
                    if (!CGame::InitialiseEssentialsAfterRW()) {
                        RsGlobal.quit = true;
                    }
                    CGame::InitialiseCoreDataAfterRW();
                    gGameState = GAME_STATE_FRONTEND_LOADED;
                    anisotropySupportedByGFX = (RwD3D9GetCaps()->RasterCaps & D3DPRASTERCAPS_ANISOTROPY) != 0; // todo: func
                    break;
                case GAME_STATE_FRONTEND_LOADED:
                    FrontEndMenuManager.m_bMenuActive = true;
                    FrontEndMenuManager.m_bMainMenuSwitch = true;
                    if (VideoModeNotSelected) {
                        FrontEndMenuManager.m_nPrefsVideoMode = gCurrentVideoMode;
                        FrontEndMenuManager.m_nDisplayVideoMode = gCurrentVideoMode;
                        VideoModeNotSelected = false;
                    }
                    gGameState = GAME_STATE_FRONTEND_IDLE;
                    CLoadingScreen::DoPCTitleFadeIn();
                    break;
                case GAME_STATE_FRONTEND_IDLE: // 0x748CB2
                {
                    WINDOWPLACEMENT windowPlacement{.length = 44};
                    GetWindowPlacement(PSGLOBAL(window), &windowPlacement);
                    if (windowPlacement.showCmd != SW_SHOWMINIMIZED) {
                        RsEventHandler(rsFRONTENDIDLE, nullptr);
                    }

                    if (!FrontEndMenuManager.m_bMenuActive || FrontEndMenuManager.m_bLoadingData) {
                        gGameState = GAME_STATE_LOADING_STARTED;
                        if (FrontEndMenuManager.m_bLoadingData) {
                            gGameState = GAME_STATE_LOADING_STARTED;
                        }
                    }
                    break;
                }
                case GAME_STATE_LOADING_STARTED:
                    AudioEngine.StartLoadingTune();
                    InitialiseGame();
                    gGameState = GAME_STATE_IDLE;
                    FrontEndMenuManager.m_bMainMenuSwitch = false;
                    AudioEngine.InitialisePostLoading();
                    break;
                case GAME_STATE_IDLE: {
                    if (!RwInitialized)
                        break;

                    auto v9_1 = 1000.0f / (float)RsGlobal.frameLimit;
                    auto v9_2 = (float)CTimer::GetCurrentTimeInCycles() / (float)CTimer::GetCyclesPerMillisecond();
                    if (!FrontEndMenuManager.m_bPrefsFrameLimiter && CReplay::Mode != eReplayMode::MODE_PLAYBACK && !AudioEngine.IsBeatInfoPresent() || v9_1 < v9_2) {
                        RsEventHandler(rsIDLE, (void*)true);
                    }
                    break;
                }
                default:
                    break;
                }
                if (!isForeground) {
                    isForeground = true;
                }
            } else {
                if (isForeground) {
                    isForeground = false;
                }
                Sleep(100);
            }
        }

        // 0x748DDA
        RwInitialized = false;
        FrontEndMenuManager.UnloadTextures();
        if (!FrontEndMenuManager.m_bStartGameLoading) {
            break;
        }

        // load game
        CCheat::ResetCheats();
        CTimer::Stop();

        if (FrontEndMenuManager.m_bStartGameLoading) {
            CGame::ShutDownForRestart();
            CGame::InitialiseWhenRestarting();
            FrontEndMenuManager.m_bStartGameLoading = false;
        } else if (bNewGameFirstTime) {
            CTimer::Stop();
            gGameState = [&] {
                if (FrontEndMenuManager.m_nGameState != 1) {
                    return GAME_STATE_LOADING_STARTED;
                } else {
                    return GAME_STATE_FRONTEND_LOADED;
                }
            }();
        } else {
            CCheat::ResetCheats();
            CGame::ShutDownForRestart();
            CTimer::Stop();
            CGame::InitialiseWhenRestarting();
        }
        bNewGameFirstTime = false;
        FrontEndMenuManager.m_nGameState = 0;
        FrontEndMenuManager.m_bStartGameLoading = false;
    }

    // if game is loaded, shutdown it
    if (gGameState == GAME_STATE_IDLE) {
        CGame::Shutdown();
    }

    // now quit 0x748E75
    AudioEngine.Shutdown();
    FreeVideoModeList();
    RsEventHandler(rsRWTERMINATE, nullptr);
    DestroyWindow(PSGLOBAL(window));
    RsEventHandler(rsTERMINATE, nullptr);
    free(argv);
    ShowCursor(true);

    SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &pvParam, 2u);
    SystemParametersInfo(SPI_SETPOWEROFFACTIVE, 1u, nullptr, 2u); // TODO: GUID_VIDEO_POWERDOWN_TIMEOUT
    SystemParametersInfo(SPI_SETLOWPOWERACTIVE, 1u, nullptr, 2u);
    SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, 1u, nullptr, 2u);
    // nullsub_0x72F3C0()
    SetErrorMode(0);

    return Msg.wParam;
}
