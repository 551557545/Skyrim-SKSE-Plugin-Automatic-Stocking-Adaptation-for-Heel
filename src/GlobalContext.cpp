#include "GlobalContext.h"

namespace global {

    GlobalContext& GlobalContext::GetInstance() {
        static GlobalContext instance;
        return instance;
    }

    JsonConfigReader::SlideDataConfig& GlobalContext::GetSlideData()
	{
		return GLOBAL_CONTEXT.Get<JsonConfigReader::SlideDataConfig>("SlideData");
    }

    void GlobalContext::SetSlideData(JsonConfigReader::SlideDataConfig& data)
	{
		GLOBAL_CONTEXT.Set("SlideData",data);
    }

    // ¶Á²Ù×÷£º¹²ÏíËø
    bool GlobalContext::Exists(const std::string& key) {
        std::shared_lock<std::shared_mutex> lock(m_rwMutex);
        return m_dataMap.find(key) != m_dataMap.end();
    }

    // ¶Á²Ù×÷£º¹²ÏíËø
    size_t GlobalContext::Size() const {
        std::shared_lock<std::shared_mutex> lock(m_rwMutex);
        return m_dataMap.size();
    }

    // Ð´²Ù×÷£º¶ÀÕ¼Ëø
    void GlobalContext::Remove(const std::string& key) {
        std::unique_lock<std::shared_mutex> lock(m_rwMutex);
        m_dataMap.erase(key);
    }

    // Ð´²Ù×÷£º¶ÀÕ¼Ëø
    void GlobalContext::Clear() {
        std::unique_lock<std::shared_mutex> lock(m_rwMutex);
        m_dataMap.clear();
    }

}  // namespace global