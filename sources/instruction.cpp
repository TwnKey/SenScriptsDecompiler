#include "headers/instruction.h"
#include "headers/functions.h"
Instruction::Instruction(int addr, int OP, Builder *Maker)
{
            OPCode = OP;
            this->Maker = Maker;
            this->addr_instr = addr;
}
Instruction::Instruction(int addr, QString name, int OP, Builder *Maker){
     this->addr_instr = addr;
     OPCode = OP;
     this->name = name;
     this->Maker = Maker;
}
Instruction::~Instruction(){

}
Instruction::Instruction(int &addr, int idx_row, QXlsx::Document  &excelScenarioSheet, QString name, int OP,Builder *Maker){
        this->name = name;
        this->OPCode = OP;
        this->Maker = Maker;
        addr_instr = addr;
        if (OPCode<=0xFF) addr++;

        int idx_operande = 3;
        QString type = excelScenarioSheet.read(idx_row, idx_operande).toString();
        while(!type.isEmpty()){

            QByteArray Value;
            operande op;
            if (type == "int"){
                int Operande = excelScenarioSheet.read(idx_row+1, idx_operande).toInt();

                Value = GetBytesFromInt(Operande);
                op = operande(addr,"int", Value);

            }
            else if (type == "float"){

                float Operande = excelScenarioSheet.read(idx_row+1, idx_operande).toFloat();

                Value = GetBytesFromFloat(Operande);
                op = operande(addr,"float", Value);
            }
            else if (type == "short"){
                short Operande = excelScenarioSheet.read(idx_row+1, idx_operande).toInt();
                Value = GetBytesFromShort(Operande);
                op = operande(addr,"short", Value);

            }
            else if (type == "byte"){
                unsigned char Operande = ((excelScenarioSheet.read(idx_row+1, idx_operande).toInt())& 0x000000FF);
                Value;
                Value.push_back(Operande);
                op = operande(addr,"byte", Value);

            }
            else if (type == "bytearray"){

                while(type == "bytearray"){
                    unsigned char Operande = ((excelScenarioSheet.read(idx_row+1, idx_operande).toInt())& 0x000000FF);
                    Value.push_back(Operande);
                    idx_operande++;
                    type = excelScenarioSheet.read(idx_row, idx_operande).toString();
                }
                op = operande(addr,"bytearray", Value);
                idx_operande--;
            }
            else if ((type == "string")||(type == "dialog")){
                QString Operande = (excelScenarioSheet.read(idx_row+1, idx_operande).toString());
                Value = Operande.toUtf8();
                Value.replace('\n', 1);

                op = operande(addr,type, Value);
            }
            else if (type == "pointer"){
                QString Operande = (excelScenarioSheet.read(idx_row+1, idx_operande).toString());

                QString RowPointedStr = Operande.right(Operande.length()-2);

                int actual_row = RowPointedStr.toInt();

                Value = GetBytesFromInt(actual_row);

                op = operande(addr,"pointer", Value);

            }
            this->AddOperande(op);
            idx_operande++;
            addr = addr + op.getLength();
            type = excelScenarioSheet.read(idx_row, idx_operande).toString();
        }
    //qDebug() << this->getBytes() << " " << idx_row;
}
int Instruction::get_Nb_operandes(){
    return operandes.size();
}
operande Instruction::get_operande(int i){
    return operandes[i];
}

int Instruction::get_addr_instr(){
    return this->addr_instr;
}
void Instruction::WriteDat()
{
            qDebug() << "WriteDat test";
}
void Instruction::WriteXLSX(QXlsx::Document &excelScenarioSheet, std::vector<function> funs, int row)
{
    excelScenarioSheet.write(row, 1, "Location");
    excelScenarioSheet.write(row+1, 1, addr_instr);
    excelScenarioSheet.write(row, 2, "OP Code");
    excelScenarioSheet.write(row+1, 2, OPCode);
    int col_cnt = 0;
    for (int idx_op = 0; idx_op<operandes.size(); idx_op++){

        QString type = operandes[idx_op].getType();

        QByteArray Value = operandes[idx_op].getValue();
        if (type == "int"){
            excelScenarioSheet.write(row, 3+col_cnt, type);
            excelScenarioSheet.write(row+1, 3+col_cnt, ReadIntegerFromByteArray(0,Value));
            col_cnt++;
        }
        else if (type == "float"){
            excelScenarioSheet.write(row, 3+col_cnt, type);
            excelScenarioSheet.write(row+1, 3+col_cnt, ReadFloatFromByteArray(0,Value));
            col_cnt++;
        }
        else if (type == "short"){
            excelScenarioSheet.write(row, 3+col_cnt, type);
            excelScenarioSheet.write(row+1, 3+col_cnt, ReadShortFromByteArray(0,Value));
            col_cnt++;
        }
        else if (type == "byte"){
            excelScenarioSheet.write(row, 3+col_cnt, type);
            excelScenarioSheet.write(row+1, 3+col_cnt, (int)Value[0]);
            col_cnt++;
        }
        else if (type == "bytearray"){

            for (int idx_byte = 0; idx_byte<Value.size(); idx_byte++){
                excelScenarioSheet.write(row, 3+col_cnt, type);
                excelScenarioSheet.write(row+1, 3+col_cnt, (int)Value[idx_byte]);
                col_cnt++;
            }

        }
        else if ((type == "string")||(type == "dialog")){

            excelScenarioSheet.write(row, 3+col_cnt, type);
            QByteArray value_ = Value;

            value_.replace(1, "\n");
            QTextCodec *codec = QTextCodec::codecForName("UTF-8");

            QString string = codec->toUnicode(value_);

            excelScenarioSheet.write(row+1, 3+col_cnt, string);
            col_cnt++;
        }
        else if (type == "pointer"){
            excelScenarioSheet.write(row, 3+col_cnt, type);
            int ID = funs[0].ID;
            int nb_row = 3;
            int idx_fun = 0;
            while (ID != operandes[idx_op].getDestination().FunctionID){

                nb_row = nb_row + 1; //row with function name
                nb_row = nb_row + 2 * funs[idx_fun].InstructionsInFunction.size();
                idx_fun++;
                ID = funs[idx_fun].ID;
            }
            nb_row = nb_row + 1; //row with function name
            nb_row = nb_row + 2 * (operandes[idx_op].getDestination().InstructionID+1);
            QString ptrExcel = "=A"+QString::number((nb_row));

            QXlsx::Format format;
            format.setFontBold(true);
            QColor FontColor = QColor(qRgb(255,0,0));
            format.setFontColor(FontColor);

            excelScenarioSheet.write(row+1, 3+col_cnt, ptrExcel,format);
            col_cnt++;
        }


    }
    excelScenarioSheet.setRowHidden(row, true);
}
void Instruction::set_addr_instr(int i){
    addr_instr = i;
}
void Instruction::AddOperande(operande op){

    operandes.push_back(op);

    if (op.getType() == "pointer") {
        qDebug() << "POINTER !! " << hex << op.getAddr() << " " << op.getIntegerValue();
        //Maker->pointers.push_back(&*(operandes.end()-1));
    }
}
int Instruction::get_length_in_bytes(){

    return getBytes().size();
}

int Instruction::get_OP(){
    return OPCode;
}
QByteArray Instruction::getBytes(){
    QByteArray bytes;
    if (OPCode<=0xFF) bytes.push_back((unsigned char) OPCode);
    for (std::vector<operande>::iterator it = operandes.begin(); it!=operandes.end();it++) {

        QByteArray op_bytes = it->getValue();
        if (it->getType()=="string") op_bytes.push_back('\x0');
        for (int i = 0; i < op_bytes.size(); i++) bytes.push_back(op_bytes[i]);
    }
    return bytes;
}
