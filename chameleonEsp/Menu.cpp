#include "includes.hpp"

// Human-readable name for a virtual-key code, falling back to hex for keys Windows can't name (mouse buttons, etc).
static const char* KeyName(int vk)
{
	static char name[32];
	UINT sc = MapVirtualKeyA(vk, MAPVK_VK_TO_VSC);
	switch (vk) // extended keys need the extended-scancode bit to name correctly
	{
	case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
	case VK_PRIOR: case VK_NEXT: case VK_END: case VK_HOME:
	case VK_INSERT: case VK_DELETE: case VK_DIVIDE: case VK_NUMLOCK:
		sc |= 0x100;
		break;
	}
	if (sc && GetKeyNameTextA((LONG)(sc << 16), name, sizeof(name)) > 0)
		return name;
	snprintf(name, sizeof(name), "0x%02X", vk);
	return name;
}

void Menu::Init()
{
	ImGui::SetNextWindowSize({ 300, 480 }, ImGuiCond_Once);
	ImGui::Begin("ChameleonESP", nullptr, 0);

	const float footerH = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().WindowPadding.y;

	ImGui::BeginChild("##content", ImVec2(0, -footerH), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	if (ImGui::BeginTabBar("##tabs"))
	{
		if (ImGui::BeginTabItem("ESP"))
		{
			ImGui::BeginChild("##esp_list", ImVec2(0, 0), false);

			ImGui::Checkbox("Изменение FOV", &cfg->bFovChanger);
			if (cfg->bFovChanger)
				ImGui::SliderFloat("Значение FOV", &cfg->fFovValue, 50.0f, 180.0f);

			ImGui::Checkbox("Только враги", &cfg->bEnemyOnly);
			ImGui::Checkbox("Видимость персонажа (заражение)", &cfg->bForceCharacterVisibility);
			ImGui::Checkbox("Рамка", &cfg->bBox);
			ImGui::Checkbox("Линии", &cfg->bLines);
			ImGui::Checkbox("Имя", &cfg->bNames);
			ImGui::Checkbox("Роль", &cfg->bRoles);
			ImGui::Checkbox("Скелет", &cfg->bSkeleton);
			ImGui::Checkbox("Дистанция", &cfg->bDistance);
			// ImGui::Checkbox("Патроны охотника", &cfg->bHunterAmmo);
			ImGui::Checkbox("Приманки", &cfg->bDecoys);

			ImGui::Separator();
			ImGui::Text("Цвета");

			if (ImGui::ColorButton("##colVisible", *(ImVec4*)cfg->colVisible))
				ImGui::OpenPopup("popup_colVisible");
			ImGui::SameLine();
			ImGui::Text("Видимый");
			if (ImGui::BeginPopup("popup_colVisible"))
			{
				ImGui::ColorPicker4("##pick", cfg->colVisible);
				ImGui::EndPopup();
			}

			if (ImGui::ColorButton("##colNotVisible", *(ImVec4*)cfg->colNotVisible))
				ImGui::OpenPopup("popup_colNotVisible");
			ImGui::SameLine();
			ImGui::Text("Невидимый");
			if (ImGui::BeginPopup("popup_colNotVisible"))
			{
				ImGui::ColorPicker4("##pick", cfg->colNotVisible);
				ImGui::EndPopup();
			}

			if (ImGui::ColorButton("##colLines", *(ImVec4*)cfg->colLines))
				ImGui::OpenPopup("popup_colLines");
			ImGui::SameLine();
			ImGui::Text("Линии");
			if (ImGui::BeginPopup("popup_colLines"))
			{
				ImGui::ColorPicker4("##pick", cfg->colLines);
				ImGui::EndPopup();
			}

			if (ImGui::ColorButton("##colDecoy", *(ImVec4*)cfg->colDecoy))
				ImGui::OpenPopup("popup_colDecoy");
			ImGui::SameLine();
			ImGui::Text("Приманки");
			if (ImGui::BeginPopup("popup_colDecoy"))
			{
				ImGui::ColorPicker4("##pick", cfg->colDecoy);
				ImGui::EndPopup();
			}

			ImGui::EndChild();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Телепорт"))
		{
			ImGui::BeginChild("##tp_list", ImVec2(0, 0), false);

			if (cheat->PlayerInfos.empty())
			{
				ImGui::TextDisabled("Игроки не найдены");
			}
			else
			{
				for (int i = 0; i < (int)cheat->PlayerInfos.size(); i++)
				{
					ImGui::PushID(i);
					if (ImGui::Button("TP"))
						cheat->RequestTeleport(cheat->PlayerInfos[i].Actor);
					ImGui::SameLine();
					ImGui::Text("%s", cheat->PlayerInfos[i].Name.c_str());
					ImGui::PopID();
				}
			}

			ImGui::EndChild();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Инструменты"))
		{
			ImGui::BeginChild("##tools_list", ImVec2(0, 0), false);

			ImGui::Text("Выжившие");
			ImGui::Separator();
			ImGui::Checkbox("Антиобнаружение", &cfg->bAntiDetection);
			ImGui::Checkbox("Бесконечная перезарядка приманок", &cfg->bNoDecoyCooldown);

			ImGui::Separator();
			ImGui::Text("Охотник");
			ImGui::Separator();
			ImGui::Checkbox("Бесконечная перезарядка оружия", &cfg->bNoGunCooldown);
			ImGui::Checkbox("Бесконечные патроны", &cfg->bInfiniteBullets);

			// Magnet toggle key rebind: click the button, then press any key (ESC cancels).
			static bool bindingMagnet = false;
			ImGui::Text("Клавиша магнита:");
			ImGui::SameLine();
			if (ImGui::Button(bindingMagnet ? "Нажмите любую клавишу..." : KeyName(cfg->iMagnetKey)))
				bindingMagnet = true;
			if (bindingMagnet)
			{
				for (int vk = 0x08; vk <= 0xFE; vk++)
				{
					if (vk == VK_LBUTTON || vk == VK_RBUTTON) // reserved for UI interaction
						continue;
					// 0x8000 = key is down, 0x0001 = key was pressed since last call
					if (GetAsyncKeyState(vk) & 0x8000)
					{
						if (vk != VK_ESCAPE)
							cfg->iMagnetKey = vk;
						bindingMagnet = false;
						break;
					}
				}
			}

			if (ImGui::Button("Убить всех выживших"))
				cheat->RequestKillAllSurvivors();

			ImGui::Separator();
			ImGui::Text("Убить конкретного игрока");

			// Track the pick by actor pointer, not list index - PlayerInfos is rebuilt every frame and
			// indices can drift. Resolve the selected actor's current name for the combo preview, and
			// drop the selection if that actor no longer exists this frame.
			static SDK::AActor* selectedKillActor = nullptr;
			const char* killPreview = "Выберите выжившего";
			bool killStillPresent = false;
			int survivorCount = 0;
			for (const auto& p : cheat->PlayerInfos)
			{
				if (!p.IsSurvivor)
					continue; // only survivors can be killed
				survivorCount++;
				if (p.Actor == selectedKillActor)
				{
					killPreview = p.Name.c_str();
					killStillPresent = true;
				}
			}
			if (!killStillPresent)
				selectedKillActor = nullptr;
			if (survivorCount == 0)
				killPreview = "Выжившие не найдены";

			// Combo on the left filling the row, fixed-width "Kill" button on the right.
			const float killBtnW = ImGui::CalcTextSize("Убить").x + ImGui::GetStyle().FramePadding.x * 2.0f;
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - killBtnW - ImGui::GetStyle().ItemSpacing.x);
			if (ImGui::BeginCombo("##kill_target", killPreview))
			{
				for (int i = 0; i < (int)cheat->PlayerInfos.size(); i++)
				{
					if (!cheat->PlayerInfos[i].IsSurvivor)
						continue;
					ImGui::PushID(i);
					const bool isSelected = (cheat->PlayerInfos[i].Actor == selectedKillActor);
					if (ImGui::Selectable(cheat->PlayerInfos[i].Name.c_str(), isSelected))
						selectedKillActor = cheat->PlayerInfos[i].Actor;
					if (isSelected)
						ImGui::SetItemDefaultFocus();
					ImGui::PopID();
				}
				ImGui::EndCombo();
			}

			ImGui::SameLine();
			if (ImGui::Button("Убить", ImVec2(killBtnW, 0)) && selectedKillActor)
				cheat->RequestKillSurvivor(selectedKillActor);

			ImGui::Separator();
			ImGui::Text("Общее");
			ImGui::Separator();
			ImGui::Checkbox("Защита от кика", &cfg->bPreventKick);

			ImGui::Separator();

			if (ImGui::Button("Дамп костей (отладка)"))
				cfg->bDumpBones = true;

			ImGui::EndChild();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Смена имени"))
		{
			ImGui::BeginChild("##name_list", ImVec2(0, 0), false);

			static SDK::AActor* selectedNameActor = nullptr;
			const char* namePreview = "Выберите игрока";
			std::string selectedName;
			bool nameStillPresent = false;
			for (const auto& p : cheat->PlayerInfos)
			{
				if (p.Actor == selectedNameActor)
				{
					namePreview = p.Name.c_str();
					selectedName = p.Name;
					nameStillPresent = true;
				}
			}
			if (!nameStillPresent)
				selectedNameActor = nullptr;
			if (cheat->PlayerInfos.empty())
				namePreview = "Игроки не найдены";

			const float nameBtnW = ImGui::CalcTextSize("Изменить").x + ImGui::GetStyle().FramePadding.x * 2.0f;
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - nameBtnW - ImGui::GetStyle().ItemSpacing.x);
			if (ImGui::BeginCombo("##name_target", namePreview))
			{
				for (int i = 0; i < (int)cheat->PlayerInfos.size(); i++)
				{
					ImGui::PushID(i);
					const bool isSelected = (cheat->PlayerInfos[i].Actor == selectedNameActor);
					if (ImGui::Selectable(cheat->PlayerInfos[i].Name.c_str(), isSelected))
						selectedNameActor = cheat->PlayerInfos[i].Actor;
					if (isSelected)
						ImGui::SetItemDefaultFocus();
					ImGui::PopID();
				}
				ImGui::EndCombo();
			}

			ImGui::SameLine();
			if (ImGui::Button("Изменить", ImVec2(nameBtnW, 0)) && selectedNameActor && !selectedName.empty())
				cheat->RequestChangeName(selectedName);

			// Custom name: type anything and apply it to our own player.
			static char customName[64] = "";
			const float setBtnW = ImGui::CalcTextSize("Установить").x + ImGui::GetStyle().FramePadding.x * 2.0f;
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - setBtnW - ImGui::GetStyle().ItemSpacing.x);
			const bool nameEntered = ImGui::InputText("##custom_name", customName, sizeof(customName), ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::SameLine();
			if ((ImGui::Button("Установить", ImVec2(setBtnW, 0)) || nameEntered) && customName[0] != '\0')
				cheat->RequestChangeName(customName);

			ImGui::EndChild();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::EndChild();

	ImGui::Separator();

	float buttonW = 55.0f;
	if (ImGui::Button("Сохранить", ImVec2(buttonW, 0)))
		cfg->SaveSettings();
	ImGui::SameLine();
	if (ImGui::Button("Загрузить", ImVec2(buttonW, 0)))
		cfg->LoadSettings();

	ImGui::SameLine();
	float checkboxW = ImGui::CalcTextSize("Активно").x + ImGui::GetFrameHeight() + ImGui::GetStyle().ItemInnerSpacing.x;
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - checkboxW - ImGui::GetStyle().WindowPadding.x);
	ImGui::Checkbox("Активно", &cfg->bInitHooks);

	ImGui::End();
}
