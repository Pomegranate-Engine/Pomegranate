namespace Pomegranate
{
    template <typename T> T* ResourceManager::load(const std::string& path)
    {
        if(resources.find(path)!=resources.end())
        {
            return (T*)resources[path];
        }
        else
        {
            Resource* r = new T(path.c_str());
            r->path = path;
            resources[path] = r;
            return (T*)r;
        }
        return nullptr;
    }

    template <typename T> void ResourceManager::unload(const std::string& path)
    {
        delete (T)resources[path];
        resources.erase(path);
    }
}