#include <SKSE/SKSE.h>
class EquipEvent : public RE::BSTEventSink<RE::TESEquipEvent> {
private:
	EquipEvent() = default;
	~EquipEvent() = default;
	EquipEvent(const EquipEvent&) = delete;
	EquipEvent(EquipEvent&&) = delete;
	EquipEvent& operator=(const EquipEvent&) = delete;
	EquipEvent& operator=(EquipEvent&&) = delete;
public:
	RE::BSEventNotifyControl ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource)override;
	// 单例模式（避免重复创建）
	static EquipEvent& GetSingleton();
};