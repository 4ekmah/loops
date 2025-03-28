/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "backend.hpp"
#include "printer.hpp"
#include <stdarg.h>
#include <cstdio>
#include <sstream>
#include <iomanip>

LOOPS_LIST_DEFINE(loops_span_char)
LOOPS_SPAN_DEFINE(loops_Syntop)
LOOPS_SPAN_DEFINE(loops_Arg)
LOOPS_SPAN_DEFINE(column_printer)

enum {SUFFIX_ELEMTYPE, SUFFIX_CONDITION, SUFFIX_VOID};
typedef struct one_name_one_suffix
{
    char prefix[30];
    int argnum;
    int suffix_type;
    int fracture_size; /*If op->arg_size >= fracture_size, argnum converts to (argnum + 1). Ignored if fracture_size == 0*/
} one_name_one_suffix;

typedef struct suffixed_opname
{
    int pieces_size;
    one_name_one_suffix pieces[3];
} suffixed_opname;

LOOPS_HASHMAP_STATIC(int, loops_cstring) opstrings_[] = 
{
                  /*  |       enum_id        |         string_id       |    */
    LOOPS_HASHMAP_ELEM(loops::OP_MOV         , "mov"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_XCHG        , "xchg"                  ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_ADD         , "add"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_SUB         , "sub"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_MUL         , "mul"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_DIV         , "div"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_MOD         , "mod"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_SHL         , "shl"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_SHR         , "shr"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_SAR         , "sar"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_AND         , "and"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_OR          , "or"                    ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_XOR         , "xor"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_NOT         , "not"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_NEG         , "neg"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_CMP         , "cmp"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_MIN         , "min"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_MAX         , "max"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_ABS         , "abs"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_SIGN        , "sign"                  ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_SPILL       , "spill"                 ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_UNSPILL     , "unspill"               ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_GT          , "gt"                    ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_UGT         , "ugt"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_GE          , "ge"                    ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_LT          , "lt"                    ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_LE          , "le"                    ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_ULE         , "ule"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_NE          , "ne"                    ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_EQ          , "eq"                    ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_S           , "s"                     ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_NS          , "ns"                    ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_LOGICAL_AND , "log_and"               ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_LOGICAL_OR  , "log_or"                ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_LOGICAL_NOT , "log_not"               ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_JMP         , "jmp"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_RET         , "ret"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_CALL        , "call"                  ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_CALL_NORET  , "call_noret"            ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_STEM_CSTART , "annotation:stemcstart" ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_IF_CSTART   , "annotation:ifcstart"   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_ELIF_CSTART , "annotation:elif"       ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_IF_CEND     , "annotation:ifcend"     ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_ELSE        , "annotation:else"       ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_ENDIF       , "annotation:endif"      ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_WHILE_CSTART, "annotation:whilecstart") ,
    LOOPS_HASHMAP_ELEM(loops::OP_WHILE_CEND  , "annotation:whilecend"  ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_ENDWHILE    , "annotation:endwhile"   ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_BREAK       , "annotation:break"      ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_CONTINUE    , "annotation:continue"   ) ,
    LOOPS_HASHMAP_ELEM(loops::VOP_AND        , "and"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::VOP_OR         , "or"                    ) ,
    LOOPS_HASHMAP_ELEM(loops::VOP_XOR        , "xor"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::VOP_NOT        , "not"                   ) ,
    LOOPS_HASHMAP_ELEM(loops::VOP_SELECT     , "select"                ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_X86_ADC     , "x86_adc"               ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_X86_CQO     , "x86_cqo"               ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_ARM_CINC    , "arm_cinc"              ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_ARM_CNEG    , "arm_cneg"              ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_ARM_MOVK    , "arm_movk"              ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_ARM_LDP     , "arm_ldp"               ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_ARM_STP     , "arm_stp"               ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_RV_LUI      , "rv_lui"                ) ,
    LOOPS_HASHMAP_ELEM(loops::OP_DEF         , "def"                   ) ,
};

LOOPS_HASHMAP_STATIC(int, loops_cstring) cond_suffixes_[] =
{
/*  |   enum_id   | string_id|   */
    LOOPS_HASHMAP_ELEM(loops::OP_EQ ,     "eq" ),
    LOOPS_HASHMAP_ELEM(loops::OP_NE ,     "ne" ),
    LOOPS_HASHMAP_ELEM(loops::OP_GE ,     "ge" ),
    LOOPS_HASHMAP_ELEM(loops::OP_LE ,     "le" ),
    LOOPS_HASHMAP_ELEM(loops::OP_ULE,     "ule"),
    LOOPS_HASHMAP_ELEM(loops::OP_GT ,     "gt" ),
    LOOPS_HASHMAP_ELEM(loops::OP_UGT,     "ugt"),
    LOOPS_HASHMAP_ELEM(loops::OP_LT ,     "gt" ),
    LOOPS_HASHMAP_ELEM(loops::OP_S  ,     "s"  ),
    LOOPS_HASHMAP_ELEM(loops::OP_NS ,     "ns" ),
};

LOOPS_HASHMAP_STATIC(int, loops_cstring) type_suffixes_[] =
{
                  /*  |    enum_id     |string_id|  */
    LOOPS_HASHMAP_ELEM(loops::TYPE_U8  , "u8"  ),
    LOOPS_HASHMAP_ELEM(loops::TYPE_I8  , "i8"  ),
    LOOPS_HASHMAP_ELEM(loops::TYPE_U16 , "u16" ),
    LOOPS_HASHMAP_ELEM(loops::TYPE_I16 , "i16" ),
    LOOPS_HASHMAP_ELEM(loops::TYPE_U32 , "u32" ),
    LOOPS_HASHMAP_ELEM(loops::TYPE_I32 , "i32" ),
    LOOPS_HASHMAP_ELEM(loops::TYPE_U64 , "u64" ),
    LOOPS_HASHMAP_ELEM(loops::TYPE_I64 , "i64" ),
    LOOPS_HASHMAP_ELEM(loops::TYPE_FP16, "fp16"),
    LOOPS_HASHMAP_ELEM(loops::TYPE_BF16, "bf16"),
    LOOPS_HASHMAP_ELEM(loops::TYPE_FP32, "fp32"),
    LOOPS_HASHMAP_ELEM(loops::TYPE_FP64, "fp64"),
};

LOOPS_HASHMAP_DECLARE(int, suffixed_opname);
LOOPS_HASHMAP_DEFINE(int, suffixed_opname)
LOOPS_HASHMAP_STATIC(int, suffixed_opname) suffixed_opnames_[] = 
{
/*  |         enum_id        |pieces_size|                pieces                    |          */
/*                                     |        prefix    |argnum|suffix_type|fracture_size|...*/
    LOOPS_HASHMAP_ELEM(loops::OP_LOAD              , {1, {{"load."             , 0, SUFFIX_ELEMTYPE, 0}}}),	
    LOOPS_HASHMAP_ELEM(loops::OP_STORE             , {1, {{"store."            , 1, SUFFIX_ELEMTYPE, 3}}}),
    LOOPS_HASHMAP_ELEM(loops::OP_SELECT            , {1, {{"select_"           , 1, SUFFIX_CONDITION,0}}}),
    LOOPS_HASHMAP_ELEM(loops::OP_IVERSON           , {1, {{"iverson_"          , 1, SUFFIX_CONDITION,0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_LOAD             , {1, {{"vld."              , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_STORE            , {1, {{"vst."              , 1, SUFFIX_ELEMTYPE, 3}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_ADD              , {1, {{"add."              , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_SUB              , {1, {{"sub."              , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_MUL              , {1, {{"mul."              , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_DIV              , {1, {{"div."              , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_FMA              , {1, {{"fma."              , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_SAL              , {1, {{"sal."              , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_SHL              , {1, {{"shl."              , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_SAR              , {1, {{"sar."              , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_SHR              , {1, {{"shr."              , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_NEG              , {1, {{"neg."              , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_MIN              , {1, {{"min."              , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_MAX              , {1, {{"max."              , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_GT               , {1, {{"gt."               , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_GE               , {1, {{"ge."               , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_LT               , {1, {{"lt."               , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_LE               , {1, {{"le."               , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_NE               , {1, {{"ne."               , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_EQ               , {1, {{"eq."               , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_TRUNC            , {2, {{"trunc."            , 1, SUFFIX_ELEMTYPE, 0}, {"_"     , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_FLOOR            , {2, {{"floor."            , 1, SUFFIX_ELEMTYPE, 0}, {"_"     , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_CAST             , {2, {{"cast."             , 1, SUFFIX_ELEMTYPE, 0}, {"_"     , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_BROADCAST        , {1, {{"broadcast."        , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_CAST_LOW         , {3, {{"cast."             , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}, {".low" , 0, SUFFIX_VOID, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_CAST_HIGH        , {3, {{"cast."             , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}, {".high", 0, SUFFIX_VOID, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_SHRINK           , {2, {{"shrink."           , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_POPCOUNT         , {1, {{"popcount."         , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_REDUCE_MAX       , {1, {{"reduce.max."       , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_REDUCE_MIN       , {1, {{"reduce.min."       , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_REDUCE_SUM       , {1, {{"reduce.sum."       , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_REDUCE_WSUM      , {2, {{"reduce.wmax"       , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_ARM_LD1          , {1, {{"vld_lane."         , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_ARM_ST1          , {1, {{"vst_lane."         , 1, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_ARM_LD2          , {1, {{"vld_deinterleave2.", 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_ARM_EXT          , {1, {{"ext."              , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_ARM_SHRINK_LOW   , {3, {{"cast."             , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}, {".low" , 0, SUFFIX_VOID, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_ARM_SHRINK_HIGH  , {3, {{"cast."             , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}, {".high", 0, SUFFIX_VOID, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_GETLANE          , {1, {{"getlane."          , 1, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_SETLANE          , {1, {{"getlane."          , 0, SUFFIX_ELEMTYPE, 0}}}),
    LOOPS_HASHMAP_ELEM(loops::VOP_DEF              , {1, {{"vdef."             , 0, SUFFIX_ELEMTYPE, 0}}}),
};

static LOOPS_HASHMAP(int, loops_cstring) opstrings = NULL;
static LOOPS_HASHMAP(int, loops_cstring) cond_suffixes = NULL;
static LOOPS_HASHMAP(int, loops_cstring) type_suffixes = NULL;
static LOOPS_HASHMAP(int, suffixed_opname) suffixed_opnames = NULL;

int printer_h_initialize()
{
    LOOPS_CALL_THROW(loops_hashmap_construct_static(&opstrings, opstrings_, sizeof(opstrings_) / sizeof(opstrings_[0])));
    LOOPS_CALL_THROW(loops_hashmap_construct_static(&cond_suffixes, cond_suffixes_, sizeof(cond_suffixes_) / sizeof(cond_suffixes_[0])));
    LOOPS_CALL_THROW(loops_hashmap_construct_static(&type_suffixes, type_suffixes_, sizeof(type_suffixes_) / sizeof(type_suffixes_[0])));
    LOOPS_CALL_THROW(loops_hashmap_construct_static(&suffixed_opnames, suffixed_opnames_, sizeof(suffixed_opnames_) / sizeof(suffixed_opnames_[0])));
    return LOOPS_ERR_SUCCESS;
}

void printer_h_deinitialize()
{
    loops_hashmap_destruct(opstrings);
    loops_hashmap_destruct(cond_suffixes);
    loops_hashmap_destruct(type_suffixes);
    loops_hashmap_destruct(suffixed_opnames);
}

static int augment_buffer(LOOPS_LIST(loops_span_char) head, int buffer_size)
{
    int err;
    loops_span_char first;
    err = loops_list_head(head, &first);
    if(err == LOOPS_ERR_SUCCESS)
        buffer_size = first->size;
    loops_span_char to_add;
    LOOPS_CALL_THROW(loops_span_construct_alloc(&to_add, buffer_size));
    err = loops_list_push_back(head, to_add);
    if(err != LOOPS_ERR_SUCCESS)
    {
        loops_span_destruct(to_add);
        LOOPS_THROW(err);
    }
    return LOOPS_ERR_SUCCESS;
}

int loops_printf(program_printer* printer, const char *__restrict __format,...)
{
    LOOPS_SPAN(char) buffers_tail;
    LOOPS_CALL_THROW(loops_list_tail(printer->buffers, &buffers_tail));
    if(buffers_tail->size == 0 || buffers_tail->data == NULL)
        LOOPS_THROW(LOOPS_ERR_NULL_POINTER);
    int chars_left = buffers_tail->size - printer->current_offset;
    char* nextcharpos = buffers_tail->data + printer->current_offset;
    va_list var_args;
    va_start(var_args, __format);
    int written = vsnprintf(nextcharpos, chars_left, __format, var_args);
    va_end ( var_args );
    if(written < 0 || written >= chars_left)
    {
        if(printer->current_cell == 0)
            LOOPS_THROW(LOOPS_ERR_UNIMAGINARY_BIG_STRING);
        char* current_cell_start = printer->cells[printer->current_cell - 1] + printer->cell_sizes[printer->current_cell - 1] + 1;
        int current_cell_size = (int)(buffers_tail->data + printer->current_offset - current_cell_start);
        if(current_cell_size < 0) 
            LOOPS_THROW(LOOPS_ERR_POINTER_ARITHMETIC_ERROR);
        if(current_cell_size + written >= buffers_tail->size)
            LOOPS_THROW(LOOPS_ERR_UNIMAGINARY_BIG_STRING);
        LOOPS_CALL_THROW(augment_buffer(printer->buffers, 0));
        LOOPS_SPAN(char) newtail;
        LOOPS_CALL_THROW(loops_list_tail(printer->buffers, &newtail));
        if(current_cell_size > 0)
            memcpy(newtail->data, current_cell_start, current_cell_size);
        buffers_tail = newtail;
        printer->current_offset = current_cell_size;
        chars_left = buffers_tail->size;
        nextcharpos = buffers_tail->data + printer->current_offset;
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
    LOOPS_SPAN(char) buffers_tail;
    LOOPS_CALL_THROW(loops_list_tail(printer->buffers, &buffers_tail));
    int buffers_tail_size = buffers_tail->size;
    char* buffers_tail_data = buffers_tail->data;
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

static int col_num_printer(program_printer* printer, column_printer* /*colprinter*/, syntfunc2print* /*func*/, int row)
{
    LOOPS_CALL_THROW(loops_printf(printer, "%6d :", row));
    LOOPS_CALL_THROW(close_printer_cell(printer));
    return LOOPS_ERR_SUCCESS;
}

static int col_delimeter_printer(program_printer* printer, column_printer* /*colprinter*/, syntfunc2print* /*func*/, int /*row*/)
{
    LOOPS_CALL_THROW(loops_printf(printer, ";"));
    LOOPS_CALL_THROW(close_printer_cell(printer));
    return LOOPS_ERR_SUCCESS;
}

static int col_ir_opname_printer(program_printer* printer, column_printer* /*colprinter*/, syntfunc2print* func, int row)
{
    int err;
    loops_cstring found_name = NULL;
    loops::Syntop* op = func->program->data;
    op += row;
    err = loops_hashmap_get(opstrings, op->opcode, &found_name);
    if(err == LOOPS_ERR_ELEMENT_NOT_FOUND)
    {
        suffixed_opname found_suffixed_name;
        err = loops_hashmap_get(suffixed_opnames, op->opcode, &found_suffixed_name);
        if(err != LOOPS_ERR_ELEMENT_NOT_FOUND && err != LOOPS_ERR_SUCCESS)
            LOOPS_THROW(err);
        else if(err == LOOPS_ERR_ELEMENT_NOT_FOUND)
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
                err = loops_hashmap_get(cond_suffixes, (int)op->args[0].value, &found_name);
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
                        err = loops_hashmap_get(cond_suffixes, (int)op->args[argnum].value, &found_name);
                        if(err == LOOPS_ERR_ELEMENT_NOT_FOUND)
                            LOOPS_THROW(LOOPS_ERR_UNKNOWN_TYPE);
                        else if(err != LOOPS_ERR_SUCCESS)
                            LOOPS_THROW(err);
                        break;
                    case SUFFIX_ELEMTYPE:
                        if(op->args[argnum].tag != loops::Arg::IREG && op->args[argnum].tag != loops::Arg::VREG && op->args[argnum].tag != loops::Arg::IIMMEDIATE)
                            LOOPS_THROW(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
                        err = loops_hashmap_get(type_suffixes, op->args[argnum].elemtype, &found_name);
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

static int basic_arg_printer(program_printer* printer, loops::Arg* arg)
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

static int col_ir_opargs_printer(program_printer* printer, column_printer* /*colprinter*/, syntfunc2print* func, int row)
{
    loops::Syntop* op = func->program->data;
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
    int err;
    if(res == NULL) 
        LOOPS_THROW(LOOPS_ERR_NULL_POINTER);
    if(~(~columnflags | loops::Func::PC_OPNUM | loops::Func::PC_OP))
        LOOPS_THROW(LOOPS_ERR_UNKNOWN_FLAG);

    *res = (program_printer*)malloc(sizeof(program_printer));
    if(*res == NULL)
        LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
    if(*res == NULL)
        LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
    memset(*res, 0, sizeof(program_printer));
    err = loops_list_construct(&((*res)->buffers));
    if(err != LOOPS_ERR_SUCCESS)
    {
        free(*res);
        LOOPS_THROW(err);
    }
    int colprinters_size = 0; 
    colprinters_size += ((columnflags & loops::Func::PC_OPNUM) > 0);
    colprinters_size += 2 * ((columnflags & loops::Func::PC_OP) > 0);
    err = loops_span_construct_alloc(&((*res)->colprinters), colprinters_size);
    if(err != LOOPS_ERR_SUCCESS)
    {
        loops_list_destruct((*res)->buffers);
        free(*res);
        LOOPS_THROW(err);
    }
    column_printer* curcolprinter = (*res)->colprinters->data;
    memset(curcolprinter, 0, colprinters_size * sizeof(column_printer));
    if(columnflags & loops::Func::PC_OPNUM)
    {
        curcolprinter->func = &col_num_printer;
        curcolprinter++;
    }

    if(columnflags & loops::Func::PC_OP)
    {
        curcolprinter->func = &col_ir_opname_printer;
        curcolprinter++;
        curcolprinter->func = &col_ir_opargs_printer;
        curcolprinter++;
    }
    return LOOPS_ERR_SUCCESS;
}

int col_opname_table_printer(program_printer* printer, column_printer* colprinter, syntfunc2print* func, int row)
{
    int err;
    loops_cstring found_name = NULL;
    loops::Syntop* op = func->program->data;
    op += row;
    err = loops_hashmap_get((LOOPS_HASHMAP(int, loops_cstring))colprinter->auxdata, op->opcode, &found_name);
    if(err != LOOPS_ERR_ELEMENT_NOT_FOUND && err != LOOPS_ERR_SUCCESS )
        LOOPS_THROW(err);
    else if(err == LOOPS_ERR_SUCCESS)
        LOOPS_CALL_THROW(loops_printf(printer, "%s", found_name));
    else
        LOOPS_THROW(LOOPS_ERR_UNPRINTABLE_OPERATION);
    LOOPS_CALL_THROW(close_printer_cell(printer));
    return LOOPS_ERR_SUCCESS;
}

int create_assembly_printer(int columnflags, loops::Backend* backend, program_printer** res)
{
    int err; 
    if(res == NULL) 
        LOOPS_THROW(LOOPS_ERR_NULL_POINTER);
    if(~(~columnflags | loops::Func::PC_OPNUM | loops::Func::PC_OP | loops::Func::PC_HEX))
        LOOPS_THROW(LOOPS_ERR_UNKNOWN_FLAG);

    *res = (program_printer*)malloc(sizeof(program_printer));
    if(*res == NULL) 
        LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
    memset(*res, 0, sizeof(program_printer));
    err = loops_list_construct(&((*res)->buffers));
    if(err != LOOPS_ERR_SUCCESS)
    {
        free(*res);
        LOOPS_THROW(err);
    }
    int colprinters_size = 0;
    colprinters_size += ((columnflags & loops::Func::PC_OPNUM) > 0);
    colprinters_size += 2 * ((columnflags & loops::Func::PC_OP) > 0);
    colprinters_size += 2 * ((columnflags & loops::Func::PC_HEX) > 0);
    err = loops_span_construct_alloc(&((*res)->colprinters), colprinters_size);
    if(err != LOOPS_ERR_SUCCESS)
    {
        loops_list_destruct((*res)->buffers);
        free(*res);
        LOOPS_THROW(err);
    }
    column_printer* curcolprinter = (*res)->colprinters->data;
    memset(curcolprinter, 0, colprinters_size * sizeof(column_printer));
    if(columnflags & loops::Func::PC_OPNUM)
    {
        curcolprinter->func = &col_num_printer;
        curcolprinter++;
    }
    (*res)->backend = backend;

    if(columnflags & loops::Func::PC_OP)
    {
        *curcolprinter = backend->get_opname_printer();
        curcolprinter++;
        *curcolprinter = backend->get_opargs_printer();
        curcolprinter++;
    }

    if(columnflags & loops::Func::PC_HEX)
    {
        curcolprinter->func = &col_delimeter_printer;
        curcolprinter++;
        *curcolprinter = backend->get_hex_printer();
        curcolprinter++;
    }

    return LOOPS_ERR_SUCCESS;
}

void free_printer(program_printer* tofree)
{
    int prnum = 0;
    for (; prnum < tofree->colprinters->size; prnum++)
        if (tofree->colprinters->data[prnum].free_func != NULL)
            tofree->colprinters->data[prnum].free_func(tofree->colprinters->data + prnum);
    loops_span_destruct(tofree->colprinters);
    free(tofree);
}

static void printer_buffer_destruct(void* pb)
{
    loops_span_destruct((LOOPS_SPAN(char))pb);
}

enum {PRINT_TO_FILE, PRINT_TO_STRING};
static int print_syntfunc(program_printer* printer, FILE* fout, char** sout, int outtype, syntfunc2print* func)
{
    int params_size = func->params->size;
    loops::Arg* params = func->params->data;

    int err = 0;
    int cells = 0;
    static int MAX_LINE_SIZE = 82; //taken from statistics
    int cols = printer->colprinters->size;
    int rows = func->program->size;
    int row;
    int col;

    LOOPS_CALL_THROW(augment_buffer(printer->buffers, MAX_LINE_SIZE * rows));

    printer->cells = NULL; 
    printer->cell_sizes = NULL; 
    int* max_widthes = NULL;
    char** printtasks = NULL;
    char* printtasksbuf = NULL;
    printer->cells = (char**)malloc(cols * rows * sizeof(char*)); 
    printer->cell_sizes = (int*)malloc(cols * rows * sizeof(int)); 
    max_widthes = (int*)malloc(cols * sizeof(int));
    printtasks = (char**)malloc(cols * sizeof(char*));
    printtasksbuf = (char*)malloc(cols * 10);
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
            err = printer->colprinters->data[col].func(printer, printer->colprinters->data + col, func, row);
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
        fprintf(fout, "%s(", func->name);
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
        int bufferleft = (int)strlen(func->name) + 6;
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
        *sout = (char*)malloc(bufferleft);
        if (*sout == NULL)
            LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
        char* currentout = *sout;

#define PRINT_SYNTFUNC_SPRINT(...)                                                                \
do {                                                                                              \
    int __print_syntfunc_sprint_written__ = snprintf(currentout, bufferleft, __VA_ARGS__);        \
    if (__print_syntfunc_sprint_written__ < 0 || __print_syntfunc_sprint_written__ >= bufferleft) \
    {                                                                                             \
        free(*sout);                                                                              \
        goto print_syntfunc_end;                                                                  \
    }                                                                                             \
    currentout += __print_syntfunc_sprint_written__;                                              \
    bufferleft -= __print_syntfunc_sprint_written__;                                              \
} while (0)

        //Write header:
        PRINT_SYNTFUNC_SPRINT("%s(", func->name);
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
    loops_list_destruct(printer->buffers, printer_buffer_destruct);
    free(max_widthes);
    free(printtasks);
    free(printtasksbuf);
    free(printer->cell_sizes);
    free(printer->cells);
    return err;
}

int fprint_syntfunc(program_printer* printer, FILE* out, syntfunc2print* func)
{
    return print_syntfunc(printer, out, NULL, PRINT_TO_FILE, func);
}

int sprint_syntfunc(program_printer* printer, char** out, syntfunc2print* func)
{
    return print_syntfunc(printer, NULL, out, PRINT_TO_STRING, func);
}

std::string IR_instruction2string(const loops::Syntop& op)
{
    const int columns = loops::Func::PC_OP;
    program_printer* _printer;
    Assert(create_ir_printer(columns, &_printer) == 0);
    syntfunc2print s2p;
    std::string name = "";
    s2p.name = const_cast<char*>(name.c_str());
    int err = loops_span_construct(&(s2p.program), const_cast<loops::Syntop*>(&op), 1);
    if(err != LOOPS_ERR_SUCCESS)
        throw std::runtime_error(get_errstring(err));
    err = loops_span_construct(&(s2p.params), nullptr, 0);
    if(err != LOOPS_ERR_SUCCESS)
        throw std::runtime_error(get_errstring(err));
    char* printed_str;
    err = sprint_syntfunc(_printer, &printed_str, &s2p);
    if(err != LOOPS_ERR_SUCCESS)
        throw std::runtime_error(get_errstring(err));
    free_printer(_printer);
    std::string result = printed_str + 3;
    if(result.size()) 
        result.resize(result.size()-1);
    free(printed_str);
    return result;
}

std::string assembly_instruction2string(const loops::Syntop& op, const loops::Backend& backend)
{
    const int columns = loops::Func::PC_OP;
    program_printer* _printer;
    Assert(create_assembly_printer(columns, const_cast<loops::Backend*>(&backend), &_printer) == 0);
    syntfunc2print s2p;
    std::string name = "";
    s2p.name = const_cast<char*>(name.c_str());
    int err = loops_span_construct(&(s2p.program), const_cast<loops::Syntop*>(&op), 1);
    if(err != LOOPS_ERR_SUCCESS)
        throw std::runtime_error(get_errstring(err));
    err = loops_span_construct(&(s2p.params), nullptr, 0);
    if(err != LOOPS_ERR_SUCCESS)
        throw std::runtime_error(get_errstring(err));
    char* printed_str;
    err = sprint_syntfunc(_printer, &printed_str, &s2p);
    if(err != LOOPS_ERR_SUCCESS)
        throw std::runtime_error(get_errstring(err));
    free_printer(_printer);
    std::string result = printed_str + 3;
    if(result.size()) 
        result.resize(result.size()-1);
    free(printed_str);
    return result;
}
