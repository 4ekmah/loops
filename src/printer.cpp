/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "backend.hpp"
#include "printer.hpp"
#include <stdarg.h>
#include <cstdio>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <unordered_map>

namespace loops
{

enum {SUFFIX_ELEMTYPE, SUFFIX_CONDITION, SUFFIX_VOID};
typedef struct one_name_one_suffix
{
    char prefix[30];
    int argnum;
    int suffix_type;
    int fracture_size; //If op->arg_size >= fracture_size, argnum converts to (argnum + 1). Ignored if fracture_size == 0
} one_name_one_suffix;

typedef struct suffixed_opname
{
    int pieces_size;
    one_name_one_suffix pieces[3];
} suffixed_opname;


static inline cstring opstrings_ir_getter(int opcode)
{
    switch (opcode)
    {
    //   |         enum_id          |              string_id           |
    case (OP_MOV             ) : return "mov"                   ;
    case (OP_XCHG            ) : return "xchg"                  ;
    case (OP_ADD             ) : return "add"                   ;
    case (OP_SUB             ) : return "sub"                   ;
    case (OP_MUL             ) : return "mul"                   ;
    case (OP_DIV             ) : return "div"                   ;
    case (OP_MOD             ) : return "mod"                   ;
    case (OP_SHL             ) : return "shl"                   ;
    case (OP_SHR             ) : return "shr"                   ;
    case (OP_SAR             ) : return "sar"                   ;
    case (OP_AND             ) : return "and"                   ;
    case (OP_OR              ) : return "or"                    ;
    case (OP_XOR             ) : return "xor"                   ;
    case (OP_NOT             ) : return "not"                   ;
    case (OP_NEG             ) : return "neg"                   ;
    case (OP_CMP             ) : return "cmp"                   ;
    case (OP_MIN             ) : return "min"                   ;
    case (OP_MAX             ) : return "max"                   ;
    case (OP_ABS             ) : return "abs"                   ;
    case (OP_SIGN            ) : return "sign"                  ;
    case (OP_SPILL           ) : return "spill"                 ;
    case (OP_UNSPILL         ) : return "unspill"               ;
    case (OP_GT              ) : return "gt"                    ;
    case (OP_UGT             ) : return "ugt"                   ;
    case (OP_GE              ) : return "ge"                    ;
    case (OP_LT              ) : return "lt"                    ;
    case (OP_LE              ) : return "le"                    ;
    case (OP_ULE             ) : return "ule"                   ;
    case (OP_NE              ) : return "ne"                    ;
    case (OP_EQ              ) : return "eq"                    ;
    case (OP_S               ) : return "s"                     ;
    case (OP_NS              ) : return "ns"                    ;
    case (OP_LOGICAL_AND     ) : return "log_and"               ;
    case (OP_LOGICAL_OR      ) : return "log_or"                ;
    case (OP_LOGICAL_NOT     ) : return "log_not"               ;
    case (OP_JMP             ) : return "jmp"                   ;
    case (OP_RET             ) : return "ret"                   ;
    case (OP_CALL            ) : return "call"                  ;
    case (OP_CALL_NORET      ) : return "call_noret"            ;
    case (OP_STEM_CSTART     ) : return "annotation:stemcstart" ;
    case (OP_IF_CSTART       ) : return "annotation:ifcstart"   ;
    case (OP_ELIF_CSTART     ) : return "annotation:elif"       ;
    case (OP_IF_CEND         ) : return "annotation:ifcend"     ;
    case (OP_ELSE            ) : return "annotation:else"       ;
    case (OP_ENDIF           ) : return "annotation:endif"      ;
    case (OP_WHILE_CSTART    ) : return "annotation:whilecstart";
    case (OP_WHILE_CEND      ) : return "annotation:whilecend"  ;
    case (OP_ENDWHILE        ) : return "annotation:endwhile"   ;
    case (OP_BREAK           ) : return "annotation:break"      ;
    case (OP_CONTINUE        ) : return "annotation:continue"   ;
    case (VOP_AND            ) : return "and"                   ;
    case (VOP_OR             ) : return "or"                    ;
    case (VOP_XOR            ) : return "xor"                   ;
    case (VOP_NOT            ) : return "not"                   ;
    case (OP_X86_ADC         ) : return "x86_adc"               ;
    case (OP_X86_CQO         ) : return "x86_cqo"               ;
    case (VOP_X86_VEXTRACT128) : return "x86_vextract128"       ;
    case (VOP_X86_VINSERT128 ) : return "x86_vinsert128"        ;
    case (VOP_X86_VPERM2I128 ) : return "x86_vperm2i128"        ;
    case (VOP_X86_VPALIGNR   ) : return "x86_vpalignr"          ;
    case (VOP_X86_VPSHUFD    ) : return "x86_vpshufd"           ;
    case (VOP_X86_VPSADBW    ) : return "x86_vpsadbw"           ;
    case (VOP_X86_VPHADDD    ) : return "x86_vphaddd"           ;
    case (VOP_X86_VHADDPS    ) : return "x86_vhaddps"           ;
    case (VOP_X86_VADDSS     ) : return "x86_vaddss"            ;
    case (VOP_X86_VHADDPD    ) : return "x86_vhaddpd"           ;
    case (VOP_X86_VADDSD     ) : return "x86_vaddsd"            ;
    case (OP_ARM_CINC        ) : return "arm_cinc"              ;
    case (OP_ARM_CNEG        ) : return "arm_cneg"              ;
    case (OP_ARM_MOVK        ) : return "arm_movk"              ;
    case (OP_ARM_LDP         ) : return "arm_ldp"               ;
    case (OP_ARM_STP         ) : return "arm_stp"               ;
    case (OP_RV_LUI          ) : return "rv_lui"                ;
    case (OP_DEF             ) : return "def"                   ;
    };
    return nullptr;
}

static inline cstring cond_suffixes_getter(int condcode)
{
    switch (condcode)
    {
//       |   enum_id   |   string_id   |   
    case (OP_EQ ) : return "eq" ;
    case (OP_NE ) : return "ne" ;
    case (OP_GE ) : return "ge" ;
    case (OP_LE ) : return "le" ;
    case (OP_ULE) : return "ule";
    case (OP_GT ) : return "gt" ;
    case (OP_UGT) : return "ugt";
    case (OP_LT ) : return "gt" ;
    case (OP_S  ) : return "s"  ;
    case (OP_NS ) : return "ns" ;
    };
    return nullptr;
}

static inline cstring type_suffixes_getter(int typecode)
{
    switch (typecode)
    {
//       |    enum_id     |   string_id    |   
    case (TYPE_U8  ) : return "u8"  ;
    case (TYPE_I8  ) : return "i8"  ;
    case (TYPE_U16 ) : return "u16" ;
    case (TYPE_I16 ) : return "i16" ;
    case (TYPE_U32 ) : return "u32" ;
    case (TYPE_I32 ) : return "i32" ;
    case (TYPE_U64 ) : return "u64" ;
    case (TYPE_I64 ) : return "i64" ;
    case (TYPE_FP16) : return "fp16";
    case (TYPE_BF16) : return "bf16";
    case (TYPE_FP32) : return "fp32";
    case (TYPE_FP64) : return "fp64";
    };
    return nullptr;
}

std::unordered_map<int, suffixed_opname> suffixed_opnames = 
{
//  |         enum_id            |pieces_size|                pieces                    |      
//                                     |        prefix    |argnum|suffix_type|fracture_size|...

    {OP_LOAD              , {1, {{"load."             , 0, SUFFIX_ELEMTYPE, 0}}}},
    {OP_STORE             , {1, {{"store."            , 1, SUFFIX_ELEMTYPE, 3}}}},
    {OP_SELECT            , {1, {{"select_"           , 1, SUFFIX_CONDITION,0}}}},
    {OP_IVERSON           , {1, {{"iverson_"          , 1, SUFFIX_CONDITION,0}}}},
    {VOP_LOAD             , {1, {{"vld."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_STORE            , {1, {{"vst."              , 1, SUFFIX_ELEMTYPE, 3}}}},
    {VOP_ADD              , {1, {{"add."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_SUB              , {1, {{"sub."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_MUL              , {1, {{"mul."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_DIV              , {1, {{"div."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_FMA              , {1, {{"fma."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_SAL              , {1, {{"sal."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_SHL              , {1, {{"shl."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_SAR              , {1, {{"sar."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_SHR              , {1, {{"shr."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_NEG              , {1, {{"neg."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_MIN              , {1, {{"min."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_MAX              , {1, {{"max."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_GT               , {1, {{"gt."               , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_GE               , {1, {{"ge."               , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_LT               , {1, {{"lt."               , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_LE               , {1, {{"le."               , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_NE               , {1, {{"ne."               , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_EQ               , {1, {{"eq."               , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_SELECT           , {1, {{"select."           , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_TRUNC            , {2, {{"trunc."            , 1, SUFFIX_ELEMTYPE, 0}, {"_"     , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_FLOOR            , {2, {{"floor."            , 1, SUFFIX_ELEMTYPE, 0}, {"_"     , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_CAST             , {2, {{"cast."             , 1, SUFFIX_ELEMTYPE, 0}, {"_"     , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_BROADCAST        , {1, {{"broadcast."        , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_CAST_LOW         , {3, {{"cast."             , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}, {".low" , 0, SUFFIX_VOID, 0}}}},
    {VOP_CAST_HIGH        , {3, {{"cast."             , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}, {".high", 0, SUFFIX_VOID, 0}}}},
    {VOP_SHRINK           , {2, {{"shrink."           , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_POPCOUNT         , {1, {{"popcount."         , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_REDUCE_MAX       , {1, {{"reduce.max."       , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_REDUCE_MIN       , {1, {{"reduce.min."       , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_REDUCE_SUM       , {1, {{"reduce.sum."       , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_REDUCE_WSUM      , {2, {{"reduce.wsum."      , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_ARM_LD1          , {1, {{"vld_lane."         , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_ARM_ST1          , {1, {{"vst_lane."         , 1, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_ARM_LD2          , {1, {{"vld_deinterleave2.", 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_EXT              , {1, {{"ext."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_ARM_SHRINK_LOW   , {3, {{"cast."             , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}, {".low" , 0, SUFFIX_VOID, 0}}}},
    {VOP_ARM_SHRINK_HIGH  , {3, {{"cast."             , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}, {".high", 0, SUFFIX_VOID, 0}}}},
    {VOP_GETLANE          , {1, {{"getlane."          , 1, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_SETLANE          , {1, {{"setlane."          , 0, SUFFIX_ELEMTYPE, 0}}}},
    {VOP_DEF              , {1, {{"vdef."             , 0, SUFFIX_ELEMTYPE, 0}}}}, 
};



void loops_printf(program_printer* printer, const char *__restrict __format,...)
{
    std::vector<char>& buffers_tail = printer->buffers.back();
    int chars_left = (int)buffers_tail.size() - printer->current_offset;
    char* nextcharpos = buffers_tail.data() + printer->current_offset;
    va_list var_args;
    va_start(var_args, __format);
    int written = vsnprintf(nextcharpos, chars_left, __format, var_args);
    va_end ( var_args );
    if(written < 0 || written >= chars_left)
    {
        if(printer->cells.empty())
            throw loops::exception(LOOPS_ERR_UNIMAGINARY_BIG_STRING);
        char* current_cell_start = printer->cells.back().ptr + printer->cells.back().size + 1;
        int current_cell_size = (int)(buffers_tail.data() + printer->current_offset - current_cell_start);
        if(current_cell_size < 0) 
            throw loops::exception(LOOPS_ERR_POINTER_ARITHMETIC_ERROR);
        if(current_cell_size + written >= (int)buffers_tail.size())
            throw loops::exception(LOOPS_ERR_UNIMAGINARY_BIG_STRING);
        printer->augment_buffer(0);
        std::vector<char>& newtail = printer->buffers.back();
        if(current_cell_size > 0)
            memcpy(newtail.data(), current_cell_start, current_cell_size);
        buffers_tail = newtail;
        printer->current_offset = current_cell_size;
        chars_left = (int)buffers_tail.size();
        nextcharpos = buffers_tail.data() + printer->current_offset;
        va_list var_args2;
        va_start(var_args2, __format);
        written = vsnprintf(nextcharpos, chars_left, __format, var_args2);
        va_end(var_args2);
        if(written < 0 || written >= chars_left)
            throw loops::exception(LOOPS_ERR_UNIMAGINARY_BIG_STRING);
    }
    printer->current_offset += written;
    va_end ( var_args );
}

void print_address(program_printer* printer, int64_t addr)
{
    static char hexsymb[] = "0123456789ABCDEF";
    char* bytes = (char*)(&addr);
    loops_printf(printer, "0x");
    for (int i = 0; i < 8; i++)
        loops_printf(printer, "%c%c", hexsymb[(bytes[7 - i] & 0xF0) >> 4], hexsymb[bytes[7 - i] & 0x0F]);
}

void program_printer::close_printer_cell()
{
    std::vector<char>& buffers_tail = buffers.back();
    int buffers_tail_size = (int)buffers_tail.size();
    char* buffers_tail_data = buffers_tail.data();
    char* newcell = buffers_tail_data;
    bool newbuffer = cells.empty(); 
    buffers_tail_data[current_offset] = 0;
    if (!newbuffer)
    {
        char* prevcell = cells.back().ptr;
        if (prevcell < buffers_tail_data || //Buffer augmentation happened
            prevcell >= (buffers_tail_data + buffers_tail_size))
            newbuffer = true;
        else
            newcell = prevcell + cells.back().size + 1;
    }
    int len = (newcell >= buffers_tail_data + buffers_tail_size) ? 0 : (int)strlen(newcell);
    if(len == 0 && !newbuffer)
        newcell--; //Empty strings doesn't use space
    else 
        current_offset++;
    cells.push_back({newcell, len});
}

void program_printer::augment_buffer(int buffer_size)
{
    if(buffers.size())
    {
        buffer_size = (int)buffers.front().size();
    }
    buffers.emplace_back(std::vector<char>(buffer_size));
}

enum {PRINT_TO_FILE, PRINT_TO_STRING};

inline void snprintf_wrapped(char*& currentout, int& bufferleft, const char *__restrict __format,...)
{
    va_list var_args;
    va_start(var_args, __format);
    int written = vsnprintf(currentout, bufferleft, __format, var_args);
    va_end ( var_args );
    currentout += written;
    bufferleft -= written;
}

std::string program_printer::print_syntfunc(FILE* fout, int outtype, const Syntfunc& func)
{
    std::string sout;
    int params_size = (int)func.params.size();
    const Arg* params = func.params.data();

    int cells_amount = 0;
    static int MAX_LINE_SIZE = 82; //taken from statistics
    int cols = (int)colprinters.size();
    int rows = (int)func.program.size();

    augment_buffer(MAX_LINE_SIZE * rows);

    std::vector<int> max_widthes(cols, 0);
    std::vector<char*> printtasks(cols, nullptr);
    std::vector<char> printtasksbuf(cols *10);
    cells.reserve(cols*rows);
    current_offset = 0;

    try
    {
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                colprinters[col]->func(this, colprinters[col].get(), func, row);
                int collen = cells[row * cols + col].size + 1;
                max_widthes[col] = (max_widthes[col] < collen ? collen : max_widthes[col]);
                cells_amount++;
            }
        }
    }
    catch(const std::exception& e)
    {
        fout = stderr;
        fprintf(fout, "Loops: printing error: %s\nCurrently printed:\n", e.what());
        outtype = PRINT_TO_FILE;
    }

    for(int col = 0; col < cols; col++)
    {
        printtasks[col] = &printtasksbuf[10 * col]; 
        snprintf(printtasks[col], 10, "%%-%ds", max_widthes[col]); 
    }

    if (outtype == PRINT_TO_FILE)
    {
        fprintf(fout, "%s(", func.name.c_str());
        for (int parnum = 0; parnum < params_size - 1; parnum++)
            fprintf(fout, "i%d, ", (params + parnum)->idx);
        if (params_size)
            fprintf(fout, "i%d", (params + params_size - 1)->idx);
        fprintf(fout, ")\n");
        for(int col = 0, cell = 0; cell < cells_amount; cell++)
        {
            fprintf(fout, printtasks[col], cells[cell].ptr);
            if(col == cols - 1 || cell == cells_amount - 1)
                fprintf(fout, "\n");
            col++;
            if (col == cols)
                col = 0;
        }
    }
    else if (outtype == PRINT_TO_STRING)
    {
        //Let's calculate out buffer size and allocate it
        int bufferleft = (int)func.name.size() + 6;
        for (int parnum = 0; parnum < params_size; parnum++)
        {
            assert((params + parnum)->idx < 100);
            bufferleft += ((params + parnum)->idx > 10 ? 2 : 1) + 3;
        }
        {
            int lensize = 2;
            for (int col = 0; col < cols; col++)
                lensize += max_widthes[col];
            bufferleft += lensize * rows;
        }
        sout.resize(bufferleft+1, 0);
        char* currentout = sout.data();
        //Write header:
        snprintf_wrapped(currentout, bufferleft, "%s(", func.name.c_str());
        for (int parnum = 0; parnum < params_size - 1; parnum++)
            snprintf_wrapped(currentout, bufferleft, "i%d, ", (params + parnum)->idx);
        if (params_size)
            snprintf_wrapped(currentout, bufferleft, "i%d", (params + params_size - 1)->idx);
        snprintf_wrapped(currentout, bufferleft, ")\n");
        //Write instructions:
        for (int col = 0, cell = 0; cell < cells_amount; cell++)
        {
            snprintf_wrapped(currentout, bufferleft, printtasks[col], cells[cell].ptr);
            if (col == cols - 1 || cell == cells_amount - 1)
                snprintf_wrapped(currentout, bufferleft, "\n");
            col++;
            if (col == cols)
                col = 0;
        }
    }
    else
        throw loops::exception(LOOPS_ERR_INTERNAL_UNKNOWN_PRINT_DESTINATION);
    sout.resize(strlen(sout.data()));
    return sout;
}

void program_printer::fprint_syntfunc(FILE* out, const Syntfunc& func)
{
    print_syntfunc(out, PRINT_TO_FILE, func);
}

std::string program_printer::sprint_syntfunc(const Syntfunc& func)
{
    return print_syntfunc(nullptr, PRINT_TO_STRING, func);
}

void col_num_printer(program_printer* printer, column_printer* /*colprinter*/, const Syntfunc& /*func*/, int row)
{
    loops_printf(printer, "%6d :", row);
    printer->close_printer_cell();
}

void col_delimeter_printer(program_printer* printer, column_printer* /*colprinter*/, const Syntfunc& /*func*/, int /*row*/)
{
    loops_printf(printer, ";");
    printer->close_printer_cell();
}

void col_ir_opname_printer(program_printer* printer, column_printer* /*colprinter*/, const Syntfunc& func, int row)
{
    const Syntop* op = func.program.data();
    op += row;
    cstring found_name = opstrings_ir_getter(op->opcode);
    if(found_name == nullptr)
    {
        if(suffixed_opnames.count(op->opcode) == 0)
        {
            switch(op->opcode)
            {
            case OP_JCC:
            {
                if (!(op->args_size == 2 && op->args[0].tag == Arg::IIMMEDIATE && op->args[1].tag == Arg::IIMMEDIATE))
                {//TODO(ch)[1]: Change OP_IVERSON, OP_JCC general format to format of Risc-V.
#if __LOOPS_ARCH == __LOOPS_RISCV
                    if (!(op->args_size == 4 && op->args[0].tag == Arg::IIMMEDIATE && op->args[1].tag == Arg::IREG && op->args[2].tag == Arg::IREG && op->args[3].tag == Arg::IIMMEDIATE))
#endif
                        throw loops::exception(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
                }
                found_name = cond_suffixes_getter((int)op->args[0].value);
                if(found_name == nullptr)
                    throw loops::exception(LOOPS_ERR_UNKNOWN_CONDITION);
                loops_printf(printer, "jmp_%s", found_name);
                break;
            }
            case OP_LABEL:
            {
                if (!(op->args_size == 1 && op->args[0].tag == Arg::IIMMEDIATE))
                    throw loops::exception(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
                loops_printf(printer, "__loops_label_%d:", op->args[0].value);
                break;
            }
            default:
                throw loops::exception(LOOPS_ERR_UNPRINTABLE_OPERATION);
            }; 
        }
        else 
        {
            suffixed_opname found_suffixed_name = suffixed_opnames.at(op->opcode);
            int i = 0;
            for(; i < found_suffixed_name.pieces_size; i++) 
            {
                one_name_one_suffix* onam_osuf= found_suffixed_name.pieces + i;
                char dummy[] = "";
                found_name = dummy;
                if(onam_osuf->suffix_type != SUFFIX_VOID)
                {
                    int argnum = onam_osuf->argnum;
                    if(onam_osuf->fracture_size > 0 && op->args_size >= onam_osuf->fracture_size) 
                        argnum++;
                    if(op->args_size <= argnum)
                        throw loops::exception(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
                    switch (onam_osuf->suffix_type)
                    {
                    case SUFFIX_CONDITION:
                        if(op->args[argnum].tag != Arg::IIMMEDIATE)
                            throw loops::exception(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
                        found_name = cond_suffixes_getter((int)op->args[argnum].value);
                        if(found_name == nullptr)
                            throw loops::exception(LOOPS_ERR_UNKNOWN_TYPE);
                        break;
                    case SUFFIX_ELEMTYPE:
                        if(op->args[argnum].tag != Arg::IREG && op->args[argnum].tag != Arg::VREG && op->args[argnum].tag != Arg::IIMMEDIATE)
                            throw loops::exception(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
                        found_name = type_suffixes_getter(op->args[argnum].elemtype);
                        if(found_name == nullptr)
                            throw loops::exception(LOOPS_ERR_UNKNOWN_TYPE);
                        break;
                    default: 
                        throw loops::exception(LOOPS_ERR_INCORRECT_ARGUMENT);
                    }
                }
                loops_printf(printer, "%s%s", onam_osuf->prefix, found_name);
            }
        }
    }
    else
        loops_printf(printer, "%s", found_name);
    printer->close_printer_cell();
}

void basic_arg_printer(program_printer* printer, const Arg* arg)
{
    switch (arg->tag)
    {
    case Arg::IREG:
        if(arg->idx == Syntfunc::RETREG)
        {
            loops_printf(printer, "iR");
            return;
        }
        else
        {
            loops_printf(printer, "i%d", arg->idx);
            return;
        }
    case Arg::ISPILLED:
    {
        loops_printf(printer, "s%d", arg->value);  //TODO(ch): Can we avoid spilled registers in IR?
        return;
    }
    case Arg::IIMMEDIATE:
    {
        loops_printf(printer, "%d", arg->value);
        return;
    }
    case Arg::VREG:
    {
        loops_printf(printer, "v%d", arg->idx);
        return;
    }
    default:
        throw loops::exception(LOOPS_ERR_UNKNOWN_ARGUMENT_TYPE);
    };
}

void col_ir_opargs_printer(program_printer* printer, column_printer* /*colprinter*/, const Syntfunc& func, int row)
{
    const Syntop* op = func.program.data();
    op += row;
    switch(op->opcode)
    {
    case OP_LABEL:
        break;
    case OP_JCC:
        if(op->args_size != 2  //TODO(ch)[1]: Change OP_IVERSON, OP_JCC general format to format of Risc-V.
#if __LOOPS_ARCH == __LOOPS_RISCV 
            && op->args_size != 4 
#endif
            )
            throw loops::exception(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
        if(op->args[op->args_size - 1].tag != Arg::IIMMEDIATE)
            throw loops::exception(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
        for(int anum = 1; anum < op->args_size - 1; anum++)
        {
            basic_arg_printer(printer, op->args + anum);
            loops_printf(printer, ", ");
        }
        loops_printf(printer, "__loops_label_%d", op->args[op->args_size - 1].value);
        break;
    case VOP_DEF:
        basic_arg_printer(printer, op->args);
        break;
    case OP_CALL:
        if (op->args_size < 2 || op->args[0].tag == Arg::VREG)
            throw loops::exception(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
        loops_printf(printer, "[");
        if(op->args[1].tag == Arg::IIMMEDIATE)
            print_address(printer, op->args[1].value);
        else
            basic_arg_printer(printer, op->args + 1);
        loops_printf(printer, "](");
        basic_arg_printer(printer, op->args);
        for(int anum = 2; anum < op->args_size; anum++)
        {
            loops_printf(printer, ", ");
            basic_arg_printer(printer, op->args + anum);
        }
        loops_printf(printer, ")");
        break;
    case OP_CALL_NORET:
        if (op->args_size < 1 || op->args[0].tag == Arg::VREG)
            throw loops::exception(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
        loops_printf(printer, "[");
        if(op->args[0].tag == Arg::IIMMEDIATE)
            print_address(printer, op->args[0].value);
        else
            basic_arg_printer(printer, op->args);
        loops_printf(printer, "](");
        for(int anum = 1; anum < op->args_size - 1; anum++)
        {
            basic_arg_printer(printer, op->args + anum);
            loops_printf(printer, ", ");
        }
        if(op->args_size > 1)
            basic_arg_printer(printer, op->args + op->args_size - 1);
        loops_printf(printer, ")");
        break;
    case OP_IVERSON://TODO(ch)[1]: Change OP_IVERSON, OP_JCC general format to format of Risc-V.
        basic_arg_printer(printer, op->args);
        if(op->args_size > 2)
        {
            loops_printf(printer, ", ");
            basic_arg_printer(printer, op->args + 2);
            loops_printf(printer, ", ");
            basic_arg_printer(printer, op->args + 3);
        }
        break;
    case OP_SELECT:
        basic_arg_printer(printer, op->args);
        loops_printf(printer, ", ");
        basic_arg_printer(printer, op->args + 2);
        loops_printf(printer, ", ");
        basic_arg_printer(printer, op->args + 3);
        break;
    default:
        for(int anum = 0; anum < op->args_size - 1; anum++)
        {
            basic_arg_printer(printer, op->args + anum);
            loops_printf(printer, ", ");
        }
        if(op->args_size > 0)
            basic_arg_printer(printer, op->args + op->args_size - 1);
        break;
    }
    printer->close_printer_cell();
}

program_printer_ptr program_printer::create_ir_printer(int columnflags)
{
    if(~(~columnflags | Func::PC_OPNUM | Func::PC_OP))
        throw loops::exception(LOOPS_ERR_UNKNOWN_FLAG);
    program_printer_ptr res;
    res.reset(new program_printer());
    int colprinters_size = 0; 
    colprinters_size += ((columnflags & Func::PC_OPNUM) > 0);
    colprinters_size += 2 * ((columnflags & Func::PC_OP) > 0);
    res->colprinters.reserve(colprinters_size);
    if(columnflags & Func::PC_OPNUM)
        res->colprinters.push_back(std::make_shared<column_printer>(&col_num_printer));

    if(columnflags & Func::PC_OP)
    {
        res->colprinters.push_back(std::make_shared<column_printer>(&col_ir_opname_printer));
        res->colprinters.push_back(std::make_shared<column_printer>(&col_ir_opargs_printer));
    }
    return res;
}

void col_opname_table_printer::print(program_printer* printer, column_printer* colprinter, const Syntfunc& func, int row)
{
    const Syntop* op = func.program.data();
    op += row;
    cstring found_name = ((col_opname_table_printer*)colprinter)->name_getter(op->opcode);
    if(found_name == nullptr)
        throw loops::exception(LOOPS_ERR_ELEMENT_NOT_FOUND);
    else
        loops_printf(printer, "%s", found_name);
    printer->close_printer_cell();
}

program_printer_ptr program_printer::create_assembly_printer(int columnflags, Backend* backend)
{
    if(~(~columnflags | Func::PC_OPNUM | Func::PC_OP | Func::PC_HEX))
        throw loops::exception(LOOPS_ERR_UNKNOWN_FLAG);
    program_printer_ptr res;
    res.reset(new program_printer());
    int colprinters_size = 0;
    colprinters_size += ((columnflags & Func::PC_OPNUM) > 0);
    colprinters_size += 2 * ((columnflags & Func::PC_OP) > 0);
    colprinters_size += 2 * ((columnflags & Func::PC_HEX) > 0);
    res->colprinters.reserve(colprinters_size);
    if(columnflags & Func::PC_OPNUM)
        res->colprinters.push_back(std::make_shared<column_printer>(&col_num_printer));
    res->backend = backend;

    if(columnflags & Func::PC_OP)
    {
        res->colprinters.push_back(backend->get_opname_printer());
        res->colprinters.push_back(backend->get_opargs_printer());
    }

    if(columnflags & Func::PC_HEX)
    {
        res->colprinters.push_back(std::make_shared<column_printer>(&col_delimeter_printer));
        res->colprinters.push_back(backend->get_hex_printer());
    }

    return res;
}

std::string IR_instruction2string(const Syntop& op)
{
    const int columns = Func::PC_OP;
    program_printer_ptr _printer = program_printer::create_ir_printer(columns);
    Syntfunc s2p;
    s2p.program.push_back(op);
    std::string result = _printer->sprint_syntfunc(s2p);
    result.erase(result.begin(), result.begin() + 3);
    if(result.size()) 
        result.resize(result.size()-1);
    return result;
}

std::string assembly_instruction2string(const Syntop& op, const Backend& backend)
{
    const int columns = Func::PC_OP;
    program_printer_ptr _printer = program_printer::create_assembly_printer(columns, const_cast<Backend*>(&backend));
    Syntfunc s2p;
    s2p.program.push_back(op);
    std::string result = _printer->sprint_syntfunc(s2p);
    result.erase(result.begin(), result.begin() + 3);
    if(result.size()) 
        result.resize(result.size()-1);
    return result;
}
};