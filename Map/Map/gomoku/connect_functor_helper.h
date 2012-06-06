#ifndef CONNECT_FUNCTION_HELPER_H
#define CONNECT_FUNCTION_HELPER_H

#include <QObject>
#include <functional>

class connect_functor_helper : public QObject {
    Q_OBJECT
public:
    connect_functor_helper(QObject *parent, const std::function<void()> &f);

public Q_SLOTS:
    void signaled();

private:
    std::function<void()> function_;
};

template <class T>
bool connect(QObject *sender, const char *signal, const T &reciever, Qt::ConnectionType type = Qt::AutoConnection) {
    return QObject::connect(sender, signal, new connect_functor_helper(sender, reciever), SLOT(signaled()), type);
}

#endif // CONNECT_FUNCTION_HELPER_H
