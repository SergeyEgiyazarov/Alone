#pragma once

class AUtils
{
public:
    template <typename T>
    static T* GetAPlayerComponent(AActor* PlayerPawn)
    {
        if (!PlayerPawn) return nullptr;

        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());

        return Cast<T>(Component);
    }
};