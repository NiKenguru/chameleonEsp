class Settings
{
public:
	bool bMenuOpen;
	bool bInitHooks;
	bool bFovChanger;
	float fFovValue;
	bool bLines;
	bool bNames;
	bool bRoles;
	bool bBox;
	bool bSkeleton;
	bool bDistance;
	bool bHunterAmmo;
	bool bDecoys;
	bool bDumpBones;
	bool bEnemyOnly;
	bool bForceCharacterVisibility;
	bool bNoGunCooldown;
	bool bAntiDetection;
	bool bMagnetEnabled;
	int iMagnetKey;
	bool bPreventKick;
	bool bInfiniteBullets;
	bool bNoDecoyCooldown;
	float colVisible[4];
	float colNotVisible[4];
	float colLines[4];
	float colDecoy[4];
	int iLanguage; // 0 = English, 1 = Russian
	void InitializeSettings();
	void SaveSettings();
	void LoadSettings();
};