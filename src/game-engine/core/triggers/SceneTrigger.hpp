//
// Created by raykuzu on 09/11/2020.
//

#pragma once

#include "ABiTrigger.hpp"
#include "SceneChange.hpp"
#include "Scene.hpp"

template <typename Compare>
class SceneTrigger : public ABiTrigger<SceneChange<Compare>, Scene, sharedGO> {
    public:
        SceneTrigger(SceneChange<Compare> const &sceneChange, std::function<void (Scene &, sharedGO &)> const &triggerFunc)
            : ABiTrigger(ITrigger::SCENE, sceneChange, triggerFunc) {};
        SceneTrigger(SceneChange<Compare> const &sceneChange, std::function<bool (Scene const &, sharedGO const &)> const &conditionFunc,
                     std::function<void (Scene &, sharedGO &)> const &triggerFunc)
            : ABiTrigger(ITrigger::SCENE, sceneChange, conditionFunc, triggerFunc) {};
        ~SceneTrigger() override = default;
    private:
};
