#include "Game.h"
//DXTK headers
#include "SimpleMath.h"

//system headers
#include <windows.h>
#include <time.h>

//our headers
#include "ObjectList.h"
#include "GameData.h"
#include "drawdata.h"
#include "DrawData2D.h"
#include <DirectXColors.h>

//anttweakbar
#include <AntTweakBar.h>
#include <Application\Application.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;


Game::Game(ID3D11Device* _pd3dDevice, HWND _hWnd, HINSTANCE _hInstance) 
{
	//set up audio
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	m_audioEngine.reset(new AudioEngine(eflags));

	
	//Create DirectXTK spritebatch stuff
	_pd3dDevice->GetImmediateContext(&pd3dImmediateContext);
	m_DD2D = new DrawData2D();
	m_DD2D->m_Sprites.reset(new SpriteBatch(pd3dImmediateContext));
	m_DD2D->m_Font.reset(new SpriteFont(_pd3dDevice, L"..\\Assets\\italic.spritefont"));


	//seed the random number generator
	srand((UINT)time(NULL));

	//Direct Input Stuff
	m_hWnd = _hWnd;
	m_pKeyboard = nullptr;
	m_pDirectInput = nullptr;

	HRESULT hr = DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);
	hr = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = m_pKeyboard->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	hr = m_pMouse->SetDataFormat(&c_dfDIMouse);
	hr = m_pMouse->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//create GameData struct and populate its pointers
	m_GD = new GameData;
	m_GD->m_keyboardState = m_keyboardState;
	m_GD->m_prevKeyboardState = m_prevKeyboardState;
	m_GD->m_GS = GS_PLAY_MAIN_CAM;
	m_GD->m_mouseState = &m_mouseState;

	//set up DirectXTK Effects system
	m_fxFactory = new EffectFactory(_pd3dDevice);

	//Tell the fxFactory to look to the correct build directory to pull stuff in from
#ifdef DEBUG
	((EffectFactory*)m_fxFactory)->SetDirectory(L"../Debug");
#else
	((EffectFactory*)m_fxFactory)->SetDirectory(L"../Release");
#endif

	// Create other render resources here
	m_states = new CommonStates(_pd3dDevice);

	//init render system for VBGOs
	VBGO::Init(_pd3dDevice);

	//find how big my window is to correctly calculate my aspect ratio
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	float AR = (float)width / (float)height;

	//add window dimensions to game data
	m_GD->window_width = width;
	m_GD->window_height = height;

	//create a base camera
	m_cam = new Camera(0.25f * XM_PI, AR, 1.0f, 10000.0f, Vector3::UnitY, Vector3::Zero);
	m_cam->SetPos(Vector3(0.0f, 100.0f, 100.0f));
	m_GameObjects.push_back(m_cam);

	//create a base light
	m_light = new Light(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.4f, 0.1f, 0.1f, 1.0f));
	m_GameObjects.push_back(m_light);


	//add Player
	pPlayer = new Player("BirdModelV1.cmo", _pd3dDevice, m_fxFactory);
	m_GameObjects.push_back(pPlayer);

	Terrain* table = new Terrain("table.cmo", _pd3dDevice, m_fxFactory, Vector3(0, -200, 0), 0.0f, 0.0f, 0.0f, Vector3::One);
	m_GameObjects.push_back(table);
	
	////add a secondary camera
	m_TPScam = new TPSCamera(0.25f * XM_PI, AR, 1.0f, 10000.0f, pPlayer, Vector3::UnitY, Vector3(0.0f, 30.0f, 100.0f));
	m_GameObjects.push_back(m_TPScam);

	//create DrawData struct and populate its pointers
	m_DD = new DrawData;
	m_DD->m_pd3dImmediateContext = nullptr;
	m_DD->m_states = m_states;
	m_DD->m_cam = m_cam;
	m_DD->m_light = m_light;

	srand(time(0));
	
	ParticleEmitter2D* explosionEmitter = new ParticleEmitter2D(_pd3dDevice, "whitecircle", 300.0f, 200.0f, 1.0f, 0.1f, 180.0f, 180.0f, 700.0f, 100.0f, 0.03f, 0.01f, Color(1.0f, 1.0, 0.9, 0.8f), Color(1.0f, 0, 0, 0.0f), 1.0f, 0.0f, -1.0f, 50);
	m_GameObject2Ds.push_back(explosionEmitter);

	ParticleEmitter2D* explosionEmitter2 = new ParticleEmitter2D(_pd3dDevice, "whitecircle", 300.0f, 200.0f, 1.0f, 0.25f, 180.0f, 180.0f, 150.0f, 70.0f, 0.07f, 0.01f, Color(1.0f, 1.0f, 1.0, 0.9f), Color(1.0f, 1.0f, 0.0f, 0.4f), 1.0f, 0.0f, -0.0f, 100);
	m_GameObject2Ds.push_back(explosionEmitter2);

	ParticleEmitter2D* smokeEmitter = new ParticleEmitter2D(_pd3dDevice, "whitecircle", 400.0f, 700.0f, 2.0f, 1.0f, 90.0f, 90.0f, 80.0f, 20.0f, 0.07f, 0.01f, Color(0.1f, 0.1f, 0.1f, 0.3f), Color(0.1f, 0.1f, 0.1f, 0.3f), 0.1f, 0.0f, 0.0f, 75);
	m_GameObject2Ds.push_back(smokeEmitter);

	ParticleEmitter2D* fireEmitter = new ParticleEmitter2D(_pd3dDevice, "whitecircle", 400.0f, 700.0f, 2.0f, 1.0f, 90.0f, 20.0f, 100.0f, 20.0f, 0.05f, 0.01f, Color(0.9f, 0.5f, 0.1f, 0.3f), Color(0.9f, 0.1f, 0.1f, 0.1f), 0.05f, 0.0f, 0.01f, 75);
	m_GameObject2Ds.push_back(fireEmitter);

	ParticleEmitter2D* waterEmitter = new ParticleEmitter2D(_pd3dDevice, "whitecircle", 
		700.0f, 200.0f, 
		6.0f, 1.5f, 45.0f, 5.0f, 
		400.0f, 20.0f, 
		0.09f, 0.01f, 
		Color(0.2f, 0.0f,0.8f, 0.9f), Color(0.8f, 0.2f, 0.2f, 0.2f), 
		1.5f, 0.0f, 0.0f, 200);
	m_GameObject2Ds.push_back(waterEmitter);

	//ParticleEmitter2D* waterEmitter = new ParticleEmitter2D(_pd3dDevice, "whitecircle", 700.0f, 200.0f, 5.0f, 1.5f, 80.0f, 5.0f, 120.0f, 20.0f, 0.09f, 0.01f, DirectX::Colors::Red, DirectX::Colors::Blue, 1.5f, true, 200);
	//m_GameObject2Ds.push_back(waterEmitter);
	//
	//ParticleEmitter2D* muzzle = new ParticleEmitter2D(_pd3dDevice, "whitecircle", 500.0f, 500.0f, 0.1f, 0.02f, 0.0f, 30.0f, 500.0f, 0.0f, 0.05f, 0.0f, Color(0.3f, 0.1f, 0, 0.9f), Color(0.3f, 0.1f, 0, 0.9f), 3.0f, false, 20);
	//m_GameObject2Ds.push_back(muzzle);
		

	emitter = new ParticleEmitter3D("..Assets/whitecircle.png", _pd3dDevice, m_fxFactory, m_TPScam, Vector3(0.0f, 0.0f, 0.0f),  //"table.cmo", _pd3dDevice, m_fxFactory, Vector3(0.0f, 0.0f, 0.0f),
		1.0f, 0.5f, 
		0.0f, 360.0f, 360.0f, -360.0f, 
		200.0f, 5.0f, 
		1.2f, 0.2f, 
		0.0f, -2.0f, 100);
	m_GameObjects.push_back(emitter);

	//pColour = Color(1.0f, 0.0f, 0.0f, 1.0f);
	pNum = emitter->GetParticleNum();
	// Initialize AntTweakBar
	if (!TwInit(TW_DIRECT3D11, _pd3dDevice))
	{
		MessageBoxA(_hWnd, TwGetLastError(), "AntTweakBar initialization failed", MB_OK | MB_ICONERROR);
		//Cleanup();
		//return 0;
	}
	//anttweakbar
	float atbVar = 42.0f;
	//TwInit(TW_DIRECT3D11, _pd3dDevice);
	TwWindowSize(m_GD->window_width, m_GD->window_height);

	

	TwBar *bar;
	bar = TwNewBar("Tweakerino");
	TwDefine(" GLOBAL help='This is an Ant Tweak Bar?.' ");
	int barSize[2] = { 224, 320 };
	TwSetParam(bar, NULL, "size", TW_PARAM_INT32, 2, barSize);

	//bar variables
	TwAddVarRW(bar, "Max No.", TW_TYPE_INT32, &pNum, "min=0 max=200 group=Emitter keyincr=< keydecr=>");
	TwAddVarRO(bar, "Current No.", TW_TYPE_INT32, &pCurNum, "group=Emitter");
	TwAddVarRW(bar, "Speed", TW_TYPE_FLOAT, &pSpeed, "min=0 max=1000 group=Emitter");
	TwAddVarRW(bar, "Speed Variation", TW_TYPE_FLOAT, &pSpeedVar, "min=0 max=100 group=Emitter");
	TwAddVarRW(bar, "Lifetime", TW_TYPE_FLOAT, &pLife, "min=0.1 max=30 step=0.1 group=Emitter");
	TwAddVarRW(bar, "X", TW_TYPE_FLOAT, &pPos[0], "min=-200 max=200 group=Location");
	TwAddVarRW(bar, "Y", TW_TYPE_FLOAT, &pPos[1], "min=-200 max=200 group=Location");
	TwAddVarRW(bar, "Z", TW_TYPE_FLOAT, &pPos[2], "min=-200 max=200 group=Location");
	TwAddVarRW(bar, "Follow Player", TW_TYPE_BOOLCPP, &pFollow, "group=Location"); 
	TwAddVarRW(bar, "Reset Position", TW_TYPE_BOOLCPP, &pResetPos, "group=Location");
	TwAddVarRW(bar, "Scale", TW_TYPE_FLOAT, &pScale, "min=0.1f max=10.0f step=0.1f group=Emitter");
	TwAddVarRW(bar, "Gravity", TW_TYPE_FLOAT, &pGrav, "min=-10 max=10 step = 0.2f group=Emitter");
	TwAddVarRW(bar, "Drag", TW_TYPE_FLOAT, &pDrag, "min=0 max = 5 step=0.2 group=Emitter");
	//TwAddVarRW(bar, "Colour", TW_TYPE_COLOR4F, &pCol, "colormode = hls"); due to how the vertices are created, I'm not sure how to change their colour on after initialization
};

void TW_CALL Game::SetCallBackPNum(const void *value, void *clientData)
{
	//tw_particleNum = *static_cast<const int *>(value);
	static_cast<ParticleEmitter3D *>(clientData)->SetParticleNum(value);
	static_cast<ParticleEmitter3D *>(clientData)->BuildEmitter();
}

void TW_CALL Game::GetCallBackPNum(void *value, void *clientData)
{
	//*static_cast<int *>(value) = tw_particleNum;
	*static_cast<float *>(value) = static_cast<ParticleEmitter3D *>(clientData)->GetParticleNum();
}

void TW_CALL Game::SetCallBackPColor(const void *value, void *clientData)
{
	//static_cast<ParticleEmitter3D *>(clientData)->SetParticleCol(value);
}

void TW_CALL Game::GetCallBackPColor(void *value, void *clientData)
{
	*static_cast<Color *>(value) = static_cast<ParticleEmitter3D *>(clientData)->GetParticleCol();
}

void TW_CALL Game::SetCallBackPRot(const void *value, void *clientData)
{
	//static_cast<ParticleEmitter3D *>(clientData)->SetParticleCol(value);
}

void TW_CALL Game::GetCallBackPRot(void *value, void *clientData)
{
	*static_cast<Quaternion *>(value) = static_cast<ParticleEmitter3D *>(clientData)->GetParticleCol();
}

LRESULT CALLBACK MessageProc(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//send event message to anttweakbar
	if (TwEventWin(wnd, message, wParam, lParam))
	{
		return 0;
	}
	switch (message)
	{
	default: 
		return DefWindowProc(wnd, message, wParam, lParam);
	}
}

Game::~Game() 
{
	//delete Game Data & Draw Data
	delete m_GD;
	delete m_DD;

	//tidy up VBGO render system
	VBGO::CleanUp();

	//tidy away Direct Input Stuff
	if (m_pKeyboard)
	{
		m_pKeyboard->Unacquire();
		m_pKeyboard->Release();
	}
	if (m_pMouse)
	{
		m_pMouse->Unacquire();
		m_pMouse->Release();
	}
	if (m_pDirectInput)
	{
		m_pDirectInput->Release();
	}

	//get rid of the game objects here
	for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		delete (*it);
	}
	m_GameObjects.clear();


	//and the 2D ones
	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{
		delete (*it);
	}
	m_GameObject2Ds.clear();

	//clear away CMO render system
	delete m_states;
	delete m_fxFactory;

	TwTerminate();

	delete m_DD2D;

};

bool Game::Tick() 
{
	emitter->SetParticleNum(&pNum);
	emitter->SetPScale(pScale);
	emitter->SetPDrag(pDrag);
	emitter->SetPGrav(pGrav);
	emitter->SetPSpeed(pSpeed);
	emitter->SetPSpeedVar(pSpeedVar);
	emitter->SetPCol(Color(pCol[0], pCol[1], pCol[2], pCol[3]));
	emitter->SetPLife(pLife);
	if (!pResetPos) 
	{
		if (pFollow)
		{
			emitter->SetPos(pPlayer->GetPos());
		}
		else
		{
			emitter->SetPos(Vector3(pPos[0], pPos[1], pPos[2]));
		}
	}
	else
	{
		pFollow = false;
		emitter->SetPos(Vector3::Zero);
		pResetPos = false;
	}
	//emitter->SetPitchYawRoll(pRot->x, pRot->y, pRot->z);
	//emitter->SetParticleCol(pColour);
	//tick audio engine
	if (!m_audioEngine->Update())
	{
		// No audio device is active
		if (m_audioEngine->IsCriticalError())
		{
			//something has gone wrong with audio so QUIT!
			return false;
		}
	}
	//Set up variables used to control tick rate
	auto now = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_updated);

	if (elapsed.count() >= m_targetFPS)
	{
		//Poll Keyboard & Mouse
		ReadInput();

		//Upon pressing escape QUIT
		if (m_keyboardState[DIK_ESCAPE] & 0x80)
		{
			return false;
		}

		//lock the cursor to the centre of the window
		RECT window;
		GetWindowRect(m_hWnd, &window);
		//SetCursorPos((window.left + window.right) >> 1, (window.bottom + window.top) >> 1);

		//calculate frame time-step dt for passing down to game objects
		DWORD currentTime = GetTickCount();
		m_GD->m_dt = min((float)(currentTime - m_playTime) / 1000.0f, 0.1f);
		m_playTime = currentTime;

		//start to a VERY simple FSM
		switch (m_GD->m_GS)
		{
		case GS_ATTRACT:
			break;
		case GS_PAUSE:
			break;
		case GS_GAME_OVER:
			break;
		case GS_PLAY_MAIN_CAM:
		case GS_PLAY_TPS_CAM:
			PlayTick();
			break;
		}
		m_updated = now;
	}
	
	//pRot->x = emitter->GetRoll();
	//pRot->y = emitter->GetPitch();
	//pRot->z = emitter->GetYaw();
	pCurNum = emitter->GetCurrentPNum();
	return true;
};

void Game::PlayTick()
{
	//upon space bar switch camera state
	if ((m_keyboardState[DIK_SPACE] & 0x80) && !(m_prevKeyboardState[DIK_SPACE] & 0x80))
	{
		if (m_GD->m_GS == GS_PLAY_MAIN_CAM)
		{
			m_GD->m_GS = GS_PLAY_TPS_CAM;
		}
		else
		{
			m_GD->m_GS = GS_PLAY_MAIN_CAM;
		}
	}


	//update all objects
	for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Tick(m_GD);
	}
	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{
		(*it)->Tick(m_GD);
	}
}

void Game::Draw(ID3D11DeviceContext* _pd3dImmediateContext) 
{
	//set immediate context of the graphics device
	m_DD->m_pd3dImmediateContext = _pd3dImmediateContext;

	//set which camera to be used
	m_DD->m_cam = m_cam;
	if (m_GD->m_GS == GS_PLAY_TPS_CAM)
	{
		m_DD->m_cam = m_TPScam;
	}
	if (m_GD->m_GS == GS_PLAY_MAIN_CAM)
	{
		m_DD->m_cam = m_cam;
	}

	//update the constant buffer for the rendering of VBGOs
	VBGO::UpdateConstantBuffer(m_DD);

	//draw all objects
	for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Draw(m_DD);
	}

	// Draw sprite batch stuff 
	m_DD2D->m_Sprites->Begin();
	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{
		(*it)->Draw(m_DD2D);
	}
	m_DD2D->m_Sprites->End();

	//draw anttweakbar
	TwDraw();

	//drawing text screws up the Depth Stencil State, this puts it back again!
	_pd3dImmediateContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);
};



bool Game::ReadInput()
{
	//copy over old keyboard state
	memcpy(m_prevKeyboardState, m_keyboardState, sizeof(unsigned char) * 256);

	//clear out previous state
	ZeroMemory(&m_keyboardState, sizeof(unsigned char) * 256);
	ZeroMemory(&m_mouseState, sizeof(DIMOUSESTATE));

	// Read the keyboard device.
	HRESULT hr = m_pKeyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pKeyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	// Read the Mouse device.
	hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(hr))
	{
		// If the Mouse lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pMouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

/*LRESULT CALLBACK Game::MessageProc(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam, ID3D11Device* _pd3dDevice)
{
	// Send event message to AntTweakBar
	if (TwEventWin(wnd, message, wParam, lParam))
		return 0; // Event has been handled by AntTweakBar

	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(wnd, &ps);
		EndPaint(wnd, &ps);
		return 0;
	}
	case WM_SIZE: // Window size has been changed
		if (_pd3dDevice) // Resize D3D render target
		{
			// Release render target and depth-stencil view
			ID3D11RenderTargetView *nullRTV = NULL;
			pd3dImmediateContext->OMSetRenderTargets(1, &nullRTV, NULL);
			if (m_pRenderTargetView)
			{
				g_RenderTargetView->Release();
				g_RenderTargetView = NULL;
			}
			if (g_DepthStencilView)
			{
				g_DepthStencilView->Release();
				g_DepthStencilView = NULL;
			}

			if (g_SwapChain)
			{
				// Resize swap chain
				g_SwapChainDesc.BufferDesc.Width = LOWORD(lParam);
				g_SwapChainDesc.BufferDesc.Height = HIWORD(lParam);
				g_SwapChain->ResizeBuffers(g_SwapChainDesc.BufferCount, g_SwapChainDesc.BufferDesc.Width,
					g_SwapChainDesc.BufferDesc.Height, g_SwapChainDesc.BufferDesc.Format,
					g_SwapChainDesc.Flags);

				// Re-create a render target and depth-stencil view
				ID3D11Texture2D *backBuffer = NULL, *dsBuffer = NULL;
				g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
				_pd3dDevice->CreateRenderTargetView(backBuffer, NULL, &g_RenderTargetView);
				backBuffer->Release();
				g_DepthStencilDesc.Width = g_SwapChainDesc.BufferDesc.Width;
				g_DepthStencilDesc.Height = g_SwapChainDesc.BufferDesc.Height;
				_pd3dDevice->CreateTexture2D(&g_DepthStencilDesc, NULL, &dsBuffer);
				_pd3dDevice->CreateDepthStencilView(dsBuffer, NULL, &g_DepthStencilView);
				dsBuffer->Release();
				g_D3DDevCtx->OMSetRenderTargets(1, &g_RenderTargetView, g_DepthStencilView);

				// Setup the viewport
				D3D11_VIEWPORT vp;
				vp.Width = (float)g_SwapChainDesc.BufferDesc.Width;
				vp.Height = (float)g_SwapChainDesc.BufferDesc.Height;
				vp.MinDepth = 0.0f;
				vp.MaxDepth = 1.0f;
				vp.TopLeftX = 0;
				vp.TopLeftY = 0;
				g_D3DDevCtx->RSSetViewports(1, &vp);
			}

			// TwWindowSize has been called by TwEventWin, so it is not necessary to call it again here.
		}
		return 0;
	case WM_CHAR:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(wnd, message, wParam, lParam);
	}
}*/