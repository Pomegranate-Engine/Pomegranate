namespace Pomegranate
{
    template <typename T> T* ResourceManager::load(std::string path)
    {
        if(resources.find(path)!=resources.end())
        {
            return (T*)resources[path];
        }
        else
        {
            auto* r = new T(path.c_str());
            resources[path] = r;
            return r;
        }
        return nullptr;
    }

    template <typename T> void ResourceManager::unload(std::string path)
    {
        delete (T)resources[path];
        resources.erase(path);
    }
}