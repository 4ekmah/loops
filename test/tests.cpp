/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "tests.hpp"
#include "src/common.hpp"
#include "src/reg_allocator.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <locale>
#include <sys/stat.h> //TODO(ch): *nix-only. Use winbase.h for Windows 

using namespace loops;

static inline std::string OSname()
{
    #if __LOOPS_OS == __LOOPS_LINUX
        return "Linux";
    #elif __LOOPS_OS == __LOOPS_WINDOWS
        return "Windows";
    #elif __LOOPS_OS == __LOOPS_MAC
        return "Macos";
    #else
        #error Unknown OS
    #endif
}

inline std::string toLower(const std::string& tL)
{
    std::string res = tL;
    std::transform(res.begin(), res.end(), res.begin(), [](char t) {return (char)::tolower(t); });
    return res;
}
#undef min
#undef max
void switch_spill_stress_test_mode_on(loops::Func& func)
{
    ContextImpl* CTX = ((FuncImpl*)getImpl(&func))->getContext();
    std::vector<int> sparam = CTX->getBackend()->parameterRegisters(RB_INT);
    std::vector<int> sreturn = CTX->getBackend()->returnRegisters(RB_INT);
    std::vector<int> scallers = CTX->getBackend()->callerSavedRegisters(RB_INT);
    std::vector<int> scallees = CTX->getBackend()->calleeSavedRegisters(RB_INT);

    std::vector<int> vparam = CTX->getBackend()->parameterRegisters(RB_VEC);
    std::vector<int> vreturn = CTX->getBackend()->returnRegisters(RB_VEC);
    std::vector<int> vcallers = CTX->getBackend()->callerSavedRegisters(RB_VEC);
    std::vector<int> vcallees = CTX->getBackend()->calleeSavedRegisters(RB_VEC);

    const size_t common_amount = 8; //Well, tell me why?
    const size_t default_params_amount = 4;
    const size_t params_amount = std::min((size_t)std::max(func.signature(), 4), sparam.size()); //Accordingly to function signature. Can be more than default_params_amount, but not vice versa.
    {
        size_t callees_amount = common_amount - (default_params_amount + (std::count(sparam.begin(), sparam.begin() + default_params_amount, sreturn[0]) == 0 ? 1 : 0));
        scallers.clear();
        if(callees_amount < scallees.size())
            scallees.erase(scallees.begin(), scallees.begin() + scallees.size() - callees_amount);
        if(params_amount < sparam.size())
            sparam.erase(sparam.begin() + params_amount, sparam.end());
        std::set<int> ireturn({sreturn[0]});
        for(int retreg : sreturn)
            if(std::count(sparam.begin(), sparam.end(), retreg) > 0)
                ireturn.insert(retreg);
        for(int regnum = (int)sreturn.size() - 1; regnum >= 0; regnum--)
            if(ireturn.count(sreturn[regnum]) == 0)
                sreturn.erase(sreturn.begin()+regnum);
    }
    getImpl(&func)->overrideRegisterSet(RB_INT, sparam, sreturn, scallers, scallees);
    getImpl(&func)->overrideRegisterSet(RB_VEC, vparam, vreturn, vcallers, vcallees);
}

void direct_translation_on(loops::Func& func)
{
    getImpl(&func)->directTranslationOn();
}

struct listing_token
{
    std::string token;
    size_t line;
    size_t col;
    listing_token(const std::string& a_token, size_t a_line, size_t a_col): token(a_token), line(a_line), col(a_col){}
};

struct tokenized_line
{
    std::string wholeline;
    std::vector<listing_token> words;
    size_t line;
    tokenized_line(const std::string a_wholeline, size_t a_line): wholeline(a_wholeline), line(a_line)
    {
        size_t pos = 0;
        const size_t sz = wholeline.size();
        while(pos < sz && wholeline[pos] == ' ') pos++;
        while(pos < sz)
        {
            size_t tokenend = pos;
            while(tokenend < sz && wholeline[tokenend] != ' ') tokenend++;
            words.push_back(listing_token(std::string(wholeline.begin()+pos, wholeline.begin()+tokenend), line, pos));
            pos = tokenend;
            while(pos < sz && wholeline[pos] == ' ') pos++;
        }
    }
};

static std::vector<tokenized_line> tokenizer(const std::string& text)
{
    std::vector<tokenized_line> res;
    const size_t sz = text.size();
    size_t pos = 0;
    size_t line = 0;
    while (pos < sz)
    {
        size_t newpos = text.find('\n',pos);
        newpos = (newpos == std::string::npos ? sz : newpos);
        size_t justafter = newpos;
        res.push_back(tokenized_line(std::string(text.begin()+pos, text.begin()+justafter), line));
        pos = newpos + 1;
        line++;
    }
    while(res.size() && res.back().words.size() == 0)
        res.pop_back();
    return res;
}

std::string create_pointing_lines(size_t ptr, size_t underlying_start, size_t underlying_end)
{
    std::stringstream resstream;
    resstream << std::string(8 + underlying_start,' ') + std::string(underlying_end - underlying_start, '~') << std::endl;
    resstream << std::string(8 + ptr,' ') + "^" << std::endl;
    return resstream.str();
}


bool check_listing_equality(const std::string& curL, const std::string& refL, const std::string& errMes, std::ostream& out_stream)
{
    std::string endline; {std::stringstream tmpstream; tmpstream << std::endl; endline = tmpstream.str();}
    std::vector<tokenized_line> ref_tokenized = tokenizer(refL); 
    std::vector<tokenized_line> cur_tokenized = tokenizer(curL);
    bool result = true;
    std::string main_error_line;
    std::string refline;
    std::string ref_underline;
    std::string curline;
    std::string cur_underline;

    size_t lines_amount = std::max(ref_tokenized.size(), cur_tokenized.size()); 
    for(size_t line = 0; line < lines_amount; line++)
    {
        if(line >= ref_tokenized.size())
        {
            main_error_line = std::string("    ") + errMes + ": Listing have extra line number " + std::to_string(line + 1) + endline;
            curline = std::string("    Current line: ") + endline + "        "  + cur_tokenized[line].wholeline + endline;
            result = false;
            break;
        }

        if(line >= cur_tokenized.size())
        {
            main_error_line = std::string("    ") + errMes + ": Listing doesn't have line number " + std::to_string(line + 1) + endline;
            curline = std::string("    Reference line: ") + endline + "        "  + ref_tokenized[line].wholeline + endline;
            result = false;
            break;
        }

        size_t words_amount = std::max(ref_tokenized[line].words.size(), cur_tokenized[line].words.size()); 
        for(size_t word = 0; word < words_amount; word++)
        {
            if(word >= ref_tokenized[line].words.size())
            {
                std::string& curword = cur_tokenized[line].words[word].token;
                main_error_line = std::string("    ") + errMes + ": Extra token in listing on line " + std::to_string(line + 1) + endline;
                refline = std::string("    Reference line, column ") + std::to_string(ref_tokenized[line].wholeline.size()) + ":" + endline + "        " + ref_tokenized[line].wholeline + endline;
                curline = std::string("    Current line, column ") + std::to_string(cur_tokenized[line].words[word].col + 1) + ":" + endline + "        "  + cur_tokenized[line].wholeline + endline;
                cur_underline = create_pointing_lines(cur_tokenized[line].words[word].col, cur_tokenized[line].words[word].col, cur_tokenized[line].words[word].col + curword.size());
                result = false;
                break;
            }

            if(word >= cur_tokenized[line].words.size())
            {
                std::string& refword = ref_tokenized[line].words[word].token;
                main_error_line = std::string("    ") + errMes + ": Missing token in listing on line " + std::to_string(line + 1) + endline;
                refline = std::string("    Reference line, column ") + std::to_string(ref_tokenized[line].words[word].col + 1) + ":" + endline + "        " + ref_tokenized[line].wholeline + endline;
                ref_underline = create_pointing_lines(ref_tokenized[line].words[word].col, ref_tokenized[line].words[word].col, ref_tokenized[line].words[word].col + refword.size());
                curline = std::string("    Current line, column ") + std::to_string(cur_tokenized[line].wholeline.size()) + ":" + endline + "        "  + cur_tokenized[line].wholeline + endline;
                result = false;
                break;
            }

            std::string& refword = ref_tokenized[line].words[word].token;
            std::string& curword = cur_tokenized[line].words[word].token;
            if(refword != curword)
            {
                size_t inequal_pos = 0; for(;inequal_pos < refword.size() && inequal_pos < curword.size() && refword[inequal_pos] == curword[inequal_pos]; inequal_pos++);
                assert(curword.size() > 0 && refword.size() > 0);
                size_t ref_inpos = std::min(inequal_pos, refword.size() - 1);
                size_t cur_inpos = std::min(inequal_pos, curword.size() - 1);
                main_error_line = std::string("    ") + errMes + ": Listing doesn't equal to reference on line " + std::to_string(line + 1) + endline;
                refline = std::string("    Reference line, column ") + std::to_string(ref_tokenized[line].words[word].col + 1) + ":" + endline + "        " + ref_tokenized[line].wholeline + endline;
                ref_underline = create_pointing_lines(ref_tokenized[line].words[word].col + ref_inpos, ref_tokenized[line].words[word].col, ref_tokenized[line].words[word].col + refword.size());
                curline = std::string("    Current line, column ") + std::to_string(cur_tokenized[line].words[word].col + 1) + ":" + endline + "        "  + cur_tokenized[line].wholeline + endline;
                cur_underline = create_pointing_lines(cur_tokenized[line].words[word].col + cur_inpos, cur_tokenized[line].words[word].col, cur_tokenized[line].words[word].col + curword.size());
                result = false;
                break;
            }
        }
        if(!result) 
            break;
    }
    if(!result)
        out_stream << main_error_line << refline << ref_underline << curline << cur_underline;
    return result;
}

inline bool fileexists(const std::string& name) //TODO(ch): use GetFileAttributes for windows
{
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

bool intermediate_representation_is_stable(loops::Func func, std::string& errmessage)
{
    std::stringstream errstream;
    const std::string func_name = func.name();
    const std::string tarcname = ((FuncImpl*)getImpl(&func))->getContext()->getPlatformName();
    const std::string arcOSsuffix = toLower(tarcname) + "/" + toLower(OSname());
    const std::string irfilename = std::string(LOOPS_TEST_DIR"/refasm/") + arcOSsuffix + "/bytecode/" + func_name + ".tst"; //DUBUG: change dir: bytecode -> IR
    bool result = true;
    bool rewrite = false;
    std::string irtext;
    ::std::ostringstream irstream(irtext, ::std::ios::out);
    func.printBytecode(irstream);
    irtext = irstream.str();
    if(fileexists(irfilename))
    {
        std::ifstream refstream(irfilename.c_str(), ::std::ios::in);
        if(!refstream.good())
        {
            errstream << "    Intermediate representation check failed: cannot open file: \""<<irfilename<<"\"."<<std::endl;
            rewrite = RECREATE_REFERENCE_TEXTS;
        }
        else
        {
            std::string irref((std::istreambuf_iterator<char>(refstream)), std::istreambuf_iterator<char>());
            if(!check_listing_equality(irtext, irref, "Intermediate representation check failed", errstream))
            {
                rewrite = RECREATE_REFERENCE_TEXTS;
                result = false;
            }
        }
    }
    else
    {
        errstream<<"    Intermediate representation check failed: file doesn't exist: \""<<irfilename<<"\"."<<std::endl;
        rewrite = RECREATE_REFERENCE_TEXTS;
        result = false;
    }
    if(rewrite)
    {
        std::ofstream rwrtstream(irfilename.c_str(), ::std::ios::out);
        if(!rwrtstream.good())
            errstream<<"    Intermediate representation check failed: cannot open file for rewrite: \""<<irfilename<<"\"."<<std::endl;
        rwrtstream<<irtext;
        rwrtstream.close();
    }
    errmessage = errstream.str();
    return result;
}

bool assembly_is_stable(loops::Func func, std::string& errmessage)
{
    std::stringstream errstream;
    const std::string func_name = func.name();
    const std::string tarcname = ((FuncImpl*)getImpl(&func))->getContext()->getPlatformName();
    const std::string arcOSsuffix = toLower(tarcname) + "/" + toLower(OSname());
    const std::string afilename = std::string(LOOPS_TEST_DIR"/refasm/") + arcOSsuffix + "/" + func_name + ".tst";
    bool result = true;
    bool rewrite = false;
    std::string atext;
    ::std::ostringstream astream(atext, ::std::ios::out);
    func.printAssembly(astream);
    atext = astream.str();
    if(fileexists(afilename))
    {
        std::ifstream refstream(afilename.c_str(), ::std::ios::in);
        if(!refstream.good())
        {
            errstream<<"    Check for "<<tarcname<<" failed: cannot open file: \""<<afilename<<"\"."<<std::endl;
            rewrite = RECREATE_REFERENCE_TEXTS;
        }
        else
        {
            std::string bcref((std::istreambuf_iterator<char>(refstream)), std::istreambuf_iterator<char>());
            if(!check_listing_equality(atext, bcref, "Check for "+tarcname+" failed", errstream))
            {
                rewrite = RECREATE_REFERENCE_TEXTS;
                result = false;
            }
        }
    }
    else
    {
        errstream<<"    Check for "<<tarcname<<" failed: file doesn't exist: \""<<afilename<<"\"."<<std::endl;
        rewrite = RECREATE_REFERENCE_TEXTS;
        result = false;
    }
    if(rewrite)
    {
        std::ofstream rwrtstream(afilename.c_str(), ::std::ios::out);
        if(!rwrtstream.good())
            errstream<<"    Check for "<<tarcname<<" failed: cannot open file for rewrite: \""<<afilename<<"\"."<<std::endl;
        rwrtstream<<atext;
        rwrtstream.close();
    }
    errmessage = errstream.str();
    return result;
}

loops::Func* get_assembly_reg_param(loops::Func& func)
{
    return getImpl(&func);
}

bool memok(uint8_t* canvas, int64_t w, int64_t h)
{
    for(int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
        {
            if (canvas[i * w + j] != 0)
            {
                std::cout << "    Memory writing violation at output [" << -1 << ", " << i << ", " << j << "]" << std::endl;
                return false;
            }
            if(canvas[2*h*w + i*w + j] != 0)
            {
                std::cout << "    Memory writing violation at output ["<< 1 <<", "<< i <<", "<< j<<"]"<<std::endl;
                return false;
            }
        }
    return true;
}

::std::ostringstream test_stream;

std::ostream& get_test_ostream()
{
    return test_stream;
}

void reset_test_ostream()
{
    test_stream = ::std::ostringstream();
}

std::string get_test_ostream_result()
{
    return test_stream.str();
}
