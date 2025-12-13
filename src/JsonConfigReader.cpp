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

	// 安全读取字符串字段
	result.highHeelSlideName = jsonObj["highHeelSlideName"].get<std::string>();
	result.noHighHeelSlideName = jsonObj["noHighHeelSlideName"].get<std::string>();
	result.reverse = jsonObj["reverse"].get<boolean>();

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
	if (!std::filesystem::exists(jsonPath)) {
		logger::error("Json file not exists：{}", jsonPath);
		return std::nullopt;  // 返回"无值"，表示读取失败
    }
	std::ifstream file(jsonPath);
    if (!file.is_open()) {
		logger::error("Failed to open high heel definition file: {}", jsonPath);
		logger::trace("<<<< Exiting HighHeelDetector::Init (result: false)");
		return std::nullopt;  // 返回"无值"，表示读取失败
    }
    json j;
    try {
        file >> j;
		file.close();
		logger::debug("JSON file parsed successfully.");
        return j;
    }
    catch (const nlohmann::json::parse_error& e) {
		logger::error("Failed to parse JSON from '{}'. Details: {}", jsonPath, e.what());
		logger::trace("<<<< Exiting HighHeelDetector::Init (result: false)");
        return std::nullopt;
	}
}

// 解析滑块
bool JsonConfigReader::ParseSlideDataJson(const json& jsonObj)
{
	std::optional <JsonConfigReader::SlideDataConfig> optConfig = JsonConfigReader::SlideDataConfig::parseTo(jsonObj);
	if (!optConfig.has_value()) {
		return false;  // 解析失败（字段缺失/错误）
	}
	if (!optConfig->empty()) {
		GLOBAL_CONTEXT.SetSlideData(optConfig.value());
		return true;
	}
	return false;
}




