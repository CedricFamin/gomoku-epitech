#ifndef CONNECT_FUNCTION_HELPER_H
#define CONNECT_FUNCTION_HELPER_H

#include <QObject>
#ifndef WIN32
#include <tr1/functional>
#else
#include <functional>
#endif

class connect_functor_helper : public QObject {
    Q_OBJECT
public:
    connect_functor_helper(QObject *parent, const std::tr1::function<void()> &f);

public Q_SLOTS:
    void signaled();

private:
    std::tr1::function<void()> function_;
};

template <class T>
bool connect(QObject *sender, const char *signal, const T &reciever, Qt::ConnectionType type = Qt::AutoConnection) {
    return QObject::connect(sender, signal, new connect_functor_helper(sender, reciever), SLOT(signaled()), type);
}

#endif // CONNECT_FUNCTION_HELPER_H
