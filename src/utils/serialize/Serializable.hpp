//
// Created by raykuzu on 05/10/2020.
//

#pragma once

class Serializable {
    public:
        virtual ~Serializable() = default;

        virtual void serialize() = 0;
};
