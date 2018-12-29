#ifndef CARTEMODEL_H
#define CARTEMODEL_H

#include <QObject>
#include <QPalette>
#include <QString>
#include <QMap>

#include "catalog.h"

class ButtonDataWrapper;

class CarteModel : public QObject
{
    Q_OBJECT
public:
    explicit CarteModel(QString filename, QObject *parent = nullptr);
    const ButtonDataWrapper *getButton(unsigned int id) const;
    bool addEntry(unsigned int id, ButtonDataWrapper data);
private:
    static const unsigned int CARTE_VERSION=0;
    bool exportCarte() const;
    bool importCarte();
    QString filename;
    QMap<unsigned int, ButtonDataWrapper> table;

signals:
    void modelUpdated();
    void articleClicked(QString);

public slots:
    void buttonClicked(int id);

};

class ButtonDataWrapper
{
public:
    explicit ButtonDataWrapper(QString name, QColor backgroundColor, QColor textColor);
    QString getName() const;
    QColor getBackgroundColor() const;
    QColor getTextColor() const;
private:
    QString name;
    QColor backgroundColor, textColor;

};

#endif // CARTEMODEL_H
