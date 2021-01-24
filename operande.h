#ifndef OPERANDE_H
#define OPERANDE_H
#include <QString>
struct Destination{
    Destination(int FID, int IID){
        FunctionID = FID;
        InstructionID = IID;
    }
    int FunctionID;
    int InstructionID;
};
class operande{

public:
    operande(){}
    operande(Destination pointer, int position, QString type, QByteArray value){
        Dest = pointer;
        Position = position;
        Type = type;
        Value = value;
    }
    operande(int position, QString type, QByteArray value){
        Position = position;
        Type = type;
        Value = value;
    }
    bool IsPointer(){
        if ((Dest.FunctionID == -1) || (Dest.InstructionID == -1)) return false;
        else return true;
    }
    QByteArray getValue(){
        return Value;
    }
    int getLength(){
        return Value.size();
    }
private:
    Destination Dest = Destination(-1,-1);
    int Position;
    QString Type;
    QByteArray Value;
};
#endif // OPERANDE_H
