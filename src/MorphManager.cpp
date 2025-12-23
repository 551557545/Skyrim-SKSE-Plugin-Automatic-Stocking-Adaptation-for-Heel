#include "MorphManager.h"
#include <RaceMenuInterface/IPluginInterface.h>


namespace logger = SKSE::log;

MorphManager::MorphManager(): bodyMorphInterface(nullptr) {}

MorphManager& MorphManager::GetSingleton()
{
	static MorphManager instance;
	return instance;
}

bool MorphManager::Init() {
	logger::info("begin init MorphManager");
	const SKSE::MessagingInterface* messagingInterface = SKSE::GetMessagingInterface();
	if (!messagingInterface) {
		logger::error("GetMessagingInterface Fail!");
		return false;
	}
	SKEE::InterfaceExchangeMessage interfaceExchangeMessage{};
	// 3. 向 SKEE 发送接口交换请求
	bool dispatchSuccess = messagingInterface->Dispatch(
		static_cast<uint32_t>(SKEE::InterfaceExchangeMessage::kMessage_ExchangeInterface),
		&interfaceExchangeMessage,
		sizeof(interfaceExchangeMessage),
		"SKEE"
	);
	if (!dispatchSuccess) {
		logger::error("Dispatch InterfaceExchangeMessage Fail!");
		return false;
	}
	if (!interfaceExchangeMessage.interfaceMap) {
		logger::error("InterfaceMap is Null!");
		return false;
	}
	SKEE::IPluginInterface* ipifp = interfaceExchangeMessage.interfaceMap->QueryInterface("BodyMorph");
	if (!ipifp) {
		logger::error("IPluginInterface is Null!");
		return false;
	}
	SKEE::IBodyMorphInterface*  bodyMorphInterfaceP = static_cast<SKEE::IBodyMorphInterface*>(ipifp);
	if (!bodyMorphInterfaceP) {
		logger::error("Cast IBodyMorphInterface fail!");
		return false;
	}
	MorphManager::GetSingleton().bodyMorphInterface = bodyMorphInterfaceP;
	logger::info("Current BodyMorphInterface version {}", MorphManager::GetSingleton().bodyMorphInterface->GetVersion());
	return true;
}
bool MorphManager::ClearPlayerMorph(
	SKEE::TESObjectREFR* actorP,
	const std::string& cleanMorphName1,
	const std::string& cleanMorphName2
) {
	if (!cleanMorphName1.empty()) {
		MorphManager::GetSingleton().bodyMorphInterface->ClearMorph(actorP, cleanMorphName1.c_str(), (cleanMorphName1 + "Key").c_str());
	}
	if (!cleanMorphName2.empty()) {
		MorphManager::GetSingleton().bodyMorphInterface->ClearMorph(actorP, cleanMorphName2.c_str(), (cleanMorphName2 + "Key").c_str());
	}
	MorphManager::GetSingleton().bodyMorphInterface->ApplyBodyMorphs(actorP, false);
	return true;
}
bool MorphManager::ModifyPlayerMorph(
	SKEE::TESObjectREFR* actorP,
	const std::string& cleanMorphName,
	const std::string& changeMorphName
	)
{	
	try {
		if (!cleanMorphName.empty()) {
			MorphManager::GetSingleton().bodyMorphInterface->SetMorph(actorP, cleanMorphName.c_str(), (cleanMorphName + "Key").c_str(), 0.0F);
		}
		if (!changeMorphName.empty()) {
			MorphManager::GetSingleton().bodyMorphInterface->SetMorph(actorP, changeMorphName.c_str(), (changeMorphName + "Key").c_str(), 1.0F);
		}
		MorphManager::GetSingleton().bodyMorphInterface->ApplyBodyMorphs(actorP, false);
        return true;
    }
    catch (const std::exception& e) {
        logger::error("update Morph error: {}", e.what());
        return false;
    }
    catch (...) {
        logger::critical("un catch update Morph error!!");
        return false;
    }
}