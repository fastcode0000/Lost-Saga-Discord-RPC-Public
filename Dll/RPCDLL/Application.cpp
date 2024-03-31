#include "StdAfx.h"
#include "Application.h"
#include "MemoryManager.hpp"
#include "ioHashString.h"

using namespace std;

App* App::sg_Instance = NULL;

#define ApplicationArr "8D 4E 20 88 5D FC E8 ?? ?? ?? ?? 89 1D ?? ?? ?? ?? 8B 4D F4 64 89 0D 00 00 00 00 59 5F 5E 5B"
#define ModeArr "53 75 72 76 69 76 61 6C 43 6F 6E 74 72 69 62 75 74 65 40 40"


bool App::MemoryInit()
{
	m_dwMemoryInitTime = GetTickCount();
	while (true)
	{
		DWORD dwAddress = 0;
		dwAddress = g_Memory.AobScan(ApplicationArr, 0x01200000, 0x01500000) + 0xB;
		string szOperand = g_Memory.GetOperand(dwAddress, 0);
		m_dwApp = g_Memory.ParseOperand(szOperand);


		/*int iCnt = g_Memory.GetAobScanFindCount(ModeArr, 0x02600000, 0x02800000);
		if (iCnt)
		{
			dwAddress = g_Memory.AobScan(ModeArr, 0x02600000, 0x02800000, iCnt) + 0x19D;
			m_dwMode = dwAddress;
		}*/

		dwAddress = g_Memory.AobScan(ModeArr, 0x02400000, 0x02800000, 3) + 0x19D;
		m_dwMode = dwAddress;

		//printf("App : %p\nMode : %p\n", m_dwApp, m_dwMode);

		if (m_dwApp != 0 && m_dwMode > 0x02400000)
		{
			//printf("App : %p\nMode : %p Ok\n", m_dwApp, m_dwMode);
			break;
		}

		DWORD dwGapTime = GetTickCount() - m_dwMemoryInitTime;
		if (dwGapTime > 10000) //10���̻� ���� ��ã�� ���
		{
			m_dwMemoryInitTime = GetTickCount();
			return false;
		}

		Sleep(100);
	}

	return true;
}

void App::GetModeAndImageName(char* szMode, char* szImageName)
{
	switch (m_ModeType) //������ ��带 ���� �޾ƿ�����..
	{
	case ModeType::MT_NONE: //mode_anything
		strcpy(szMode, "Loading");
		strcpy(szImageName, "mode_anything");
		break;
	case ModeType::MT_SYMBOL: //�Ŀ����� mode_symbol
		strcpy(szMode, "Power Stone");
		strcpy(szImageName, "mode_symbol");
		break;
	case ModeType::MT_CATCH: //����Ż�� mode_pirate
		strcpy(szMode, "Prisoner");
		strcpy(szImageName, "mode_pirate");
		break;
	case ModeType::MT_KING: //����ũ��� mode_king
		strcpy(szMode, "Hidden Crown");
		strcpy(szImageName, "mode_king");
		break;
	case ModeType::MT_TRAINING: //���� mode_openspace_training
		strcpy(szMode, "Plaza");
		strcpy(szImageName, "mode_openspace_training");
		break;
	case ModeType::MT_SURVIVAL: //������ġ mode_survivial
		strcpy(szMode, "Death Match");
		strcpy(szImageName, "mode_survivial");
		break;
	case ModeType::MT_TEAM_SURVIVAL: //��������ġ mode_teamdeathmatch
		strcpy(szMode, "Team Match");
		strcpy(szImageName, "mode_teamdeathmatch");
		break;
	case ModeType::MT_BOSS: //�������̵� mode_bossraid
		strcpy(szMode, "Boss Mode");
		strcpy(szImageName, "mode_bossraid");
		break;
	case ModeType::MT_MONSTER_SURVIVAL: //�ذ񿵿� pve_boss2
		strcpy(szMode, "Crusade");
		strcpy(szImageName, "pve_boss2");
		break;
	case ModeType::MT_FOOTBALL: //�౸��� mode_soccer
		strcpy(szMode, "Soccer Match");
		strcpy(szImageName, "mode_soccer");
		break;
	case ModeType::MT_HEROMATCH: //������ mode_ladder
		strcpy(szMode, "Ladder");
		strcpy(szImageName, "mode_ladder");
		break;
	case ModeType::MT_GANGSI: //���ü����̹� mode_gangsi
		strcpy(szMode, "Zombie Mode");
		strcpy(szImageName, "mode_gangsi");
		break;
	case ModeType::MT_DUNGEON_A: //���� ��� ��� mode_random
		strcpy(szMode, "Dungeon Defense");
		strcpy(szImageName, "mode_random");
		break;
	case ModeType::MT_HEADQUARTERS: //���� mode_lobby
		strcpy(szMode, "HQ");
		strcpy(szImageName, "mode_lobby");
		break;
	case ModeType::MT_CATCH_RUNNINGMAN: //�������� mode_dwarf
		strcpy(szMode, "Dwarf Mode");
		strcpy(szImageName, "mode_dwarf");
		break;
	case ModeType::MT_FIGHT_CLUB: //è�Ǿ��� mode_champion
		strcpy(szMode, "Champion Mode");
		strcpy(szImageName, "mode_champion");
		break;
	case ModeType::MT_TOWER_DEFENSE: //���� ����:���� �� mode_monster001
		strcpy(szMode, "Deep Forest");
		strcpy(szImageName, "mode_monster001");
		break;
	case ModeType::MT_DARK_XMAS: //���� ����:�����Ÿ�� mode_monster002
		strcpy(szMode, "Show Town");
		strcpy(szImageName, "mode_monster002");
		break;
	case ModeType::MT_FIRE_TEMPLE: //���� ����:���� ���� mode_monster003
		strcpy(szMode, "Fire Temple");
		strcpy(szImageName, "mode_monster003");
		break;
	case ModeType::MT_DOUBLE_CROWN: //����ũ��� (���� ũ��� ������) mode_king
		strcpy(szMode, "Double Crown");
		strcpy(szImageName, "mode_king");
		break;
	case ModeType::MT_SHUFFLE_BONUS: //���ø�� ���ʽ� �������� mode_random
		strcpy(szMode, "Shuffle Mode");
		strcpy(szImageName, "mode_random");
		break;
	case ModeType::MT_FACTORY: //���ʹ���:��а��� mode_monster004
		strcpy(szMode, "Secret Factory");
		strcpy(szImageName, "mode_monster004");
		break;
	case ModeType::MT_TEAM_SURVIVAL_AI: //�������̹� AI mode_teamdeathmatch_ai
		strcpy(szMode, "Team Match AI");
		strcpy(szImageName, "mode_teamdeathmatch_ai");
		break;
	case ModeType::MT_HOUSE: //���κ��� event_player_minihome_icon
		strcpy(szMode, "Personal HQ");
		strcpy(szImageName, "event_player_minihome_icon");
		break;
	case ModeType::MT_MYROOM: //�κ� quest_251
		strcpy(szMode, "Lobby");
		strcpy(szImageName, "quest_251");
		break;
	case ModeType::MT_UNDERWEAR: //��Ƽ��  panty_mon
		strcpy(szMode, "Panty Mode");
		strcpy(szImageName, "panty_mon");
		break;
	case ModeType::MT_CBT: //Ŭ���� ��� classic
		strcpy(szMode, "Classic");
		strcpy(szImageName, "classic");
		break;
	case ModeType::MT_RAID: //���̵� raid
		strcpy(szMode, "Raid");
		strcpy(szImageName, "raid");
		break;
	case ModeType::MT_SUCCESSION: //1vs1 ��� mode_1vs1
		strcpy(szMode, "Solo");
		strcpy(szImageName, "mode_1vs1");
		break;
	case ModeType::MT_PRACTICE: //������ mode_openspace_training
		strcpy(szMode, "Trainning");
		strcpy(szImageName, "mode_openspace_training");
		break;
	case ModeType::MT_FLAG_CAPTURE: //��߸�� mode_flag
		strcpy(szMode, "Flag Mode");
		strcpy(szImageName, "mode_flag");
		break;
	case ModeType::MT_ARENA: //�Ʒ��� ��� arena_mode
		strcpy(szMode, "Arena Mode");
		strcpy(szImageName, "arena_mode");
		break;
	case ModeType::MT_BATTLE: //��Ʋ ��� Icon_battlemod
		strcpy(szMode, "Battle Mode");
		strcpy(szImageName, "Icon_battlemod");
		break;
	case ModeType::MT_FARMING: //�Ĺ� ��� mode_farming
		strcpy(szMode, "Farming Mode");
		strcpy(szImageName, "mode_farming");
		break;
	default:
		strcpy(szMode, "Unknown Mode"); //mode_random
		strcpy(szImageName, "mode_random");
		break;
	}

}

void App::GetMode(ModeType& rkMode)
{
	int iMode = 0;
	iMode = g_Memory.RPM<int>(m_dwMode);
	rkMode = (ModeType)iMode;
}

void App::GetNickName(char * szName)
{
	DWORD Buf = g_Memory.RPM<DWORD>(m_dwApp + 0x3F0);
	Buf = g_Memory.RPM<DWORD>(Buf + 0x1C);
	strcpy(szName, (const char *)Buf);
}

bool App::IsModeChanged()
{
	ModeType modeType;

	GetMode(modeType);

	//printf("Mode : %d\n",(int)modeType);

	if (m_ModeType != modeType)
	{
		m_ModeType = modeType;
		return true;
	}
	return false;
}

App& App::GetInstance()
{
	if (sg_Instance == NULL)
		sg_Instance = new App;
	return *sg_Instance;
}

void App::Initialize()
{
	m_dwMemoryInitTime = 0;
	m_dwApp = 0;
	m_dwMode = 0;
	m_ModeType = ModeType::MT_NONE;
	m_iCharType = 0;
}

App::App()
{
	Initialize();
}

App::~App()
{

}