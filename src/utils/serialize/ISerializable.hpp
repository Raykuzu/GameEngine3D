//
// Created by raykuzu on 05/10/2020.
//

#pragma once

class ISerializable {
    public:
        virtual ~ISerializable() = default;

        virtual void serialize() = 0;
};
