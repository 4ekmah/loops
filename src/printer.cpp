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
    one_name_one_suffix pieces[3];
    int pieces_size;
    UT_hash_handle hh;
} suffixed_opname;

static void add_1suffix_opname_to_map(suffixed_opname** map_to_append, int id, const char* prefix, int argnum, int suffix_type, int fracture_size = 0)
{
    suffixed_opname* map_to_append_ = *map_to_append;
    suffixed_opname* newelem = (suffixed_opname*)malloc(sizeof(suffixed_opname));
    newelem->enum_id = id;
    newelem->pieces_size = 1;
    newelem->pieces[0].argnum = argnum;
    newelem->pieces[0].suffix_type = suffix_type;
    newelem->pieces[0].fracture_size = fracture_size;
    strncpy(newelem->pieces[0].prefix, prefix, LOOPS_MAX_OPERATION_NAME_WIDTH);
    HASH_ADD_INT(map_to_append_, enum_id, newelem );
    *map_to_append = map_to_append_;
}

static void add_2suffix_opname_to_map(suffixed_opname** map_to_append, int id, const char* prefix_0, int argnum_0, int suffix_type_0,
                                                                               const char* prefix_1, int argnum_1, int suffix_type_1)
{
    suffixed_opname* map_to_append_ = *map_to_append;
    suffixed_opname* newelem = (suffixed_opname*)malloc(sizeof(suffixed_opname));
    newelem->enum_id = id;
    newelem->pieces_size = 2;
    newelem->pieces[0].argnum = argnum_0;
    newelem->pieces[0].suffix_type = suffix_type_0;
    newelem->pieces[0].fracture_size = 0;
    strncpy(newelem->pieces[0].prefix, prefix_0, LOOPS_MAX_OPERATION_NAME_WIDTH);
    newelem->pieces[1].argnum = argnum_1;
    newelem->pieces[1].suffix_type = suffix_type_1;
    newelem->pieces[1].fracture_size = 0;
    strncpy(newelem->pieces[1].prefix, prefix_1, LOOPS_MAX_OPERATION_NAME_WIDTH);
    HASH_ADD_INT(map_to_append_, enum_id, newelem );
    *map_to_append = map_to_append_;
}

static void add_3suffix_opname_to_map(suffixed_opname** map_to_append, int id, const char* prefix_0, int argnum_0, int suffix_type_0,
                                                                               const char* prefix_1, int argnum_1, int suffix_type_1,
                                                                               const char* prefix_2, int argnum_2, int suffix_type_2)
{
    suffixed_opname* map_to_append_ = *map_to_append;
    suffixed_opname* newelem = (suffixed_opname*)malloc(sizeof(suffixed_opname));
    newelem->enum_id = id;
    newelem->pieces_size = 3;
    newelem->pieces[0].argnum = argnum_0;
    newelem->pieces[0].suffix_type = suffix_type_0;
    newelem->pieces[0].fracture_size = 0;
    strncpy(newelem->pieces[0].prefix, prefix_0, LOOPS_MAX_OPERATION_NAME_WIDTH);
    newelem->pieces[1].argnum = argnum_1;
    newelem->pieces[1].suffix_type = suffix_type_1;
    newelem->pieces[1].fracture_size = 0;
    strncpy(newelem->pieces[1].prefix, prefix_1, LOOPS_MAX_OPERATION_NAME_WIDTH);
    newelem->pieces[2].argnum = argnum_2;
    newelem->pieces[2].suffix_type = suffix_type_2;
    newelem->pieces[2].fracture_size = 0;
    strncpy(newelem->pieces[2].prefix, prefix_2, LOOPS_MAX_OPERATION_NAME_WIDTH);
    HASH_ADD_INT(map_to_append_, enum_id, newelem );
    *map_to_append = map_to_append_;
}

static void free_suffixed_opname_map(suffixed_opname** map_to_free)
{
  suffixed_opname* map_to_free_ = *map_to_free;
  suffixed_opname* current_name;
  suffixed_opname* tmp;
  HASH_ITER(hh, map_to_free_, current_name, tmp) 
  {
    HASH_DEL(map_to_free_, current_name);
    free(current_name);
  }
  *map_to_free = NULL;
}

name_map_elem* opstrings = NULL;
name_map_elem* cond_suffixes = NULL;
name_map_elem* type_suffixes = NULL;
suffixed_opname* suffixed_opnames = NULL;

void printer_h_initialize()
{
    add_name_to_map(&opstrings, loops::OP_MOV         , "mov"                   );
    add_name_to_map(&opstrings, loops::OP_XCHG        , "xchg"                  );
    add_name_to_map(&opstrings, loops::OP_ADD         , "add"                   );
    add_name_to_map(&opstrings, loops::OP_SUB         , "sub"                   );
    add_name_to_map(&opstrings, loops::OP_MUL         , "mul"                   );
    add_name_to_map(&opstrings, loops::OP_DIV         , "div"                   );
    add_name_to_map(&opstrings, loops::OP_MOD         , "mod"                   );
    add_name_to_map(&opstrings, loops::OP_SHL         , "shl"                   );
    add_name_to_map(&opstrings, loops::OP_SHR         , "shr"                   );
    add_name_to_map(&opstrings, loops::OP_SAR         , "sar"                   );
    add_name_to_map(&opstrings, loops::OP_AND         , "and"                   );
    add_name_to_map(&opstrings, loops::OP_OR          , "or"                    );
    add_name_to_map(&opstrings, loops::OP_XOR         , "xor"                   );
    add_name_to_map(&opstrings, loops::OP_NOT         , "not"                   );
    add_name_to_map(&opstrings, loops::OP_NEG         , "neg"                   );
    add_name_to_map(&opstrings, loops::OP_CMP         , "cmp"                   );
    add_name_to_map(&opstrings, loops::OP_MIN         , "min"                   );
    add_name_to_map(&opstrings, loops::OP_MAX         , "max"                   );
    add_name_to_map(&opstrings, loops::OP_ABS         , "abs"                   );
    add_name_to_map(&opstrings, loops::OP_SIGN        , "sign"                  );
    add_name_to_map(&opstrings, loops::OP_SPILL       , "spill"                 );
    add_name_to_map(&opstrings, loops::OP_UNSPILL     , "unspill"               );
    add_name_to_map(&opstrings, loops::OP_GT          , "gt"                    );
    add_name_to_map(&opstrings, loops::OP_UGT         , "ugt"                   );
    add_name_to_map(&opstrings, loops::OP_GE          , "ge"                    );
    add_name_to_map(&opstrings, loops::OP_LT          , "lt"                    );
    add_name_to_map(&opstrings, loops::OP_LE          , "le"                    );
    add_name_to_map(&opstrings, loops::OP_ULE         , "ule"                   );
    add_name_to_map(&opstrings, loops::OP_NE          , "ne"                    );
    add_name_to_map(&opstrings, loops::OP_EQ          , "eq"                    );
    add_name_to_map(&opstrings, loops::OP_S           , "s"                     );
    add_name_to_map(&opstrings, loops::OP_NS          , "ns"                    );
    add_name_to_map(&opstrings, loops::OP_LOGICAL_AND , "log_and"               );
    add_name_to_map(&opstrings, loops::OP_LOGICAL_OR  , "log_or"                );
    add_name_to_map(&opstrings, loops::OP_LOGICAL_NOT , "log_not"               );
    add_name_to_map(&opstrings, loops::OP_JMP         , "jmp"                   );
    add_name_to_map(&opstrings, loops::OP_RET         , "ret"                   );
    add_name_to_map(&opstrings, loops::OP_CALL        , "call"                  );
    add_name_to_map(&opstrings, loops::OP_CALL_NORET  , "call_noret"            );
    add_name_to_map(&opstrings, loops::OP_STEM_CSTART , "annotation:stemcstart" );
    add_name_to_map(&opstrings, loops::OP_IF_CSTART   , "annotation:ifcstart"   );
    add_name_to_map(&opstrings, loops::OP_ELIF_CSTART , "annotation:elif"       );
    add_name_to_map(&opstrings, loops::OP_IF_CEND     , "annotation:ifcend"     );
    add_name_to_map(&opstrings, loops::OP_ELSE        , "annotation:else"       );
    add_name_to_map(&opstrings, loops::OP_ENDIF       , "annotation:endif"      );
    add_name_to_map(&opstrings, loops::OP_WHILE_CSTART, "annotation:whilecstart");
    add_name_to_map(&opstrings, loops::OP_WHILE_CEND  , "annotation:whilecend"  );
    add_name_to_map(&opstrings, loops::OP_ENDWHILE    , "annotation:endwhile"   );
    add_name_to_map(&opstrings, loops::OP_BREAK       , "annotation:break"      );
    add_name_to_map(&opstrings, loops::OP_CONTINUE    , "annotation:continue"   );
    add_name_to_map(&opstrings, loops::VOP_AND        , "and"                   );
    add_name_to_map(&opstrings, loops::VOP_OR         , "or"                    );
    add_name_to_map(&opstrings, loops::VOP_XOR        , "xor"                   );
    add_name_to_map(&opstrings, loops::VOP_NOT        , "not"                   );
    add_name_to_map(&opstrings, loops::VOP_SELECT     , "select"                );
    add_name_to_map(&opstrings, loops::OP_X86_ADC     , "x86_adc"               );
    add_name_to_map(&opstrings, loops::OP_X86_CQO     , "x86_cqo"               );
    add_name_to_map(&opstrings, loops::OP_ARM_CINC    , "arm_cinc"              );
    add_name_to_map(&opstrings, loops::OP_ARM_CNEG    , "arm_cneg"              );
    add_name_to_map(&opstrings, loops::OP_ARM_MOVK    , "arm_movk"              );
    add_name_to_map(&opstrings, loops::OP_ARM_LDP     , "arm_ldp"               );
    add_name_to_map(&opstrings, loops::OP_ARM_STP     , "arm_stp"               );
    add_name_to_map(&opstrings, loops::OP_DEF         , "def"                   );

    add_name_to_map(&cond_suffixes, loops::OP_EQ ,     "eq" );
    add_name_to_map(&cond_suffixes, loops::OP_NE ,     "ne" );
    add_name_to_map(&cond_suffixes, loops::OP_GE ,     "ge" );
    add_name_to_map(&cond_suffixes, loops::OP_LE ,     "le" );
    add_name_to_map(&cond_suffixes, loops::OP_ULE,     "ule");
    add_name_to_map(&cond_suffixes, loops::OP_GT ,     "gt" );
    add_name_to_map(&cond_suffixes, loops::OP_UGT,     "ugt");
    add_name_to_map(&cond_suffixes, loops::OP_LT ,     "gt" );
    add_name_to_map(&cond_suffixes, loops::OP_S  ,     "s"  );
    add_name_to_map(&cond_suffixes, loops::OP_NS ,     "ns" );

    add_name_to_map(&type_suffixes, loops::TYPE_U8  , "u8"  );
    add_name_to_map(&type_suffixes, loops::TYPE_I8  , "i8"  );
    add_name_to_map(&type_suffixes, loops::TYPE_U16 , "u16" );
    add_name_to_map(&type_suffixes, loops::TYPE_I16 , "i16" );
    add_name_to_map(&type_suffixes, loops::TYPE_U32 , "u32" );
    add_name_to_map(&type_suffixes, loops::TYPE_I32 , "i32" );
    add_name_to_map(&type_suffixes, loops::TYPE_U64 , "u64" );
    add_name_to_map(&type_suffixes, loops::TYPE_I64 , "i64" );
    add_name_to_map(&type_suffixes, loops::TYPE_FP16, "fp16");
    add_name_to_map(&type_suffixes, loops::TYPE_BF16, "bf16");
    add_name_to_map(&type_suffixes, loops::TYPE_FP32, "fp32");
    add_name_to_map(&type_suffixes, loops::TYPE_FP64, "fp64");

    add_1suffix_opname_to_map(&suffixed_opnames, loops::OP_LOAD              , "load."             , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::OP_STORE             , "store."            , 1, SUFFIX_ELEMTYPE, 3);
    add_1suffix_opname_to_map(&suffixed_opnames, loops::OP_SELECT            , "select_"           , 1, SUFFIX_CONDITION  );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::OP_IVERSON           , "iverson_"          , 1, SUFFIX_CONDITION  );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_LOAD             , "vld."              , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_STORE            , "vst."              , 1, SUFFIX_ELEMTYPE, 3);
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_ADD              , "add."              , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_SUB              , "sub."              , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_MUL              , "mul."              , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_DIV              , "div."              , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_FMA              , "fma."              , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_SAL              , "sal."              , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_SHL              , "shl."              , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_SAR              , "sar."              , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_SHR              , "shr."              , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_NEG              , "neg."              , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_MIN              , "min."              , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_MAX              , "max."              , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_GT               , "gt."               , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_GE               , "ge."               , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_LT               , "lt."               , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_LE               , "le."               , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_NE               , "ne."               , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_EQ               , "eq."               , 0, SUFFIX_ELEMTYPE   );
    add_2suffix_opname_to_map(&suffixed_opnames, loops::VOP_TRUNC            , "trunc."            , 1, SUFFIX_ELEMTYPE, "_"     , 0, SUFFIX_ELEMTYPE);
    add_2suffix_opname_to_map(&suffixed_opnames, loops::VOP_FLOOR            , "floor."            , 1, SUFFIX_ELEMTYPE, "_"     , 0, SUFFIX_ELEMTYPE);
    add_2suffix_opname_to_map(&suffixed_opnames, loops::VOP_CAST             , "cast."             , 1, SUFFIX_ELEMTYPE, "_"     , 0, SUFFIX_ELEMTYPE);
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_BROADCAST        , "broadcast."        , 0, SUFFIX_ELEMTYPE   );
    add_3suffix_opname_to_map(&suffixed_opnames, loops::VOP_CAST_LOW         , "cast."             , 0, SUFFIX_ELEMTYPE, ".from.", 1, SUFFIX_ELEMTYPE, ".low" , 0, SUFFIX_VOID);
    add_3suffix_opname_to_map(&suffixed_opnames, loops::VOP_CAST_HIGH        , "cast."             , 0, SUFFIX_ELEMTYPE, ".from.", 1, SUFFIX_ELEMTYPE, ".high", 0, SUFFIX_VOID);
    add_2suffix_opname_to_map(&suffixed_opnames, loops::VOP_SHRINK           , "shrink."           , 0, SUFFIX_ELEMTYPE, ".from.", 1, SUFFIX_ELEMTYPE);
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_POPCOUNT         , "popcount."         , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_REDUCE_MAX       , "reduce.max."       , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_REDUCE_MIN       , "reduce.min."       , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_REDUCE_SUM       , "reduce.sum."       , 0, SUFFIX_ELEMTYPE   );
    add_2suffix_opname_to_map(&suffixed_opnames, loops::VOP_REDUCE_WSUM      , "reduce.wmax"       , 0, SUFFIX_ELEMTYPE, ".from.", 1, SUFFIX_ELEMTYPE);
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_ARM_LD1          , "vld_lane."         , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_ARM_ST1          , "vst_lane."         , 1, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_ARM_LD2          , "vld_deinterleave2.", 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_ARM_EXT          , "ext."              , 0, SUFFIX_ELEMTYPE   );
    add_3suffix_opname_to_map(&suffixed_opnames, loops::VOP_ARM_SHRINK_LOW   , "cast."             , 0, SUFFIX_ELEMTYPE, ".from.", 1, SUFFIX_ELEMTYPE, ".low" , 0, SUFFIX_VOID);
    add_3suffix_opname_to_map(&suffixed_opnames, loops::VOP_ARM_SHRINK_HIGH  , "cast."             , 0, SUFFIX_ELEMTYPE, ".from.", 1, SUFFIX_ELEMTYPE, ".high", 0, SUFFIX_VOID);
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_GETLANE          , "getlane."          , 1, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_SETLANE          , "getlane."          , 0, SUFFIX_ELEMTYPE   );
    add_1suffix_opname_to_map(&suffixed_opnames, loops::VOP_DEF              , "vdef."             , 0, SUFFIX_ELEMTYPE   );
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
    if(buffer_size <= LOOPS_POSITIVE_SIZE_NEEDED)
        return LOOPS_POSITIVE_SIZE_NEEDED; //DUBUG: I think, it's needed LOOPS_THROW, which is return LOOPS_POSITIVE_SIZE_NEEDED, but in other cases is something more convinient in diagnostics, like usual throw or a place where it's possible to set breakpoint.
    (*tail) = (buffer_list*)malloc(sizeof(buffer_list));
    if(*tail == NULL) 
        return LOOPS_ERR_OUT_OF_MEMORY;
    memset((*tail), 0, sizeof(buffer_list));
    (*tail)->buffer = (char*)malloc(buffer_size);
    if((*tail)->buffer == NULL)
    {
        free(*tail);
        return LOOPS_ERR_OUT_OF_MEMORY;
    }
    (*tail)->buffer_size = buffer_size;
    LL_APPEND(*head, *tail);
    return 0;
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

namespace loops
{
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
            return LOOPS_UNIMAGINARY_BIG_STRING;
        char* current_cell_start = printer->cells[printer->current_cell - 1] + printer->cell_sizes[printer->current_cell - 1] + 1;
        int current_cell_size = printer->buffers_tail->buffer + printer->current_offset - current_cell_start;
        if(current_cell_size < 0) 
            return LOOPS_ERR_POINTER_ARITHMETIC_ERROR;
        int err = 0;
        buffer_list* newtail;
        err = augment_buffer(&(printer->buffers_head), 0, &newtail); 
        if(err != 0) 
            return err;
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
            return LOOPS_UNIMAGINARY_BIG_STRING;
    }
    nextcharpos[written] = 0;
    printer->current_offset += written;
    va_end ( var_args );
    return 0;    
}

void close_printer_cell(printer_new* printer)
{
    char* newcell = printer->current_cell == 0 ? printer->buffers_tail->buffer : printer->cells[printer->current_cell - 1] + printer->cell_sizes[printer->current_cell - 1] + 1;
    if(newcell < printer->buffers_tail->buffer || newcell >= (printer->buffers_tail->buffer + printer->buffers_tail->buffer_size)) //Buffer augmentation happened
        newcell = printer->buffers_tail->buffer;
    printer->cells[printer->current_cell] = newcell;
    printer->cell_sizes[printer->current_cell] = strlen(newcell);
    printer->current_cell++;
    printer->current_offset++;
}

int col_num_printer(printer_new* printer, column_printer* colprinter, syntfunc2print* /*func*/, int /*row*/)
{
    int err = loops_printf(printer, "%6d :", colprinter->auxdata++);
    if(err != 0) 
        return err;
    close_printer_cell(printer);
    return 0;
}

#define ASSERT_OP_FORMAT(x) if (!(x)) return LOOPS_INCORRECT_OPERATION_FORMAT;

int col_ir_opname_printer(printer_new* printer, column_printer* /*colprinter*/, syntfunc2print* func, int row)
{
    int err = 0;
    name_map_elem* found_name; 
    Syntop* op = func->program + row;
    HASH_FIND_INT(opstrings, &(op->opcode), found_name);
    if(found_name == NULL)
    {
        suffixed_opname* found_suffixed_name;
        HASH_FIND_INT(suffixed_opnames, &(op->opcode), found_suffixed_name);
        if(found_suffixed_name == NULL)
        {
            switch(op->opcode)
            {

                case OP_JCC:
                {
                    ASSERT_OP_FORMAT(op->args_size == 2 && op->args[0].tag == Arg::IIMMEDIATE && op->args[1].tag == Arg::IIMMEDIATE);
                    HASH_FIND_INT(cond_suffixes, &(op->args[0].value), found_name); if(found_name == NULL) return LOOPS_UNKNOWN_CONDITION;
                    err = loops_printf(printer, "jmp_%s %d", found_name->string_id, op->args[1].value); if(err != 0) return err;
                    break;
                }
                case OP_LABEL: //DUBUG: Frankly speaking, we have to overwrite here arguments, not name.
                {
                    ASSERT_OP_FORMAT(op->args_size == 1 && op->args[0].tag == Arg::IIMMEDIATE);
                    err = loops_printf(printer, "label %d:", op->args[0].value); if(err != 0) return err;
                    break;
                }
                default:
                    return LOOPS_UNPRINTABLE_OPERATION;
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
                        return LOOPS_INCORRECT_OPERATION_FORMAT;
                    switch (onam_osuf->suffix_type)
                    {
                        case SUFFIX_CONDITION:
                            if(op->args[argnum].tag != loops::Arg::IIMMEDIATE)
                                return LOOPS_INCORRECT_OPERATION_FORMAT;
                            HASH_FIND_INT(cond_suffixes, &(op->args[argnum].value), found_name);
                            if(found_name == NULL)
                                return LOOPS_UNKNOWN_TYPE;
                            break;
                        case SUFFIX_ELEMTYPE:
                            if(op->args[argnum].tag != loops::Arg::IREG && op->args[argnum].tag != loops::Arg::VREG)
                                return LOOPS_INCORRECT_OPERATION_FORMAT;
                            HASH_FIND_INT(type_suffixes, &(op->args[argnum].elemtype), found_name);
                            if(found_name == NULL)
                                return LOOPS_UNKNOWN_TYPE;
                            break;
                        default: 
                            return LOOPS_INCORRECT_ARGUMENT;
                    }
                    suffixstr = found_name->string_id;
                }
                err = loops_printf(printer, "%s%s", onam_osuf->prefix, suffixstr);
                if(err != 0)
                    return err;
            }
        }
    }
    else
    {
        err = loops_printf(printer, "%s", found_name->string_id);
        if(err != 0) 
            return err;
    }
    if(err != 0) 
        return err;
    close_printer_cell(printer);
    return 0;
}
#undef ASSERT_OP_FORMAT

int create_ir_printer(int columnflags, printer_new** res)
{
    if(res == NULL) 
        return LOOPS_ERR_NULL_POINTER;
    if(~(~columnflags | Func::PC_OPNUM | Func::PC_OP))
        return LOOPS_ERR_UNKNOWN_FLAG;
 
    *res = (printer_new*)malloc(sizeof(printer_new));
    if(*res == NULL) 
        return LOOPS_ERR_OUT_OF_MEMORY;
    memset(*res, 0, sizeof(printer_new));
    (*res)->colprinters_size += ((columnflags & Func::PC_OPNUM) > 0);
    (*res)->colprinters_size += ((columnflags & Func::PC_OP) > 0);
    (*res)->colprinters = (column_printer*)malloc((*res)->colprinters_size * sizeof(column_printer));
    if((*res)->colprinters == NULL)
    {
        free(*res);
        return LOOPS_ERR_OUT_OF_MEMORY;
    }
    memset((*res)->colprinters, 0, (*res)->colprinters_size * sizeof(column_printer));
    column_printer* curcolprinter = (*res)->colprinters; 
    if(columnflags & Func::PC_OPNUM)
    {
        curcolprinter->func = &col_num_printer;
        curcolprinter++;
    }

    if(columnflags & Func::PC_OP)
    {
        curcolprinter->func = &col_ir_opname_printer;
        // curcolprinter->func = &col_ir_opargs_printer;
        curcolprinter++;
    }
    return 0;
}

void free_printer(printer_new* tofree)
{
    free(tofree->colprinters);
    free(tofree);
}

int print_syntfunc(printer_new* printer, FILE* out, syntfunc2print* func)
{
    int err = 0;
    static int MAX_LINE_SIZE = 82; //took from statistics  //DUBUG: as testing of all mechanincsof new printer you have to make it lesser.
    int cols = printer->colprinters_size;
    int rows = func->program_size;

    err = augment_buffer(&(printer->buffers_head), MAX_LINE_SIZE * rows, &(printer->buffers_tail));
    if(err != 0) 
        return err;

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
    for(int row = 0; row < rows; row++)
        for(int col = 0; col < cols; col++) 
        {
            printer->colprinters[col].func(printer, printer->colprinters + col, func, row); //DUBUG: you have to handle it with printing all you can to print.
            int collen = printer->cell_sizes[row*cols + col] + 1;
            max_widthes[col] = (max_widthes[col] < collen ? collen : max_widthes[col]);
        }

    for(int col = 0; col < cols; col++)
    {
        printtasks[col] = printtasksbuf + 10 * col; 
        snprintf(printtasks[col], 10, "%%-%ds", max_widthes[col]); 
    }

    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++) 
            fprintf(out, printtasks[col], printer->cells[row * cols + col]);
        fprintf(out, "\n");
    }
print_syntfunc_end:
    free_buffer_list(printer->buffers_head);
    free(max_widthes);
    free(printtasks);
    free(printtasksbuf);
    free(printer->cell_sizes);
    free(printer->cells);
    return err;
}

void print_address(::std::ostream& str, int64_t addr)
{
    static char hexsymb[] = "0123456789ABCDEF";
    char* bytes = (char*)(&addr);
    str<<"0x";
    for(int i = 0; i < 8 ; i++)
        str<<hexsymb[(bytes[7-i]&0xF0)>>4]<<hexsymb[bytes[7-i]&0x0F];
}

Printer::Printer(const std::vector<ColPrinter>& columns) : m_columns(columns), m_backend(nullptr) {}

void Printer::print(std::ostream& out, const Syntfunc& toPrint, bool printheader, int firstop, int lastop) const
{
    lastop = (lastop == -1) ? (int)toPrint.program.size() : lastop;
    if(lastop > (int)toPrint.program.size())
        throw std::runtime_error("Printer error: too far last operation.");
    if (printheader)
        printHeader(out, toPrint);
    std::vector<std::string> pdetails;
    std::vector<int> max_widthes(m_columns.size(), 0);
    pdetails.reserve(m_columns.size() * (lastop-firstop));
    for(int rownum = firstop; rownum < lastop; rownum++)
        for (int colnum = 0; colnum < (int)m_columns.size(); colnum++)
        {
            ColPrinter& colprinter = const_cast<ColPrinter&>(m_columns[colnum]);
            std::string col;
            ::std::ostringstream str(col, ::std::ios::out);
            colprinter(str, toPrint.program[rownum], rownum, m_backend);
            str.flush();
            col = str.str();
            max_widthes[colnum] = std::max(max_widthes[colnum], (int)col.size());
            pdetails.push_back(col);
        }
    for(int rownum = firstop; rownum < lastop; rownum++)
    {
        for (int colnum = 0; colnum < (int)m_columns.size(); colnum++)
            out << std::setw(max_widthes[colnum] + 1)<< std::left << pdetails[m_columns.size()*rownum + colnum];
        out << std::endl;
    }
}

Printer::ColPrinter Printer::colNumPrinter(int /*firstRow*/)
{
    return [](::std::ostream& out, const Syntop& /*toPrint*/, int rowNum, Backend*)
    {
        out << std::setw(6) << rowNum << " :";
    };
}

Printer::ColPrinter Printer::colDelimeterPrinter()
{
    return [](::std::ostream& out, const Syntop& /*toPrint*/, int /*rowNum*/, Backend* /*backend*/)
    {
        out << ";";
    };
}

Printer::ColPrinter Printer::colOpnamePrinter(const std::unordered_map<int, std::string>& opstrings, const std::unordered_map<int, Printer::ColPrinter >& p_overrules)
{
    return [opstrings, p_overrules](::std::ostream& out, const Syntop& toPrint, int rowNum, Backend* backend)
    {
        if(p_overrules.count(toPrint.opcode) == 0)
        {
            if (opstrings.count(toPrint.opcode) == 0)
                throw std::runtime_error("Printer: unprintable operation");
            out<<opstrings.at(toPrint.opcode);
        }
        else
            p_overrules.at(toPrint.opcode)(out, toPrint, rowNum, backend);
    };
}

Printer::ColPrinter Printer::colArgListPrinter(const Syntfunc& suppfunc, const std::unordered_map<int, Printer::ColPrinter>& p_overrules)
{
    return [suppfunc, p_overrules](::std::ostream& out, const Syntop& toPrint, int rowNum, Backend* backend)
    {
        if(p_overrules.count(toPrint.opcode) == 0)
        {
            Printer::ArgPrinter argprinter = [](::std::ostream& out, const Syntop& toPrint, int /*rowNum*/, int argNum)
                {
                    out<<toPrint[argNum];
                };
            if(backend)
                argprinter = backend->argPrinter(suppfunc); //TODO(ch): We shouldn't request printer at any row. It must be called once in start.
            int aamount = toPrint.size();
            for(int anum = 0; anum + 1 < aamount ; anum++)
            {
                if(toPrint[anum].flags & AF_NOPRINT)
                    continue;
                argprinter(out, toPrint, rowNum, anum);
                out<<", ";
            }
            if(aamount)
                argprinter(out, toPrint, rowNum, aamount - 1);
        }
        else
            p_overrules.at(toPrint.opcode)(out, toPrint, rowNum, backend);
    };
}

void Printer::printHeader(std::ostream& out, const Syntfunc& toPrint) const
{
    out << toPrint.name << "(";
    for (int argnum = 0; argnum + 1 < (int)toPrint.params.size(); argnum++)
        out << toPrint.params[argnum]  << ", ";
    if (toPrint.params.size())
        out<< toPrint.params.back();
    out << ")" << std::endl;
}

}
