#include "JsonConfigReader.h"
#include <SKSE/SKSE.h>
#include <fstream>
#include <sstream>
#include <REL/Module.h>
#include <filesystem>
#include "GlobalContext.h"

// 定义日志别名（简化日志输出）
namespace logger = SKSE::log;
using json = nlohmann::json;

// 初始化
bool JsonConfigReader::Init(const std::string& jsonPath)
{
	json slideDataJson = ReadJsonFile(jsonPath);
	if (slideDataJson.empty()) {
		return false;
	}
	return ParseSlideDataJson(slideDataJson);
}

// 解析json转为实体类
std::optional<JsonConfigReader::SlideDataConfig> JsonConfigReader::SlideDataConfig::parseTo(const json& jsonObj)
{
	SlideDataConfig result;
	// 校验必填字符串字段
	if (!jsonObj.contains("highHeelSlideName") && !jsonObj.contains("noHighHeelSlideName")) {
		SKSELog::error("SlideDataConfig parsing failed: highHeelSlideName and noHighHeelSlideName all is null");
		return std::nullopt;
	}
	if (jsonObj.contains("highHeelSlideName") && !jsonObj["highHeelSlideName"].is_string()) {
		SKSELog::error("SlideDataConfig parsing failed: highHeelSlideName not is string");
		return std::nullopt;
	}
	if (jsonObj.contains("noHighHeelSlideName") && !jsonObj["noHighHeelSlideName"].is_string()) {
		SKSELog::error("SlideDataConfig parsing failed: noHighHeelSlideName not is string");
		return std::nullopt;
	}
	if (jsonObj.contains("reverse") && !jsonObj["reverse"].is_boolean()) {
		SKSELog::error("SlideDataConfig parsing failed: reverse not is boolean");
		return std::nullopt;
	}
	if (jsonObj.contains("playerOnly") && !jsonObj["playerOnly"].is_boolean()) {
		SKSELog::error("SlideDataConfig parsing failed: playerOnly not is boolean");
		return std::nullopt;
	}
	if (jsonObj.contains("noHighHeelSlideThreshold") && !jsonObj["noHighHeelSlideThreshold"].is_number_float()) {
		SKSELog::error("SlideDataConfig parsing failed: noHighHeelSlideThreshold not is float number");
		return std::nullopt;
	}

	// 安全读取字符串字段
	result.highHeelSlideName = jsonObj["highHeelSlideName"].get<std::string>();
	result.noHighHeelSlideName = jsonObj["noHighHeelSlideName"].get<std::string>();
	result.reverse = jsonObj["reverse"].get<boolean>();
	result.playerOnly = jsonObj["playerOnly"].get<boolean>();
	result.noHighHeelSlideThreshold = jsonObj["noHighHeelSlideThreshold"].get<float>();

	// 安全读取 stockingSlotList（字段缺失/类型错误时赋空向量，避免抛异常）
	if (jsonObj.contains("stockingSlotList") && jsonObj["stockingSlotList"].is_array()) {
		try {
			result.stockingSlotList = jsonObj["stockingSlotList"].get<std::vector<uint32_t>>();
			for (uint32_t slotIndex : result.stockingSlotList) {
				result.bipedObjectSlotList.push_back(global::CustomIDToBipedSlot(slotIndex));
			}
		} catch (const json::exception& e) {
			SKSELog::warn("SlideDataConfig stockingSlotList parsing exception: {}, assigned empty vector", e.what());
			result.stockingSlotList = {};  // 赋空向量，而非返回 nullopt
			result.bipedObjectSlotList = {};
		}
	} else {
		SKSELog::warn("SlideDataConfig missing stockingSlotList field or invalid type, assigned empty vector");
		result.stockingSlotList = {};
		result.bipedObjectSlotList = {};
	}
	return result;
}

// 读取json对象
std::optional<json> JsonConfigReader::ReadJsonFile(const std::string& jsonPath) {
	// 1. 检查文件存在性，处理filesystem可能的异常
	try {
		if (!std::filesystem::exists(jsonPath)) {
			logger::error("JSON file does not exist: {}", jsonPath);
			return std::nullopt;
		}
	} catch (const std::filesystem::filesystem_error& e) {
		logger::error("Failed to check JSON file existence (path: {}). Details: {}", jsonPath, e.what());
		return std::nullopt;
	}

	// 2. 打开文件并检查状态
	std::ifstream file(jsonPath);
	if (!file.is_open()) {
		logger::error("Failed to open JSON file: {}", jsonPath);
		logger::trace("<<<< Exiting JsonConfigReader::ReadJsonFile (result: false)");
		return std::nullopt;
	}

	// 3. 解析JSON，完善异常处理和流检查
	json j;
	try {
		// 读取并解析，同时检查流状态
		if (!(file >> j)) {
			logger::error("Failed to read content from JSON file: {}", jsonPath);
			logger::trace("<<<< Exiting JsonConfigReader::ReadJsonFile (result: false)");
			return std::nullopt;
		}

		logger::debug("JSON file parsed successfully: {}", jsonPath);
		// 4. 移动语义返回，避免拷贝
		return std::move(j);
	} catch (const json::exception& e) {  // 捕获所有JSON相关异常（基类）
		logger::error("Failed to parse JSON file '{}'. Details: {}", jsonPath, e.what());
		logger::trace("<<<< Exiting JsonConfigReader::ReadJsonFile (result: false)");
		return std::nullopt;
	} catch (const std::exception& e) {  // 兜底捕获其他标准异常
		logger::error("Unexpected error when reading JSON file '{}'. Details: {}", jsonPath, e.what());
		logger::trace("<<<< Exiting JsonConfigReader::ReadJsonFile (result: false)");
		return std::nullopt;
	}

	// 无需手动close()，ifstream析构会自动关闭文件
}

// 解析滑块
bool JsonConfigReader::ParseSlideDataJson(const json& jsonObj)
{
	std::optional <JsonConfigReader::SlideDataConfig> optConfig = JsonConfigReader::SlideDataConfig::parseTo(jsonObj);
	if (!optConfig.has_value()) {
		return false;  // 解析失败（字段缺失/错误）
	}
	if (!optConfig->empty()) {
		GLOBAL_CONTEXT.SetSlideData(std::move(*optConfig));
		return true;
	}
	return false;
}




