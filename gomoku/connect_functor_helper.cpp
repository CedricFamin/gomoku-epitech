#include "connect_functor_helper.h"

connect_functor_helper::connect_functor_helper(QObject *parent, const std::tr1::function<void()> &f) : QObject(parent), function_(f)
{
}

void connect_functor_helper::signaled() {
    function_();
}
