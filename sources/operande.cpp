#include "headers/operande.h"
#include "headers/functions.h"
#include "headers/Builder.h"
#include <deque>
operande::operande(std::string type, int& addr, int idx_row, QXlsx::Document& excelScenarioSheet, int &idx_column){

    if (Type == "int") {
        int Operande = excelScenarioSheet.read(idx_row + 1, idx_column).toInt();

        this->Value = GetBytesFromInt(Operande);
        this->Position = addr;

        this->Type = type;
        addr = addr + getLength();
    } else if (type == "float") {

        float Operande = excelScenarioSheet.read(idx_row + 1, idx_column).toFloat();

        this->Value = GetBytesFromFloat(Operande);
        this->Position = addr;

        this->Type = type;
        addr = addr + getLength();
    } else if (type == "short") {
        int16_t Operande = static_cast<int16_t>(excelScenarioSheet.read(idx_row + 1, idx_column).toInt());
        this->Value = GetBytesFromShort(Operande);
        this->Position = addr;
        this->Type = type;
        addr = addr + getLength();

    } else if (type == "byte")  {
        uint8_t byte = excelScenarioSheet.read(idx_row + 1, idx_column).toInt();

        auto Operande = static_cast<char>(((byte)&0x000000FF));
        this->Value.push_back(Operande);
        this->Position = addr;
        this->Type = type;
        addr = addr + getLength();

    }else if (type == "fill") {
        QString Operande_prev = excelScenarioSheet.read(idx_row + 1, idx_column - 1).toString();
        QString Operande = excelScenarioSheet.read(idx_row + 1, idx_column).toString();
        QString str_max_length = Operande.mid(1, Operande.indexOf('-') - 1);

        for (int id = 0; id < str_max_length.toInt() - Operande_prev.toUtf8().size() - 1; id++) {
            this->Value.push_back('\0');
        }
        this->Position = addr;
        this->Type = type;
        addr = addr + getLength();

    }

    else if ((type == "string") || (type == "dialog")) {
        QString Operande = (excelScenarioSheet.read(idx_row + 1, idx_column).toString());
        this->Value = Operande.toUtf8();
        QTextCodec* codec = QTextCodec::codecForName(QString::fromStdString(OutputDatFileEncoding).toUtf8());
        this->Value = codec->fromUnicode(Operande);
        if (type == "string") this->Value.push_back('\0');
        this->Value.replace('\n', 1);

        this->Position = addr;
        this->Type = type;
        addr = addr + getLength();
    } else if (Type == "bytearray") {

        while (Type == "bytearray") {
            auto Operande = static_cast<char>(((excelScenarioSheet.read(idx_row + 1, idx_column).toInt()) & 0x000000FF));
            this->Value.push_back(Operande);
            idx_column++;
            Type = excelScenarioSheet.read(idx_row, idx_column).toString().toStdString();
        }
        this->Position = addr;
        this->Type = type;
        idx_column--;
        addr = addr + getLength();
    } else if (type == "pointer") {
        QString Operande = (excelScenarioSheet.read(idx_row + 1, idx_column).toString());
        QString RowPointedStr = Operande.right(Operande.size() - 2);
        int actual_row = RowPointedStr.toInt();
        this->Value = GetBytesFromInt(actual_row);
        this->Position = addr;
        this->Type = type;
        addr = addr + getLength();
    }
}

bool operande::write_xlsx(QXlsx::Document& excelScenarioSheet, int row, int& col, int &col_cnt,
                QXlsx::Format FormatInstr,
                QXlsx::Format FormatType,
                QXlsx::Format FormatOP,
                QXlsx::Format FormatStartEnd,
                std::vector<function> funs,
                Builder *b

                ){
    QString type = QString::fromStdString(getType());
    QByteArray Value = getValue();

    if (type == "int") {
        excelScenarioSheet.write(row, col + 3 + col_cnt, type, FormatType);
        excelScenarioSheet.write(row + 1, col + 3 + col_cnt, ReadIntegerFromByteArray(0, Value), FormatInstr);
        col_cnt++;

    } else if (type == "header") {
        uint32_t header_value = ReadIntegerFromByteArray(0, Value);
        uint8_t length_byte = header_value >> 0x18;
        uint32_t group_id = header_value & 0xFFFFFF;
        excelScenarioSheet.write(row, col + 3 + col_cnt, "Group ID", FormatType);
        excelScenarioSheet.write(row + 1, col + 3 + col_cnt, group_id, FormatInstr);
        col_cnt++;
        excelScenarioSheet.write(row, col + 3 + col_cnt, "Length", FormatType);
        excelScenarioSheet.write(row + 1, col + 3 + col_cnt, length_byte, FormatInstr);
        col_cnt++;
    } else if (type == "float") {
        excelScenarioSheet.write(row, col + 3 + col_cnt, type, FormatType);
        excelScenarioSheet.write(row + 1, col + 3 + col_cnt, QByteArrayToFloat(Value), FormatInstr);
        col_cnt++;
    } else if (type == "short") {
        excelScenarioSheet.write(row, col + 3 + col_cnt, type, FormatType);
        excelScenarioSheet.write(row + 1, col + 3 + col_cnt, (uint16_t)ReadShortFromByteArray(0, Value), FormatInstr);
        col_cnt++;
    } else if (type == "byte") {

        excelScenarioSheet.write(row, col + 3 + col_cnt, type, FormatType);
        excelScenarioSheet.write(row + 1, col + 3 + col_cnt, (unsigned char)Value[0], FormatInstr);
        col_cnt++;
    } else if (type == "fill") {
        excelScenarioSheet.write(row, col + 3 + col_cnt, type, FormatType);
        excelScenarioSheet.write(row + 1,
                                 col + 3 + col_cnt,
                                 "=" + QString::number(getBytesToFill()) + "-LENB(INDIRECT(ADDRESS(" +
                                   QString::number(row + 1) + "," + QString::number(3 + col_cnt - 1) + ")))",
                                 FormatInstr);
        col_cnt++;
    } else if (type == "bytearray") {

        for (auto&& idx_byte : Value) {
            excelScenarioSheet.write(row, col + 3 + col_cnt, type, FormatType);
            excelScenarioSheet.write(row + 1, col + 3 + col_cnt, (unsigned char)idx_byte, FormatInstr);
            col_cnt++;
        }

    } else if (type == "instruction") {
        int addr = 0;
        excelScenarioSheet.write(row, col + 3 + col_cnt, "Start Instruction", FormatStartEnd);
        excelScenarioSheet.write(row + 1, col + 3 + col_cnt, "", FormatStartEnd);
        col_cnt++;

        std::shared_ptr<Instruction> instr = b->CreateInstructionFromDAT(
          addr, Value, 0); // function type is 0 here because sub05 is only called by OP Code instructions.
        int column_instr = col + 3 + col_cnt - 2;
        int cnt = instr->WriteXLSX(excelScenarioSheet, funs, row, column_instr);
        col = col + cnt + 1;

        excelScenarioSheet.write(row, col + 3 + col_cnt, "End Instruction", FormatStartEnd);
        excelScenarioSheet.write(row + 1, col + 3 + col_cnt, "", FormatStartEnd);
        col_cnt++;

    }

    else if ((type == "string") || (type == "dialog")) {

        excelScenarioSheet.write(row, col + 3 + col_cnt, type, FormatType);
        QByteArray value_ = Value;

        value_.replace(1, "\n");

        QTextCodec* codec = QTextCodec::codecForName(QString::fromStdString(InputDatFileEncoding).toUtf8());

        QString string = codec->toUnicode(value_);

        excelScenarioSheet.write(row + 1, col + 3 + col_cnt, string, FormatInstr);
        col_cnt++;
    } else if (type == "pointer") {
        excelScenarioSheet.write(row, col + 3 + col_cnt, type, FormatType);
        int ID = funs[0].ID;
        size_t nb_row = 3;
        int idx_fun = 0;
        while (ID != getDestination().FunctionID) {

            nb_row = nb_row + 1; // row with function name
            nb_row = nb_row + 2 * funs[idx_fun].InstructionsInFunction.size();
            idx_fun++;
            ID = funs[idx_fun].ID;
        }
        nb_row = nb_row + 1; // row with function name
        nb_row = nb_row + static_cast<uint64_t>(2 * (getDestination().InstructionID + 1));
        QString ptrExcel = "=A" + QString::number((nb_row));

        QXlsx::Format format;
        format.setBottomBorderStyle(QXlsx::Format::BorderThin);
        format.setLeftBorderStyle(QXlsx::Format::BorderThin);
        format.setRightBorderStyle(QXlsx::Format::BorderThin);
        format.setTopBorderStyle(QXlsx::Format::BorderThin);
        format.setFontBold(true);
        auto FontColor = QColor(qRgb(255, 0, 0));
        format.setFontColor(FontColor);
        excelScenarioSheet.write(row + 1, col + 3 + col_cnt, ptrExcel, format);
        col_cnt++;
    }
    else if (type == "condition"){
        excelScenarioSheet.write(row, col + 3 + col_cnt, "Start Condition", FormatStartEnd);
        excelScenarioSheet.write(row + 1, col + 3 + col_cnt, "", FormatStartEnd);
        col_cnt++;
        for (auto op : operandes)
            op.write_xlsx(excelScenarioSheet, row, col, col_cnt,
                          FormatInstr,
                          FormatType,
                          FormatOP,
                          FormatStartEnd,
                          funs,
                          b);
        excelScenarioSheet.write(row, col + 3 + col_cnt, "End Condition", FormatStartEnd);
        excelScenarioSheet.write(row + 1, col + 3 + col_cnt, "", FormatStartEnd);
        col_cnt++;
    }
    return true;
}

std::string operande::to_string(Builder * ib) {
    if (Type == "int") {
        return std::to_string(getIntegerValue());

    } else if (Type == "header") {
        uint32_t header_value = ReadIntegerFromByteArray(0, Value);
        uint8_t length_byte = header_value >> 0x18;
        uint32_t group_id = header_value & 0xFFFFFF;
        //for those, they don't affect anything in game afaik
        return std::to_string(group_id) + ", " + std::to_string(length_byte);

    } else if (Type == "float") {
        float value = QByteArrayToFloat(Value);
        return std::to_string(value);

    } else if (Type == "short") {
        uint16_t value = ReadShortFromByteArray(0,Value);
        return std::to_string(value);

    } else if (Type == "byte") {
        uint8_t value = Value[0];
        return std::to_string(value);

    }
    else if (Type == "instruction"){
        /*
         * shouldn't happen because it is only used inside a "condition" which is taken care of below
        */

    }
    else if (Type == "condition"){ //This is "only" for CS4 (afaik, it's valid for each game, but i need to keep that in mind)
        //if it ends up being different for each game, I'll have to add this part to the builder (and use the ib parameter to build the condition)

        std::deque<std::string> queue;
        unsigned int idx_operand = 0;
        while (idx_operand < operandes.size()){
            //interpreting each "operator"

            switch(operandes[idx_operand].Value[0]){
                //some operators
                case 0:

                    queue.push_back(std::to_string(operandes[idx_operand+1].getIntegerValue())); //not sure if it could be float sometimes? I doubt it.
                    idx_operand+=2;
                    break;
                case 1:

                    return queue.front();
                case 2:
                    queue.front() = "(" + queue.front() + " == " + queue.back() +")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 3:
                    queue.front()= "(" + queue.front()+ " != " + queue.back()+ ")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 4:
                    queue.front()= "(" + queue.front()+ " < " + queue.back()+ ")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 5:
                    queue.front()= "(" + queue.front()+ " > " + queue.back()+ ")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 6:
                    queue.front()= "(" + queue.front()+ " <= " + queue.back()+ ")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 7:
                    queue.front()= "(" + queue.front()+ " >= " + queue.back()+ ")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 8:
                    queue.front()= "(" + queue.front()+ " == 0" + ")";

                    idx_operand++;
                    break;
                case 9:
                    queue.front()= "(" + queue.front()+ " && " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 10:
                    queue.front()= "(" + queue.front()+ " & " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 11:
                    queue.front()= "(" + queue.front()+ " | " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 12:
                    queue.front()= "(" + queue.front()+ " + " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 13:
                    queue.front()= "(" + queue.front()+ " - " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 14:
                    queue.back()= "-" + queue.back() +"";
                    idx_operand++;
                    break;
                case 15:
                    queue.front()= "(" + queue.front()+ " ^ " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 16:
                    queue.front()= "(" + queue.front()+ " * " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 17:
                    queue.front()= "(" + queue.front()+ " / " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 18:
                    queue.front()= "(" + queue.front()+ " % " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 20:
                    queue.front()= "(" + queue.front()+ " * " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 21:
                    queue.front()= "(" + queue.front()+ " / " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 22:
                    queue.front()= "(" + queue.front()+ " % " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 23:
                    queue.front()= "(" + queue.front()+ " + " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 24:
                    queue.front()= "(" + queue.front()+ " - " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 25:
                    queue.front()= "(" + queue.front()+ " & " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 26:
                    queue.front()= "(" + queue.front()+ " ^ " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 27:
                    queue.front()= "(" + queue.front()+ " | " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 28:{
                    int addr = 0;
                    std::shared_ptr<Instruction> instr = (ib->CreateInstructionFromDAT(addr,operandes[idx_operand+1].Value, 0));
                    queue.push_back(instr->to_string());
                    idx_operand+=2;
                    break;
                }
                case 29:
                    queue.front()= "(" + queue.front()+ " ~ " + queue.back()+")";
                    queue.pop_back();
                    idx_operand++;
                    break;
                case 30:
                    {
                    uint16_t parameter = ReadShortFromByteArray(0, operandes[idx_operand+1].Value);
                    uint16_t ptr =  parameter >> 3;
                    uint8_t i = parameter & 0x7;

                    queue.push_back("some_location30_"+std::to_string(i)+"["+std::to_string(ptr)+"]");

                    idx_operand+=2;
                    break;
                }
                case 31:
                    {
                    uint8_t parameter = operandes[idx_operand+1].Value[0];
                    queue.push_back("some_location31["+std::to_string(parameter)+"]");
                    idx_operand+=2;
                    break;
                }
                case 32:
                    {
                    uint8_t parameter = operandes[idx_operand+1].Value[0];
                    queue.push_back("some_location32["+std::to_string(parameter)+"]");
                    idx_operand+=2;
                    break;
                }
                case 33:
                    {
                    uint16_t wd = ReadShortFromByteArray(0, operandes[idx_operand+1].Value);
                    uint8_t parameter = operandes[idx_operand+2].Value[0];
                    queue.push_back("some_function33("+std::to_string(wd)+", " + std::to_string(parameter)+")");
                    idx_operand+=3;
                    break;
                }
                case 34:
                    {

                    queue.push_back("some_function34()");
                    idx_operand++;
                    break;
                }
                case 35:
                    {
                    uint8_t parameter = operandes[idx_operand+1].Value[0];
                    queue.push_back("some_location35["+std::to_string(parameter)+"]");
                    idx_operand+=2;
                    break;
                }
                case 36:
                    {
                    uint32_t parameter = operandes[idx_operand+1].getIntegerValue();
                    queue.push_back("CheckSomething("+std::to_string(parameter)+")");
                    idx_operand+=2;
                    break;
                }
                case 37:
                    {
                    uint8_t parameter = operandes[idx_operand+1].Value[0];
                    queue.push_back("some_location37["+std::to_string(parameter)+"]");
                    idx_operand+=2;
                    break;
                }
                case 19:

                    idx_operand++;
                    break;
                default:
                std::string msg = "Wrong operator " + std::to_string(operandes[idx_operand].getAddr()) + " " + std::to_string(operandes[idx_operand].Value[0]);
                qFatal(msg.c_str());
            }

        }


    }
    else if (Type == "fill") {
        /*those are skipped and taken care of during the recompilation (all of the instruction using a fill
        will have their own implementation of the recompilation, with hardcoded length*/

    }

    else if ((Type == "string") || (Type == "dialog")) {

        QByteArray value_ = Value;
        if (value_[value_.size()-1] == '\0') value_.chop(1);
        value_.replace(1, "\n");

        QTextCodec* codec = QTextCodec::codecForName(QString::fromStdString(InputDatFileEncoding).toUtf8());
        QString string = "\""+codec->toUnicode(value_)+"\"";


        return string.toStdString();
    } else if (Type == "bytearray") {
        std::string str;
        for (uint8_t x : Value){
            str = str + std::to_string(x) + ",";
        }
        str.pop_back();
        return str;
    } else if (Type == "pointer") {
        return std::to_string(getIntegerValue());
        //those are skipped and taken care of during the decompilation, by
        //setting up labels and goto (as in EDDecompiler/Python)
    }

return "";
}

