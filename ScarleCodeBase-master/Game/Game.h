#ifndef _GAME_H_
#define _GAME_H_

//=================================================================
//Basic Game Manager Class
//=================================================================

#include <windows.h>
#include <list>
#include <dinput.h>
#include "Effects.h"
#include "CommonStates.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "Audio.h"
#include <chrono>
#include <AntTweakBar.h>

using std::list;
using std::unique_ptr;
using namespace DirectX;

// Forward declarations
class GameObject;
class GameObject2D;
class Camera;
class TPSCamera;
struct GameData;
struct DrawData;
struct DrawData2D;
class Light;
class Player;
class ParticleEmitter3D;


class Game
{
public:

	Game(ID3D11Device* _pd3dDevice, HWND _hWnd, HINSTANCE _hInstance);
	virtual ~Game();

	bool Tick(); //tick the game state

	void Draw(ID3D11DeviceContext* _pd3dImmediateContext); //render the current game state


	//anttweakbar callbacks
	static  void TW_CALL SetCallBackPNum(const void *value, void *clientData);
	static  void TW_CALL GetCallBackPNum(void *value, void *clientData);
	static  void TW_CALL SetCallBackPColor(const void *value, void *clientData);
	static  void TW_CALL GetCallBackPColor(void *value, void *clientData);
	static  void TW_CALL SetCallBackPRot(const void *value, void *clientData);
	static  void TW_CALL GetCallBackPRot(void *value, void *clientData);

	//LRESULT CALLBACK MessageProc(HWND, UINT, WPARAM, LPARAM, ID3D11Device*);
	
	ID3D11DeviceContext* pd3dImmediateContext;

	

	int pNum = 30;
	int pCurNum = 0;
	float pScale = 1.0f;
	float pDrag = 1.0f;
	float pGrav = -5.0f;
	float pSpeed = 750.0f;
	float pSpeedVar = 50.0f;
	float pCol[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float pPos[3] = { 0.0f, 0.0f, 0.0f };
	bool pFollow = false;
	bool pResetPos = false;
	//Color pColour = Color(1.0f, 0.0f, 0.0f);
	//Quaternion* pRot;
	
protected:
	DWORD m_playTime; //amount of time since the game started

	Camera* m_cam; //principle camera
	TPSCamera* m_TPScam;//TPS cam
	Light* m_light; //base light

	ParticleEmitter3D* emitter;
	int tw_particleNum = 0;

	list<GameObject *> m_GameObjects; //data structure storing all GameObjects of this Game
	list<GameObject2D *> m_GameObject2Ds;//ditto 2D objects

	//required for the CMO model rendering system
	CommonStates* m_states;
	IEffectFactory* m_fxFactory;

	Player* pPlayer;

	//direct input stuff
	bool ReadInput(); //Get current Mouse and Keyboard states
	IDirectInput8*			m_pDirectInput;
	IDirectInputDevice8*	m_pKeyboard;
	IDirectInputDevice8*	m_pMouse;
	unsigned char			m_keyboardState[256];
	unsigned char			m_prevKeyboardState[256];
	DIMOUSESTATE			m_mouseState;
	HWND m_hWnd;

	GameData* m_GD;			//Data to be shared to all Game Objects as they are ticked
	DrawData* m_DD;			//Data to be shared to all Game Objects as they are drawn
	DrawData2D * m_DD2D;	//Data to be passed by game to all 2D Game Objects via Draw 

	//sound stuff
	std::unique_ptr<AudioEngine> m_audioEngine;

	//Tick functions for each state
	void PlayTick();

	//Framerate variables
	const float m_targetFPS = (1 / 60.0f) * 1000;
	const float m_maxFPS = (1 / 100.0f) * 1000;
	std::chrono::steady_clock::time_point m_updated;
	double m_updateLag = 0;
};


#endif