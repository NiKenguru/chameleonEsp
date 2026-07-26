#pragma once

struct LocStrings
{
	const char* langLabel;
	const char* langName;

	// ESP tab
	const char* tabEsp;
	const char* fovChanger;
	const char* fovValue;
	const char* enemyOnly;
	const char* charVisibility;
	const char* box;
	const char* lines;
	const char* name;
	const char* role;
	const char* skeleton;
	const char* distance;
	const char* decoys;
	const char* colors;
	const char* colorVisible;
	const char* colorNotVisible;
	const char* colorLines;
	const char* colorDecoys;

	// Teleport tab
	const char* tabTeleport;
	const char* noPlayers;

	// Tools tab
	const char* tabTools;
	const char* survivors;
	const char* antiDetection;
	const char* infiniteDecoyCooldown;
	const char* hunter;
	const char* noGunCooldown;
	const char* infiniteBullets;
	const char* magnetKey;
	const char* pressAnyKey;
	const char* killAllSurvivors;
	const char* killSpecificPlayer;
	const char* selectSurvivor;
	const char* noSurvivorsFound;
	const char* kill;
	const char* general;
	const char* antiKick;
	const char* dumpBones;

	// Name tab
	const char* tabName;
	const char* selectPlayer;
	const char* change;
	const char* set;

	// Footer
	const char* save;
	const char* load;
	const char* active;

	// Language selector
	const char* language;
};

static const LocStrings g_English = {
	"EN", "English",
	"ESP", "FOV Changer", "FOV Value", "Enemy Only",
	"Character Visibility (Infection)", "Box", "Lines", "Name",
	"Role", "Skeleton", "Distance", "Decoys",
	"Colors", "Visible", "Not Visible", "Lines", "Decoys",
	"Teleport", "No players found",
	"Tools", "Survivors", "Anti Detection", "Infinite Decoy Cooldown",
	"Hunter", "No Gun Cooldown", "Infinite Bullets",
	"Magnet Key:", "Press any key...",
	"Kill All Survivors", "Kill Specific Player",
	"Select survivor", "No survivors found", "Kill",
	"General", "Anti Kick", "Dump Bones (Debug)",
	"Change Name", "Select player", "Change", "Set",
	"Save", "Load", "Active",
	"Language",
};

static const LocStrings g_Russian = {
	"RU", "Русский",
	"ESP", "Изменение FOV", "Значение FOV", "Только враги",
	"Видимость персонажа (заражение)", "Рамка", "Линии", "Имя",
	"Роль", "Скелет", "Дистанция", "Приманки",
	"Цвета", "Видимый", "Невидимый", "Линии", "Приманки",
	"Телепорт", "Игроки не найдены",
	"Инструменты", "Выжившие", "Антиобнаружение", "Бесконечная перезарядка приманок",
	"Охотник", "Бесконечная перезарядка оружия", "Бесконечные патроны",
	"Клавиша магнита:", "Нажмите любую клавишу...",
	"Убить всех выживших", "Убить конкретного игрока",
	"Выберите выжившего", "Выжившие не найдены", "Убить",
	"Общее", "Защита от кика", "Дамп костей (отладка)",
	"Смена имени", "Выберите игрока", "Изменить", "Установить",
	"Сохранить", "Загрузить", "Активно",
	"Язык",
};

inline const LocStrings& Loc()
{
	return cfg->iLanguage == 1 ? g_Russian : g_English;
}
