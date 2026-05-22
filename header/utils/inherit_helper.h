#pragma once
#define VKM_UTILS_SPECIAL_FUNCTION_INHERIT(Self,Base) \
    template<typename... Args>\
    decltype(auto) operator=(Args&&... args) {\
        return Base::operator=(std::forward<Args>(args)...);\
    } \
    template<typename... Args>\
    Self(Args&&... args):Base(std::forward<Args>(args)...) {}