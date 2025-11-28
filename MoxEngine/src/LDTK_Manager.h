#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <filesystem>
#include <iostream>
#include "LDtkLoader/Project.hpp"
#include"Event.h"

struct CachedProject {
    std::unique_ptr<ldtk::Project> project;
    std::filesystem::file_time_type timeStamp;
};

class LDTK_Manager {
public:

    Event < const std::string&, ldtk::Project&> onProjectReload;

    

    static LDTK_Manager& instance() {
        static LDTK_Manager inst;
        return inst;
    }

    ldtk::Project* load(const std::string& path) {
        if (!std::filesystem::exists(path)) {
            std::cerr << "LDtk project not found: " << path << "\n";
            return nullptr;
        }

        // Check if already loaded
        auto it = _projects.find(path);
        if (it != _projects.end()) {
            return it->second.project.get();
        }

        // Create a new project inside a CachedProject
        CachedProject entry;
        entry.project = std::make_unique<ldtk::Project>();

        try {
            entry.project->loadFromFile(path);
            entry.timeStamp = std::filesystem::last_write_time(path);
        }
        catch (const std::exception& e) {
            std::cerr << "Failed to load LDtk project '" << path << "': " << e.what() << "\n";
            return nullptr;
        }

        ldtk::Project* ptr = entry.project.get();  // store raw pointer to return
        _projects.emplace(path, std::move(entry)); // move CachedProject into map

        return ptr;
    }


    void Update() {
        for (auto& [path, cachedProj] : _projects) {
            auto newTIme = std::filesystem::last_write_time(path);
            if (newTIme != cachedProj.timeStamp) {
                cachedProj.timeStamp = newTIme;
                cachedProj.project->loadFromFile(path);

                onProjectReload(path, *cachedProj.project);
            }
        }
    }


    void remove(const std::string& path) {
        _projects.erase(path);
    }


    void Clear() { // this will almost certainly crash if anything using the ldtk projects are alive when this is called.
        _projects.clear();
    }

    /// <summary>
    /// Attempts to get the LDTK project from the cache of loaded projects.
    /// If not already loaded, will load the given LDTK project path.
    /// </summary>
    /// <param name="path"></param>
    /// <returns></returns>
    ldtk::Project* getProject(const std::string& path) {
        auto it = _projects.find(path);
        if (it != _projects.end())
            return it->second.project.get();

      
        return load(path);
    }


private:
    LDTK_Manager() = default;

    std::unordered_map<std::string, CachedProject> _projects;
};
