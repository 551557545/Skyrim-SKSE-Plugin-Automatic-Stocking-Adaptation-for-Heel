#pragma once
#include <format>  // C++20 格式化（需编译选项 -std=c++20）
#include <nlohmann/json.hpp>
#include <optional>
#include <sstream>  // 兼容 C++17 及以下的格式化备用方案
#include <string>
#include <vector>

// 命名空间别名（简化代码）
using json = nlohmann::json;
namespace SKSELog = SKSE::log;

class JsonConfigReader
{
public:
	// 初始化 JSON 配置读取
	static bool Init(const std::string& jsonPath);

	// 滑动数据配置结构体
	struct SlideDataConfig
	{
		// 成员变量（与 JSON 字段一一对应）
		std::string highHeelSlideName;
		std::string noHighHeelSlideName;
		std::vector<uint32_t> stockingSlotList;
		std::vector<RE::BGSBipedObjectForm::BipedObjectSlot> bipedObjectSlotList;
		bool reverse;

		// 注意：const 必须放在函数后（修饰成员函数，保证 const 对象可调用）
		std::string toString() const
		{
			// 格式化 uint32_t 向量为 "[x, y, z]" 格式
			auto formatUIntVector = [](const std::vector<uint32_t>& vec) -> std::string {
				if (vec.empty()) {
					return "[]";
				}
				std::ostringstream oss;
				oss << "[";
				for (size_t i = 0; i < vec.size(); ++i) {
					oss << vec[i];
					if (i != vec.size() - 1) {
						oss << ", ";
					}
				}
				oss << "]";
				return oss.str();
			};

			// 格式化 BipedObjectSlot 枚举向量（转为数值打印，也可扩展为枚举名）
			auto formatBipedSlotVector = [](const std::vector<RE::BGSBipedObjectForm::BipedObjectSlot>& vec) -> std::string {
				if (vec.empty()) {
					return "[]";
				}
				std::ostringstream oss;
				oss << "[";
				for (size_t i = 0; i < vec.size(); ++i) {
					// 将枚举类型转为 uint32_t 数值打印（直观易读）
					oss << static_cast<uint32_t>(vec[i]);
					if (i != vec.size() - 1) {
						oss << ", ";
					}
				}
				oss << "]";
				return oss.str();
			};

			// C++20：补充 bipedObjectSlotList 的打印
			return std::format(
				"SlideDataConfig{{reverse: \"{}\",highHeelSlideName: \"{}\", noHighHeelSlideName: \"{}\", stockingSlotList: {}, bipedObjectSlotList: {}}}",
				reverse,highHeelSlideName, noHighHeelSlideName,
				formatUIntVector(stockingSlotList),
				formatBipedSlotVector(bipedObjectSlotList));
		}

		bool empty() const
		{
			return highHeelSlideName.empty() && noHighHeelSlideName.empty() && stockingSlotList.empty();
		}

		static std::optional<SlideDataConfig> parseTo(const json& jsonObj);
	};

private:
	// 解析 SlideData 配置的 JSON 逻辑
	static bool ParseSlideDataJson(const json& jsonObj);

	// 读取 JSON 文件
	static std::optional<json> ReadJsonFile(const std::string& jsonPath);
};