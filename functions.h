#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <QString>
class function{
public:
    function(){}
    function(QString n,int declr_pos,int addr){
        name = n;
        declr_position = declr_pos;
        actual_addr = addr;
    }
    QString name;
    int declr_position; //covers the list of offsets at the beginning
    int actual_addr; //covers the pointers before that
};
#endif // FUNCTIONS_H
