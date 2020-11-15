//
// Created by raykuzu on 09/11/2020.
//

#pragma once

#include <string>
#include <type_traits>
#include <exception>
#include "ArcLogger.hpp"
#include "EngineMath.hpp"


template<typename Base, typename T>
inline bool instanceOf(const T*) {
    return std::is_base_of<Base, T>::value;
}

template <typename Compare>
class SceneChange {
    public:
        enum Type {
            POSITION
        };

        explicit SceneChange(Type const &type, Compare const &compare)  {
            bool error = false;

            switch (type) {
                case POSITION:
                    if (!instanceOf<EngineMath::Vector3>(compare)) {
                        error = true;
                    }
                    break;
            }
            if (error) {
                ArcLogger::error("You are doing shit with your sceneChange please fill the compare with the correspondent data of your type");
                throw std::exception();
            }
            _type = type;
            _compare = compare;
        };
        ~SceneChange() = default;

        bool compare(const SceneChange &rhs) {
            if (_type != rhs._type)
                return (false);
            if (_compare == rhs._compare)
                return (true);
            return (false);
        }

        bool operator==(const SceneChange &rhs) const {
            return compare(rhs);
        }

        bool operator!=(const SceneChange &rhs) const {
            return !(rhs == *this);
        }

    private:

        Type _type;
        std::string _nextScene;

        Compare _compare;
};

