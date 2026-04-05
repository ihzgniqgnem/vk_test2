#pragma once
#include <type_traits>
namespace vml_impl::type_traits {
    template <typename Ty>
    struct remove_const {
        using type = Ty;
    };

    template <typename Ty>
    struct remove_const<const Ty> {
        using type = Ty;
    };

    template <typename Ty>
    struct remove_volatile {
        using type = Ty;
    };

    template <typename Ty>
    struct remove_volatile<volatile Ty> {
        using type = Ty;
    };

    template <typename Ty>
    struct remove_ref {
        using type = Ty;
    };
    template <typename Ty>
    struct remove_ref<Ty&> {
        using type = Ty;
    };
    template <typename Ty>
    struct remove_ref<Ty&&> {
        using type = Ty;
    };

    template <bool Test, typename Ty1, typename Ty2>
    struct conditional {
        using type = Ty1;
    };

    template <class Ty1, typename Ty2>
    struct conditional<false, Ty1, Ty2 > {
        using type = Ty2;
    };
}
namespace vml {
    inline namespace type_traits {
        template <typename Ty>
        using remove_const = typename vml_impl::type_traits::remove_const<Ty>::type;

        template <typename Ty>
        using remove_volatile = typename vml_impl::type_traits::remove_volatile<Ty>::type;

        template <typename Ty>
        using remove_cv = remove_const<remove_volatile<Ty>>;

        template <typename Ty>
        using add_const = const Ty;

        template<typename Ty>
        using add_volatile = volatile Ty;

        template<typename Ty>
        using add_cv = const volatile Ty;

        template<typename Ty>
        using remove_ref = typename vml_impl::type_traits::remove_ref<Ty>::type;

        template<typename Ty>
        using add_lvalue_reference = Ty&;

        template<typename Ty>
        using add_rvalue_reference = Ty&&;

        template <bool Test, typename Ty1, typename Ty2>
        using conditional = vml_impl::type_traits::conditional<Test,Ty1,Ty2>::type;



        template <typename, typename>
        constexpr bool is_same = false;
        template <typename Ty>
        constexpr bool is_same<Ty, Ty> = true;

        template <typename Ty>
        constexpr bool is_void = is_same<void, Ty>;

    }
}
