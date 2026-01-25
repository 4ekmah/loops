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
//DUBUG: don't forget to work with namespaces correctly!

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


static inline loops_cstring opstrings_getter_(int opcode)
{
    switch (opcode)
    {
    /*   |         enum_id          |              string_id           |*/
    case (loops::OP_MOV             ) : return "mov"                   ;
    case (loops::OP_XCHG            ) : return "xchg"                  ;
    case (loops::OP_ADD             ) : return "add"                   ;
    case (loops::OP_SUB             ) : return "sub"                   ;
    case (loops::OP_MUL             ) : return "mul"                   ;
    case (loops::OP_DIV             ) : return "div"                   ;
    case (loops::OP_MOD             ) : return "mod"                   ;
    case (loops::OP_SHL             ) : return "shl"                   ;
    case (loops::OP_SHR             ) : return "shr"                   ;
    case (loops::OP_SAR             ) : return "sar"                   ;
    case (loops::OP_AND             ) : return "and"                   ;
    case (loops::OP_OR              ) : return "or"                    ;
    case (loops::OP_XOR             ) : return "xor"                   ;
    case (loops::OP_NOT             ) : return "not"                   ;
    case (loops::OP_NEG             ) : return "neg"                   ;
    case (loops::OP_CMP             ) : return "cmp"                   ;
    case (loops::OP_MIN             ) : return "min"                   ;
    case (loops::OP_MAX             ) : return "max"                   ;
    case (loops::OP_ABS             ) : return "abs"                   ;
    case (loops::OP_SIGN            ) : return "sign"                  ;
    case (loops::OP_SPILL           ) : return "spill"                 ;
    case (loops::OP_UNSPILL         ) : return "unspill"               ;
    case (loops::OP_GT              ) : return "gt"                    ;
    case (loops::OP_UGT             ) : return "ugt"                   ;
    case (loops::OP_GE              ) : return "ge"                    ;
    case (loops::OP_LT              ) : return "lt"                    ;
    case (loops::OP_LE              ) : return "le"                    ;
    case (loops::OP_ULE             ) : return "ule"                   ;
    case (loops::OP_NE              ) : return "ne"                    ;
    case (loops::OP_EQ              ) : return "eq"                    ;
    case (loops::OP_S               ) : return "s"                     ;
    case (loops::OP_NS              ) : return "ns"                    ;
    case (loops::OP_LOGICAL_AND     ) : return "log_and"               ;
    case (loops::OP_LOGICAL_OR      ) : return "log_or"                ;
    case (loops::OP_LOGICAL_NOT     ) : return "log_not"               ;
    case (loops::OP_JMP             ) : return "jmp"                   ;
    case (loops::OP_RET             ) : return "ret"                   ;
    case (loops::OP_CALL            ) : return "call"                  ;
    case (loops::OP_CALL_NORET      ) : return "call_noret"            ;
    case (loops::OP_STEM_CSTART     ) : return "annotation:stemcstart" ;
    case (loops::OP_IF_CSTART       ) : return "annotation:ifcstart"   ;
    case (loops::OP_ELIF_CSTART     ) : return "annotation:elif"       ;
    case (loops::OP_IF_CEND         ) : return "annotation:ifcend"     ;
    case (loops::OP_ELSE            ) : return "annotation:else"       ;
    case (loops::OP_ENDIF           ) : return "annotation:endif"      ;
    case (loops::OP_WHILE_CSTART    ) : return "annotation:whilecstart";
    case (loops::OP_WHILE_CEND      ) : return "annotation:whilecend"  ;
    case (loops::OP_ENDWHILE        ) : return "annotation:endwhile"   ;
    case (loops::OP_BREAK           ) : return "annotation:break"      ;
    case (loops::OP_CONTINUE        ) : return "annotation:continue"   ;
    case (loops::VOP_AND            ) : return "and"                   ;
    case (loops::VOP_OR             ) : return "or"                    ;
    case (loops::VOP_XOR            ) : return "xor"                   ;
    case (loops::VOP_NOT            ) : return "not"                   ;
    case (loops::OP_X86_ADC         ) : return "x86_adc"               ;
    case (loops::OP_X86_CQO         ) : return "x86_cqo"               ;
    case (loops::VOP_X86_VEXTRACT128) : return "x86_vextract128"       ;
    case (loops::VOP_X86_VINSERT128 ) : return "x86_vinsert128"        ;
    case (loops::VOP_X86_VPERM2I128 ) : return "x86_vperm2i128"        ;
    case (loops::VOP_X86_VPALIGNR   ) : return "x86_vpalignr"          ;
    case (loops::VOP_X86_VPSHUFD    ) : return "x86_vpshufd"           ;
    case (loops::VOP_X86_VPSADBW    ) : return "x86_vpsadbw"           ;
    case (loops::VOP_X86_VPHADDD    ) : return "x86_vphaddd"           ;
    case (loops::VOP_X86_VHADDPS    ) : return "x86_vhaddps"           ;
    case (loops::VOP_X86_VADDSS     ) : return "x86_vaddss"            ;
    case (loops::VOP_X86_VHADDPD    ) : return "x86_vhaddpd"           ;
    case (loops::VOP_X86_VADDSD     ) : return "x86_vaddsd"            ;
    case (loops::OP_ARM_CINC        ) : return "arm_cinc"              ;
    case (loops::OP_ARM_CNEG        ) : return "arm_cneg"              ;
    case (loops::OP_ARM_MOVK        ) : return "arm_movk"              ;
    case (loops::OP_ARM_LDP         ) : return "arm_ldp"               ;
    case (loops::OP_ARM_STP         ) : return "arm_stp"               ;
    case (loops::OP_RV_LUI          ) : return "rv_lui"                ;
    case (loops::OP_DEF             ) : return "def"                   ;
    };
    return nullptr;
}

static int opstrings_getter(int opcode, loops_cstring* found_name)
{
    *found_name = opstrings_getter_(opcode);
    return ((*found_name) == nullptr) ? LOOPS_ERR_ELEMENT_NOT_FOUND : LOOPS_ERR_SUCCESS; //DUBUG: not sure it's okay, but we need better scheme at all.
}

static inline loops_cstring cond_suffixes_getter_(int condcode)
{
    switch (condcode)
    {
/*       |   enum_id   |   string_id   |   */
    case (loops::OP_EQ ) : return "eq" ;
    case (loops::OP_NE ) : return "ne" ;
    case (loops::OP_GE ) : return "ge" ;
    case (loops::OP_LE ) : return "le" ;
    case (loops::OP_ULE) : return "ule";
    case (loops::OP_GT ) : return "gt" ;
    case (loops::OP_UGT) : return "ugt";
    case (loops::OP_LT ) : return "gt" ;
    case (loops::OP_S  ) : return "s"  ;
    case (loops::OP_NS ) : return "ns" ;
    };
    return nullptr;
}

static int cond_suffixes_getter(int condcode, loops_cstring* found_name)
{
    *found_name = cond_suffixes_getter_(condcode);
    return ((*found_name) == nullptr) ? LOOPS_ERR_ELEMENT_NOT_FOUND : LOOPS_ERR_SUCCESS;
}

static inline loops_cstring type_suffixes_getter_(int typecode)
{
    switch (typecode)
    {
/*       |    enum_id     |   string_id    |   */
    case (loops::TYPE_U8  ) : return "u8"  ;
    case (loops::TYPE_I8  ) : return "i8"  ;
    case (loops::TYPE_U16 ) : return "u16" ;
    case (loops::TYPE_I16 ) : return "i16" ;
    case (loops::TYPE_U32 ) : return "u32" ;
    case (loops::TYPE_I32 ) : return "i32" ;
    case (loops::TYPE_U64 ) : return "u64" ;
    case (loops::TYPE_I64 ) : return "i64" ;
    case (loops::TYPE_FP16) : return "fp16";
    case (loops::TYPE_BF16) : return "bf16";
    case (loops::TYPE_FP32) : return "fp32";
    case (loops::TYPE_FP64) : return "fp64";
    };
    return nullptr;
}

static int type_suffixes_getter(int typecode, loops_cstring* found_name)
{
    *found_name = type_suffixes_getter_(typecode);
    return ((*found_name) == nullptr) ? LOOPS_ERR_ELEMENT_NOT_FOUND : LOOPS_ERR_SUCCESS;
}

std::unordered_map<int, suffixed_opname> suffixed_opnames = 
{
/*  |         enum_id            |pieces_size|                pieces                    |      */
/*                                     |        prefix    |argnum|suffix_type|fracture_size|...*/

    {loops::OP_LOAD              , {1, {{"load."             , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::OP_STORE             , {1, {{"store."            , 1, SUFFIX_ELEMTYPE, 3}}}},
    {loops::OP_SELECT            , {1, {{"select_"           , 1, SUFFIX_CONDITION,0}}}},
    {loops::OP_IVERSON           , {1, {{"iverson_"          , 1, SUFFIX_CONDITION,0}}}},
    {loops::VOP_LOAD             , {1, {{"vld."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_STORE            , {1, {{"vst."              , 1, SUFFIX_ELEMTYPE, 3}}}},
    {loops::VOP_ADD              , {1, {{"add."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_SUB              , {1, {{"sub."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_MUL              , {1, {{"mul."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_DIV              , {1, {{"div."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_FMA              , {1, {{"fma."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_SAL              , {1, {{"sal."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_SHL              , {1, {{"shl."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_SAR              , {1, {{"sar."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_SHR              , {1, {{"shr."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_NEG              , {1, {{"neg."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_MIN              , {1, {{"min."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_MAX              , {1, {{"max."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_GT               , {1, {{"gt."               , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_GE               , {1, {{"ge."               , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_LT               , {1, {{"lt."               , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_LE               , {1, {{"le."               , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_NE               , {1, {{"ne."               , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_EQ               , {1, {{"eq."               , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_SELECT           , {1, {{"select."           , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_TRUNC            , {2, {{"trunc."            , 1, SUFFIX_ELEMTYPE, 0}, {"_"     , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_FLOOR            , {2, {{"floor."            , 1, SUFFIX_ELEMTYPE, 0}, {"_"     , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_CAST             , {2, {{"cast."             , 1, SUFFIX_ELEMTYPE, 0}, {"_"     , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_BROADCAST        , {1, {{"broadcast."        , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_CAST_LOW         , {3, {{"cast."             , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}, {".low" , 0, SUFFIX_VOID, 0}}}},
    {loops::VOP_CAST_HIGH        , {3, {{"cast."             , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}, {".high", 0, SUFFIX_VOID, 0}}}},
    {loops::VOP_SHRINK           , {2, {{"shrink."           , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_POPCOUNT         , {1, {{"popcount."         , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_REDUCE_MAX       , {1, {{"reduce.max."       , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_REDUCE_MIN       , {1, {{"reduce.min."       , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_REDUCE_SUM       , {1, {{"reduce.sum."       , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_REDUCE_WSUM      , {2, {{"reduce.wsum."      , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_ARM_LD1          , {1, {{"vld_lane."         , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_ARM_ST1          , {1, {{"vst_lane."         , 1, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_ARM_LD2          , {1, {{"vld_deinterleave2.", 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_EXT              , {1, {{"ext."              , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_ARM_SHRINK_LOW   , {3, {{"cast."             , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}, {".low" , 0, SUFFIX_VOID, 0}}}},
    {loops::VOP_ARM_SHRINK_HIGH  , {3, {{"cast."             , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}, {".high", 0, SUFFIX_VOID, 0}}}},
    {loops::VOP_GETLANE          , {1, {{"getlane."          , 1, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_SETLANE          , {1, {{"setlane."          , 0, SUFFIX_ELEMTYPE, 0}}}},
    {loops::VOP_DEF              , {1, {{"vdef."             , 0, SUFFIX_ELEMTYPE, 0}}}}, 
};

static int augment_buffer(std::list<std::vector<char>>& head, int buffer_size)
{
    if(head.size())
    {
        buffer_size = head.front().size();
    }
    head.emplace_back(std::vector<char>(buffer_size));
    return LOOPS_ERR_SUCCESS;
}

int loops_printf(program_printer* printer, const char *__restrict __format,...)
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
        if(printer->current_cell == 0)
            LOOPS_THROW(LOOPS_ERR_UNIMAGINARY_BIG_STRING);
        char* current_cell_start = printer->cells[printer->current_cell - 1] + printer->cell_sizes[printer->current_cell - 1] + 1;
        int current_cell_size = (int)(buffers_tail.data() + printer->current_offset - current_cell_start);
        if(current_cell_size < 0) 
            LOOPS_THROW(LOOPS_ERR_POINTER_ARITHMETIC_ERROR);
        if(current_cell_size + written >= (int)buffers_tail.size())
            LOOPS_THROW(LOOPS_ERR_UNIMAGINARY_BIG_STRING);
        LOOPS_CALL_THROW(augment_buffer(printer->buffers, 0));
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
            LOOPS_THROW(LOOPS_ERR_UNIMAGINARY_BIG_STRING);
    }
    printer->current_offset += written;
    va_end ( var_args );
    return LOOPS_ERR_SUCCESS;    
}

int new_print_address(program_printer* printer, int64_t addr)
{
    static char hexsymb[] = "0123456789ABCDEF";
    char* bytes = (char*)(&addr);
    LOOPS_CALL_THROW(loops_printf(printer, "0x")); 
    for (int i = 0; i < 8; i++)
        LOOPS_CALL_THROW(loops_printf(printer, "%c%c", hexsymb[(bytes[7 - i] & 0xF0) >> 4], hexsymb[bytes[7 - i] & 0x0F]));
    return LOOPS_ERR_SUCCESS;
}

int close_printer_cell(program_printer* printer)
{
    std::vector<char>& buffers_tail = printer->buffers.back();
    int buffers_tail_size = (int)buffers_tail.size();
    char* buffers_tail_data = buffers_tail.data();
    char* newcell = buffers_tail_data;
    bool newbuffer = printer->current_cell == 0; 
    buffers_tail_data[printer->current_offset] = 0;
    if (!newbuffer)
    {
        char* prevcell = printer->cells[printer->current_cell - 1];
        if (prevcell < buffers_tail_data || /*Buffer augmentation happened*/
            prevcell >= (buffers_tail_data + buffers_tail_size))
            newbuffer = true;
        else
            newcell = prevcell + printer->cell_sizes[printer->current_cell - 1] + 1;
    }

    int len = (newcell >= buffers_tail_data + buffers_tail_size) ? 0 : (int)strlen(newcell);
    printer->cell_sizes[printer->current_cell] = len;
    if(len == 0 && !newbuffer)
        newcell--; //Empty strings doesn't use space
    else 
        printer->current_offset++;
    printer->cells[printer->current_cell] = newcell;
    printer->current_cell++;
    return LOOPS_ERR_SUCCESS;
}

static int col_num_printer(program_printer* printer, column_printer* /*colprinter*/, const loops::Syntfunc& /*func*/, int row)
{
    LOOPS_CALL_THROW(loops_printf(printer, "%6d :", row));
    LOOPS_CALL_THROW(close_printer_cell(printer));
    return LOOPS_ERR_SUCCESS;
}

static int col_delimeter_printer(program_printer* printer, column_printer* /*colprinter*/, const loops::Syntfunc& /*func*/, int /*row*/)
{
    LOOPS_CALL_THROW(loops_printf(printer, ";"));
    LOOPS_CALL_THROW(close_printer_cell(printer));
    return LOOPS_ERR_SUCCESS;
}

static int col_ir_opname_printer(program_printer* printer, column_printer* /*colprinter*/, const loops::Syntfunc& func, int row)
{
    int err;
    loops_cstring found_name = NULL;
    const loops::Syntop* op = func.program.data();
    op += row;
    err = opstrings_getter(op->opcode, &found_name);
    if(err == LOOPS_ERR_ELEMENT_NOT_FOUND)
    {
        if(suffixed_opnames.count(op->opcode) == 0)
        {
            switch(op->opcode)
            {
            case loops::OP_JCC:
            {
                if (!(op->args_size == 2 && op->args[0].tag == loops::Arg::IIMMEDIATE && op->args[1].tag == loops::Arg::IIMMEDIATE))
                {//TODO(ch)[1]: Change OP_IVERSON, OP_JCC general format to format of Risc-V.
#if __LOOPS_ARCH == __LOOPS_RISCV
                    if (!(op->args_size == 4 && op->args[0].tag == loops::Arg::IIMMEDIATE && op->args[1].tag == loops::Arg::IREG && op->args[2].tag == loops::Arg::IREG && op->args[3].tag == loops::Arg::IIMMEDIATE))
#endif
                        LOOPS_THROW(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
                }
                err = cond_suffixes_getter((int)op->args[0].value, &found_name);
                if(err == LOOPS_ERR_ELEMENT_NOT_FOUND)
                    LOOPS_THROW(LOOPS_ERR_UNKNOWN_CONDITION);
                else if(err != LOOPS_ERR_SUCCESS)
                    LOOPS_THROW(err);
                LOOPS_CALL_THROW(loops_printf(printer, "jmp_%s", found_name));
                break;
            }
            case loops::OP_LABEL:
            {
                if (!(op->args_size == 1 && op->args[0].tag == loops::Arg::IIMMEDIATE))
                    LOOPS_THROW(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
                LOOPS_CALL_THROW(loops_printf(printer, "__loops_label_%d:", op->args[0].value));
                break;
            }
            default:
                LOOPS_THROW(LOOPS_ERR_UNPRINTABLE_OPERATION);
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
                        LOOPS_THROW(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
                    switch (onam_osuf->suffix_type)
                    {
                    case SUFFIX_CONDITION:
                        if(op->args[argnum].tag != loops::Arg::IIMMEDIATE)
                            LOOPS_THROW(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
                        err = cond_suffixes_getter((int)op->args[argnum].value, &found_name);
                        if(err == LOOPS_ERR_ELEMENT_NOT_FOUND)
                            LOOPS_THROW(LOOPS_ERR_UNKNOWN_TYPE);
                        else if(err != LOOPS_ERR_SUCCESS)
                            LOOPS_THROW(err);
                        break;
                    case SUFFIX_ELEMTYPE:
                        if(op->args[argnum].tag != loops::Arg::IREG && op->args[argnum].tag != loops::Arg::VREG && op->args[argnum].tag != loops::Arg::IIMMEDIATE)
                            LOOPS_THROW(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
                        err = type_suffixes_getter(op->args[argnum].elemtype, &found_name);
                        if(err == LOOPS_ERR_ELEMENT_NOT_FOUND)
                            LOOPS_THROW(LOOPS_ERR_UNKNOWN_TYPE);
                        else if(err != LOOPS_ERR_SUCCESS)
                            LOOPS_THROW(err);
                        break;
                    default: 
                        LOOPS_THROW(LOOPS_ERR_INCORRECT_ARGUMENT);
                    }
                }
                LOOPS_CALL_THROW(loops_printf(printer, "%s%s", onam_osuf->prefix, found_name));
            }
        }
    }
    else
        LOOPS_CALL_THROW(loops_printf(printer, "%s", found_name));
    LOOPS_CALL_THROW(close_printer_cell(printer));
    return LOOPS_ERR_SUCCESS;
}

static int basic_arg_printer(program_printer* printer, const loops::Arg* arg)
{
    switch (arg->tag)
    {
    case loops::Arg::IREG:
        if(arg->idx == loops::Syntfunc::RETREG)
            return loops_printf(printer, "iR");
        else
            return loops_printf(printer, "i%d", arg->idx);
    case loops::Arg::ISPILLED: return loops_printf(printer, "s%d", arg->value);  //TODO(ch): Can we avoid spilled registers in IR?
    case loops::Arg::IIMMEDIATE: return loops_printf(printer, "%d", arg->value);
    case loops::Arg::VREG: return loops_printf(printer, "v%d", arg->idx);
    default:
        return LOOPS_ERR_UNKNOWN_ARGUMENT_TYPE;
    };
}

static int col_ir_opargs_printer(program_printer* printer, column_printer* /*colprinter*/, const loops::Syntfunc& func, int row)
{
    const loops::Syntop* op = func.program.data();
    op += row;
    switch(op->opcode)
    {
    case loops::OP_LABEL:
        break;
    case loops::OP_JCC:
        if(op->args_size != 2  //TODO(ch)[1]: Change OP_IVERSON, OP_JCC general format to format of Risc-V.
#if __LOOPS_ARCH == __LOOPS_RISCV 
            && op->args_size != 4 
#endif
            )
            LOOPS_THROW(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
        if(op->args[op->args_size - 1].tag != loops::Arg::IIMMEDIATE)
            LOOPS_THROW(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
        for(int anum = 1; anum < op->args_size - 1; anum++)
        {
            LOOPS_CALL_THROW(basic_arg_printer(printer, op->args + anum));
            LOOPS_CALL_THROW(loops_printf(printer, ", "));
        }
        LOOPS_CALL_THROW(loops_printf(printer, "__loops_label_%d", op->args[op->args_size - 1].value));
        break;
    case loops::VOP_DEF:
        LOOPS_CALL_THROW(basic_arg_printer(printer, op->args));
        break;
    case loops::OP_CALL:
        if (op->args_size < 2 || op->args[0].tag == loops::Arg::VREG)
            LOOPS_THROW(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
        LOOPS_CALL_THROW(loops_printf(printer, "["));
        if(op->args[1].tag == loops::Arg::IIMMEDIATE)
            LOOPS_CALL_THROW(new_print_address(printer, op->args[1].value));
        else
            LOOPS_CALL_THROW(basic_arg_printer(printer, op->args + 1));
        LOOPS_CALL_THROW(loops_printf(printer, "]("));
        LOOPS_CALL_THROW(basic_arg_printer(printer, op->args));
        for(int anum = 2; anum < op->args_size; anum++)
        {
            LOOPS_CALL_THROW(loops_printf(printer, ", "));
            LOOPS_CALL_THROW(basic_arg_printer(printer, op->args + anum));
        }
        LOOPS_CALL_THROW(loops_printf(printer, ")"));
        break;
    case loops::OP_CALL_NORET:
        if (op->args_size < 1 || op->args[0].tag == loops::Arg::VREG)
            LOOPS_THROW(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
        LOOPS_CALL_THROW(loops_printf(printer, "["));
        if(op->args[0].tag == loops::Arg::IIMMEDIATE)
            LOOPS_CALL_THROW(new_print_address(printer, op->args[0].value));
        else
            LOOPS_CALL_THROW(basic_arg_printer(printer, op->args));
        LOOPS_CALL_THROW(loops_printf(printer, "]("));
        for(int anum = 1; anum < op->args_size - 1; anum++)
        {
            LOOPS_CALL_THROW(basic_arg_printer(printer, op->args + anum));
            LOOPS_CALL_THROW(loops_printf(printer, ", "));
        }
        if(op->args_size > 1)
            LOOPS_CALL_THROW(basic_arg_printer(printer, op->args + op->args_size - 1));
        LOOPS_CALL_THROW(loops_printf(printer, ")"));
        break;
    case loops::OP_IVERSON://TODO(ch)[1]: Change OP_IVERSON, OP_JCC general format to format of Risc-V.
        LOOPS_CALL_THROW(basic_arg_printer(printer, op->args));
        if(op->args_size > 2)
        {
            LOOPS_CALL_THROW(loops_printf(printer, ", "));
            LOOPS_CALL_THROW(basic_arg_printer(printer, op->args + 2));
            LOOPS_CALL_THROW(loops_printf(printer, ", "));
            LOOPS_CALL_THROW(basic_arg_printer(printer, op->args + 3));
        }
        break;
    case loops::OP_SELECT:
        LOOPS_CALL_THROW(basic_arg_printer(printer, op->args));
        LOOPS_CALL_THROW(loops_printf(printer, ", "));
        LOOPS_CALL_THROW(basic_arg_printer(printer, op->args + 2));
        LOOPS_CALL_THROW(loops_printf(printer, ", "));
        LOOPS_CALL_THROW(basic_arg_printer(printer, op->args + 3));
        break;
    default:
        for(int anum = 0; anum < op->args_size - 1; anum++)
        {
            LOOPS_CALL_THROW(basic_arg_printer(printer, op->args + anum));
            LOOPS_CALL_THROW(loops_printf(printer, ", "));
        }
        if(op->args_size > 0)
            LOOPS_CALL_THROW(basic_arg_printer(printer, op->args + op->args_size - 1));
        break;
    }
    LOOPS_CALL_THROW(close_printer_cell(printer));
    return LOOPS_ERR_SUCCESS;
}

int create_ir_printer(int columnflags, program_printer** res)
{
    if(res == NULL) 
        LOOPS_THROW(LOOPS_ERR_NULL_POINTER);
    if(~(~columnflags | loops::Func::PC_OPNUM | loops::Func::PC_OP))
        LOOPS_THROW(LOOPS_ERR_UNKNOWN_FLAG);

    *res = new program_printer();
    if(*res == NULL)
        LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
    int colprinters_size = 0; 
    colprinters_size += ((columnflags & loops::Func::PC_OPNUM) > 0);
    colprinters_size += 2 * ((columnflags & loops::Func::PC_OP) > 0);
    (*res)->colprinters.reserve(colprinters_size);
    if(columnflags & loops::Func::PC_OPNUM)
        (*res)->colprinters.push_back(std::make_shared<column_printer>(&col_num_printer));

    if(columnflags & loops::Func::PC_OP)
    {
        (*res)->colprinters.push_back(std::make_shared<column_printer>(&col_ir_opname_printer));
        (*res)->colprinters.push_back(std::make_shared<column_printer>(&col_ir_opargs_printer));
    }
    return LOOPS_ERR_SUCCESS;
}

int col_opname_table_printer(program_printer* printer, column_printer* colprinter, const loops::Syntfunc& func, int row)
{
    int err;
    loops_cstring found_name = NULL;
    const loops::Syntop* op = func.program.data();
    op += row;
    err = ((table_opname_getter)colprinter->auxdata)(op->opcode, &found_name);
    if(err != LOOPS_ERR_SUCCESS )
        LOOPS_THROW(err);
    else if(err == LOOPS_ERR_SUCCESS)
        LOOPS_CALL_THROW(loops_printf(printer, "%s", found_name));
    LOOPS_CALL_THROW(close_printer_cell(printer));
    return LOOPS_ERR_SUCCESS;
}

int create_assembly_printer(int columnflags, loops::Backend* backend, program_printer** res)
{
    if(res == NULL) 
        LOOPS_THROW(LOOPS_ERR_NULL_POINTER);
    if(~(~columnflags | loops::Func::PC_OPNUM | loops::Func::PC_OP | loops::Func::PC_HEX))
        LOOPS_THROW(LOOPS_ERR_UNKNOWN_FLAG);

    *res = new program_printer();
    if(*res == NULL) 
        LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
    int colprinters_size = 0;
    colprinters_size += ((columnflags & loops::Func::PC_OPNUM) > 0);
    colprinters_size += 2 * ((columnflags & loops::Func::PC_OP) > 0);
    colprinters_size += 2 * ((columnflags & loops::Func::PC_HEX) > 0);
    (*res)->colprinters.reserve(colprinters_size);
    if(columnflags & loops::Func::PC_OPNUM)
        (*res)->colprinters.push_back(std::make_shared<column_printer>(&col_num_printer));
    (*res)->backend = backend;

    if(columnflags & loops::Func::PC_OP)
    {
        (*res)->colprinters.push_back(backend->get_opname_printer());
        (*res)->colprinters.push_back(backend->get_opargs_printer());
    }

    if(columnflags & loops::Func::PC_HEX)
    {
        (*res)->colprinters.push_back(std::make_shared<column_printer>(&col_delimeter_printer));
        (*res)->colprinters.push_back(backend->get_hex_printer());
    }

    return LOOPS_ERR_SUCCESS;
}

void free_printer(program_printer* tofree)
{
    int prnum = 0;
    for (; prnum < (int)tofree->colprinters.size(); prnum++)
        if (tofree->colprinters[prnum]->free_func != NULL)
            tofree->colprinters[prnum]->free_func(tofree->colprinters[prnum].get());
    delete tofree;
}

enum {PRINT_TO_FILE, PRINT_TO_STRING};
static int print_syntfunc(program_printer* printer, FILE* fout, char** sout, int outtype, const loops::Syntfunc& func)
{
    int params_size = (int)func.params.size();
    const loops::Arg* params = func.params.data();

    int err = 0;
    int cells = 0;
    static int MAX_LINE_SIZE = 82; //taken from statistics
    int cols = (int)printer->colprinters.size();
    int rows = (int)func.program.size();
    int row;
    int col;

    LOOPS_CALL_THROW(augment_buffer(printer->buffers, MAX_LINE_SIZE * rows));

    printer->cells = NULL; 
    printer->cell_sizes = NULL; 
    int* max_widthes = new int[cols];
    char** printtasks = new char*[cols];
    char* printtasksbuf = new char[cols * 10];
    printer->cells = new char*[cols*rows];
    printer->cell_sizes = new int[cols*rows];
    memset(max_widthes, 0, cols * sizeof(int));
    memset(printtasksbuf, 0, cols * 10);

    if(printer->cells == NULL || printer->cell_sizes == NULL || max_widthes == NULL || printtasks == NULL || printtasksbuf == NULL)
    {
        err = LOOPS_ERR_OUT_OF_MEMORY;
        goto print_syntfunc_end;
    }
    printer->current_cell = 0;
    printer->current_offset = 0;

    for (row = 0; row < rows; row++)
    {
        for (col = 0; col < cols; col++)
        {
            err = printer->colprinters[col]->func(printer, printer->colprinters[col].get(), func, row);
            if (err != 0)
            {
                fout = stderr;
                fprintf(fout, "Loops: printing error. Currently printed:\n");
                outtype = PRINT_TO_FILE;
                break;
            }
            int collen = printer->cell_sizes[row * cols + col] + 1;
            max_widthes[col] = (max_widthes[col] < collen ? collen : max_widthes[col]);
            cells++;
        }
        if (err != 0)
            break;
    }

    for(col = 0; col < cols; col++)
    {
        printtasks[col] = printtasksbuf + 10 * col; 
        snprintf(printtasks[col], 10, "%%-%ds", max_widthes[col]); 
    }

    if (outtype == PRINT_TO_FILE)
    {
        int parnum;
        int cell;
        fprintf(fout, "%s(", func.name.c_str());
        for (parnum = 0; parnum < params_size - 1; parnum++)
            fprintf(fout, "i%d, ", (params + parnum)->idx);
        if (params_size)
            fprintf(fout, "i%d", (params + params_size - 1)->idx);
        fprintf(fout, ")\n");
        for(col = 0, cell = 0; cell < cells; cell++)
        {
            fprintf(fout, printtasks[col], printer->cells[cell]);
            if(col == cols - 1 || cell == cells - 1)
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
        int parnum;
        int cell;
        for (parnum = 0; parnum < params_size; parnum++)
        {
            assert((params + parnum)->idx < 100);
            bufferleft += ((params + parnum)->idx > 10 ? 2 : 1) + 3;
        }
        {
            int lensize = 2;
            for (col = 0; col < cols; col++)
                lensize += max_widthes[col];
            bufferleft += lensize * rows;
        }
        *sout = new char[bufferleft];
        if (*sout == NULL)
            LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
        char* currentout = *sout;

#define PRINT_SYNTFUNC_SPRINT(...)                                                                \
do {                                                                                              \
    int __print_syntfunc_sprint_written__ = snprintf(currentout, bufferleft, __VA_ARGS__);        \
    if (__print_syntfunc_sprint_written__ < 0 || __print_syntfunc_sprint_written__ >= bufferleft) \
    {                                                                                             \
        delete(*sout);                                                                            \
        goto print_syntfunc_end;                                                                  \
    }                                                                                             \
    currentout += __print_syntfunc_sprint_written__;                                              \
    bufferleft -= __print_syntfunc_sprint_written__;                                              \
} while (0)

        //Write header:
        PRINT_SYNTFUNC_SPRINT("%s(", func.name.c_str());
        for (parnum = 0; parnum < params_size - 1; parnum++)
            PRINT_SYNTFUNC_SPRINT("i%d, ", (params + parnum)->idx);
        if (params_size)
            PRINT_SYNTFUNC_SPRINT("i%d", (params + params_size - 1)->idx);
        PRINT_SYNTFUNC_SPRINT(")\n");
        //Write instructions:
        for (col = 0, cell = 0; cell < cells; cell++)
        {
            PRINT_SYNTFUNC_SPRINT(printtasks[col], printer->cells[cell]);
            if (col == cols - 1 || cell == cells - 1)
                PRINT_SYNTFUNC_SPRINT("\n");
            col++;
            if (col == cols)
                col = 0;
        }
#undef PRINT_SYNTFUNC_SPRINT
    }
    else
        err = LOOPS_ERR_INTERNAL_UNKNOWN_PRINT_DESTINATION;
print_syntfunc_end:
    delete [] max_widthes;
    delete [] printtasks;
    delete [] printtasksbuf;
    delete printer->cell_sizes;
    delete printer->cells;
    return err;
}

int fprint_syntfunc(program_printer* printer, FILE* out, const loops::Syntfunc& func)
{
    return print_syntfunc(printer, out, NULL, PRINT_TO_FILE, func);
}

int sprint_syntfunc(program_printer* printer, char** out, const loops::Syntfunc& func)
{
    return print_syntfunc(printer, NULL, out, PRINT_TO_STRING, func);
}

std::string IR_instruction2string(const loops::Syntop& op)
{
    const int columns = loops::Func::PC_OP;
    program_printer* _printer;
    Assert(create_ir_printer(columns, &_printer) == 0);
    loops::Syntfunc s2p;
    s2p.program.push_back(op);
    char* printed_str;
    int err = sprint_syntfunc(_printer, &printed_str, s2p);
    if(err != LOOPS_ERR_SUCCESS)
        throw std::runtime_error(get_errstring(err));
    free_printer(_printer);
    std::string result = printed_str + 3;
    if(result.size()) 
        result.resize(result.size()-1);
    delete [] printed_str;
    return result;
}

std::string assembly_instruction2string(const loops::Syntop& op, const loops::Backend& backend)
{
    const int columns = loops::Func::PC_OP;
    program_printer* _printer;
    Assert(create_assembly_printer(columns, const_cast<loops::Backend*>(&backend), &_printer) == 0);
    loops::Syntfunc s2p;
    s2p.program.push_back(op);
    char* printed_str;
    int err = sprint_syntfunc(_printer, &printed_str, s2p);
    if(err != LOOPS_ERR_SUCCESS)
        throw std::runtime_error(get_errstring(err));
    free_printer(_printer);
    std::string result = printed_str + 3;
    if(result.size()) 
        result.resize(result.size()-1);
    delete [] printed_str;
    return result;
}
