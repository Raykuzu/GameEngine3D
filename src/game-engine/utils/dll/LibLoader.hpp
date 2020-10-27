/*
** EPITECH PROJECT, 2019
** Arcade
** File description:
** 
*/

#pragma once

#include <dlfcn.h>
#include "ArcLogger.hpp"

class LibLoader {
	public:
        LibLoader() : _lib(nullptr) {};
		~LibLoader() {
            this->unload();
		};

		/**
        * Load a shared library corresponding to the path
        */
        int load(std::string const &path) {
            ArcLogger::trace("LibLoader::load [path: " + path + ']');

            if (_lib != nullptr)
                unload();
            _lib = dlopen(path.c_str(), RTLD_NOW);
            if (!_lib) {
                ArcLogger::error(dlerror());
                return (-1);
            }
            return (0);
        };

        /**
        * Unload the shared library previously loaded
        */
        int unload() {
            ArcLogger::trace("LibLoader::unload");

            if (!_lib) {
                return (-1);
            }
            if (dlclose(_lib)) {
                ArcLogger::warn(dlerror());
                return (-1);
            }
            _lib = nullptr;
            return (0);
        };

        /**
        * Get a function in the shared library previously loaded
        */
        void *getFunction(std::string const &funcName) {
            ArcLogger::trace("LibLoader::getFunction [funcName: " + funcName + ']');
            void *func = nullptr;

            if (!_lib) {
                ArcLogger::warn("Can't get '" + funcName + "' symbol: no lib loaded");
                return (nullptr);
            }
            func = dlsym(_lib, funcName.c_str());
            if (!func) {
                ArcLogger::warn(dlerror());
            }
            return (func);
        };

        bool somethingLoaded() {
            return (_lib != nullptr);
        }

	private:

        /**
         *  The library loaded as void * buffer by dlopen (library dl)
         */
        void *_lib;
};
