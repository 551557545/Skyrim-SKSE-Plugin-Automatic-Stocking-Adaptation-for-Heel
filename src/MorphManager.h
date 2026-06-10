#pragma once
#include <RaceMenuInterface/IPluginInterface.h>
class MorphManager {
private:
	MorphManager();
	~MorphManager() = default;
	MorphManager(const MorphManager&) = delete;
	MorphManager(MorphManager&&) = delete;
	MorphManager& operator=(const MorphManager&) = delete;
	MorphManager& operator=(MorphManager&&) = delete;

	RE::IBodyMorphInterface* bodyMorphInterface;

public:
	static MorphManager& GetSingleton();
	static bool Init();
	bool ClearPlayerMorph(
		RE::TESObjectREFR* actorP,
		const std::string& cleanMorphName1,
		const std::string& cleanMorphName2
	);
	bool ModifyPlayerMorph(
		RE::TESObjectREFR* actorP,
		const std::string& cleanMorphName,
		const std::string& changeMorphName,
		const float threshold
	);
};