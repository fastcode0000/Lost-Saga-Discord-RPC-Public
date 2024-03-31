#pragma once
#include <vector>
enum ModeType
{
	MT_NONE,
	MT_SYMBOL,
	MT_CATCH,
	MT_KING,
	MT_TRAINING,
	MT_SURVIVAL,
	MT_TEAM_SURVIVAL,
	MT_BOSS,
	MT_MONSTER_SURVIVAL,
	MT_FOOTBALL,
	MT_HEROMATCH,
	MT_GANGSI,
	MT_DUNGEON_A,
	MT_HEADQUARTERS,
	MT_CATCH_RUNNINGMAN,
	MT_FIGHT_CLUB,
	MT_TOWER_DEFENSE,
	MT_DARK_XMAS,
	MT_FIRE_TEMPLE,
	MT_DOUBLE_CROWN,
	MT_SHUFFLE_BONUS,
	MT_FACTORY,
	MT_TEAM_SURVIVAL_AI,
	MT_HOUSE,
	MT_MYROOM,
	MT_UNDERWEAR,
	MT_CBT,
	MT_RAID,
	MT_SUCCESSION,
	MT_PRACTICE,
	MT_FLAG_CAPTURE,
	MT_ARENA,
	MT_BATTLE,
	MT_FARMING,
	MT_BRAWL,
	MAX_MODE_TYPE,
};

class App
{
private:
	DWORD m_dwMemoryInitTime;


	DWORD m_dwApp;
	DWORD m_dwMode;
public:
	bool MemoryInit();

private:
	ModeType m_ModeType;
	int m_iCharType;
public:
	void GetModeAndImageName(char* szMode, char* szImageName);
	void GetMode(ModeType &rkMode);
	void GetNickName(char * szName);

	bool IsModeChanged();
private:
	static App* sg_Instance;
public:
	static App& GetInstance();
public:
	void Initialize();
	App();
	~App();
};

#define g_App App::GetInstance()