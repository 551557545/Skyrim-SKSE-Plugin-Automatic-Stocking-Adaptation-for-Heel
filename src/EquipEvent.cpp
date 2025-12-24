#include <SKSE/SKSE.h>
#include "EquipEvent.h"
#include <MorphManager.h>
#include <GlobalContext.h>
namespace logger = SKSE::log;

EquipEvent& EquipEvent::GetSingleton()
{
	static EquipEvent instance;
	return instance;
}

bool HasEquipingStocking(
	const std::vector<RE::BGSBipedObjectForm::BipedObjectSlot>& slotArray,
	const RE::PlayerCharacter& playerP)
{
	// 容器为空，直接返回false（提前退出，优化逻辑）
	if (slotArray.empty()) {
		return false;
	}

	// 临时移除const限定（GetWornArmor实际不修改对象，安全）
	RE::PlayerCharacter& mutablePlayer = const_cast<RE::PlayerCharacter&>(playerP);

	// 遍历所有目标Slot，检查玩家是否穿戴了对应Slot的护甲
	for (const auto& targetSlot : slotArray) {
		// 调用非const版本的GetWornArmor（此时对象无const限定，匹配成功）
		RE::TESObjectARMO* wornArmor = mutablePlayer.GetWornArmor(targetSlot, false);
		// 如果该Slot有穿戴的护甲，返回true
		if (wornArmor != nullptr) {
			return true;
		}
	}
	return false;
}

bool HasBipedSlot(const std::vector<RE::BGSBipedObjectForm::BipedObjectSlot>& slotArray,
	const RE::BGSBipedObjectForm::BipedObjectSlot& targetSlot)
{
	// 容器为空直接返回false
	if (slotArray.empty()) {
		return false;
	}
	// 1. 显式转换targetSlot为底层数值类型
	const std::uint32_t targetMask = static_cast<std::uint32_t>(targetSlot);

	// 2. 遍历容器：循环变量显式指定类型
	for (const RE::BGSBipedObjectForm::BipedObjectSlot& slot : slotArray) {
		// 显式转换当前插槽为数值类型
		const std::uint32_t slotValue = static_cast<std::uint32_t>(slot);
		// 按位与判断包含关系
		if ((targetMask & slotValue) != 0) {
			return true;
		}
	}

	// 无匹配插槽
	return false;
}


RE::BSEventNotifyControl EquipEvent::ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource)
{
	if (!a_event) {
		logger::warn("EquipEvent - Received null event");
		logger::trace("Exiting EquipEvent::ProcessEvent (null event)");
		return RE::BSEventNotifyControl::kContinue;
	}

	RE::TESObjectREFR* eventActorP = a_event->actor.get();
	RE::PlayerCharacter* playerP = RE::PlayerCharacter::GetSingleton();

	if (eventActorP != playerP) {
		logger::warn("Non player event");
		return RE::BSEventNotifyControl::kContinue;
	}	

	RE::FormID formID = a_event->baseObject;
	RE::TESForm* form = RE::TESForm::LookupByID(formID);
	// 判断是否是装备
	if (!form) {
		logger::warn("EquipEvent::ProcessEvent - Form not found for ID: {:X}", formID);
		logger::trace("Exiting EquipEvent::ProcessEvent (no form)");
		return RE::BSEventNotifyControl::kContinue;
	}
	if (!form->IsArmor()) {
		logger::trace("EquipEvent::ProcessEvent - Form is not armor");
		logger::trace("Exiting EquipEvent::ProcessEvent (not armor)");
		return RE::BSEventNotifyControl::kContinue;
	}


	// 换换为armor对象
	RE::TESObjectARMO* armor = form->As<RE::TESObjectARMO>();
	if (!armor) {
		SKSE::log::warn("EquipEvent::ProcessEvent - Failed to cast form to armor");
		SKSE::log::trace("Exiting EquipEvent::ProcessEvent (cast failed)");
		return RE::BSEventNotifyControl::kContinue;
	}

		// 获取json配置
	const JsonConfigReader::SlideDataConfig& slideData = GLOBAL_CONTEXT.GetSlideData();
	if (slideData.empty()) {
		logger::error("slideData is empty");
		return RE::BSEventNotifyControl::kContinue;
	}

	std::string highHeelSlideName = slideData.highHeelSlideName;
	std::string noHighHeelSlideName = slideData.noHighHeelSlideName;
	
	logger::info("equip : {}", a_event->equipped);
	
	
	bool isFeetArmor = armor->HasPartOf(RE::BGSBipedObjectForm::BipedObjectSlot::kFeet);
	bool isStockingArmor = !isFeetArmor ? HasBipedSlot(slideData.bipedObjectSlotList, armor->GetSlotMask()) : false;
	if (!isFeetArmor && !isStockingArmor) {
		logger::info("Not a feet armor and Not a stocking armor");
		return RE::BSEventNotifyControl::kContinue;
	}
	bool equipStocking = false;
	if (isFeetArmor && (equipStocking = HasEquipingStocking(slideData.bipedObjectSlotList, *playerP))) {
		// 穿脱鞋子 同时装备了丝袜
		logger::info("change morph");
		const bool isFromHighToNoHigh = (a_event->equipped == slideData.reverse);
		const std::string fromSlide = isFromHighToNoHigh ? highHeelSlideName : noHighHeelSlideName;
		const std::string toSlide = isFromHighToNoHigh ? noHighHeelSlideName : highHeelSlideName;
		MorphManager::GetSingleton().ModifyPlayerMorph(eventActorP, fromSlide, toSlide);
	} else if (isStockingArmor && !a_event->equipped) {
		// 穿脱丝袜
		// 清空滑块
		logger::info("clear morph");
		MorphManager::GetSingleton().ClearPlayerMorph(eventActorP, highHeelSlideName, noHighHeelSlideName);
	}
	logger::info("isFeetArmor : {} equipStocking: {}", isFeetArmor, equipStocking);
	return RE::BSEventNotifyControl::kContinue;
}

