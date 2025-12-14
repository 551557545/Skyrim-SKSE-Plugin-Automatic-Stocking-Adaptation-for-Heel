#include <GlobalContext.h>
#include <JsonConfigReader.h>
#include <SKSE/SKSE.h>
#include <spdlog/spdlog.h>  // CLS内置的日志库
#include <EquipEvent.h>
#include <RaceMenuInterface/IPluginInterface.h>
#include <MorphManager.h>



// 定义日志别名（简化日志输出）
namespace logger = SKSE::log;
using namespace global;

void SKSEOnMessage(SKSE::MessagingInterface::Message* message) {
	if (!message) return;
	// 根据消息类型处理逻辑
	switch (message->type) {
	// 1. SKSE 自身初始化完成
	case SKSE::MessagingInterface::kPostLoad:
		{
			logger::info("SKSE 初始化完成，开始注册插件逻辑...");
			break;
		}

	// 游戏主菜单加载完成（可安全访问游戏数据）
	case SKSE::MessagingInterface::kPostPostLoad:
		{
			logger::info("游戏主菜单加载完成，初始化 MorphManager...");
			// 初始化 Morph 管理、注册装备事件等核心逻辑
			MorphManager::Init();
			break;
		}
	case SKSE::MessagingInterface::kPreLoadGame:
	{
			logger::info("存档准备加载");
			break;
	}
	// 存档加载完成（此时主角/世界数据已就绪）
	case SKSE::MessagingInterface::kPostLoadGame:
		{
			logger::info("存档加载完成，同步 Morph 数据...");
			break;
		}
	// 数据加载完成 在这里设置监听 因为有可能开的是新档
	case SKSE::MessagingInterface::kDataLoaded:
		{
			logger::info("数据加载完成，同步 Morph 数据...");
			logger::info("注册装备监听事件");
			RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(&EquipEvent::GetSingleton());
			logger::info("注册装备监听事件完成");
			break;
		}
	default:
		{
			logger::info("plugin custom message [{}]ID: {}", message->sender, message->type);
			break;
		}
	}
}

// SKSE插件入口
SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	//Sleep(5000);
	SKSE::Init(a_skse);
	
	DKUtil::Logger::Init(Plugin::NAME, REL::Module::get().version().string());
	const SKSE::PluginDeclaration* plugin = SKSE::PluginDeclaration::GetSingleton();
	const auto pluginName = plugin->GetName();
	const auto pluginVersion = plugin->GetVersion().string(".");
	const auto gameVersion = REL::Module::get().version().string(".");
	const auto authorName = plugin->GetAuthor();
	const auto supportEmail = plugin->GetSupportEmail();

	logger::info("==================================================");
	logger::info("Loading plugin: {}", pluginName);
	logger::info("Plugin version: {}", pluginVersion);
	logger::info("Game version: {}", gameVersion);
	logger::info("Author: {}", authorName);
	logger::info("Support: {}", supportEmail);
	logger::info("==================================================");

	// 1. 加载json配置 读取滑块名
	bool successReadJsonConfig = JsonConfigReader::Init("Data/SKSE/Automatic Hosiery Adaptation for High Heel Config/config.json");
	if (!successReadJsonConfig) {
		logger::error("json config read error!");
		return false;
	}
	std::string slideDataInfo = GLOBAL_CONTEXT.GetSlideData().toString();
	logger::info("slideData load success: {}", slideDataInfo);

	//3. 注册消息
	logger::info("注册消息");
	const SKSE::MessagingInterface* mif = SKSE::GetMessagingInterface();
	mif->RegisterListener(SKSEOnMessage);
	logger::info("插件加载完成！");
	return true;
}

