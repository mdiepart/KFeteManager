#ifndef CLIENTLIST_H
#define CLIENTLIST_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QException>
#include <QtGlobal>

class Client;
class DuplicateClientException;

class Client
{
public:
    explicit Client(QString name = "");
    Client(const Client &c);

    void create(QString phone = "", QString address = "", QString email = "", qreal limit = 0, bool isJobist = false, qreal balance = 0);
    bool exists() const;

    QString getName() const;

    void setPhone(QString tel);
    QString getPhone() const;

    void setAddress(QString adr);
    QString getAddress() const;

    void setEmail(QString email);
    QString getEmail() const;

    void setJobist(bool isJobist);
    bool isJobist() const;

    void setLimit(qreal limit);
    qreal getLimit() const;

    bool deposit(qreal amount);
    qreal getBalance() const;

    bool isNull() const;

    Client &operator=(const Client &u);

    bool operator==(const Client &c) const;
    bool operator<(const Client &c) const;
    operator QString() const{return this->getName();}

private:
    QString name;
    static constexpr qreal LIMIT_MIN = -10;

};

Q_DECLARE_METATYPE(Client)

#endif // CLIENTLIST_H
