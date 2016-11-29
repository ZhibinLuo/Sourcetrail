#include "data/StorageProvider.h"

#include "utility/logging/logging.h"

int StorageProvider::getStorageCount() const
{
	std::lock_guard<std::mutex> lock(m_storagesMutex);
	return m_storages.size();
}

void StorageProvider::pushIndexerTarget(std::shared_ptr<IntermediateStorage> storage)
{
	const std::size_t storageSize = storage->getSourceLocationCount();
	std::list<std::shared_ptr<IntermediateStorage>>::iterator it;

	std::lock_guard<std::mutex> lock(m_storagesMutex);
	for (it = m_storages.begin(); it != m_storages.end(); it++)
	{
		if ((*it)->getSourceLocationCount() < storageSize)
		{
			break;
		}
	}
	m_storages.insert(it, storage);
}

std::shared_ptr<IntermediateStorage> StorageProvider::popIndexerTarget()
{
	std::shared_ptr<IntermediateStorage> ret;
	{
		std::lock_guard<std::mutex> lock(m_storagesMutex);
		if (m_storages.size() > 1)
		{
			std::list<std::shared_ptr<IntermediateStorage>>::iterator it = m_storages.begin();
			it++;
			ret = *it;
			m_storages.erase(it);
		}
		else
		{
			ret = std::make_shared<IntermediateStorage>();
		}
	}
	return ret;
}

std::shared_ptr<IntermediateStorage> StorageProvider::popInjectionSource()
{
	std::shared_ptr<IntermediateStorage> ret;
	{
		std::lock_guard<std::mutex> lock(m_storagesMutex);
		if (!m_storages.empty())
		{
			ret = m_storages.front();
			m_storages.pop_front();
		}
	}

	return ret;
}

void StorageProvider::logCurrentState() const
{
	std::string logString = "Storages waiting for injection:";
	{
		std::lock_guard<std::mutex> lock(m_storagesMutex);
		for (std::shared_ptr<IntermediateStorage> storage: m_storages)
		{
			logString += " " + std::to_string(storage->getSourceLocationCount()) + ";";
		}
	}
	LOG_INFO(logString);
}



