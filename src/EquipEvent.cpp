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

bool HasBipedSlot(
	const std::vector<RE::BGSBipedObjectForm::BipedObjectSlot>& slotArray,
	RE::BGSBipedObjectForm::BipedObjectSlot targetSlot)
{
	// 容器为空，直接返回false（提前退出，优化逻辑）
	if (slotArray.empty()) {
		return false;
	}
	// 查找目标元素：值类型用 . 访问begin/end（核心修复）
	auto it = std::find(slotArray.begin(), slotArray.end(), targetSlot);
	return it != slotArray.end();
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
	JsonConfigReader::SlideDataConfig& slideData = GLOBAL_CONTEXT.GetSlideData();
	if (slideData.empty()) {
		logger::error("slideData is empty");
		return RE::BSEventNotifyControl::kContinue;
	}

	// 转换角色指针类型
	SKEE::TESObjectREFR* skeeRefr = reinterpret_cast<SKEE::TESObjectREFR*>(eventActorP);
	if (!skeeRefr) {
		logger::error("RE::TESObjectREFR cast to SKEE::TESObjectREFR error");
		return RE::BSEventNotifyControl::kContinue;
	}

	std::string highHeelSlideName = slideData.highHeelSlideName;
	std::string noHighHeelSlideName = slideData.noHighHeelSlideName;
	
	// 判断是否是脚部装备
	if (armor->HasPartOf(RE::BGSBipedObjectForm::BipedObjectSlot::kFeet)) {

		logger::info("equip : {}", a_event->equipped);

		const bool isFromHighToNoHigh = (a_event->equipped == slideData.reverse);
		const std::string fromSlide = isFromHighToNoHigh ? highHeelSlideName : noHighHeelSlideName;
		const std::string toSlide = isFromHighToNoHigh ? noHighHeelSlideName : highHeelSlideName;

		// 3. 统一调用方法，无需重复写ModifyPlayerMorph
		MorphManager::GetSingleton().ModifyPlayerMorph(skeeRefr, fromSlide, toSlide);
	}
	else if (HasBipedSlot(slideData.bipedObjectSlotList, armor->GetSlotMask())) {
		// 说明触发的是丝袜装备
		// 清空滑块
		MorphManager::GetSingleton().ClearPlayerMorph(skeeRefr, highHeelSlideName, noHighHeelSlideName);
	}
	return RE::BSEventNotifyControl::kContinue;
}

