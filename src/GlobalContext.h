#pragma once
#include <unordered_map>
#include <string>
#include <any>
#include <shared_mutex>  // 替换std::mutex为std::shared_mutex（C++17+）
#include <stdexcept>
#include <optional>
#include <JsonConfigReader.h>

namespace global {
	// 1. 正向映射：自定义编号 → BipedObjectSlot 枚举
	const std::unordered_map<uint32_t, RE::BGSBipedObjectForm::BipedObjectSlot> CUSTOM_TO_BIPED_SLOT = {
		{ 30, RE::BIPED_MODEL::BipedObjectSlot::kHead },
		{ 31, RE::BIPED_MODEL::BipedObjectSlot::kHair },
		{ 32, RE::BIPED_MODEL::BipedObjectSlot::kBody },
		{ 33, RE::BIPED_MODEL::BipedObjectSlot::kHands },
		{ 34, RE::BIPED_MODEL::BipedObjectSlot::kForearms },
		{ 35, RE::BIPED_MODEL::BipedObjectSlot::kAmulet },
		{ 36, RE::BIPED_MODEL::BipedObjectSlot::kRing },
		{ 37, RE::BIPED_MODEL::BipedObjectSlot::kFeet },
		{ 38, RE::BIPED_MODEL::BipedObjectSlot::kCalves },
		{ 39, RE::BIPED_MODEL::BipedObjectSlot::kShield },
		{ 40, RE::BIPED_MODEL::BipedObjectSlot::kTail },
		{ 41, RE::BIPED_MODEL::BipedObjectSlot::kLongHair },
		{ 42, RE::BIPED_MODEL::BipedObjectSlot::kCirclet },
		{ 43, RE::BIPED_MODEL::BipedObjectSlot::kEars },
		{ 44, RE::BIPED_MODEL::BipedObjectSlot::kModMouth },
		{ 45, RE::BIPED_MODEL::BipedObjectSlot::kModNeck },
		{ 46, RE::BIPED_MODEL::BipedObjectSlot::kModChestPrimary },
		{ 47, RE::BIPED_MODEL::BipedObjectSlot::kModBack },
		{ 48, RE::BIPED_MODEL::BipedObjectSlot::kModMisc1 },
		{ 49, RE::BIPED_MODEL::BipedObjectSlot::kModPelvisPrimary },
		{ 50, RE::BIPED_MODEL::BipedObjectSlot::kDecapitateHead },
		{ 51, RE::BIPED_MODEL::BipedObjectSlot::kDecapitate },
		{ 52, RE::BIPED_MODEL::BipedObjectSlot::kModPelvisSecondary },
		{ 53, RE::BIPED_MODEL::BipedObjectSlot::kModLegRight },
		{ 54, RE::BIPED_MODEL::BipedObjectSlot::kModLegLeft },
		{ 55, RE::BIPED_MODEL::BipedObjectSlot::kModFaceJewelry },
		{ 56, RE::BIPED_MODEL::BipedObjectSlot::kModChestSecondary },
		{ 57, RE::BIPED_MODEL::BipedObjectSlot::kModShoulder },
		{ 58, RE::BIPED_MODEL::BipedObjectSlot::kModArmLeft },
		{ 59, RE::BIPED_MODEL::BipedObjectSlot::kModArmRight },
		{ 60, RE::BIPED_MODEL::BipedObjectSlot::kModMisc2 },
		{ 61, RE::BIPED_MODEL::BipedObjectSlot::kFX01 },
	};

    // 3. 正向转换函数（自定义编号 → 枚举，带错误处理）
	static [[nodiscard]] RE::BGSBipedObjectForm::BipedObjectSlot CustomIDToBipedSlot(uint32_t code)
	{
		auto it = CUSTOM_TO_BIPED_SLOT.find(code);
		if (it != CUSTOM_TO_BIPED_SLOT.end()) {
			return it->second;
		}
		return RE::BGSBipedObjectForm::BipedObjectSlot::kNone;
	}

    // 1. 反向映射：BipedObjectSlot 枚举 → 自定义编号
	const std::unordered_map<RE::BIPED_MODEL::BipedObjectSlot, uint32_t> BIPED_TO_CUSTOM_SLOT = {
		{ RE::BIPED_MODEL::BipedObjectSlot::kHead, 30 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kHair, 31 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kBody, 32 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kHands, 33 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kForearms, 34 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kAmulet, 35 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kRing, 36 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kFeet, 37 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kCalves, 38 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kShield, 39 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kTail, 40 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kLongHair, 41 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kCirclet, 42 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kEars, 43 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kModMouth, 44 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kModNeck, 45 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kModChestPrimary, 46 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kModBack, 47 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kModMisc1, 48 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kModPelvisPrimary, 49 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kDecapitateHead, 50 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kDecapitate, 51 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kModPelvisSecondary, 52 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kModLegRight, 53 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kModLegLeft, 54 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kModFaceJewelry, 55 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kModChestSecondary, 56 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kModShoulder, 57 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kModArmLeft, 58 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kModArmRight, 59 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kModMisc2, 60 },
		{ RE::BIPED_MODEL::BipedObjectSlot::kFX01, 61 }
	};

	// 2. 反向转换函数（枚举 → 自定义编号，带错误处理）
	static [[nodiscard]] uint32_t BipedSlotToCustomID(RE::BIPED_MODEL::BipedObjectSlot slot)
	{
		auto it = BIPED_TO_CUSTOM_SLOT.find(slot);
		if (it != BIPED_TO_CUSTOM_SLOT.end()) {
			return it->second;  // 找到匹配的自定义编号，返回
		}
		return 0;  // 未找到时返回 0（可根据业务调整为其他默认值，如 UINT32_MAX）
	}

    class GlobalContext {
    public:
        // 禁用拷贝/移动
        GlobalContext(const GlobalContext&) = delete;
        GlobalContext& operator=(const GlobalContext&) = delete;
        GlobalContext(GlobalContext&&) = delete;
        GlobalContext& operator=(GlobalContext&&) = delete;

        static JsonConfigReader::SlideDataConfig& GetSlideData();
		static void SetSlideData(JsonConfigReader::SlideDataConfig data);
        static GlobalContext& GetInstance();

        // ========== 核心接口（读操作：共享锁，无互斥） ==========
        // 获取对象（类型不匹配/key不存在抛异常，共享锁）
        template <typename T>
        T& Get(const std::string& key) {
            // 共享锁：多个读可同时持有，写操作会阻塞
            std::shared_lock<std::shared_mutex> lock(m_rwMutex);
            auto it = m_dataMap.find(key);
            if (it == m_dataMap.end()) {
                throw std::runtime_error("GlobalContext: Key '" + key + "' not found");
            }

            try {
                return std::any_cast<T&>(it->second);
            }
            catch (const std::bad_any_cast& e) {
                throw std::runtime_error("GlobalContext: Type mismatch for key '" + key + "': " + e.what());
            }
        }

        // 安全获取（返回optional，共享锁）
        template <typename T>
        std::optional<T> TryGet(const std::string& key) {
            std::shared_lock<std::shared_mutex> lock(m_rwMutex);
            auto it = m_dataMap.find(key);
            if (it == m_dataMap.end()) {
                return std::nullopt;
            }

            try {
                return std::any_cast<T>(it->second);
            }
            catch (const std::bad_any_cast&) {
                return std::nullopt;
            }
        }

        // 检查key是否存在（共享锁）
        bool Exists(const std::string& key);

        // 获取key数量（共享锁）
        size_t Size() const;

        // ========== 核心接口（写操作：独占锁） ==========
        // 存储对象（覆盖式，独占锁）
        template <typename T>
		void Set(const std::string& key, const T&& value)
		{
            // 独占锁：阻塞所有读/写，保证写操作原子性
            std::unique_lock<std::shared_mutex> lock(m_rwMutex);
			m_dataMap[key] = std::move(value);
        }

        // 删除指定key（独占锁）
        void Remove(const std::string& key);

        // 清空所有对象（独占锁）
        void Clear();

    private:
        GlobalContext() = default;
        ~GlobalContext() = default;

        std::unordered_map<std::string, std::any> m_dataMap;
        // 读写锁：替代原std::mutex
        mutable std::shared_mutex m_rwMutex;
    };

#define GLOBAL_CONTEXT global::GlobalContext::GetInstance()

}  // namespace global