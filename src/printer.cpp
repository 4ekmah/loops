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
#include <utlist.h>

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
    int enum_id;
    int pieces_size;
    one_name_one_suffix pieces[3];
    UT_hash_handle hh;
} suffixed_opname;

static void initialize_suffixed_map(suffixed_opname** out_map_ptr, suffixed_opname* in_map_ptr, int size)
{
    suffixed_opname* out_map_ptr_ = *out_map_ptr;
    int opnum = 0;
    for(; opnum < size; opnum++)
        HASH_ADD_INT(out_map_ptr_, enum_id, in_map_ptr + opnum );
    *out_map_ptr = out_map_ptr_;
}

static void free_suffixed_opname_map(suffixed_opname** map_to_free)
{
  suffixed_opname* map_to_free_ = *map_to_free;
  suffixed_opname* current_name;
  suffixed_opname* tmp;
  HASH_ITER(hh, map_to_free_, current_name, tmp) 
  {
    HASH_DEL(map_to_free_, current_name);
  }
  *map_to_free = NULL;
}

static name_map_elem opstrings_[] = 
{
/*  |       enum_id        |         string_id       |    */
    {loops::OP_MOV         , "mov"                   , {}} ,
    {loops::OP_XCHG        , "xchg"                  , {}} ,
    {loops::OP_ADD         , "add"                   , {}} ,
    {loops::OP_SUB         , "sub"                   , {}} ,
    {loops::OP_MUL         , "mul"                   , {}} ,
    {loops::OP_DIV         , "div"                   , {}} ,
    {loops::OP_MOD         , "mod"                   , {}} ,
    {loops::OP_SHL         , "shl"                   , {}} ,
    {loops::OP_SHR         , "shr"                   , {}} ,
    {loops::OP_SAR         , "sar"                   , {}} ,
    {loops::OP_AND         , "and"                   , {}} ,
    {loops::OP_OR          , "or"                    , {}} ,
    {loops::OP_XOR         , "xor"                   , {}} ,
    {loops::OP_NOT         , "not"                   , {}} ,
    {loops::OP_NEG         , "neg"                   , {}} ,
    {loops::OP_CMP         , "cmp"                   , {}} ,
    {loops::OP_MIN         , "min"                   , {}} ,
    {loops::OP_MAX         , "max"                   , {}} ,
    {loops::OP_ABS         , "abs"                   , {}} ,
    {loops::OP_SIGN        , "sign"                  , {}} ,
    {loops::OP_SPILL       , "spill"                 , {}} ,
    {loops::OP_UNSPILL     , "unspill"               , {}} ,
    {loops::OP_GT          , "gt"                    , {}} ,
    {loops::OP_UGT         , "ugt"                   , {}} ,
    {loops::OP_GE          , "ge"                    , {}} ,
    {loops::OP_LT          , "lt"                    , {}} ,
    {loops::OP_LE          , "le"                    , {}} ,
    {loops::OP_ULE         , "ule"                   , {}} ,
    {loops::OP_NE          , "ne"                    , {}} ,
    {loops::OP_EQ          , "eq"                    , {}} ,
    {loops::OP_S           , "s"                     , {}} ,
    {loops::OP_NS          , "ns"                    , {}} ,
    {loops::OP_LOGICAL_AND , "log_and"               , {}} ,
    {loops::OP_LOGICAL_OR  , "log_or"                , {}} ,
    {loops::OP_LOGICAL_NOT , "log_not"               , {}} ,
    {loops::OP_JMP         , "jmp"                   , {}} ,
    {loops::OP_RET         , "ret"                   , {}} ,
    {loops::OP_CALL        , "call"                  , {}} ,
    {loops::OP_CALL_NORET  , "call_noret"            , {}} ,
    {loops::OP_STEM_CSTART , "annotation:stemcstart" , {}} ,
    {loops::OP_IF_CSTART   , "annotation:ifcstart"   , {}} ,
    {loops::OP_ELIF_CSTART , "annotation:elif"       , {}} ,
    {loops::OP_IF_CEND     , "annotation:ifcend"     , {}} ,
    {loops::OP_ELSE        , "annotation:else"       , {}} ,
    {loops::OP_ENDIF       , "annotation:endif"      , {}} ,
    {loops::OP_WHILE_CSTART, "annotation:whilecstart", {}} ,
    {loops::OP_WHILE_CEND  , "annotation:whilecend"  , {}} ,
    {loops::OP_ENDWHILE    , "annotation:endwhile"   , {}} ,
    {loops::OP_BREAK       , "annotation:break"      , {}} ,
    {loops::OP_CONTINUE    , "annotation:continue"   , {}} ,
    {loops::VOP_AND        , "and"                   , {}} ,
    {loops::VOP_OR         , "or"                    , {}} ,
    {loops::VOP_XOR        , "xor"                   , {}} ,
    {loops::VOP_NOT        , "not"                   , {}} ,
    {loops::VOP_SELECT     , "select"                , {}} ,
    {loops::OP_X86_ADC     , "x86_adc"               , {}} ,
    {loops::OP_X86_CQO     , "x86_cqo"               , {}} ,
    {loops::OP_ARM_CINC    , "arm_cinc"              , {}} ,
    {loops::OP_ARM_CNEG    , "arm_cneg"              , {}} ,
    {loops::OP_ARM_MOVK    , "arm_movk"              , {}} ,
    {loops::OP_ARM_LDP     , "arm_ldp"               , {}} ,
    {loops::OP_ARM_STP     , "arm_stp"               , {}} ,
    {loops::OP_DEF         , "def"                   , {}} ,
};

static name_map_elem cond_suffixes_[] =
{
/*  |   enum_id   | string_id|   */
    {loops::OP_EQ ,     "eq" , {}},
    {loops::OP_NE ,     "ne" , {}},
    {loops::OP_GE ,     "ge" , {}},
    {loops::OP_LE ,     "le" , {}},
    {loops::OP_ULE,     "ule", {}},
    {loops::OP_GT ,     "gt" , {}},
    {loops::OP_UGT,     "ugt", {}},
    {loops::OP_LT ,     "gt" , {}},
    {loops::OP_S  ,     "s"  , {}},
    {loops::OP_NS ,     "ns" , {}},
};

static name_map_elem type_suffixes_[] =
{
/*  |    enum_id     |string_id|   */
    {loops::TYPE_U8  , "u8"  , {}},
    {loops::TYPE_I8  , "i8"  , {}},
    {loops::TYPE_U16 , "u16" , {}},
    {loops::TYPE_I16 , "i16" , {}},
    {loops::TYPE_U32 , "u32" , {}},
    {loops::TYPE_I32 , "i32" , {}},
    {loops::TYPE_U64 , "u64" , {}},
    {loops::TYPE_I64 , "i64" , {}},
    {loops::TYPE_FP16, "fp16", {}},
    {loops::TYPE_BF16, "bf16", {}},
    {loops::TYPE_FP32, "fp32", {}},
    {loops::TYPE_FP64, "fp64", {}},
};

static suffixed_opname suffixed_opnames_[] = 
{
/*  |         enum_id        |pieces_size|                pieces                    |          */
/*                                     |        prefix    |argnum|suffix_type|fracture_size|...*/
    {loops::OP_LOAD              , 1, {{"load."             , 0, SUFFIX_ELEMTYPE, 0}}, {}},	
    {loops::OP_STORE             , 1, {{"store."            , 1, SUFFIX_ELEMTYPE, 3}}, {}},
    {loops::OP_SELECT            , 1, {{"select_"           , 1, SUFFIX_CONDITION,0}}, {}},
    {loops::OP_IVERSON           , 1, {{"iverson_"          , 1, SUFFIX_CONDITION,0}}, {}},
    {loops::VOP_LOAD             , 1, {{"vld."              , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_STORE            , 1, {{"vst."              , 1, SUFFIX_ELEMTYPE, 3}}, {}},
    {loops::VOP_ADD              , 1, {{"add."              , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_SUB              , 1, {{"sub."              , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_MUL              , 1, {{"mul."              , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_DIV              , 1, {{"div."              , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_FMA              , 1, {{"fma."              , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_SAL              , 1, {{"sal."              , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_SHL              , 1, {{"shl."              , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_SAR              , 1, {{"sar."              , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_SHR              , 1, {{"shr."              , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_NEG              , 1, {{"neg."              , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_MIN              , 1, {{"min."              , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_MAX              , 1, {{"max."              , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_GT               , 1, {{"gt."               , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_GE               , 1, {{"ge."               , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_LT               , 1, {{"lt."               , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_LE               , 1, {{"le."               , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_NE               , 1, {{"ne."               , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_EQ               , 1, {{"eq."               , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_TRUNC            , 2, {{"trunc."            , 1, SUFFIX_ELEMTYPE, 0}, {"_"     , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_FLOOR            , 2, {{"floor."            , 1, SUFFIX_ELEMTYPE, 0}, {"_"     , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_CAST             , 2, {{"cast."             , 1, SUFFIX_ELEMTYPE, 0}, {"_"     , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_BROADCAST        , 1, {{"broadcast."        , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_CAST_LOW         , 3, {{"cast."             , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}, {".low" , 0, SUFFIX_VOID, 0}}, {}},
    {loops::VOP_CAST_HIGH        , 3, {{"cast."             , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}, {".high", 0, SUFFIX_VOID, 0}}, {}},
    {loops::VOP_SHRINK           , 2, {{"shrink."           , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_POPCOUNT         , 1, {{"popcount."         , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_REDUCE_MAX       , 1, {{"reduce.max."       , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_REDUCE_MIN       , 1, {{"reduce.min."       , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_REDUCE_SUM       , 1, {{"reduce.sum."       , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_REDUCE_WSUM      , 2, {{"reduce.wmax"       , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_ARM_LD1          , 1, {{"vld_lane."         , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_ARM_ST1          , 1, {{"vst_lane."         , 1, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_ARM_LD2          , 1, {{"vld_deinterleave2.", 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_ARM_EXT          , 1, {{"ext."              , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_ARM_SHRINK_LOW   , 3, {{"cast."             , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}, {".low" , 0, SUFFIX_VOID, 0}}, {}},
    {loops::VOP_ARM_SHRINK_HIGH  , 3, {{"cast."             , 0, SUFFIX_ELEMTYPE, 0}, {".from.", 1, SUFFIX_ELEMTYPE, 0}, {".high", 0, SUFFIX_VOID, 0}}, {}},
    {loops::VOP_GETLANE          , 1, {{"getlane."          , 1, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_SETLANE          , 1, {{"getlane."          , 0, SUFFIX_ELEMTYPE, 0}}, {}},
    {loops::VOP_DEF              , 1, {{"vdef."             , 0, SUFFIX_ELEMTYPE, 0}}, {}},
};

static name_map_elem* opstrings = NULL;
static name_map_elem* cond_suffixes = NULL;
static name_map_elem* type_suffixes = NULL;
static suffixed_opname* suffixed_opnames = NULL;

void printer_h_initialize()
{
    initialize_name_map(&opstrings, opstrings_, sizeof(opstrings_) / sizeof(name_map_elem));
    initialize_name_map(&cond_suffixes, cond_suffixes_, sizeof(cond_suffixes_) / sizeof(name_map_elem));
    initialize_name_map(&type_suffixes, type_suffixes_, sizeof(type_suffixes_) / sizeof(name_map_elem));
    initialize_suffixed_map(&suffixed_opnames, suffixed_opnames_, sizeof(suffixed_opnames_) / sizeof(suffixed_opname));

}

void printer_h_deinitialize()
{
    free_name_map(&opstrings);
    free_name_map(&cond_suffixes);
    free_name_map(&type_suffixes);
    free_suffixed_opname_map(&suffixed_opnames);
}

static int augment_buffer(buffer_list** head, int buffer_size, buffer_list** tail)
{
    if(*head != NULL)
        buffer_size = (*head)->buffer_size;
    if (buffer_size <= 0)
        LOOPS_THROW(LOOPS_ERR_POSITIVE_SIZE_NEEDED);
    (*tail) = (buffer_list*)malloc(sizeof(buffer_list));
    if(*tail == NULL) 
        LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
    memset((*tail), 0, sizeof(buffer_list));
    (*tail)->buffer = (char*)malloc(buffer_size);
    if((*tail)->buffer == NULL)
    {
        free(*tail);
        LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
    }
    (*tail)->buffer_size = buffer_size;
    buffer_list* unreferenced_head = *head;
    LL_APPEND(unreferenced_head, *tail);
    *head = unreferenced_head;
    return LOOPS_ERR_SUCCESS;
}

static void free_buffer_list(buffer_list* to_free)
{
    buffer_list* elt;
    buffer_list* tmp;
    LL_FOREACH_SAFE(to_free, elt, tmp)
    {
        LL_DELETE(to_free, elt);
        free(elt);
    }    
}

int loops_printf(printer_new* printer, const char *__restrict __format,...)
{
    int chars_left = printer->buffers_tail->buffer_size - printer->current_offset;
    char* nextcharpos = printer->buffers_tail->buffer + printer->current_offset;
    va_list var_args;
    va_start(var_args, __format);
    int written = vsnprintf(nextcharpos, chars_left, __format, var_args);
    va_end ( var_args );
    if(written < 0 || written >= chars_left)
    {
        if(printer->current_cell == 0)
            LOOPS_THROW(LOOPS_ERR_UNIMAGINARY_BIG_STRING);
        char* current_cell_start = printer->cells[printer->current_cell - 1] + printer->cell_sizes[printer->current_cell - 1] + 1;
        int current_cell_size = (int)(printer->buffers_tail->buffer + printer->current_offset - current_cell_start);
        if(current_cell_size < 0) 
            LOOPS_THROW(LOOPS_ERR_POINTER_ARITHMETIC_ERROR);
        if(current_cell_size + written >= printer->buffers_tail->buffer_size)
            LOOPS_THROW(LOOPS_ERR_UNIMAGINARY_BIG_STRING);
        buffer_list* newtail;
        LOOPS_CALL_THROW(augment_buffer(&(printer->buffers_head), 0, &newtail));
        if(current_cell_size > 0)
            memcpy(newtail->buffer, current_cell_start, current_cell_size);
        printer->buffers_tail = newtail;
        printer->current_offset = current_cell_size;
        chars_left = printer->buffers_tail->buffer_size;
        nextcharpos = printer->buffers_tail->buffer + printer->current_offset;
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

int new_print_address(printer_new* printer, int64_t addr)
{
    static char hexsymb[] = "0123456789ABCDEF";
    char* bytes = (char*)(&addr);
    LOOPS_CALL_THROW(loops_printf(printer, "0x")); 
    for (int i = 0; i < 8; i++)
        LOOPS_CALL_THROW(loops_printf(printer, "%c%c", hexsymb[(bytes[7 - i] & 0xF0) >> 4], hexsymb[bytes[7 - i] & 0x0F]));
    return LOOPS_ERR_SUCCESS;
}

void close_printer_cell(printer_new* printer)
{
    char* newcell = printer->buffers_tail->buffer;  
    bool newbuffer = printer->current_cell == 0; 
    printer->buffers_tail->buffer[printer->current_offset] = 0;
    if (!newbuffer)
    {
        char* prevcell = printer->cells[printer->current_cell - 1];
        if (prevcell < printer->buffers_tail->buffer || /*Buffer augmentation happened*/
            prevcell >= (printer->buffers_tail->buffer + printer->buffers_tail->buffer_size))
            newbuffer = true;
        else
            newcell = prevcell + printer->cell_sizes[printer->current_cell - 1] + 1;
    }

    int len = (newcell >= printer->buffers_tail->buffer + printer->buffers_tail->buffer_size) ? 0 : (int)strlen(newcell);
    printer->cell_sizes[printer->current_cell] = len;
    if(len == 0 && !newbuffer)
        newcell--; //Empty strings doesn't use space
    else 
        printer->current_offset++;
    printer->cells[printer->current_cell] = newcell;
    printer->current_cell++;

}

static int col_num_printer(printer_new* printer, column_printer* /*colprinter*/, syntfunc2print* /*func*/, int row)
{
    LOOPS_CALL_THROW(loops_printf(printer, "%6d :", row));
    close_printer_cell(printer);
    return LOOPS_ERR_SUCCESS;
}

static int col_delimeter_printer(printer_new* printer, column_printer* /*colprinter*/, syntfunc2print* /*func*/, int /*row*/)
{
    LOOPS_CALL_THROW(loops_printf(printer, ";"));
    close_printer_cell(printer);
    return LOOPS_ERR_SUCCESS;
}


static int col_ir_opname_printer(printer_new* printer, column_printer* /*colprinter*/, syntfunc2print* func, int row)
{
    name_map_elem* found_name; 
    loops::Syntop* op = func->program + row;
    HASH_FIND_INT(opstrings, &(op->opcode), found_name);
    if(found_name == NULL)
    {
        suffixed_opname* found_suffixed_name;
        HASH_FIND_INT(suffixed_opnames, &(op->opcode), found_suffixed_name);
        if(found_suffixed_name == NULL)
        {
            switch(op->opcode)
            {

            case loops::OP_JCC:
            {
                if (!(op->args_size == 2 && op->args[0].tag == loops::Arg::IIMMEDIATE && op->args[1].tag == loops::Arg::IIMMEDIATE))
                    LOOPS_THROW(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
                HASH_FIND_INT(cond_suffixes, &(op->args[0].value), found_name); if(found_name == NULL) LOOPS_THROW(LOOPS_ERR_UNKNOWN_CONDITION);
                LOOPS_CALL_THROW(loops_printf(printer, "jmp_%s %d", found_name->string_id, op->args[1].value));
                break;
            }
            case loops::OP_LABEL:
            {
                if (!(op->args_size == 1 && op->args[0].tag == loops::Arg::IIMMEDIATE))
                    LOOPS_THROW(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
                LOOPS_CALL_THROW(loops_printf(printer, "label %d:", op->args[0].value));
                break;
            }
            default:
                LOOPS_THROW(LOOPS_ERR_UNPRINTABLE_OPERATION);
            }; 
        }
        else 
        {
            int i = 0;
            for(; i < found_suffixed_name->pieces_size; i++) 
            {
                one_name_one_suffix* onam_osuf= found_suffixed_name->pieces + i;
                char dummy[] = "";
                char* suffixstr = dummy; 
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
                        HASH_FIND_INT(cond_suffixes, &(op->args[argnum].value), found_name);
                        if(found_name == NULL)
                            LOOPS_THROW(LOOPS_ERR_UNKNOWN_TYPE);
                        break;
                    case SUFFIX_ELEMTYPE:
                        if(op->args[argnum].tag != loops::Arg::IREG && op->args[argnum].tag != loops::Arg::VREG)
                            LOOPS_THROW(LOOPS_ERR_INCORRECT_OPERATION_FORMAT);
                        HASH_FIND_INT(type_suffixes, &(op->args[argnum].elemtype), found_name);
                        if(found_name == NULL)
                            LOOPS_THROW(LOOPS_ERR_UNKNOWN_TYPE);
                        break;
                    default: 
                        LOOPS_THROW(LOOPS_ERR_INCORRECT_ARGUMENT);
                    }
                    suffixstr = (char*)found_name->string_id;
                }
                LOOPS_CALL_THROW(loops_printf(printer, "%s%s", onam_osuf->prefix, suffixstr));
            }
        }
    }
    else
        LOOPS_CALL_THROW(loops_printf(printer, "%s", found_name->string_id));
    close_printer_cell(printer);
    return LOOPS_ERR_SUCCESS;
}

static int basic_arg_printer(printer_new* printer, loops::Arg* arg)
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

static int col_ir_opargs_printer(printer_new* printer, column_printer* /*colprinter*/, syntfunc2print* func, int row)
{
    loops::Syntop* op = func->program + row;
    switch(op->opcode)
    {
    case loops::OP_LABEL:
    case loops::OP_JCC:
        break;
    case loops::OP_IVERSON:
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
    close_printer_cell(printer);
    return LOOPS_ERR_SUCCESS;
}

int create_ir_printer(int columnflags, printer_new** res)
{
    if(res == NULL) 
        LOOPS_THROW(LOOPS_ERR_NULL_POINTER);
    if(~(~columnflags | loops::Func::PC_OPNUM | loops::Func::PC_OP))
        LOOPS_THROW(LOOPS_ERR_UNKNOWN_FLAG);

    *res = (printer_new*)malloc(sizeof(printer_new));
    if(*res == NULL) 
        LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
    memset(*res, 0, sizeof(printer_new));
    (*res)->colprinters_size += ((columnflags & loops::Func::PC_OPNUM) > 0);
    (*res)->colprinters_size += 2 * ((columnflags & loops::Func::PC_OP) > 0);
    (*res)->colprinters = (column_printer*)malloc((*res)->colprinters_size * sizeof(column_printer));
    if((*res)->colprinters == NULL)
    {
        free(*res);
        LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
    }
    memset((*res)->colprinters, 0, (*res)->colprinters_size * sizeof(column_printer));
    column_printer* curcolprinter = (*res)->colprinters; 
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

int col_opname_table_printer(printer_new* printer, column_printer* colprinter, syntfunc2print* func, int row)
{
    name_map_elem* found_name; 
    loops::Syntop* op = func->program + row;
    HASH_FIND_INT((name_map_elem*)colprinter->auxdata, &(op->opcode), found_name);
    if (found_name != NULL)
    {
        LOOPS_CALL_THROW(loops_printf(printer, "%s", found_name->string_id));
    }
    else
        LOOPS_THROW(LOOPS_ERR_UNPRINTABLE_OPERATION);
    close_printer_cell(printer);
    return LOOPS_ERR_SUCCESS;
}


int create_assembly_printer(int columnflags, loops::Backend* backend, printer_new** res)
{
    if(res == NULL) 
        LOOPS_THROW(LOOPS_ERR_NULL_POINTER);
    if(~(~columnflags | loops::Func::PC_OPNUM | loops::Func::PC_OP | loops::Func::PC_HEX))
        LOOPS_THROW(LOOPS_ERR_UNKNOWN_FLAG);

    *res = (printer_new*)malloc(sizeof(printer_new));
    if(*res == NULL) 
        LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
    memset(*res, 0, sizeof(printer_new));
    (*res)->colprinters_size += ((columnflags & loops::Func::PC_OPNUM) > 0);
    (*res)->colprinters_size += 2 * ((columnflags & loops::Func::PC_OP) > 0);
    (*res)->colprinters_size += 2 * ((columnflags & loops::Func::PC_HEX) > 0);
    (*res)->colprinters = (column_printer*)malloc((*res)->colprinters_size * sizeof(column_printer));
    if((*res)->colprinters == NULL)
    {
        free(*res);
        LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
    }
    memset((*res)->colprinters, 0, (*res)->colprinters_size * sizeof(column_printer));
    column_printer* curcolprinter = (*res)->colprinters; 
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

void free_printer(printer_new* tofree)
{
    int prnum = 0;
    for (; prnum < tofree->colprinters_size; prnum++)
        if (tofree->colprinters[prnum].free_func != NULL)
            tofree->colprinters[prnum].free_func(tofree->colprinters + prnum);
    free(tofree->colprinters);
    free(tofree);
}

//DUBUG: It's needed to check all this code with c89 compiler before push.
enum {PRINT_TO_FILE, PRINT_TO_STRING};
static int print_syntfunc(printer_new* printer, FILE* fout, char** sout, int outtype, syntfunc2print* func)
{
    int err = 0;
    int cells = 0;
    static int MAX_LINE_SIZE = 82; //taken from statistics
    int cols = printer->colprinters_size;
    int rows = func->program_size;
    int row;
    int col;

    LOOPS_CALL_THROW(augment_buffer(&(printer->buffers_head), MAX_LINE_SIZE * rows, &(printer->buffers_tail)));

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
            err = printer->colprinters[col].func(printer, printer->colprinters + col, func, row);
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
        for (parnum = 0; parnum < func->params_size - 1; parnum++)
            fprintf(fout, "i%d, ", (func->params + parnum)->idx);
        if (func->params_size)
            fprintf(fout, "i%d", (func->params + func->params_size - 1)->idx);
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
        for (parnum = 0; parnum < func->params_size; parnum++)
        {
            assert((func->params + parnum)->idx < 100);
            bufferleft += ((func->params + parnum)->idx > 10 ? 2 : 1) + 3;
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
        for (parnum = 0; parnum < func->params_size - 1; parnum++)
            PRINT_SYNTFUNC_SPRINT("i%d, ", (func->params + parnum)->idx);
        if (func->params_size)
            PRINT_SYNTFUNC_SPRINT("i%d", (func->params + func->params_size - 1)->idx);
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
    free_buffer_list(printer->buffers_head);
    free(max_widthes);
    free(printtasks);
    free(printtasksbuf);
    free(printer->cell_sizes);
    free(printer->cells);
    return err;
}

int fprint_syntfunc(printer_new* printer, FILE* out, syntfunc2print* func)
{
    return print_syntfunc(printer, out, NULL, PRINT_TO_FILE, func);
}

int sprint_syntfunc(printer_new* printer, char** out, syntfunc2print* func)
{
    return print_syntfunc(printer, NULL, out, PRINT_TO_STRING, func);
}
