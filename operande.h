#ifndef OPERANDE_H
#define OPERANDE_H
#include <QString>
class operande{

public:
    operande(bool pointer, int position, QString type, QByteArray value){
        IsAPointer = pointer;
        Position = position;
        Type = type;
        Value = value;
    }

private:
    bool IsAPointer;
    int Position;
    QString Type;
    QByteArray Value;
};
#endif // OPERANDE_H
