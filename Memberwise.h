/* 
 * File:   Memberwise.h
 * Author: michal
 *
 * Created on March 3, 2016, 12:03 AM
 */

#ifndef MEMBERWISE_H
#define	MEMBERWISE_H

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

template <class X, X value_>
struct member_ptr
{
    member_ptr(const char *name) : name(name) {}
    static constexpr X get() { return value_; }
    const char *name;
};

#define MEMBER(type, member) member_ptr<decltype(&type::member), &type::member>(#member)

template <class... members>
struct member_ptr_list
{
};

template <>
struct member_ptr_list<>
{
};

template <class... previous, class member_ptr_t, member_ptr_t ptr>
member_ptr_list<previous..., member_ptr<member_ptr_t, ptr>> operator,(member_ptr_list<previous...>, member_ptr<member_ptr_t, ptr>)
{
    return {};
}

template <class member_ptr1, member_ptr1 ptr1, class member_ptr2, member_ptr2 ptr2>
member_ptr_list<member_ptr<member_ptr1, ptr1>, member_ptr<member_ptr2, ptr2>> operator,(member_ptr<member_ptr1, ptr1>, member_ptr<member_ptr2, ptr2>)
{
    return {};
}


template <class target, class functor>
constexpr void apply(target &tgt, member_ptr_list<>, functor f)
{}

template <class target, class member0, class... members, class functor>
void apply(target &tgt, member_ptr_list<member0, members...>, functor f)
{
    f(tgt.*member0::get());
    apply(tgt, member_ptr_list<members...>(), f);
}


template <class type, class purpose_tag = void>
struct member_list : member_ptr_list<> {};

template <class purpose_tag = void, class target, class functor>
void memberwise(const target &tgt, functor f)
{
    apply(tgt, member_list<target, purpose_tag>(), f);
}
template <class purpose_tag = void, class target, class functor>
void memberwise(target &tgt, functor f)
{
    apply(tgt, member_list<target, purpose_tag>(), f);
}

#define MEMBER_LIST_ELEM(unused, type, x) , MEMBER(type, x)
#define MEMBER_LIST_ARGS(Type, ...) BOOST_PP_SEQ_FOR_EACH(MEMBER_LIST_ELEM, Type, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
#define MEMBER_LIST(Type, ...) template <> struct member_list<Type> : decltype(member_ptr_list<>() MEMBER_LIST_ARGS(Type, __VA_ARGS__)) {}
#define MEMBER_LIST_PURPOSE(Purpose, Type, ...) template <> struct member_list<Type, Purpose> : decltype(member_ptr_list<>() MEMBER_LIST_ARGS(Type, __VA_ARGS__)) {}


#endif	/* MEMBERWISE_H */

